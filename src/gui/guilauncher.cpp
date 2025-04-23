/*
    https://stackoverflow.com/questions/41495219/c-bidirectional-pipe-stuck-trying-to-read-from-child-process-in-a-loop
    
    This link helped me creating this abomination
*/
#include <iostream>
#include <unistd.h>

#include "gui/guilauncher.h"
#include "gui/communication.h"
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

    communication::waitForChildResponse(from_child, to_child);
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
