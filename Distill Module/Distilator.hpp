#ifndef DISTILATOR_H_
#define DISTILATOR_H_

#include "rapidxml.hpp"
#include <string>
#include <map>
using namespace rapidxml;
using namespace std;

class Distilator
{
    private:
        xml_document<> doc;
        xml_node<>* root;
        string file_name;
        map<int, int> level_counters;
        int list_level;
        string file_text;

        string unzip_file();
        void extract_text();
        void handle_text(xml_node<>* text_node);
        void handle_paragraph_properties(xml_node<>* paragraph_node);
        void handle_list(xml_node<>* pPr_node);
        void handle_levels(int level);
        void print_levels_counters();

    public:
        Distilator(char* file_name);
        ~Distilator();
        void distill();
};


#endif /* DISTILATOR_H_ */