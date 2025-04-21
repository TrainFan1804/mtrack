import os
import sys
import tkinter as tk


if (__name__ == "__main__"):
    print("Aktuelles Arbeitsverzeichnis:", os.getcwd())
    print("Python-Interpreter:", sys.executable)
    tcl = tk.Tcl()
    print(tcl.call("info", "patchlevel"))

    window = tk.Tk()
    label = tk.Label(text="Python rocks!",
                    width=10,
                    height=10)
    label.pack()

    window.mainloop()
