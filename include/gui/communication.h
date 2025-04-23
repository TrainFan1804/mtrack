#ifndef COMMUNICATION_H 
#define COMMUNICATION_H

#include <string>

void readFromChild(int from_child[2]);

void sendMessageToChild(int to_child[2], const std::string &msg);

#endif
