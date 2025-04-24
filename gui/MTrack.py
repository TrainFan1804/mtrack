import sys
import json
import tkinter as tk
from tkinter import ttk

import msgsender
import config.responsecodes as rsp
import config.mediasection as ms


def place_window_mid(window, width, height):
    # get screen width and height# 
    screen_width = window.winfo_screenwidth()
    screen_height = window.winfo_screenheight()
    
    # # calculate position x and y coordinates
    x = (screen_width / 2) - (width / 2)
    y = (screen_height / 2) - (height / 2)
    window.geometry('%dx%d+%d+%d' % (width, height, x, y))


class MTrack(tk.Tk):
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        place_window_mid(self, 500, 400)

        tk.Label(text="mTrack",
                    height=3).pack()

        self.tree = ttk.Treeview(
            selectmode='browse',
            show='headings', 
            columns=ms.NAME_LIST
        )
        
        for name in ms.NAME_LIST:
            self.tree.heading(name, text=name)
        self.tree.pack()


        btn_frame = tk.Frame()

        tk.Button(
            btn_frame, text="add", 
            command=self._add_item
        ).pack(side=tk.LEFT)

        tk.Button(
            btn_frame, text="remove", 
            command=self._rm_item
        ).pack(side=tk.LEFT)

        tk.Button(
            btn_frame, text="quit", 
            command=self.destroy
        ).pack(side=tk.LEFT)

        btn_frame.pack()


    def _add_item(self):
        AddTopLevel(self, self._receive_data_callback)


    def _receive_data_callback(self, values):
        j_str = json.dumps(values)
        msgsender.send_request_with_data(rsp.ADD_RESPONSE, j_str)

        rsp_code = sys.stdin.readline().rstrip()
        if rsp_code.startswith(rsp.SEND_ID):
            rsp_id = rsp_code[ms.RESPONSE_CODE_SIZE:]
            self.tree.insert('', 'end', 
                iid=str(rsp_id), 
                values=[values[key] for key in ms.NAME_LIST]
            )


    def _rm_item(self):
        """
            Find the id of the selected item and send the correct response code
            and id to the backend.
        """
        cur_item_id = self.tree.focus()
        msgsender.send_request_with_data(rsp.RM_RESPONSE, cur_item_id)
        rsp_code = sys.stdin.readline().rstrip()
        if rsp_code == rsp.TRN_END:
            selectd_item = self.tree.selection()[0]
            self.tree.delete(selectd_item)


class AddTopLevel(tk.Toplevel):
    def __init__(self, parent, callback):
        tk.Toplevel.__init__(self, parent)
        place_window_mid(self, 400, 300)

        self.callback = callback

        tk.Label(self, text="Add a new media", height=3).pack()
        self._build_entries()

        btn_frame = tk.Frame(self)

        tk.Button(
            btn_frame, 
            text="add", 
            command=self._send_data
        ).pack(side=tk.LEFT)

        tk.Button(
            btn_frame, 
            text="close", 
            command=self.destroy
        ).pack(side=tk.LEFT)

        btn_frame.pack()


    def _build_entries(self):
        """
            NOTE: When adding a new attribute to the database, a new label and 
            frame for the toplevel window need to be added.
        """
        self.entry_frame = tk.Frame(self)
        for _row in range(len(ms.NAME_LIST)):
            tk.Label(
                self.entry_frame, 
                text=ms.NAME_LIST[_row]
            ).grid(row=_row, column=0)

            tk.Entry(
                self.entry_frame
            ).grid(row=_row, column=1)

        self.entry_frame.pack()


    def _send_data(self):
        data = {}
        count = 0
        for child in self.entry_frame.winfo_children():
            if (isinstance(child, tk.Entry)):
                cur_entry_data = child.get().strip()
                if len(cur_entry_data) != 0:
                    data[ms.NAME_LIST[count]] = cur_entry_data
                    count += 1

        if len(data) != 2:
            return
        self.callback(data)
        self.destroy()
