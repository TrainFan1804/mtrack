/*
    https://stackoverflow.com/questions/41495219/c-bidirectional-pipe-stuck-trying-to-read-from-child-process-in-a-loop
    
    This link helped me creating this abomination
*/
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>

#include "gui/guilauncher.h"
#include "buildenv.h"

void startChild(int to_child[2], int from_child[2])
{
    // start child process
    dup2(to_child[0], STDIN_FILENO);     // change 'read' from parent to STDIN 
    /*
        Change 'write' from child to STDOUT
        This may cause trouble in the future everytime the child write
        something into the terminal!
    */
    dup2(from_child[1], STDOUT_FILENO);    
    
    // close the OUTPUT/INPUT pipe from the parent/child
    close(to_child[1]);
    close(from_child[0]);

    // starting the subprocess
    execlp(std::string(PYTHON_PATH).c_str(), "python3", std::string(GUI_PY).c_str(), nullptr);
    perror("execlp failed to launch the GUI.\n");
    exit(1);
}

void startParent(int to_child[2], int from_child[2])
{
    // start parent process
    // close the INPUT/OUTPUT pipe from the parent/child
    close(to_child[0]);
    close(from_child[1]);

    std::string msg_to_child;
    msg_to_child = "First msg from C++\n";
    write(to_child[1], msg_to_child.c_str(), msg_to_child.size());

    char buffer[128];
    while (true) 
    {
        ssize_t count = read(from_child[0], buffer, sizeof(buffer) - 1);
        if (count <= 0) break;

        buffer[count] = '\0';
        std::string msg_from_child(buffer);

        if (msg_from_child.find(ADD_RESPONSE) != std::string::npos) 
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

void launchGUI()
{
    // saving the pipes for the communication
    int to_child[2]; // parent write, child read
    int from_child[2]; // child write, parent read

    /*  
        TODO: Possible piping errors AREN'T handled! See 
        https://www.geeksforgeeks.org/pipe-system-call/
        for more information
    */
    
    // open pipe where PIPE[0] is read end and PIPE[1] is write end
    // Will this work on windows because it's a systemcall?!
    pipe(to_child);
    pipe(from_child);

    pid_t pid = fork();
    if (pid == -1)
        throw std::runtime_error("Something went wrong while trying open the gui");

    if (pid == 0) 
    {
        startChild(to_child, from_child);
    }
    else
    {
        startParent(to_child, from_child);
    }
}
