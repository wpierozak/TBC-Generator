#include<string>
#include<sstream>
#include<unordered_map>
#include<iterator>
#include"CM_datatypes.hpp"
#include"CM_ui.hpp"
#include"CM_xmlparser.hpp"
#include"CM_postproc.hpp"
#include"CM_generationOMP.hpp"

const std::string RUN = "run";
const std::string CONFIG = "config";
const std::string CONFIG_LOAD = "load";
const std::string CONFIG_CLEAR = "clear";
const std::string EXPORT = "export";
const std::string TO_BMP = "tobmp";
const std::string TO_MS_FILE = "totxt";

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
    cmd_map.emplace(EXPORT, exports);
    cmd_map.emplace(CONFIG, config);
    cmd_map.emplace(RUN, run);
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

void exports(vec_string& arg, GeneratorConfig** config)
{
    if(*config == nullptr) throw cm_error("Configuration is not specified", "export");
    if(arg.size() == 1) throw cm_error("Invalid argument", "export");

    vec_string args;
    for(int i = 1; i < arg.size(); i++) args.push_back(arg[i]);

    if(args[0] == TO_BMP) tobmp(args, config);
    else if(args[0] == TO_MS_FILE) totxt(args, config);
    else throw cm_error("Invalid argument", "export");
}

void tobmp(vec_string& arg, GeneratorConfig** config)
{
    if(*config == nullptr) throw cm_error("Configuration is not specified", "tobmp");
    if(arg.size() != 1) throw cm_error("Invalid argument", "tobmp");
    createBitmap(*(*config), (*config)->getThreadsNumber());
}

void totxt(vec_string& arg, GeneratorConfig** config)
{
    if(*config == nullptr) throw cm_error("Configuration is not specified", "totxt");
    if(arg.size() != 1) throw cm_error("Invalid argument", "totxt");
    saveMicrostructureFile(**config);
}

void config(vec_string& arg, GeneratorConfig** config)
{
    if(arg.size() == 1 && *config) (*config)->printConfiguration();
    else if(arg.size() == 1) throw cm_error("Configuration is not specified", "config");

    vec_string args;
    for(int i = 1; i < arg.size(); i++) args.push_back(arg[i]);

    if(arg[1] == CONFIG_LOAD) load(args, config);
    else if(arg[1] == CONFIG_CLEAR) clear(args, config);
    else throw cm_error("Invalid argument", "config");
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
