#ifndef DISTILATOR_H_
#define DISTILATOR_H_

#include "rapidxml.hpp"
#include <string>
using namespace rapidxml;
using namespace std;

class Distilator
{
    private:
        xml_document<> doc;
        xml_node<>* root;
        string file_name;

        string unzip_file();
        void extract_text();

    public:
        Distilator(char* file_name);
        ~Distilator();
        void distill();
};


#endif /* DISTILATOR_H_ */