import sys
import json
import tkinter as tk
from tkinter import ttk
from tkinter import messagebox

import msgsender as msgs


def place_window_mid(window, width, height):
    # get screen width and height# 
    screen_width = window.winfo_screenwidth()
    screen_height = window.winfo_screenheight()
    
    # # calculate position x and y coordinates
    x = (screen_width / 2) - (width / 2)
    y = (screen_height / 2) - (height / 2)
    window.geometry('%dx%d+%d+%d' % (width, height, x, y))


class MTrack(tk.Tk):
    TREE_COL_LIST = ['name', 'state', 'type', 'rating']
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        place_window_mid(self, 800, 400)

        tk.Label(text="mTrack",
                    height=3).pack()

        self.tree = ttk.Treeview(
            selectmode='browse',
            show='headings', 
            columns=MTrack.TREE_COL_LIST
        )
        
        for col in MTrack.TREE_COL_LIST:
            self.tree.heading(col, text=col)
        self.tree.pack()
        self._fill_window_tree()

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
        msgs.send_request_with_data(msgs.ADD_RESPONSE, j_str)

        rsp_code = sys.stdin.readline().rstrip()
        if rsp_code.startswith(msgs.SEND_ID):
            rsp_id = rsp_code[msgs.RESPONSE_CODE_SIZE:]
            self.tree.insert('', 'end', 
                iid=str(rsp_id), 
                values=[values[key] for key in MTrack.TREE_COL_LIST]
            )
        else:
            messagebox.showerror("Error", "Something went wrong")


    def _rm_item(self):
        """
            Find the id of the selected item and send the correct response code
            and id to the backend.
        """
        cur_item_id = self.tree.focus()
        msgs.send_request_with_data(msgs.RM_RESPONSE, cur_item_id)
        rsp_code = sys.stdin.readline().rstrip()
        if rsp_code == msgs.TRN_END:
            selectd_item = self.tree.selection()[0]
            self.tree.delete(selectd_item)
        else:
            messagebox.showerror("Error", "Something went wrong")


    def _fill_window_tree(self):
        # sending request for the database data
        msgs.send_request_no_data(msgs.ASK_DATA)
        j_str = sys.stdin.readline()
        j_str = j_str[msgs.RESPONSE_CODE_SIZE:]   # remove response code

        j_dir = json.loads(j_str)
        for row in j_dir:
            self.tree.insert('', 'end', 
                iid=str(row['id']), 
                values=[row[key] for key in MTrack.TREE_COL_LIST]
            )


class AddTopLevel(tk.Toplevel):
    NAME_LIST = ['name', 'state', 'type']
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
        self.entry_frame = tk.Frame(self)
        for _row in range(len(AddTopLevel.NAME_LIST)):
            tk.Label(
                self.entry_frame, 
                text=AddTopLevel.NAME_LIST[_row]
            ).grid(row=_row, column=0)

            tk.Entry(
                self.entry_frame
            ).grid(row=_row, column=1)

        self.group1 = tk.IntVar(value=1)
        tk.Radiobutton(self.entry_frame, text='1', value=1, variable=self.group1).grid(row=len(AddTopLevel.NAME_LIST), column=0)
        tk.Radiobutton(self.entry_frame, text='2', value=2, variable=self.group1).grid(row=len(AddTopLevel.NAME_LIST), column=1)

        self.entry_frame.pack()


    def _send_data(self):
        data = {}
        count = 0
        for child in self.entry_frame.winfo_children():
            if (isinstance(child, tk.Entry)):
                cur_entry_data = child.get().strip()
                if len(cur_entry_data) != 0:
                    data[AddTopLevel.NAME_LIST[count]] = cur_entry_data
                    count += 1
            if (isinstance(child, tk.Radiobutton)):
                data['rating'] = self.group1.get()

        if len(data) != len(AddTopLevel.NAME_LIST) + 1:
            print(len(data), file=sys.stderr)
            messagebox.showerror("Error", "You can't leave any field empty")
            return

        self.callback(data)
        self.destroy()
