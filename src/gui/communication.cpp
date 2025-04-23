#include <iostream>
#include <unistd.h>
#include <stdexcept>
#include <sys/wait.h>

#include "gui/communication.h"
#include "buildenv.h"

void readFromChild(int from_child[2])
{
    char buffer[128];
    while (true) 
    {
        ssize_t count = read(from_child[0], buffer, sizeof(buffer) - 1);
        if (count <= 0) break;

        buffer[count] = '\0';
        std::string msg_from_child(buffer);

        if (msg_from_child.find(ASK_DATA) != std::string::npos)
        {
            std::cout << "[C++] Frontend asked for data\n";
        }
        else if (msg_from_child.find(ADD_RESPONSE) != std::string::npos) 
        {
            std::cout << "[C++] add_btn was pressen in GUI!\n";
        }
        else if (msg_from_child.find(RM_RESPONSE) != std::string::npos) 
        {
            std::cout << "[C++] rm_btn was pressen in GUI!\n";
        }
        else
        {
            std::cout << "[C++] Unknown message from GUI: " << msg_from_child; 
        }
    }
    wait(nullptr);
}

void sendMessageToChild(int to_child[2], const std::string &msg)
{
    int ret = write(to_child[1], msg.c_str(), msg.size());
    if (ret <= 0)
        throw std::runtime_error("Error writing a message to the frontend!");
}
