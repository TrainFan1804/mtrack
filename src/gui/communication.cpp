#include <iostream>
#include <unistd.h>
#include <stdexcept>
#include <sys/wait.h>

#include "gui/communication.h"
#include "buildenv.h"
#include "debug/debprint.h"
#include "databasemanager.h"
#include "json.hpp"

void communication::waitForChildResponse(int from_child[2], int to_child[2])
{
    openDatabase();
    char buffer[128];
    while (true) 
    {
        ssize_t count = read(from_child[0], buffer, sizeof(buffer) - 1);
        if (count <= 0) break;

        buffer[count] = '\0';
        std::string msg_from_child(buffer);

        if (msg_from_child.rfind(ASK_DATA, 0) == 0)
        {
            debug::print::debprint("GUI want to fetch data");
            commands::fetchCommand(to_child);
        }
        else if (msg_from_child.rfind(ADD_RESPONSE, 0) == 0) 
        {
            debug::print::debprint("GUI want to add data");
        }
        else if (msg_from_child.rfind(RM_RESPONSE, 0) == 0) 
        {
            debug::print::debprint("GUI want to remove data");
            try
            {
                int id_int = std::stoi(msg_from_child.substr(4, msg_from_child.length()));
                commands::rmCommand(to_child, id_int);
            }
            catch(const std::exception& e)
            {
                debug::print::fdeberr("Something went wrong while converting the data: %s", e.what());
                sendMessageToChild(to_child, WRG_FORMAT);
            }
        }
        else
        {
            debug::print::fdebprint("Unknown message from GUI: %s", msg_from_child);
        }
    }
    wait(nullptr);
    closeDatabase();
}

void communication::sendMessageToChild(int to_child[2], const std::string &msg)
{
    auto temp = std::move(msg);
    temp += "\n";
    int ret = write(to_child[1], temp.c_str(), temp.size());
    if (ret <= 0)
        throw std::runtime_error("Error writing a message to the frontend!");
    debug::print::fdebprint("Send message to child: %s", msg.c_str());
}

void communication::commands::fetchCommand(int to_child[2])
{
    std::vector<std::vector<std::string>> select_result;
    selectAllQuery(select_result);
    nlohmann::json j = select_result;
    
    std::string prefix = std::string(SEND_RESPONSE) + ".";
    std::string j_str = prefix + j.dump();

    communication::sendMessageToChild(to_child, j_str);
}

void communication::commands::addCommand(const std::vector<std::string> &args)
{

}

void communication::commands::rmCommand(int to_child[2], int id)
{
    rmMedia(id);
    communication::sendMessageToChild(to_child, TRN_END);
}
