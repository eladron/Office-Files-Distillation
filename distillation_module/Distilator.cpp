#include "Distilator.hpp"
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include "Tags.h"

using namespace rapidxml;
using namespace std;

/*
    Auxilleris function
*/

void printing_children(xml_node<>* node)
{
    for (xml_node<>* child = node->first_node(); child; child = child->next_sibling())
        cout<< child->name() << endl;
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
    delete(new_doc);
    string command = "rm -r " + this->original_folder;
    system(command.c_str());
}

xml_node<>* Distilator::get_relation_node(string relation)
{
    for (auto rel = this->rels_root->first_node(); rel; rel=rel->next_sibling())
    {
        xml_attribute<>* id = rel->first_attribute("Id");
        if (id && relation.compare(id->value()) == 0)
            return rel;
    }
    return nullptr;
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

void Distilator::handle_list(xml_node<>* pPr_node)
{
    xml_node<>* numPr_node = pPr_node->first_node(NUM_PROPERTY);
    if (numPr_node != 0)
    {
        xml_node<>* ilvl_node = numPr_node->first_node(LEVEL);
        if (ilvl_node != 0)
        {
            xml_attribute<>* val = ilvl_node->first_attribute(VALUE);
            if (val !=0)
            {    
                cout<<val->value()<<endl;
                try {
                    this->handle_levels(stoi(val->value()));
                    this->list_level = stoi(val->value());
                }
                catch (const invalid_argument& e)
                {
                    cout<< "Invalid list level" << endl;
                }
            }
        }
    }
}

void Distilator::handle_hyperlink(xml_node<>* hyperlink_node)
{
    auto id = hyperlink_node->first_attribute("r:id");
    if (id)
    {
        auto relation_node = this->get_relation_node(id->value());
        if (relation_node)
        {
            //this->file_text.append(relation_node->first_attribute("Target")->value());
        }
    }
}


void Distilator::handle_paragraph_properties(xml_node<>* paragraph_node, Paragraph &p)
{
    xml_node<>* pPr_node = paragraph_node->first_node(PARAGRAPH_PROPERTY);
    if (pPr_node == nullptr)
        return;
    this->set_paragraph_allignment(pPr_node, p);
    xml_node<>* pStyle_node = pPr_node->first_node(PARAGRAPH_STYLE);
    if (pStyle_node != 0)
    {
        xml_attribute<>* att = pStyle_node->first_attribute(VALUE);
        if (att != 0 && strcmp(att->value(), LIST_PARAGRAPH) == 0)
            this->handle_list(pPr_node);
    }
}

void Distilator::set_paragraph_allignment(xml_node<>* pPr_node, Paragraph &p)
{
    xml_node<>* jc_node = pPr_node->first_node(JUSTIFICATION);
    if (jc_node != 0)
    {
        xml_attribute<>* att = jc_node->first_attribute(VALUE);
        if (att != 0)
        {
            if (strcmp(att->value(), "start") == 0 || strcmp(att->value(), "left") == 0)
                p.SetAlignment(Paragraph::Alignment::Left);
            else if (strcmp(att->value(), "end") == 0 || strcmp(att->value(), "right") == 0)
                p.SetAlignment(Paragraph::Alignment::Right);
            else if (strcmp(att->value(), "center") == 0)
                p.SetAlignment(Paragraph::Alignment::Centered);
            else if (strcmp(att->value(), "both") == 0)
                p.SetAlignment(Paragraph::Alignment::Justified);
            else if (strcmp(att->value(), "distribute") == 0)
                p.SetAlignment(Paragraph::Alignment::Distributed);
            else {
                cout << "Justification Error: File is Corrupted" << endl;
            }
        }
    }
}

void Distilator::handle_drawing(xml_node<>* drawing_node)
{
    xml_node<>* node = drawing_node;

    node = drawing_node->first_node(INLINE);
    node = (node) ? node : drawing_node->first_node(ANCHOR);

    for(const auto & name : IMAGE_PATH)
    {
        node = node->first_node(name);
        if(!node)
        {
            std::cout << "very bad 1!!!\n";
            exit(1);
        }
    }
    xml_attribute<>* att = node->first_attribute(EMBED);
    if(!att)
    {
        std::cout << "very bad 2!!!\n";
        exit(1);
    }
    std::string rid= att->value();
    printing_children(this->rels_root);
    auto relation_node = this->get_relation_node(rid);
    if (relation_node)
    {
        /*
        std::string img_name = relation_node->first_attribute("Target")->value();

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


void Distilator::handle_run(xml_node<>* run_node, Run &r)
{
    xml_node<>* rpr = run_node->first_node(RUN_PROPERTY);
    if (rpr) {
        this->handle_run_properties(rpr, r);
    }
    for (xml_node<>* node = run_node->first_node(); node; node = node->next_sibling())
        if (strcmp(node->name(), TEXT) == 0)
            r.AppendText(node->value());
        else if (strcmp(node->name(), DRAWING) == 0)
            this->handle_drawing(node);
}

void Distilator::handle_run_properties(xml_node<>* rpr, Run &r)
{
    for(auto child = rpr->first_node(); child; child=child->next_sibling())
    {
        char* name = child->name();
        if(strcmp(name, SIZE) == 0) {
            this->set_size_run(child, r);
        }
        this->set_text_formmating(child, r);
    }
}

//Sets the size of the run
void Distilator::set_size_run(xml_node<>* sz, Run &r) {
    auto val = sz->first_attribute(VALUE);
    if (!val) {
        cout<<"File is Corrupted!" << endl;
        return;
    }
    string ssize = val->value();
    if (!is_int(ssize)) {
        cout<<"File is Corrupted!" << endl;
        return;
    }
    int size = stoi(ssize);
    if (size <= 0) {
        cout<<"File is Corrupted!" << endl;
        return;
    }
    r.SetFontSize((double)size/2.0);
}

//Sets the text formmating in run
void Distilator::set_text_formmating(xml_node<>* text_style, Run &r)
{
    auto val = text_style->first_attribute(VALUE);
    if (strcmp(text_style->name(), BOLD) == 0) {
        if (val) {
            if (strcmp(val->value(), "false") == 0) {
                return;
            }
            if (strcmp(val->value(), "true") != 0) {
                cout<<"File is Corrupted!" << endl;
                return;
            }
        }
        r.SetFontStyle(Run::Bold | r.GetFontStyle());
        return;
    }
    if (strcmp(text_style->name(), ITALIC) == 0) {
        if (val) {
            if (strcmp(val->value(), "false") == 0) {
                return;
            }
            if (strcmp(val->value(), "true") != 0) {
                cout<<"File is Corrupted!" << endl;
                return;
            }
        }
        r.SetFontStyle(Run::Italic | r.GetFontStyle());
        return;
    }
    if (strcmp(text_style->name(), UNDERLINE) == 0) {
        if (val && find(UNDERLINE_VAL.begin(), UNDERLINE_VAL.end(), val->value()) == UNDERLINE_VAL.end()) {
            cout<<"File is Corrupted!" << endl;
            return;
        }
        r.SetFontStyle(Run::Underline | r.GetFontStyle(), val->value());
        return;
    }
}

// writes the paragraph to table_text
void Distilator::handle_paragraph_in_table(xml_node<>* table_box_paragraph)
{
    // iterate over runs in paragraph
    for (xml_node<>* table_box_run = table_box_paragraph->first_node(RUN); 
    table_box_run; table_box_run = table_box_run->next_sibling(RUN))
    {
        // add text in run
        this->table_text.append(table_box_run->first_node(TEXT)->value());
    }
 
}

// writes the table to table_text in csv format
void Distilator::handle_table(xml_node<>* table_node)
{
    // iterate over all rows in table
    for (xml_node<>* table_row_node = table_node->first_node(TABLE_ROW); table_row_node; table_row_node = table_row_node->next_sibling(TABLE_ROW))
    {
        // iterate over all table columns in the row
        for (xml_node<>* table_box = table_row_node->first_node(TABLE_COLUMN); 
        table_box; table_box = table_box->next_sibling(TABLE_COLUMN))
        {
            // iterate over all paragraphs in the table cell
            for (xml_node<>* table_box_paragraph = table_box->first_node(PARAGRAPH); 
            table_box_paragraph; table_box_paragraph = table_box_paragraph->next_sibling(PARAGRAPH))
            {
                handle_paragraph_in_table(table_box_paragraph);
            }
            // add ',' if box is not the last in the row
            if (table_box->next_sibling(TABLE_COLUMN))
                this->table_text.append(",");
        }
        this->table_text.append("\n"); 
    }
}

void Distilator::handle_paragraph(xml_node<>* paragraph_node, Paragraph &p)
{
    this->handle_paragraph_properties(paragraph_node, p);
    for (xml_node<>* child = paragraph_node->first_node(); child; child = child->next_sibling())
    {
        if (strcmp(child->name(), RUN) == 0) {
            auto r = p.AppendRun();
            this->handle_run(child, r);
        }
        else if (strcmp(child->name(), HYPERLINK) == 0)
            this->handle_hyperlink(child);
    }
}

void Distilator::extract_table(xml_node<>* table_node, int num)
{
    
    // create new 'csv' file for table, named - table1.csv, table2.csv,...
    //std::string file_name = this->zip_path + "table"+std::to_string(num)+".csv";
    //std::ofstream table_file(file_name);
    // handle the table - writes to table_text
    handle_table(table_node);

    // write table_text to the file
    //table_file << this->table_text;
    //table_file.close();
}

/*
    Docx Functions
*/

void Distilator::init_docx()
{
    /*
        Open document.xmnl
    */

    cout<<this->original_folder + "/word/document.xml" << endl;
    fstream doc_file(this->original_folder + "/word/document.xml");
    vector<char> buffer((istreambuf_iterator<char>(doc_file)), istreambuf_iterator<char>( ));
    buffer.push_back('\0');
    this->doc.parse<0>(&buffer[0]); 
    this->doc_root = this->doc.first_node();
    doc_file.close();

    cout<<"parsed document.xml"<<endl;

    /*
        Open relations
    */
    fstream rels_file(this->original_folder + "/word/_rels/document.xml.rels");
    vector<char> rels_buffer((istreambuf_iterator<char>(rels_file)), istreambuf_iterator<char>( ));
    rels_buffer.push_back('\0');
    this->rels.parse<0>(&rels_buffer[0]); 
    this->rels_root = this->rels.first_node();
    rels_file.close();
    cout<<"parsed relations.xml"<<endl;

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
    xml_node<> * body_node = this->doc_root->first_node("w:body");
    if (body_node == NULL) {
        cout<<"File is Corrupted!" << endl;
        return;
    }
    int i=0;
    for(xml_node<>* child = body_node->first_node(); child; child=child->next_sibling())
    {
        if (strcmp(child->name(), PARAGRAPH) == 0) {
            auto p = this->new_doc->AppendParagraph();
            this->handle_paragraph(child, p);
        }
        else if (strcmp(child->name(), TABLE) == 0)
            this->extract_table(child, i++);
    }   
}