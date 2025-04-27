import json
import sys
import tkinter as tk
from tkinter import ttk
from tkinter import messagebox

import comhandler as com
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
        com.send_request_with_data(com.ADD_RESPONSE, values)

        rsp_str = com.listen_to_backend()
        rp = json.loads(rsp_str)
        if rp[0]['CODE'] == com.SEND_ID:
            rsp_id = rp[0]['id']
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
        data = {'id' : int(rm_item_id)}
        com.send_request_with_data(com.RM_RESPONSE, data)

        rsp_str = com.listen_to_backend()
        rp = json.loads(rsp_str)
        if rp[0]['CODE'] == com.TRN_END:
            self.tree.delete(rm_item_id)
        else:
            messagebox.showerror("Error", "Something went wrong")


    def _fill_window_tree(self):
        # sending request for the database data
        com.send_request_no_data(com.ASK_DATA)
        j_str = com.listen_to_backend()

        j_list = json.loads(j_str)
        for row in j_list[:-1]:
            self.tree.insert('', 'end', 
                iid=str(row['id']), 
                values=[row[key] for key in MainFrame.TREE_COL_LIST]
            )
