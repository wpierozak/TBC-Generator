#include<iostream>
#include"CM_datatypes.hpp"
#include"CM_generationOMP.hpp"
#include"CM_xmlparser.hpp"
#include"CM_postproc.hpp"

int main()
{
    Domain* d0 = parseConfiguration("/home/wiktor/Desktop/engineering_thesis/xml/test_config.xml");
    d0->printConfiguration();
    generate(*d0, 12);
    std::cout<<"Out of generation\n";
    fillColorBuffer(*d0, 12);
    createBitmap(*d0);
    delete d0;
    return 0;
}
