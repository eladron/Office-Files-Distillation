#include "distilator.hpp"
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "tags.h"

using namespace rapidxml;
using namespace std;

string Distilator::unzip_file()
{
    string output_dir = this->file_name.substr(0, this->file_name.length()-5); 
    string command = "yes | unzip " + this->file_name + " -d " + output_dir;
    system(command.c_str());
    return output_dir;
}

Distilator::Distilator(char* file_name)
{
    this->file_name = string(file_name);
    string output_dir = unzip_file();
    fstream doc_file("./" + output_dir + "/word/document.xml");
    vector<char> buffer((istreambuf_iterator<char>(doc_file)), istreambuf_iterator<char>( ));
    buffer.push_back('\0');
    this->doc.parse<0>(&buffer[0]); 
    this->doc_root = this->doc.first_node();

    fstream rels_file("./" + output_dir + "/word/_rels/document.xml.rels");
    vector<char> rels_buffer((istreambuf_iterator<char>(rels_file)), istreambuf_iterator<char>( ));
    rels_buffer.push_back('\0');
    this->rels.parse<0>(&rels_buffer[0]); 
    this->rels_root = this->rels.first_node();

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

void printing_childs(xml_node<>* node)
{
    for (xml_node<>* child = node->first_node(); child; child = child->next_sibling())
        cout<< child->name() << endl;
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
}

void Distilator::handle_run_node(xml_node<>* run_node)
{
    for (xml_node<>* text_node = run_node->first_node(TEXT); text_node; text_node = text_node->next_sibling(TEXT))
        handle_text(text_node);
}

void Distilator::extract_text()
{
    xml_node<> * body_node = this->doc_root->first_node("w:body");
    std::ofstream out(this->file_name.substr(0, this->file_name.length()-5) + ".txt");
    for(xml_node<>* paragraph_node = body_node->first_node(PARAGRAPH); paragraph_node; paragraph_node=paragraph_node->next_sibling(PARAGRAPH))
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
    this->file_text.pop_back();
}

void Distilator::distill()
{
    extract_text();
}

Distilator::~Distilator()
{
    std::ofstream out(this->file_name.substr(0, this->file_name.length()-5) + ".txt");
    out<<this->file_text;
    system("rm -r tmp");
}