import sys
import tkinter as tk
from tkinter import ttk

import msgsender
import responsecodes as rsp


class MTrack(tk.Tk):
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        self._place_window_mid(500, 400)

        self.test_connection_label = tk.Label(text="Nothing right now", background="red")
        self.test_connection_label.pack()

        tk.Label(text="mTrack",
                    width=15,
                    height=5).pack()

        self.tree = ttk.Treeview(selectmode='browse', show='headings', columns=('name', 'rating'))
        self.tree.heading('name', text='name')
        self.tree.heading('rating', text='rating')
        self.tree.pack()

        btn_frame = tk.Frame()

        tk.Button(btn_frame, text="add", 
                    command=self._add_button).pack(side=tk.LEFT)

        tk.Button(btn_frame, text="remove", 
                    command=self._rm_button).pack(side=tk.LEFT)

        tk.Button(btn_frame, text="quit", 
                        command=self.destroy).pack(side=tk.LEFT)
        btn_frame.pack()

    
    def _place_window_mid(self, width, height):
        # get screen width and height# 
        screen_width = self.winfo_screenwidth()
        screen_height = self.winfo_screenheight()
        
        # # calculate position x and y coordinates
        x = (screen_width / 2) - (width / 2)
        y = (screen_height / 2) - (height / 2)
        self.geometry('%dx%d+%d+%d' % (width, height, x, y))


    def _add_button(self):
        # TODO open seperate window and extract data from there?
        msgsender.send_msg(rsp.ADD_RESPONSE)


    def _rm_button(self):
        """
            Find the id of the selected item and send the correct response code
            and id to the backend.
        """
        cur_item_id = self.tree.focus()
        print("selected item id: " + cur_item_id, file=sys.stderr)
        msgsender.send_msg(rsp.RM_RESPONSE + cur_item_id)
