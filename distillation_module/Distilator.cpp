#include "Distilator.hpp"
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include "Tags.h"

using namespace pugi;
using namespace std;

/*
    Auxilleris function
*/

void printing_children(xml_node node)
{
    for (xml_node child = node.first_child(); child; child = child.next_sibling())
        cout<< child.name() << endl;
}

bool is_int(string s)
{
    try {
        int a = stoi(s);
        double d = stod(s);
        return (double)a == d;
    }
    catch(...) {
        return false;
    }
}

/* 
    zip operations
*/

/*
void Distilator::zip_file()
{
    string file_name = this->zip_path;
    int last_index = this->zip_path.find_last_of("/");
    if (last_index == this->zip_path.size()-1)
    {
        file_name = file_name.substr(0, this->zip_path.size()-1);
    }
    string command = "zip " + file_name + " -rm -j " + this->zip_path;
    system(command.c_str());
    command = "rm -r " + this->zip_path;
    system(command.c_str());
    this->zip_path.push_back('/');
}
*/

void Distilator::unzip_file()
{
    this->original_folder = this->file_name.substr(0, this->file_name.length()-5); 
    string command = "unzip -q " + this->file_name + " -d " + this->original_folder;
    system(command.c_str());
}

/*
    Constructor and Destructor
*/

Distilator::Distilator(char* file_name, char* new_file)
{
    this->file_name = string(file_name);
    this->new_file = string(new_file);
    unzip_file();
}

Distilator::~Distilator()
{
    this->new_doc->Save();
    //delete(new_doc);

    for(auto image_name : image_names)
    {
        string command = "cd " + this->original_folder + " ; zip ../" + this->new_file + " word/" + image_name;
        system(command.c_str());
    }

    string command = "rm -r " + this->original_folder;
    system(command.c_str());
}

xml_node Distilator::get_relation_node(string relation)
{
    for (auto rel = this->rels_root.first_child(); rel; rel=rel.next_sibling())
    {
        xml_attribute id = rel.attribute("Id");
        if (id && relation.compare(id.value()) == 0)
            return rel;
    }
    return this->rels_root;
}

xml_node Distilator::get_style_node(string style_id, string style_tag)
{
    for (auto style = this->styles_root.child(STYLE); style; style=style.next_sibling(STYLE))
    {
        xml_attribute id = style.attribute(STYLE_ID);
        xml_attribute tag = style.attribute(STYLE_TYPE);
        if (!id || !tag)
            continue;
        if (style_id.compare(id.value()) == 0 && style_tag.compare(tag.value()) == 0)
            return style;
    }
    return this->styles_root;
}

void Distilator::print_levels_counters()
{
    for(auto iter = this->level_counters.begin(); iter != this->level_counters.end(); ++iter)
        cout<< "level = " << iter->first << ", counter = " << iter->second << endl;
}

void Distilator::handle_levels(int level)
{
    this->print_levels_counters();
    for(auto iter = this->level_counters.begin(); iter != this->level_counters.end(); ++iter)
    {
        if (iter->first > level)
            iter->second = 1;
    }
    if (this->level_counters.find(level) == this->level_counters.end())
        this->level_counters[level] = 1;
}

void Distilator::handle_list(xml_node pPr_node)
{
    xml_node numPr_node = pPr_node.child(NUM_PROPERTY);
    if (numPr_node != 0)
    {
        xml_node ilvl_node = numPr_node.child(LEVEL);
        if (ilvl_node != 0)
        {
            xml_attribute val = ilvl_node.attribute(VALUE);
            if (val !=0)
            {    
                cout<<val.value()<<endl;
                try {
                    this->handle_levels(stoi(val.value()));
                    this->list_level = stoi(val.value());
                }
                catch (const invalid_argument& e)
                {
                    cout<< "Invalid list level" << endl;
                }
            }
        }
    }
}

void Distilator::handle_hyperlink(xml_node hyperlink_node, Hyperlink &h)
{
    auto id = hyperlink_node.attribute(RELATION_ID);
    auto history = hyperlink_node.attribute(HISTORY);
    auto run_node = hyperlink_node.child(RUN);
    if (!id || ! history || !run_node) {
        cout<<"File is Corrupted Hyperlink 1" << endl;
        h.Remove();
        return;
    }
    auto relation_node = this->get_relation_node(id.value());
    if (relation_node.value() != this->rels_root.value()) {
        cout<<"File is Corrupted Hyperlink 2" << endl;
        h.Remove();
        return;
    }
    auto target = relation_node.attribute(TARGET);
    auto type = relation_node.attribute(TYPE);
    auto target_mode = relation_node.attribute(TARGET_MODE);
    auto id2 = relation_node.attribute("Id");
    if (!target || !type || !target_mode || !id2 || strcmp(id.value(),id2.value()) != 0) {
        cout<<"File is Corrupted Hyperlink 3" << endl;
        h.Remove();
        return;
    }

    this->new_doc->AddRelationship(target.value(), type.value(), id.value() ,target_mode.value());
    h.setID(id.value());
    h.setHistory(strcmp(history.value(), "1") == 0 || strcmp(history.value(), "true") == 0);
    auto r = h.AppendRun();
    this->handle_run(run_node, r);
}


