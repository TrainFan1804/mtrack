import tkinter as tk

from custom import utils
from custom import MainFrame as mf
from custom import EditFrame as ef
from custom import FilterFrame as ff


class MTrack(tk.Tk):
    TREE_COL_LIST = ['name', 'state', 'type', 'rating']
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        utils.place_window_mid(self, 800, 400)

        self.columnconfigure(0, weight=1)
        self.columnconfigure(1, weight=1)
        self.columnconfigure(2, weight=1, minsize=300)
        self.rowconfigure(0, weight=1)

        self.minsize(height=325, width=400)

        self.edit_frame = ef.EditFrame(self)
        self.edit_frame.grid(row=0, column=2, sticky='nsew')

        self.main_frame = mf.MainFrame(self, self.edit_frame.fill_selected_item)
        self.main_frame.grid(row=0, column=1, sticky='nsew')

        tk.Button(
            self, 
            text="Open Search", 
            command=self._show_filter_options
        ).grid(row=1, column=1)

        self.filter_frame = ff.FilterFrame(self)

        self.edit_frame.set_tree(self.main_frame.tree) 
        self.filter_frame.set_tree(self.main_frame.tree)

    
    def _show_filter_options(self):
        if self.filter_frame.winfo_manager():
            self.filter_frame.grid_forget()
        else:
            self.filter_frame.grid(row=0, column=0, sticky='nsew')
