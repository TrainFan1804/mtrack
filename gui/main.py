import sys
import json

import MTrack
import msgsender
import responsecodes as rsp


def fill_window_tree():
    j_str = sys.stdin.readline()
    j_str = j_str[4:]

    j_data = json.loads(j_str)
    for row in j_data:
        window.tree.insert('', 'end', 
                        iid=str(row[1]), 
                        values=(row[3], row[5]))


if (__name__ == "__main__"):
    window = MTrack.MTrack()

    # sending request for the database data
    msgsender.send_request_no_data(rsp.ASK_DATA)
    fill_window_tree()
    window.mainloop()
