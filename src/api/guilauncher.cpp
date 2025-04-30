#include <iostream>
#include <unistd.h>
#include <thread>

#include "api/guilauncher.h"
#include "api/server.h"
#include "buildenv.h"
#include <sys/wait.h>

void startChild()
{
    // starting the subprocess
    execlp(std::string(PYTHON_PATH).c_str(), "python3", std::string(GUI_PY).c_str(), nullptr);
    perror("execlp failed to launch the GUI.\n");
    exit(1);
}

void startParent()
{
    server::run();
}

void launchGUI()
{
    pid_t pid = fork();
    if (pid == -1)
        throw std::runtime_error("Something went wrong while trying open the gui");

    if (pid == 0) 
    {
        startChild();
    }
    else
    {
        // this will make sure to close the server when the gui is closed
        std::thread server_thread([] {
            server::run();
        });
        int status = 0;
        waitpid(pid, &status, 0);
        server::interruptServer();
        server_thread.join();
    }
}
