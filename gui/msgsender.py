def send_msg(msg_code : str):
    """
        Because this script is a subprocess of the C++ backend and communicate via
        pipes (or more specific of STDIN and STDOUT) EVERY print to STDOUT will 
        resolve into a message to the backend
        All sended data must have the format:
            <RESPONSE_CODE>.<DATA>
        TODO Find a represent data format for the communication.
    """
    print(msg_code, flush=True)


def send_msg_with_rsp(msg_code : str, response_code : int):
    msg_code = str(response_code) + "." + msg_code
    print(msg_code, flush=True)
