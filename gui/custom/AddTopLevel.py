import tkinter as tk
from tkinter import ttk
from tkinter import messagebox

import custom.utils as utils


class AddTopLevel(tk.Toplevel):

    NAME_LIST = ['name', 'state', 'type']

    def __init__(self, parent, callback):
        tk.Toplevel.__init__(self, parent)
        self.resizable(False, False)
        utils.place_window_mid(self, 400, 250)

        self.callback = callback

        tk.Label(self, text='Add a new media', height=3).pack()
        self._build_entries()

        btn_frame = tk.Frame(self)
        btn_frame.pack()

        tk.Button(
            btn_frame, 
            text='add', 
            command=self._send_data
        ).pack(side=tk.LEFT)

        tk.Button(
            btn_frame, 
            text='close', 
            command=self.destroy
        ).pack(side=tk.LEFT)


    def _build_entries(self):
        self.entry_frame = tk.Frame(self)
        self.entry_frame.pack()

        tk.Label(self.entry_frame, text='name').grid(row=0, column=0)
        tk.Entry(self.entry_frame).grid(row=0, column=1)

        tk.Label(self.entry_frame, text='state').grid(row=1, column=0)
        self.state_value = tk.StringVar()
        ttk.Combobox(
            self.entry_frame, 
            state='readonly',
            textvariable=self.state_value,
            values=('completed', 'droped', 'ongoing')
        ).grid(row=1, column=1)

        tk.Label(self.entry_frame, text='type').grid(row=2, column=0)
        tk.Entry(self.entry_frame).grid(row=2, column=1)

        tk.Label(self.entry_frame, text='rating').grid(row=3, column=0)
        rb_frame = tk.Frame(self.entry_frame)
        rb_frame.grid(row=3, column=1)
        self.rating_value = tk.IntVar(value=5)
        for btn_index in range(1, 11):
            _row = (btn_index - 1) // 5
            col = (btn_index - 1) % 5
            tk.Radiobutton(
                rb_frame, text=str(btn_index),
                value=btn_index, variable=self.rating_value
            ).grid(row=_row, column=col)


    def _send_data(self):
        data = {}
        count = 0
        for child in self.entry_frame.winfo_children():
            if (isinstance(child, tk.Entry)):
                cur_entry_data = child.get().strip()
                if len(cur_entry_data) != 0:
                    data[AddTopLevel.NAME_LIST[count]] = cur_entry_data
                    count += 1

        data['rating'] = self.rating_value.get()
        # because tkk.Combobox inherit from tk.Entry the for loop will already extract the data
        # but the loop may be gone in a future version
        data['state'] = self.state_value.get()

        if len(data) != len(AddTopLevel.NAME_LIST) + 1:
            messagebox.showerror("Error", "You can't leave any field empty")
            return

        self.callback(data)
        self.destroy()