void Distilator::handle_paragraph_properties(xml_node paragraph_node, Paragraph &p)
{
    xml_node pPr_node = paragraph_node.child(PARAGRAPH_PROPERTY);
    if (pPr_node.empty()) {
        cout<<"There is no paragraph property" << endl;
        return;
    }
    this->set_paragraph_allignment(pPr_node, p);
    if (pPr_node.child(BIDI)) {
        p.setParagraphLayoutRight();
    }
    xml_node pStyle_node = pPr_node.child(PARAGRAPH_STYLE);
    if (pStyle_node != 0)
    {
        xml_attribute att = pStyle_node.attribute(VALUE);
        if (att != 0 && strcmp(att.value(), LIST_PARAGRAPH) == 0)
            this->handle_list(pPr_node);
    }
}

void Distilator::set_paragraph_allignment(xml_node pPr_node, Paragraph &p)
{
    xml_node jc_node = pPr_node.child(JUSTIFICATION);
    if (jc_node != 0)
    {
        xml_attribute att = jc_node.attribute(VALUE);
        if (att != 0)
        {
            if (strcmp(att.value(), "start") == 0 || strcmp(att.value(), "left") == 0)
                p.SetAlignment(Paragraph::Alignment::Left);
            else if (strcmp(att.value(), "end") == 0 || strcmp(att.value(), "right") == 0)
                p.SetAlignment(Paragraph::Alignment::Right);
            else if (strcmp(att.value(), "center") == 0)
                p.SetAlignment(Paragraph::Alignment::Centered);
            else if (strcmp(att.value(), "both") == 0)
                p.SetAlignment(Paragraph::Alignment::Justified);
            else if (strcmp(att.value(), "distribute") == 0)
                p.SetAlignment(Paragraph::Alignment::Distributed);
            else {
                cout << "Justification Error: File is Corrupted" << endl;
            }
        }
    }
}

void Distilator::handle_drawing(xml_node drawing_node)
{
    xml_node node = drawing_node;

    node = drawing_node.child(INLINE);
    node = (node) ? node : drawing_node.child(ANCHOR);

    for(const auto & name : IMAGE_PATH)
    {
        node = node.child(name);
        if(!node)
        {
            std::cout << "very bad 1!!!\n";
            exit(1);
        }
    }
    xml_attribute att = node.attribute(EMBED);
    if(!att)
    {
        std::cout << "very bad 2!!!\n";
        exit(1);
    }
    std::string rid= att.value();
    auto relation_node = this->get_relation_node(rid);
    if (relation_node)
    {
        auto target = relation_node.attribute(TARGET);
        auto type = relation_node.attribute(TYPE);

        this->new_doc->AddRelationship(target.value(), type.value(), rid);
        image_names.push_back(string(target.value()));
        /*
        string command = "cp " + this->original_folder + "/word/" + img_name + " " + this->zip_path + img_name.substr(6);
        system(command.c_str());

        this->file_text.append(std::string("### ") + img_name.substr(6) + " ###\n");
        */
    }
    else
    {
        std::cout << "very bad 3!!!\n";
        exit(1);
    }
}

void Distilator::handle_object(xml_node object_node)
{
    string ole_img_rid = object_node.child(SHAPE).child(IMAGEDATA).attribute(RELATION_ID).value();
    string ole_rid = object_node.child(OLE).attribute(RELATION_ID).value();

    auto rnode_ole_img = this->get_relation_node(ole_img_rid);
    auto rnode_ole = this->get_relation_node(ole_rid);

    if (rnode_ole_img && rnode_ole)
    {
        auto target_img = rnode_ole_img.attribute(TARGET);
        auto type_img = rnode_ole_img.attribute(TYPE);

        auto target = rnode_ole.attribute(TARGET);
        auto type = rnode_ole.attribute(TYPE);

        this->new_doc->AddRelationship(target_img.value(), type_img.value(), ole_img_rid);
        this->new_doc->AddRelationship(target.value(), type.value(), ole_rid);

        image_names.push_back(string(target_img.value()));
        image_names.push_back(string(target.value()));
    }
    else
    {
        std::cout << "very bad\n";
        exit(1);
    }
}

