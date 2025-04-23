#ifndef COMMUNICATION_H 
#define COMMUNICATION_H

#include <string>
#include <vector>

namespace communication
{
    /**
     * This functioon contains a loop that will listen to the pipe that is leading
     * to the child process (python gui). When the frontend send a response code
     * this function will determine what kind of message it is and start the command
     * that is connected to the response code.
     */
    void waitForChildResponse(int from_child[2], int to_child[2]);

    /**
        NOTE: All incoming messages from the backend need to contain '\n' a the
        end of the message or the input will be blocked (bcs 'readline()' is used)
    */
    void sendMessageToChild(int to_child[2], const std::string &msg);
}

/**
 * Contains wrapper function for sql functions. Those functions are
 * specific designed for the GUI only!
 */
namespace communication::commands
{
    void fetchCommand(int to_child[2]);

    void addCommand(const std::vector<std::string> &args);

    void rmCommand(int to_child[2], int id);
}

#endif
