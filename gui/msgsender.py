ASK_DATA        =   "100"
ADD_RESPONSE    =   "101"
RM_RESPONSE     =   "102"
SEND_RESPONSE   =   "200"
TRN_END         =   "300"
SEND_ID         =   "301"
WRG_FORMAT      =   "400"
DB_ERROR        =   "500"

RESPONSE_CODE_SIZE  =   3 + 1

"""
    Because this script is a subprocess of the C++ backend and communicate via
    pipes (or more specific of STDIN and STDOUT) EVERY print to STDOUT will 
    resolve into a message to the backend
"""
def send_request_no_data(response_code : str):
    """
        This method is used when you just want to send simple codes to the
        backend e.g. when you want to fetch the data from the database to the
        frontend.
    """
    print(response_code, flush=True)


def send_request_with_data(response_code : int, data : str):
    """
        This function is used when you want to send data to the backend.
        When sending data you need to use a response code to determine
        the use of the sending data.

        All sended data must have the format:
            <RESPONSE_CODE>.<DATA>
    """
    data = str(response_code) + "." + data 
    print(data, flush=True)
