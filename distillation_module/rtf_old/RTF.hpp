#ifndef RTF_H
#define RTF_H

#include <string>
#include <vector>

#define DEFAULT_FONT ""
#define BOLD 1
#define ITALIC 2
#define UNDERLINE 4

using namespace std;

class RTFile
{
    private:
        vector<string> font_table;
        vector<string> paragraphs;



    public:
        RTFile() = default;
        ~RTFile() = default;
        void generate_file(string file_name);
        void add_font(string font);
        string generate_header();
        string generate_fonttbl();
        string generate_font(string font);

        void new_paragraph();        
        void close_paragraph();
        void add_text(string text, string font, int size, int flags);
        void print_fontbl();

};




#endif /* RTF_H */