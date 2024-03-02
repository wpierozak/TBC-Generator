#include<iostream>
#include"CM_ui.hpp"

int main()
{
    init_ui();
    std::string cmd;
    GeneratorConfig* config = nullptr;;

    while(true)
    {
        std::cout<<'>'; 
        std::getline(std::cin, cmd);
        if(cmd != "quit")
        {
            try
            {
                interpreter(cmd, &config);
            }
            catch(const cm_error& e)
            {
                std::cerr << e.source() << ": " << e.mess() << '\n';
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << "\n";
            }   
        }
        else break;
    }

    delete config;  
}