#include"CM_datatypes.hpp"
#include"CM_ui.hpp"

int main()
{
    Configuration* config = nullptr;
    bool run = true;
    std::string cmd;
    init_ui();
    while(run)
    {
        std::cout<<">";
        std::getline(std::cin, cmd);
        if(cmd == "quit") run = false;
        interpreter(cmd, &config);
    }
}