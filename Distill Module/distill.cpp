#include "Distilator.hpp"
#include <iostream>

using namespace std;

int main(int argc , char** argv)
{
    if(argc != 2){
        cout << "Usage: " << argv[0] << " <Office filename>" << endl;
        return 1;
    }

    Distilator* d = new Distilator(argv[1]);
    d->distill();
    delete d;
}