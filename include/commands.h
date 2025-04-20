#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <vector>

void showCommand();

void addCommand(const std::vector<std::string> &args);

void rmCommand(int id);

#endif
