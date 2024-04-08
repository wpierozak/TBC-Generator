#pragma once
#include<sstream>
#include<vector>
#include<iostream>

class  GeneratorConfig;
typedef std::vector<std::string> vec_string;
typedef void(*ui_cmd)(vec_string&, Configuration**);

class cm_error
{
    public:
    cm_error() = delete;
    cm_error(std::string mess, std::string source);

    std::string mess() const;
    std::string source() const;

    private:
    std::string _message;
    std::string _source;
};

template<typename T>
void msg_header(std::string pre, T arg, std::string post)
{
    std::cout << "\t===\t" << pre << arg << post << "\t===" << std::endl;
}

void init_ui();
void interpreter(const std::string& command, Configuration** config);
void load(vec_string& arg, Configuration** config);
void tobmp(vec_string& arg, Configuration** config);
void totxt(vec_string& arg, Configuration** config);
void config(vec_string& arg, Configuration** config);
void clear(vec_string& arg, Configuration** config);
void rungen(vec_string& arg, Configuration** config);
void exports(vec_string& arg, Configuration** config);

template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}