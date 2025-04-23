import sys
import tkinter as tk
from tkinter import ttk

import msgsender
import msgcodes


class MTrack(tk.Tk):
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        self._place_window_mid(500, 400)

        self.test_connection_label = tk.Label(text="Nothing right now", background="red")
        self.test_connection_label.pack()
        self.test_connection_label.config(text=f"First change")

        tk.Label(text="mTrack",
                    width=15,
                    height=5).pack()

        self.tree = ttk.Treeview(show='headings', columns=('name', 'rating'))
        self.tree.heading('name', text='name')
        self.tree.heading('rating', text='rating')
        self.tree.pack()

        self.tree.insert('', 'end', text='DUMMY', values=('DUMMY', 'DUMMY'))

        btn_frame = tk.Frame()

        self.add_btn = tk.Button(btn_frame, text="add", 
                        command=self._add_button).pack(side=tk.LEFT)

        self.rm_btn = tk.Button(btn_frame, text="remove", 
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
        msgsender.send_msg(msgcodes.ADD_MSG)


    def _rm_button(self):
        cur_item = self.tree.focus()
        print(self.tree.item(cur_item), file=sys.stderr)
        msgsender.send_msg(msgcodes.RM_MSG)
