"""
    Because the frontend is a subprocess of the C++ backend and communicate via
    pipes (or more specific of STDIN and STDOUT) EVERY print to STDOUT will 
    resolve into a message to the backend
"""
import sys
import json


ASK_DATA            =   100
ADD_RESPONSE        =   101
RM_RESPONSE         =   102
EDIT_RESPONSE       =   103
SEND_RESPONSE       =   200
TRN_END             =   300
SEND_ID             =   301
WRG_FORMAT          =   400
DB_ERROR            =   500

RESPONSE_CODE_SIZE  =  3 + 1


def send_request_no_data(response_code : str):
    """
        This method is used when you just want to send simple codes to the
        backend e.g. when you want to fetch the data from the database to the
        frontend.
    """
    # print(response_code, flush=True)
    send_request_with_data(response_code, {})


def send_request_with_data(response_code : int, data : dict):
    """
        This function is used when you want to send data to the backend.
        When sending data you need to use a response code to determine
        the use of the sending data.

        All sended data must have the format:
            <RESPONSE_CODE>.<DATA>
    """
    data['CODE'] = response_code
    data = json.dumps(data)
    print(data, flush=True)


def listen_to_backend():
    from_parent = sys.stdin.readline().rstrip()
    return from_parent
