#ifndef DISTILATOR_H_
#define DISTILATOR_H_

#include "rapidxml.hpp"
#include <string>
#include <map>
#include "RTF.hpp"

using namespace rapidxml;
using namespace std;

class Distilator
{
    private:
        xml_document<> doc;
        xml_node<>* doc_root;
        xml_document<> rels;
        xml_node<>* rels_root;
        string file_name;
        map<int, int> level_counters;
        int list_level;
        string file_text;
        string table_text;
        string path;
        RTFile* rtf;


        string unzip_file();
        void zip_file();
        void extract_features();
        void extract_paragraph(xml_node<>* paragraph_node);
        void extract_table(xml_node<>* table_node, int num);
        void handle_table(xml_node<>* table_node);
        void handle_text(xml_node<>* text_node);
        void handle_drawing(xml_node<>* drawing_node);
        void handle_paragraph_properties(xml_node<>* paragraph_node);
        void handle_list(xml_node<>* pPr_node);
        void handle_levels(int level);
        void handle_run_node(xml_node<>* run_node);
        void handle_hyperlink_node(xml_node<>* hyperlink_node);
        void handle_paragraph_in_table(xml_node<>* table_box_paragraph);
        void print_levels_counters();
        xml_node<>* get_relation_node(string relation);
        void build_fonttbl();

    public:
        Distilator(char* file_name, char* path_to_zip);
        ~Distilator();
        void distill();
};


#endif /* DISTILATOR_H_ */