import socket
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

_SOCKET_NAME = 'mtrack_socket'


def send_request(response_code: int):
    """
        This method is used when you just want to send simple codes to the
        backend e.g. when you want to fetch the data from the database to the
        frontend.
    """
    return send_request_with_data(response_code, {})


def send_request_with_data(response_code : int, data : dict):
    """
        This function is used when you want to send data to the backend.
        When sending data you need to use a response code to determine
        the use of the sending data.
    """
    client = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    client.connect(_SOCKET_NAME)

    data['CODE'] = response_code

    client.sendall(json.dumps(data).encode())    
    r = client.recv(1024)
    client.close()
    return r.decode()
