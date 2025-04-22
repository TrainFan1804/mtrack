import sys
import threading
import tkinter as tk
from tkinter import ttk


def sendMSG():
    """
        Because this script is a subprocess of the C++ backend and communicate via
        pipes (or more specific of STDIN and STDOUT) EVERY print to STDOUT will 
        resolve into a message to the backend
        TODO Adding one global file where every command is listed so I can use them
        here and in the backend
    """
    print("button_test", flush=True)


def listen_to_stdin():
    """
        NOTE: All incoming messages from the backend need to contain '\n' a the
        end of the message or the input will be blocked (bcs 'readline()' is used)
    """
    test_connection_label.config(text="Init thread")
    while True:
        line = sys.stdin.readline()
        print("[PY DEBUG]:", repr(line), file=sys.stderr)
        if not line:
            break
        test_connection_label.config(text=f"C++ sagt: {line.strip()}")


def center_window(width=300, height=200):
    # get screen width and height
    screen_width = window.winfo_screenwidth()
    screen_height = window.winfo_screenheight()

    # calculate position x and y coordinates
    x = (screen_width/2) - (width/2)
    y = (screen_height/2) - (height/2)
    window.geometry('%dx%d+%d+%d' % (width, height, x, y))


if (__name__ == "__main__"):
    window = tk.Tk()
    center_window(500, 400)

    test_connection_label = tk.Label(text="Nothing right now", background="red")
    test_connection_label.pack()
    test_connection_label.config(text=f"First change")

    tk.Label(text="mTrack",
                width=15,
                height=5).pack()

    tree = ttk.Treeview(show='headings', columns=('name', 'rating'))
    tree.column('name', width=100, anchor='center')
    tree.heading('name', text='name')
    tree.heading('rating', text='rating')
    tree.pack()

    tree.insert('', 'end', text='DUMMY', values=('DUMMY', 'DUMMY'))

    btn = tk.Button(text="Add", 
                    command=sendMSG).pack()

    thread = threading.Thread(target=listen_to_stdin, daemon=True)
    thread.start()

    window.mainloop()
