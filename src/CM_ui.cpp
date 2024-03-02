#include<string>
#include<sstream>
#include<unordered_map>
#include<iterator>
#include"CM_ui.hpp"
#include"CM_xmlparser.hpp"
#include"CM_postproc.hpp"
#include"CM_generationOMP.hpp"

std::unordered_map<std::string, ui_cmd> cmd_map;

cm_error::cm_error(std::string mess, std::string source)
{
    _message = mess;
    _source = source;
}

std::string cm_error::mess() const { return _message; }
std::string cm_error::source() const { return _source; }

void init_ui()
{
    cmd_map.emplace("load", load);
    cmd_map.emplace("tobmp", tobmp);
    cmd_map.emplace("config", config);
    cmd_map.emplace("clear", config);
    cmd_map.emplace("run", run);
}

void interpreter(const std::string& input, GeneratorConfig** config)
{
    std::stringstream stream(input);
    vec_string command;
    split(input, ' ', std::back_inserter(command));
    auto cmd = cmd_map.find(command[0]);
    if(cmd == cmd_map.end()) throw cm_error("Invalid command", "interpreter");
    cmd->second(command, config);
}

void load(vec_string& arg, GeneratorConfig** config)
{
    if(arg.size() == 1) throw cm_error("Configuration file was not specified", "load");
    if(*config != nullptr) throw cm_error("Configuration need to be explicitly cleared", "load");
    *config = parseConfiguration(arg[1]);
}

void tobmp(vec_string& arg, GeneratorConfig** config)
{
    if(*config == nullptr) throw cm_error("Configuration is not specified", "tobmp");
    if(arg.size() != 1) throw cm_error("Invalid argument", "tobmp");
    createBitmap(*(*config), (*config)->getThreadsNumber());
}

void config(vec_string& arg, GeneratorConfig** config)
{
    if(*config == nullptr) throw cm_error("Configuration is not specified", "config");
    if(arg.size() != 1) throw cm_error("Invalid argument", "config");
    (*config)->printConfiguration();
}

void clear(vec_string& arg, GeneratorConfig** config)
{
    if(*config == nullptr) throw cm_error("Configuration is already cleared", "clear");
    if(arg.size() != 1) throw cm_error("Invalid argument", "clear");
    delete *config;
}

void run(vec_string& arg, GeneratorConfig** config)
{
    if(*config == nullptr) throw cm_error("Configuration is not specified", "run");
    if(arg.size() == 1) throw cm_error("Invalid argument", "run");

    if(arg[1] == "-omp") generate(*(*config), (*config)->getThreadsNumber());
}
