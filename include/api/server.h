#ifndef SERVER_H 
#define SERVER_H

#include <string>

#include "json.hpp"

namespace server
{
    void run();

    void startServer();

    /**
     * This function will force a shutdown of the server from the outside.
     */
    void interruptServer();

    void runClient(int client_fc);

    void sendMessage(int client_fd, const std::string &msg);
}

/**
 * Contains wrapper function for sql functions. Those functions are
 * specific designed for the GUI only!
 */
namespace server::commands
{
    std::string fetchCommand();

    std::string addCommand(const nlohmann::json &j);

    std::string rmCommand(const nlohmann::json &j);

    std::string editCommand(const nlohmann::json &j);
}

#endif
