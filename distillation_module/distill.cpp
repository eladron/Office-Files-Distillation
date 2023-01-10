#include "Distilator.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(int argc , char** argv)
{
    if(argc != 4){
        cout << "Usage: " << argv[0] << " <Office filename> <Path to zip file> <File Type>" << endl;
        return 1;
    }

    vector<string> types = {"docx", "pptx", "xslx"};
    string type = argv[3];

    Distilator* d = new Distilator(argv[1], argv[2]);
    if (type == types[0]) {
        d->init_docx();
        d->distil_docx();
    }
    else if (type == types[1]) {
        d->init_pptx();
        d->distil_pptx();
    }
    else if (type == types[2]) {
        d->init_xslx();
        d->distil_xslx();
    }
    else {
        delete d;
        cout << "Invalid file type" << endl;
        return 1;
    }
    delete d;
    return 0;
}