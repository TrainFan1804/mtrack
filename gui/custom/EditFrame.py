import json
import tkinter as tk
from tkinter import ttk
from tkinter import messagebox

import comhandler as com
from custom import MainFrame as mf


class EditFrame(tk.Frame):
    def __init__(self, parent):
        tk.Frame.__init__(self, parent)

        self.columnconfigure(0, weight=1)
        self.columnconfigure(1, weight=1)
        self.rowconfigure(0, weight=1)
        self.rowconfigure(1, weight=1)
        self.rowconfigure(2, weight=1)
        self.rowconfigure(3, weight=1)

        tk.Label(
            self, text='name'
        ).grid(row=0, column=0, sticky='ew')
        self.name_var = tk.StringVar()
        tk.Entry(
            self, textvariable=self.name_var
        ).grid(row=0, column=1)

        tk.Label(
            self, text='state'
        ).grid(row=1, column=0, sticky='ew')
        self.state_var = tk.StringVar()
        ttk.Combobox(
            self,
            state='readonly',
            textvariable=self.state_var,
            values=('completed', 'droped', 'ongoing')
        ).grid(row=1, column=1)

        tk.Label(
            self, text='type'
        ).grid(row=2, column=0, sticky='ew')
        self.type_var = tk.StringVar()
        tk.Entry(
            self, textvariable=self.type_var
        ).grid(row=2, column=1)

        tk.Label(
            self, text='rating'
        ).grid(row=3, column=0, sticky='ew')
        rb_frame = tk.Frame(self)
        rb_frame.grid(row=3, column=1)
        self.rating_var = tk.IntVar()
        for btn_index in range(1, 11):
            _row = (btn_index - 1) // 5
            col = (btn_index - 1) % 5
            tk.Radiobutton(
                rb_frame, text=str(btn_index),
                value=btn_index, variable=self.rating_var
            ).grid(row=_row, column=col)

        self.save_btn = tk.Button(
            self, text='save', 
            command=self.save_item,
            state='disabled'
        )
        self.save_btn.grid(row=4, column=0)


    def set_tree(self, tree):
        self.tree = tree


    def save_item(self):
        data = {}
        data['id'] = int(self.edit_item_id)
        data['name'] = self.name_var.get()
        data['state'] = self.state_var.get()
        data['type'] = self.type_var.get()
        data['rating'] = self.rating_var.get()

        com.send_request_with_data(com.EDIT_RESPONSE, data)
        rsp_str = com.listen_to_backend()
        rp = json.loads(rsp_str)
        if rp[0]['CODE'] == com.TRN_END:
            self.tree.item(
                str(data['id']), 
                values=[data[key] for key in mf.TREE_COL_LIST]
            )
        else:
            messagebox.showerror("Error", "Something went wrong")


    def edit_item(self, event):
        """
            This method is called when a item from the treeview in MainFrame 
            is selected.
        """
        self.save_btn.config(state='normal')
        tree = event.widget
        self.edit_item_id = tree.focus()
        self.name_var.set(tree.item(self.edit_item_id)['values'][0])
        self.state_var.set(tree.item(self.edit_item_id)['values'][1])
        self.type_var.set(tree.item(self.edit_item_id)['values'][2])
        self.rating_var.set(tree.item(self.edit_item_id)['values'][3]) 
