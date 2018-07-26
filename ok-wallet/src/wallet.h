#pragma once

#include <string>
#include <list>


extern "C"
__attribute__ ((visibility("default")))
void execute(const std::string& _nettype, const std::string& args, std::list<std::string>& result);
