import tkinter as tk


class FilterFrame(tk.Frame):
    def __init__(self, parent):
        tk.Frame.__init__(self, parent)

        self.main_frame = tk.Frame(self)
        self.main_frame.grid(row=0, column=0)

        self.search_frame = tk.Frame(self.main_frame)
        self.search_frame.grid(row=0, column=0)

        tk.Label(
            self.search_frame,
            text='Search'
        ).grid(row=0, column=0)

        self.search_var = tk.StringVar()
        tk.Entry(
            self.search_frame,
            textvariable=self.search_var
        ).grid(row=0, column=1)

        tk.Button(
            self.search_frame,
            text='Search',
            command=self._search
        ).grid(row=1, column=1)

        # self._build_filter_frame()


    def _build_filter_frame(self):
        self.filter_frame = tk.Frame(self.main_frame)
        self.filter_frame.grid(row=1, column=0)

        tk.Label(
            self.filter_frame,
            text='name'
        ).grid(row=0, column=0)

        tk.Entry(
            self.filter_frame,
        ).grid(row=0, column=1)

        tk.Label(
            self.filter_frame,
            text='state'
        ).grid(row=1, column=0)

        tk.Entry(
            self.filter_frame,
        ).grid(row=1, column=1)

        tk.Label(
            self.filter_frame,
            text='type'
        ).grid(row=2, column=0)

        tk.Entry(
            self.filter_frame,
        ).grid(row=2, column=1)

        tk.Label(
            self.filter_frame,
            text='rating'
        ).grid(row=3, column=0)

        tk.Entry(
            self.filter_frame,
        ).grid(row=3, column=1)

        tk.Button(
            self.filter_frame,
            text='Delete filter'
        ).grid(row=4, column=0)

        tk.Button(
            self.filter_frame,
            text='Use filter'
        ).grid(row=4, column=1)


    def set_tree(self, tree):
        self.tree = tree


    def _search(self):
        search_for = self.search_var.get()
        selections = []
        for child in self.tree.get_children():
            values = self.tree.item(child)['values']
            if search_for.lower() in values[0].lower():
                print(values[0])
                selections.append(child)
        # select multiple hits per search
        self.tree['selectmode'] = 'extended'
        self.tree.selection_set(selections)
        self.tree['selectmode'] = 'browse'
