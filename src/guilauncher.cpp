/*
    https://stackoverflow.com/questions/41495219/c-bidirectional-pipe-stuck-trying-to-read-from-child-process-in-a-loop
    
    This link helped me creating this abomination
*/
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>

#include "guilauncher.h"
#include "buildenv.h"

void launchGUI()
{
    // saving the pipes for the communication
    int parent_to_child[2]; // parent write, child read
    int child_to_parent[2]; // child write, parent read

    /*  
        TODO: Possible piping errors AREN'T handled! See 
        https://www.geeksforgeeks.org/pipe-system-call/
        for more information
    */
    
    // open pipe where PIPE[0] is read end and PIPE[1] is write end
    // Will this work on windows because it's a systemcall?!
    pipe(parent_to_child);
    pipe(child_to_parent);

    pid_t pid = fork();

    if (pid == 0) 
    {
        // start child process
        dup2(parent_to_child[0], STDIN_FILENO);     // change 'read' from parent to STDIN 
        /*
            Change 'write' from child to STDOUT
            This may cause trouble in the future everytime the child write
            something into the terminal!
        */
        dup2(child_to_parent[1], STDOUT_FILENO);    
        
        // close the OUTPUT/INPUT pipe from the parent/child
        close(parent_to_child[1]);
        close(child_to_parent[0]);

        // starting the subprocess
        execlp(std::string(PYTHON_PATH).c_str(), "python3", std::string(GUI_PY).c_str(), nullptr);
        perror("Something went wrong trying open the gui.\n");
        exit(1);
    }
    else
    {
        // close the INPUT/OUTPUT pipe from the parent/child
        close(parent_to_child[0]);
        close(child_to_parent[1]);

        char buffer[128];
        while (true) 
        {
            ssize_t count = read(child_to_parent[0], buffer, sizeof(buffer) - 1);
            if (count <= 0) break;

            buffer[count] = '\0';
            std::string cmd(buffer);
            if (cmd.find("button_test") != std::string::npos) 
            {
                std::cout << "[C++] Button was pressen in GUI!\n";
            }
            else
            {
                std::cout << "[C++] Unknown message from GUI: " << cmd;
            }
        }
        wait(nullptr);
    }
}
