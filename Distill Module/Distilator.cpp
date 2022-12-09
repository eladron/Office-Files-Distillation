#include "distilator.hpp"
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

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
    fstream myfile("./" + output_dir + "/word/document.xml");
    vector<char> buffer((istreambuf_iterator<char>(myfile)), istreambuf_iterator<char>( ));
    buffer.push_back('\0');
    this->doc.parse<0>(&buffer[0]); 
    this->root = this->doc.first_node();
}

void Distilator::extract_text()
{
    xml_node<> * body_node = root->first_node("w:body");
    std::ofstream out(this->file_name.substr(0, this->file_name.length()-5) + ".txt");
    
    string paragraph = "w:p";
    string run = "w:r";
    string text = "w:t";
    string res;
    for(xml_node<>* paragraph_node = body_node->first_node(paragraph.c_str()); paragraph_node; paragraph_node=paragraph_node->next_sibling(paragraph.c_str()))
    {
        for (xml_node<>* run_node = paragraph_node->first_node(run.c_str()); run_node; run_node = run_node->next_sibling(run.c_str()))
        {
            for (xml_node<>* text_node = run_node->first_node(text.c_str()); text_node; text_node = text_node->next_sibling(text.c_str()))
            {
                res.append(text_node->value());
            }
        }
        res.append("\n");
    }
    res.pop_back();
    out<<res;
}

void Distilator::distill()
{
    extract_text();
}

Distilator::~Distilator()
{
    system("rm -r tmp");
}