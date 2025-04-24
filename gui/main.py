import sys
import json

import MTrack
import msgsender
import config.responsecodes as rsp
import config.mediasection as ms


def fill_window_tree():
    j_str = sys.stdin.readline()
    j_str = j_str[rsp.RESPONSE_CODE_SIZE:]   # remove response code

    j_dir = json.loads(j_str)
    for row in j_dir:
        window.tree.insert('', 'end', 
            iid=str(row[ms.JSON_ID]), 
            values=[row[key] for key in ms.NAME_LIST]
        )


if (__name__ == "__main__"):
    window = MTrack.MTrack()

    # sending request for the database data
    msgsender.send_request_no_data(rsp.ASK_DATA)
    fill_window_tree()
    window.mainloop()
