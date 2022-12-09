#include "rapidxml.hpp"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>


using namespace rapidxml;
using namespace std;

int main(){
    ifstream myfile("./tmp/word/document.xml");
    if (!myfile)
    {
        cout <<"shit" << endl;
        return 1;
    }
    cout<< "im not fucked!?!" << endl;
    //std::ofstream out("output.txt");
    xml_document<> doc;
    cout<< "im not fucked!?" << endl;

    /* "Read file into vector<char>"  See linked thread above*/
    vector<char> buffer((istreambuf_iterator<char>(myfile)), istreambuf_iterator<char>( ));
    cout<< "im not fucked?" << endl;

    buffer.push_back('\0');
    cout<< "im fucked";
    doc.parse<0>(&buffer[0]); 
    xml_node<> * root_node = doc.first_node();
    xml_node<> * body_node = root_node->first_node("w:body");

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
    cout<<res;
}