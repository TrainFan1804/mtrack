def send_msg(msg_code : str):
    """
        Because this script is a subprocess of the C++ backend and communicate via
        pipes (or more specific of STDIN and STDOUT) EVERY print to STDOUT will 
        resolve into a message to the backend
        TODO Adding one global file where every command is listed so I can use them
        here and in the backend
    """
    print(msg_code, flush=True)
