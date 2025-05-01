import json
import tkinter as tk
from tkinter import ttk
from tkinter import messagebox

from custom import AddTopLevel as at
from api import client

TREE_COL_LIST = ['name', 'state', 'type', 'rating']

class MainFrame(tk.Frame):
    def __init__(self, parent, target_select_event):
        tk.Frame.__init__(self, parent)

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
            columns=TREE_COL_LIST
        )
        self.tree.grid(row=1, column=0, sticky='nsew')
        for col in TREE_COL_LIST:
            self.tree.heading(col, text=col, command=lambda c=col: self._sort_treeview(c, False))
            self.tree.column(col, width=100)

        self.tree.bind("<<TreeviewSelect>>", target_select_event)

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
        rsp_str = client.send_request_with_data(client.ADD_RESPONSE, values)

        rp = json.loads(rsp_str)
        if rp[0]['CODE'] == client.SEND_ID:
            rsp_id = rp[0]['id']
            self.tree.insert('', 'end', 
                iid=str(rsp_id), 
                values=[values[key] for key in TREE_COL_LIST]
            )
        else:
            messagebox.showerror("Error", "Something went wrong")


    def _rm_item(self):
        """
            Find the id of the selected item and send the correct response code
            and id to the backend.
        """
        rm_item_id = self.tree.focus()
        try:
            data = {'id' : int(rm_item_id)}
        except:
            messagebox.showwarning("Warning", "You need to select an item")
            return
        rsp_str = client.send_request_with_data(client.RM_RESPONSE, data)

        rp = json.loads(rsp_str)
        if rp[0]['CODE'] == client.TRN_END:
            self.tree.delete(rm_item_id)
        else:
            messagebox.showerror("Error", "Something went wrong")


    def _fill_window_tree(self):
        # sending request for the database data
        j_str = client.send_request(client.ASK_DATA)

        j_list = json.loads(j_str)
        for row in j_list[:-1]:
            self.tree.insert('', 'end', 
                iid=str(row['id']), 
                values=[row[key] for key in TREE_COL_LIST]
            )


    def _sort_treeview(self, col_name, descending):
        """
            This is a modified version from this source:
            https://www.w3resource.com/python-exercises/tkinter/python-tkinter-widgets-exercise-18.php
        """
        col_data = [(self.tree.set(item, col_name), item) for item in self.tree.get_children('')]
        sort_key = None
        if (col_name == 'rating'):
            sort_key = lambda t: int(t[0])
        col_data.sort(key=sort_key, reverse=descending)
        for index, (NOT_USED, item) in enumerate(col_data):
            self.tree.move(item, '', index)
        self.tree.heading(col_name, command=lambda: self._sort_treeview(col_name, not descending))
