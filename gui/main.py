import os
import sys
import tkinter as tk


def sendMSG():
    """
        Because this script is a subprocess of the C++ backend and communicate via
        pipes (or more specific of STDIN and STDOUT) EVERY print will resolve into
        a message to the backend
        TODO Adding one global file where every command is listed so I can use them
        here and in the backend
    """
    print("button_test", flush=True)


if (__name__ == "__main__"):
    # print("Aktuelles Arbeitsverzeichnis:", os.getcwd())
    # print("Python-Interpreter:", sys.executable)
    # print(sys.argv)

    tcl = tk.Tcl()
    # print(tcl.call("info", "patchlevel"))

    window = tk.Tk()
    label = tk.Label(text="Test label",
                    width=10,
                    height=10)
    label.pack()

    btn = tk.Button(text="Send to C++", 
                    command=sendMSG)
    btn.pack()

    window.mainloop()
