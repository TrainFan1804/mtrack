def place_window_mid(window, width, height):
    # get screen width and height# 
    screen_width = window.winfo_screenwidth()
    screen_height = window.winfo_screenheight()
    
    # # calculate position x and y coordinates
    x = (screen_width / 2) - (width / 2)
    y = (screen_height / 2) - (height / 2)
    window.geometry('%dx%d+%d+%d' % (width, height, x, y))
