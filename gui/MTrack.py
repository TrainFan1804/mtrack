import tkinter as tk

from custom import utils
from custom import MainFrame as mf
from custom import EditFrame as ef


class MTrack(tk.Tk):
    TREE_COL_LIST = ['name', 'state', 'type', 'rating']
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        utils.place_window_mid(self, 800, 400)

        self.columnconfigure(0, weight=1)
        self.columnconfigure(1, weight=1, minsize=300)
        self.rowconfigure(0, weight=1)

        self.minsize(height=325, width=400)

        self.edit_frame = ef.EditFrame(self)
        self.edit_frame.grid(row=0, column=1, sticky="nsew")

        self.main_frame = mf.MainFrame(self, self.edit_frame.fill_selected_item)
        self.main_frame.grid(row=0, column=0, sticky="nsew")

        self.edit_frame.set_tree(self.main_frame.tree) 