void Distilator::handle_run(xml_node run_node, Run &r)
{
    xml_node rpr = run_node.child(RUN_PROPERTY);
    if (rpr) {
        this->handle_run_properties(rpr, r);
    }
    for (xml_node node = run_node.first_child(); node; node = node.next_sibling())
    {
        if (strcmp(node.name(), TEXT) == 0) {
            r.AppendText(node.child_value());
        }
        else if (strcmp(node.name(), DRAWING) == 0)
        {
            this->handle_drawing(node);
            r.AppendDrawing(node); //maybe can do better
        }
        else if (strcmp(node.name(), OBJECT) == 0)
        {
            this->handle_object(node);
            r.AppendObject(node);
        }
    }
}

void Distilator::handle_run_properties(xml_node rpr, Run &r)
{
    for(auto child = rpr.first_child(); child; child=child.next_sibling())
    {
        auto name = child.name();
        if (strcmp(name, SIZE) == 0) {
            this->set_size_run(child, r);
        }
        this->set_text_formmating(child, r);
        if (strcmp(name, HIGHLIGHT) == 0) {
            this->set_highlight(child, r);
        }
        else if (strcmp(name, COLOR) == 0) {
            this->set_color(child, r);
        }
    }
    auto rstyle = rpr.child(RSTYLE);
    if (rstyle) {
        this->set_run_style(rstyle, r);
    }
}

//Sets the size of the run
void Distilator::set_size_run(xml_node sz, Run &r) {
    auto val = sz.attribute(VALUE);
    if (!val) {
        cout<<"File is Corrupted! SizeRun" << endl;
        return;
    }
    string ssize = val.value();
    if (!is_int(ssize)) {
        cout<<"File is Corrupted! size is not int" << endl;
        return;
    }
    int size = stoi(ssize);
    if (size <= 0) {
        cout<<"File is Corrupted! size is le 0" << endl;
        return;
    }
    r.SetFontSize((double)size/2.0);
}

//Sets the text formmating in run
void Distilator::set_text_formmating(xml_node text_style, Run &r)
{
    auto val = text_style.attribute(VALUE);
    if (strcmp(text_style.name(), BOLD) == 0) {
        if (val) {
            if (strcmp(val.value(), "false") == 0) {
                return;
            }
            if (strcmp(val.value(), "true") != 0) {
                cout<<"File is Corrupted! not a flag" << endl;
                return;
            }
        }
        r.SetFontStyle(Run::Bold | r.GetFontStyle());
        return;
    }
    if (strcmp(text_style.name(), ITALIC) == 0) {
        if (val) {
            if (strcmp(val.value(), "false") == 0) {
                return;
            }
            if (strcmp(val.value(), "true") != 0) {
                cout<<"File is Corrupted! not a flag" << endl;
                return;
            }
        }
        r.SetFontStyle(Run::Italic | r.GetFontStyle());
        return;
    }
    if (strcmp(text_style.name(), UNDERLINE) == 0) {
        if (val && find(UNDERLINE_VAL.begin(), UNDERLINE_VAL.end(), val.value()) == UNDERLINE_VAL.end()) {
            cout<<"File is Corrupted! not legitimate val" << endl;
            return;
        }
        r.SetFontStyle(Run::Underline | r.GetFontStyle(), val.value());
        return;
    }
}

//Sets the run style
void Distilator::set_run_style(xml_node rStyle, Run &r)
{
    auto val = rStyle.attribute(VALUE);
    if (!val) {
        cout<<"File is Corrupted! no val in rStyle" << endl;
        return;
    }
    auto style_node = this->get_style_node(val.value(), CHARACTER_STYLE_TYPE);
    if (style_node == this->styles_root) {
        cout<<"File is Corrupted! no style matching the " << val.value() << endl;
        return;
    }
    auto style_rpr = style_node.child(RUN_PROPERTY);
    this->handle_run_properties(style_rpr, r);
}

//Sets the highlight in run
void Distilator::set_highlight(xml_node highlight, Run &r)
{
    auto val = highlight.attribute(VALUE);
    if (!val || find(HIGHLIGHT_VAL.begin(), HIGHLIGHT_VAL.end(), val.value()) == HIGHLIGHT_VAL.end()) {
        cout<<"File is Corrupted! no val in highlight" << endl;
        return;
    }
    r.SetHighlight(val.value());
}

//Sets the color in run
void Distilator::set_color(xml_node color, Run &r)
{
    auto val = color.attribute(VALUE);
    if (!val) {
        cout<<"File is Corrupted! no val in color" << endl;
        return;
    }
    string color_str = val.value();
    if (color_str.size() < 6) {
        cout<<"File is Corrupted! color is too short" << endl;
        return;
    }
    while(color_str.size() > 6 && color_str[0] == '0')
        color_str.erase(color_str.begin());
    r.SetColor(color_str);
}

