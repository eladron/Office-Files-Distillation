#include "RTF.hpp"
#include <string>
#include <iostream>
#include <fstream>


using namespace std;

string RTFile::generate_fonttbl()
{
    string ft = "{\\fonttbl ";
    int i=0;
    for (auto f : this->font_table)
    {
        ft.append("{\\f" + to_string(i++) + " " + f + ";}");
    }
    return ft.append("}");
}

string RTFile::generate_header()
{
    string header;
    header.append("\\rtf1\\ansi");

    header.append("\\deff0 ");

    header.append(this->generate_fonttbl());
    return header;
}

void RTFile::add_font(string font)
{
    this->font_table.push_back(font);
}


void RTFile::new_paragraph()
{
    this->paragraphs.push_back("{\\pard");
}

void RTFile::close_paragraph()
{
    this->paragraphs.back().append(" \\par}");    
}

void RTFile::add_text(string text, string font = "", int size = 0, int flags = 0)
{
    auto last_par = this->paragraphs.back();
    last_par.append(" {");
    if ((flags & BOLD) == BOLD)
        last_par.append(" \\b");
    if ((flags & ITALIC) == ITALIC)
        last_par.append(" \\i");
    last_par.append(" " + text);
    last_par.append(" }");  
    this->paragraphs.pop_back();
    this->paragraphs.push_back(last_par);
}


void RTFile::generate_file(string file_name)
{
    ofstream out(file_name);
    out<<"{";
    out<<this->generate_header();
    out<<"\n\\fs34";
    for (auto p : this->paragraphs)
    {
        out<<"\n" + p;
    }
    out<<"}";
    out.close();
}

void RTFile::print_fontbl() {cout<< this->generate_fonttbl()<<endl;};

/*
int main()
{
    RTFile* rtf = new RTFile();
    rtf->add_font("Ariel");
    rtf->add_font("Calibri");
    rtf->new_paragraph();
    rtf->add_text("elad");
    rtf->close_paragraph();
    rtf->generate_file("test_elad_ron.rtf");
    cout<<rtf->generate_header()<<endl;
}
*/