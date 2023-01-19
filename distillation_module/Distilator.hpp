#ifndef DISTILATOR_H_
#define DISTILATOR_H_

#include "pugixml.hpp"
#include <string>
#include <map>
#include "minidocx.hpp"

using namespace pugi;
using namespace std;
using namespace docx;



class Distilator
{
    private:
    /*
        File Names
    */
        string new_file;
        string file_name;
        string original_folder;

    /*
        XML Attributes
    */
        xml_document doc;
        xml_node doc_root;
        xml_document rels;
        xml_node rels_root;
    /*
        Docx Properties
    */
        vector<string> image_names;
        map<int, int> level_counters;
        int list_level;
        string table_text;
        char dmz[1000]; //without this, your segfault
    
    /*
        New File Objects
    */
        Document* new_doc;


    /*
        Zip Operations
    */
        void unzip_file();
        void zip_file();
    /*
        Paragraph Operations
    */
        void handle_paragraph(xml_node paragraph_node, Paragraph &p);
        void handle_paragraph_properties(xml_node paragraph_node, Paragraph &p);
        void set_paragraph_allignment(xml_node pPr_node, Paragraph &p);
    /*
        Run Operations
    */
        void handle_run(xml_node run_node, Run &r);
        void handle_run_properties(xml_node rpr, Run &r);
        void set_size_run(xml_node sz, Run &r);
        void set_text_formmating(xml_node text_style, Run &r);
        void set_run_style(xml_node rStyle, Run &r);
        void set_highlight(xml_node highlight, Run &r);
    

        void extract_table(xml_node table_node, int num);
        void handle_table(xml_node table_node);
        void handle_text(xml_node text_node);
        void handle_drawing(xml_node drawing_node);
        void handle_list(xml_node pPr_node);
        void handle_levels(int level);
        void handle_hyperlink(xml_node hyperlink_node, Hyperlink &h);
        void handle_paragraph_in_table(xml_node table_box_paragraph);
        void print_levels_counters();
        xml_node get_relation_node(string relation);

    public:
        Distilator(char* file_name, char* path_to_zip);
        void init_docx();
        void init_pptx();
        void init_xslx();
        ~Distilator();
        void distil_docx();
        void distil_pptx();
        void distil_xslx();
};


#endif /* DISTILATOR_H_ */