#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H

#include <string>
#include <vector>

namespace po    // po := program options 
{
    void parse(int argc, char *argv[]);
}

/**
 * Contains wrapper function for sql functions. Those functions are
 * specific designed for the CLI only!
 */
namespace po::commands
{
    void showCommand();

    void addCommand(const std::vector<std::string> &args);

    void rmCommand(int id);

    void editCommand(int edit_id, const std::vector<std::string> &args);
}

#endif
