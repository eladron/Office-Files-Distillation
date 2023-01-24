#include "Distilator.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(int argc , char** argv)
{
    if(argc != 3){
        cout << "Usage: " << argv[0] << " <Office filename> <Path to zip file>" << endl;
        return 1;
    }


    Distilator* d = new Distilator(argv[1], argv[2]);
    d->init_docx();
    d->distil_docx();
    delete d;
    return 0;
}