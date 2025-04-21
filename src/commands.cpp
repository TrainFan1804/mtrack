#include "commands.h"
#include "databasemanager.h"
#include "globals.h"

void showCommand()
{
    showMedia();
}

void addCommand(const std::vector<std::string> &args)
{
    media::Media new_media;
    new_media._name = args[0];
    new_media._rating = std::stoi(args[1]);
    addMedia(new_media);
}

void rmCommand(int id)
{
    rmMedia(id);
}
