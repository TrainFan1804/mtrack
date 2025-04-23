import sys
import threading

import MTrack


def listen_to_stdin():
    """
        NOTE: All incoming messages from the backend need to contain '\n' a the
        end of the message or the input will be blocked (bcs 'readline()' is used)
    """
    window.test_connection_label.config(text="Init thread")
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

    window.mainloop()
