import sys
import threading

import MTrack
import msgsender
import responsecodes as rsp


def listen_to_stdin():
    """
        NOTE: All incoming messages from the backend need to contain '\n' a the
        end of the message or the input will be blocked (bcs 'readline()' is used)
    """
    window.test_connection_label.config(text="Init thread")

    # in the real application is test_data the data from the db that is sended from the backend
    test_data = [
        ('1', 'Test1', 5),
        ('2', 'Test2', 5),
        ('3', 'Test3', 5),
    ]
    for entry_id, name, rating in test_data:
        window.tree.insert('', 'end', iid=entry_id, values=(name, rating))

    while True:
        line = sys.stdin.readline()
        print("[PY DEBUG]:", repr(line), file=sys.stderr)
        if not line:
            break
        window.test_connection_label.config(text=f"C++ sagt: {line.strip()}")


if (__name__ == "__main__"):
    window = MTrack.MTrack()

    thread = threading.Thread(target=listen_to_stdin, daemon=True)
    thread.start()

    # sending request for the database data
    msgsender.send_msg(rsp.ASK_DATA)
    window.mainloop()
