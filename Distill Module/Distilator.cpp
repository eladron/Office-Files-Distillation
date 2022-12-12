#include "Distilator.hpp"
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "Tags.h"

using namespace rapidxml;
using namespace std;


void printing_childs(xml_node<>* node)
{
    for (xml_node<>* child = node->first_node(); child; child = child->next_sibling())
        cout<< child->name() << endl;
}

/*
    RTF operations

*/

void Distilator::build_fonttbl()
{
    string output_dir = this->file_name.substr(0, this->file_name.length()-5);
    fstream font_file("./" + output_dir + "/word/fontTable.xml");
    vector<char> buffer_font((istreambuf_iterator<char>(font_file)), istreambuf_iterator<char>( ));
    buffer_font.push_back('\0');
    xml_document<> font_table_doc;
    font_table_doc.parse<0>(&buffer_font[0]); 
    xml_node<>* font_root = font_table_doc.first_node();
    for (auto font_node = font_root->first_node(); font_node; font_node=font_node->next_sibling())
    {
        xml_attribute<>* name = font_node->first_attribute(NAME);
        if (name)
        {
            rtf->add_font(name->value());
        }
    }
    font_file.close();
}

/* 
    zip operations
*/

void Distilator::zip_file()
{
    int start = this->path.find_last_of("/");
    string tmp = this->path.substr(0, start);
    start = tmp.find_last_of("/") == -1? 0 : tmp.find_last_of("/");
    string zip_name = tmp.substr(start);
    this->path.pop_back();
    string command = "zip " + zip_name + " -rm -j " + this->path;
    system(command.c_str());
    command = "rm -r " + this->path;
    system(command.c_str());
    this->path.push_back('/');

}

string Distilator::unzip_file()
{
    string output_dir = this->file_name.substr(0, this->file_name.length()-5); 
    string command = "yes | unzip " + this->file_name + " -d " + output_dir;
    system(command.c_str());
    return output_dir;
}

/*
    Distilator operations
*/

Distilator::Distilator(char* file_name, char* path_to_zip)
{
    this->file_name = string(file_name);
    this->path = string(path_to_zip);
    this->path = this->path.substr(0, this->path.find_last_of(".")) ;
    string command = "mkdir " + this->path;
    system(command.c_str());
    this->path.append("/");
    
    this->output_dir = unzip_file();
    this->rtf = new RTFile();
    this->build_fonttbl();

    fstream doc_file("./" + this->output_dir + "/word/document.xml");
    vector<char> buffer((istreambuf_iterator<char>(doc_file)), istreambuf_iterator<char>( ));
    buffer.push_back('\0');
    this->doc.parse<0>(&buffer[0]); 
    this->doc_root = this->doc.first_node();
    doc_file.close();

    fstream rels_file("./" + this->output_dir + "/word/_rels/document.xml.rels");
    vector<char> rels_buffer((istreambuf_iterator<char>(rels_file)), istreambuf_iterator<char>( ));
    rels_buffer.push_back('\0');
    this->rels.parse<0>(&rels_buffer[0]); 
    this->rels_root = this->rels.first_node();
    rels_file.close();

    this->level_counters[0] = 1;
    this->list_level = NOT_IN_LIST;
    
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

void Distilator::handle_hyperlink_node(xml_node<>* hyperlink_node)
{
    auto id = hyperlink_node->first_attribute("r:id");
    if (id)
    {
        auto relation_node = this->get_relation_node(id->value());
        if (relation_node)
        {
            this->file_text.append(relation_node->first_attribute("Target")->value());
        }
    }
}


void Distilator::handle_paragraph_properties(xml_node<>* paragraph_node)
{
    xml_node<>* pPr_node = paragraph_node->first_node(PARAGRAPH_PROPERTY);
    if (pPr_node == nullptr)
        return;
    xml_node<>* pStyle_node = pPr_node->first_node(PARAGRAPH_STYLE);
    if (pStyle_node != 0)
    {
        xml_attribute<>* att = pStyle_node->first_attribute(VALUE);
        if (att != 0 && strcmp(att->value(), LIST_PARAGRAPH) == 0)
            this->handle_list(pPr_node);
    }
}

void Distilator::handle_text(xml_node<>* text_node)
{
    string node_string = string(text_node->value());
    if (this->list_level != NOT_IN_LIST)
    {
        auto level = this->level_counters.find(this->list_level);
        string tabs(level->first,'\t');
        this->file_text.append(tabs);
        this->file_text.append(to_string(level->second++) + " ");
        this->list_level = NOT_IN_LIST;

    }
    if (node_string.empty())
        this->file_text.append(" ");
    else
        this->file_text.append(node_string);
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
    auto relation_node = this->get_relation_node(rid);
    if (relation_node)
    {
        std::string img_name = relation_node->first_attribute("Target")->value();

        string command = "cp " + this->output_dir + "/word/" + img_name + " " + this->path + img_name.substr(6);
        system(command.c_str());

        this->file_text.append(std::string("### ") + img_name.substr(6) + " ###\n");
    }
    else
    {
        std::cout << "very bad 3!!!\n";
        exit(1);
    }
}

void Distilator::handle_run_node(xml_node<>* run_node)
{
    for (xml_node<>* node = run_node->first_node(); node; node = node->next_sibling())
        if (strcmp(node->name(), TEXT) == 0)
            handle_text(node);
        else if (strcmp(node->name(), DRAWING) == 0)
            handle_drawing(node);
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

void Distilator::extract_paragraph(xml_node<>* paragraph_node)
{

    this->handle_paragraph_properties(paragraph_node);
    for (xml_node<>* child = paragraph_node->first_node(); child; child = child->next_sibling())
    {
        if (strcmp(child->name(), RUN) == 0)
            this->handle_run_node(child);
        else if (strcmp(child->name(), HYPERLINK) == 0)
            this->handle_hyperlink_node(child);
    }
    this->file_text.append("\n");
}

void Distilator::extract_table(xml_node<>* table_node, int num)
{
    
    // create new 'csv' file for table, named - table1.csv, table2.csv,...
    std::string file_name = this->path + "table"+std::to_string(num)+".csv";
    std::ofstream table_file(file_name);
    // handle the table - writes to table_text
    handle_table(table_node);

    // write table_text to the file
    table_file << this->table_text;
    table_file.close();
    this->table_text = "";
}

void Distilator::extract_features()
{
    xml_node<> * body_node = this->doc_root->first_node("w:body");
    int i=0;
    for(xml_node<>* child = body_node->first_node(); child; child=child->next_sibling())
    {
        if (strcmp(child->name(), PARAGRAPH) == 0)
            this->extract_paragraph(child);
        else if (strcmp(child->name(), TABLE) == 0)
            this->extract_table(child, i++);
    }
    this->file_text.pop_back();

}

void Distilator::distill()
{
    extract_features();
}

Distilator::~Distilator()
{
    string clean_file_name = this->file_name.substr(0, this->file_name.length()-5);
    std::string name = this->path + clean_file_name;
    std::ofstream out(name + ".txt");
    out<<this->file_text;
    out.close();
    this->zip_file();
    string command = "rm -r " + clean_file_name;
    system(command.c_str());
    delete this->rtf;
}