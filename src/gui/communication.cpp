#include <iostream>
#include <unistd.h>
#include <stdexcept>
#include <sys/wait.h>

#include "gui/communication.h"
#include "buildenv.h"
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
        msg_from_child.erase(
            std::remove(msg_from_child.begin(), msg_from_child.end(), '\n'), 
            msg_from_child.end());

        debug::print::fdebprint("Received message from frontend: %s", msg_from_child.c_str());

        if (msg_from_child.rfind(ASK_DATA, 0) == 0)
        {
            debug::print::debprint("GUI want to fetch data.");
            commands::fetchCommand(to_child);
        }
        else if (msg_from_child.rfind(ADD_RESPONSE, 0) == 0) 
        {
            debug::print::debprint("GUI want to add data");

            auto temp = msg_from_child.substr(
                RESPONSE_CODE_SIZE, 
                msg_from_child.length()
            );
            nlohmann::json j = nlohmann::json::parse(temp);

            commands::addCommand(to_child, j);
        }
        else if (msg_from_child.rfind(RM_RESPONSE, 0) == 0) 
        {
            debug::print::debprint("GUI want to remove data");
            try
            {
                int id_int = std::stoi(
                    msg_from_child.substr(
                        RESPONSE_CODE_SIZE, 
                        msg_from_child.length()
                    )
                );
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
            debug::print::fdeberr("Unknown message from GUI: %s", msg_from_child);
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
    auto json_data = selectAllJsonQuery();
    
    std::string prefix = std::string(SEND_RESPONSE) + ".";
    std::string j_str = prefix + json_data.dump();

    communication::sendMessageToChild(to_child, j_str);
}

void communication::commands::addCommand(int to_child[2], const nlohmann::json &j)
{
    /*
        This loop will extract the integer typses from the given json (j).
        This is needed because in the db some columns are INTEGER but the json
        is a string type. So there need to be a list of all INTEGER columns (int_cols)
        and cast the represent values in the json to int to add the new media correctly.
    */
    std::vector<std::string> int_cols = { TABLE_INT_COL };
    std::vector<int> int_cols_val;
    for (const auto &col : int_cols)
    {
        auto temp = j[col].get<std::string>();
        int_cols_val.push_back(std::stoi(temp));
    }
    
    // TODO add here the params when there a new media section
    media::Media new_media(j["name"], int_cols_val[0]);
    addMedia(new_media);

    std::vector<std::vector<std::string>> select_result;
    selectSpecialQuery(select_result, "SELECT MAX(ID) FROM MEDIA;");

    std::string msg = std::string(SEND_ID) + "." + select_result[0][1];
    communication::sendMessageToChild(to_child, msg);
}

void communication::commands::rmCommand(int to_child[2], int id)
{
    rmMedia(id);
    communication::sendMessageToChild(to_child, TRN_END);
}
