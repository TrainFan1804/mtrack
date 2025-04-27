import sys
import json
import tkinter as tk
from tkinter import ttk
from tkinter import messagebox

import msgsender as msgs
from custom import AddTopLevel as at


class MainFrame(tk.Frame):

    TREE_COL_LIST = ['name', 'state', 'type', 'rating']

    def __init__(self, parent, target_select_event):
        tk.Frame.__init__(self, parent, bg='green')

        self.columnconfigure(0, weight=1)
        self.rowconfigure(0, weight=1)
        self.rowconfigure(1, weight=10)
        self.rowconfigure(2, weight=1)

        tk.Label(
            self,
            text='mTrack',
            height=3
        ).grid(row=0, column=0, sticky='nsew')

        self.tree = ttk.Treeview(
            self,
            selectmode='browse',
            show='headings', 
            columns=MainFrame.TREE_COL_LIST
        )
        self.tree.grid(row=1, column=0, sticky='nsew')
        for col in MainFrame.TREE_COL_LIST:
            self.tree.heading(col, text=col)
            self.tree.column(col, width=100)

        self.tree.bind("<<TreeviewSelect>>", target_select_event.edit_item)

        self._fill_window_tree()

        tree_scrollbar = tk.Scrollbar(self, orient='vertical', command=self.tree.yview)
        tree_scrollbar.grid(row=1, column=1, sticky='nsew')
        self.tree.configure(yscrollcommand=tree_scrollbar.set)

        btn_frame = tk.Frame(self)
        btn_frame.grid(row=2, column=0, sticky='nsew')

        tk.Button(
            btn_frame, text='add', 
            command=self._add_item
        ).pack(side=tk.LEFT, expand=True, fill='both')

        tk.Button(
            btn_frame, text='remove', 
            command=self._rm_item
        ).pack(side=tk.LEFT, expand=True, fill='both')

        tk.Button(
            btn_frame, text='quit', 
            command=parent.destroy
        ).pack(side=tk.LEFT, expand=True, fill='both')


    def _add_item(self):
        at.AddTopLevel(self, self._receive_data_callback)


    def _receive_data_callback(self, values):
        j_str = json.dumps(values)
        msgs.send_request_with_data(msgs.ADD_RESPONSE, j_str)

        rsp_code = sys.stdin.readline().rstrip()
        if rsp_code.startswith(msgs.SEND_ID):
            rsp_id = rsp_code[msgs.RESPONSE_CODE_SIZE:]
            self.tree.insert('', 'end', 
                iid=str(rsp_id), 
                values=[values[key] for key in MainFrame.TREE_COL_LIST]
            )
        else:
            messagebox.showerror("Error", "Something went wrong")


    def _rm_item(self):
        """
            Find the id of the selected item and send the correct response code
            and id to the backend.
        """
        rm_item_id = self.tree.focus()
        msgs.send_request_with_data(msgs.RM_RESPONSE, rm_item_id)
        rsp_code = sys.stdin.readline().rstrip()
        if rsp_code == msgs.TRN_END:
            self.tree.delete(rm_item_id)
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
                values=[row[key] for key in MainFrame.TREE_COL_LIST]
            )
