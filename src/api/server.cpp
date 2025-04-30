#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <csignal>
#include <cstdlib>
#include <sys/select.h>

#include "api/server.h"
#include "databasemanager.h"
#include "debug/debprint.h"
#include "globals.h"

namespace
{
    bool running = true;
    int server_fd = -1;

    void stopServer(int)
    {
        running = false;
    }
}

void server::run() 
{
    openDatabase();
    signal(SIGINT, stopServer);

    startServer();

    debug::print::debprint("Server is listening for incoming connections...");

    while (running) 
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);

        timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int ready = select(server_fd + 1, &readfds, nullptr, nullptr, &timeout);

        if (ready == -1) 
        {
            perror("select");
            break;
        } 
        else if (ready == 0) 
        {
            continue;
        }

        if (FD_ISSET(server_fd, &readfds)) 
        {
            int client_fd = accept(server_fd, nullptr, nullptr);
            if (client_fd == -1) 
            {
                perror("accept");
                continue;
            }
            debug::print::debprint("Client connected");
            runClient(client_fd);
            close(client_fd);
            debug::print::debprint("Client disconnected");
        }
    }
    unlink(SOCKET_PATH);
    close(server_fd);
    closeDatabase();
    debug::print::debprint("Socket cleaned up");
    debug::print::debprint("Server shutdown successfully");
}

void server::startServer()
{
    sockaddr_un addr{};

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    // create new socket
    if (server_fd == -1)
    {
        perror("socket error");
        return;
    }
    unlink(SOCKET_PATH);

    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) != 0)
    {
        perror("bind error");
        return;
    }

    if (listen(server_fd, 5) != 0) 
    {
        perror("listen error");
        return;
    }
}

void server::interruptServer()
{
    stopServer(0);
}

void server::runClient(int client_fd)
{
    char msg_buffer[1024];
    while (true) 
    {
        ssize_t len = read(client_fd, msg_buffer, sizeof(msg_buffer) - 1);
        if (len <= 0) break;

        msg_buffer[len] = '\0';
        debug::print::fdebprint("Received: %s", msg_buffer);
        auto client_response = nlohmann::json::parse(msg_buffer);
        int response_code = client_response["CODE"];

        std::string server_response;
        switch (response_code)
        {
        case ASK_DATA:
            server_response = commands::fetchCommand();
            break;
        case ADD_RESPONSE:
            server_response = commands::addCommand(client_response);
            break;
        case RM_RESPONSE:
            server_response = commands::rmCommand(client_response);
            break;
        case EDIT_RESPONSE:
            server_response = commands::editCommand(client_response);
            break;
        default:
            debug::print::fdeberr(
                "Unknown message from GUI: %s",
                msg_buffer
            );
            break;
        }
        sendMessage(client_fd, server_response);
    }
}

void server::sendMessage(int client_fd, const std::string &msg)
{
    auto temp = std::move(msg);
    temp += "\n";
    int ret = write(client_fd, temp.c_str(), temp.size());
    if (ret <= 0)
        throw std::runtime_error("Error writing a message to the frontend!");
    debug::print::fdebprint("Send message to child: %s", msg.c_str());
}

std::string server::commands::fetchCommand()
{
    debug::print::debprint("GUI want to fetch data.");
    auto json_data = selectAllJsonQuery();
    json_data.push_back({{"CODE", SEND_RESPONSE}});
    
    return json_data.dump();
}

std::string server::commands::addCommand(const nlohmann::json &j)
{
    debug::print::debprint("GUI want to add data");

    media::Media new_media(j);
    addMedia(new_media);

    nlohmann::json j_arr = nlohmann::json::array();
    auto new_id = selectJsonQuery("SELECT MAX(ID) FROM MEDIA;");
    j_arr.push_back({
        {"CODE", SEND_ID},
        {"id", new_id}
    });
    return j_arr.dump();
}

std::string server::commands::rmCommand(const nlohmann::json &j)
{
    debug::print::debprint("GUI want to remove data");

    rmMedia(j["id"]);
    nlohmann::json j_data = nlohmann::json::array();
    j_data.push_back({
        {"CODE", TRN_END},
    });
    return j_data.dump();
}

std::string server::commands::editCommand(const nlohmann::json &j)
{
    debug::print::debprint("GUI want to edit data");

    media::Media edit_media(j);
    editMedia(j["id"], edit_media);
    nlohmann::json j_data = nlohmann::json::array();
    j_data.push_back({
        {"CODE", TRN_END},
    });
    return j_data.dump();
}
