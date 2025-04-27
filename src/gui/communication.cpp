#include <iostream>
#include <unistd.h>
#include <stdexcept>
#include <sys/wait.h>

#include "gui/communication.h"
#include "debug/debprint.h"
#include "databasemanager.h"
#include "globals.h"

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
        // remove all new lines
        msg_from_child.erase(
            std::remove(msg_from_child.begin(), msg_from_child.end(), '\n'), 
            msg_from_child.end()
        );

        debug::print::fdebprint("Received message from frontend: %s", msg_from_child.c_str());
        
        auto temp2 = msg_from_child.substr(RESPONSE_CODE_SIZE);
        nlohmann::json response = nlohmann::json::parse(msg_from_child);
        int response_code = response["CODE"];

        switch (response_code)
        {
        case ASK_DATA:
            commands::fetchCommand(to_child);
            break;
        case ADD_RESPONSE:
            commands::addCommand(to_child, response);
            break;
        case RM_RESPONSE:
            commands::rmCommand(to_child, response);
            break;
        case EDIT_RESPONSE:
            commands::editCommand(to_child, response);
            break;
        default:
            debug::print::fdeberr(
                "Unknown message from GUI: %s",
                msg_from_child.c_str()
            );
            break;
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
    debug::print::debprint("GUI want to fetch data.");
    auto json_data = selectAllJsonQuery();
    json_data.push_back({{"CODE", SEND_RESPONSE}});
    
    communication::sendMessageToChild(to_child, json_data.dump());
}

void communication::commands::addCommand(int to_child[2], const nlohmann::json &j)
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

    communication::sendMessageToChild(to_child, j_arr.dump());
}

void communication::commands::rmCommand(int to_child[2], const nlohmann::json &j)
{
    debug::print::debprint("GUI want to remove data");

    rmMedia(j["id"]);
    nlohmann::json j_data = nlohmann::json::array();
    j_data.push_back({
        {"CODE", TRN_END},
    });
    communication::sendMessageToChild(to_child, j_data.dump());
}

void communication::commands::editCommand(int to_child[2], const nlohmann::json &j)
{
    debug::print::debprint("GUI want to edit data");

    media::Media edit_media(j);
    editMedia(j["id"], edit_media);
    nlohmann::json j_data = nlohmann::json::array();
    j_data.push_back({
        {"CODE", TRN_END},
    });
    communication::sendMessageToChild(to_child, j_data.dump());
}