// writes the paragraph to table_text
void Distilator::handle_paragraph_in_table(xml_node table_box_paragraph, Paragraph &p)
{
    // iterate over runs in paragraph
    for (xml_node table_box_run = table_box_paragraph.child(RUN); 
    table_box_run; table_box_run = table_box_run.next_sibling(RUN))
    {
        // add text in run
        // this->table_text.append(table_box_run.child(TEXT).value());
        p.AppendRun(table_box_run.child(TEXT).child_value());
    }
 
}

// writes the table to table_text in csv format
void Distilator::handle_table(xml_node table_node, Table &t)
{
    Paragraph p;
    int row=0,col=0;
    // iterate over all rows in table
    for (xml_node table_row_node = table_node.child(TABLE_ROW); table_row_node;
     table_row_node = table_row_node.next_sibling(TABLE_ROW))
    {
        // iterate over all table columns in the row
        for (xml_node table_box = table_row_node.child(TABLE_COLUMN); 
        table_box; table_box = table_box.next_sibling(TABLE_COLUMN))
        {
            p = t.GetCell(row, col).FirstParagraph();
            // iterate over all paragraphs in the table cell
            for (xml_node table_box_paragraph = table_box.child(PARAGRAPH); 
            table_box_paragraph; table_box_paragraph = table_box_paragraph.next_sibling(PARAGRAPH))
            {
               // cout << "row,col:" << row << "," << col << endl;
                handle_paragraph_in_table(table_box_paragraph,p);
            }
            col++;
        }
        row++;
        col=0;
    }
}

// writes the number of rows and cols in table t to variables given
void Distilator::get_dimensions_table(xml_node t, int *rows, int *cols)
{
    int num_rows=0,num_cols=0;
        // iterate over all rows in table
    for (xml_node table_row_node = t.child(TABLE_ROW); table_row_node;
     table_row_node = table_row_node.next_sibling(TABLE_ROW))
    {
        // iterate over all table columns in the row
        for (xml_node table_box = table_row_node.child(TABLE_COLUMN); 
        table_box; table_box = table_box.next_sibling(TABLE_COLUMN))
        {
            // inc num cols only for the first row
            if (num_rows == 0)
            {
                num_cols++;
            }
        }
        num_rows++;
    }
    *rows = num_rows;
    *cols = num_cols;
}


void Distilator::handle_paragraph(xml_node paragraph_node, Paragraph &p)
{
    this->handle_paragraph_properties(paragraph_node, p);
    for (xml_node child = paragraph_node.first_child(); child; child = child.next_sibling())
    {
        if (strcmp(child.name(), RUN) == 0) {
            auto r = p.AppendRun();
            this->handle_run(child, r);
        }
        else if (strcmp(child.name(), HYPERLINK) == 0) {
            auto h = p.AppendHyperlink();
            this->handle_hyperlink(child, h);
        }
        else if (strcmp(child.name(), MATH_PARAGRAPH) == 0){
            p.appendMath(child);
        }
    }
}


/*
    Docx Functions
*/

void Distilator::init_docx()
{
    /*
        Open document.xmnl
    */

    auto doc_path = this->original_folder + "/word/document.xml";
    xml_parse_result res = this->doc.load_file(doc_path.c_str()); 
    if (!res) {
        cout<<"Error parsing document.xml"<<endl;
        return;
    }
    this->doc_root = this->doc.first_child();
    cout<<"parsed document.xml"<<endl;

    /*
        Open relations
    */
    auto rels_path = this->original_folder + "/word/_rels/document.xml.rels";
    res = this->rels.load_file(rels_path.c_str());
    if (!res) {
        cout<<"Error parsing relations.xml"<<endl;
        return;
    }
    this->rels_root = this->rels.first_child();    
    cout<<"parsed relations.xml"<<endl;

    /*
        Open styles
    */
    auto styles_path = this->original_folder + "/word/styles.xml";
    res = this->styles.load_file(styles_path.c_str());
    if (!res) {
        cout<<"Error parsing styles.xml"<<endl;
        return;
    }
    this->styles_root = this->styles.first_child();
    cout<<"parsed styles.xml"<<endl;

    /*
        New File Initialization
    */
    this->new_doc = new Document(this->new_file);
    this->level_counters[0] = 1;
    this->list_level = NOT_IN_LIST;
    cout<< "Distilator Initialized\n";
}

void Distilator::distil_docx()
{
    xml_node body_node = this->doc_root.child("w:body");
    if (body_node.empty()) {
        cout<<"File is Corrupted! on body node" << endl;
        return;
    }
    int rows,cols;
    for(xml_node child = body_node.first_child(); child; child=child.next_sibling())
    {
        if (strcmp(child.name(), PARAGRAPH) == 0) {
            auto p = this->new_doc->AppendParagraph();
            this->handle_paragraph(child, p);
        }
        else if (strcmp(child.name(), TABLE) == 0) {
            get_dimensions_table(child,&rows,&cols);
            auto t = this->new_doc->AppendTable(rows,cols);
            this->handle_table(child, t);
        }
    }   
}