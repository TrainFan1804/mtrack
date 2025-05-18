# mTrack

> [!NOTE]
> This software is **NOT** supported for windows!

With this software you can organize your watched movies, read books or any other
media you consumed. You can choose between a CLI tool or a GUI.


# Installation

1. Clone this repository.
2. Enter the repository.
3. Run `chmod +x install.h`
4. Run `./install.sh`

Now you can run `mtrack` in your terminal to use it.

> [!NOTE]
> You may need to change your `$PATH` to use `mtrack` via your terminal.
    
## Requirements

A C++ compiler, sqlite3 and Qt6 is required to install this software. All other requirements are shipped
with the software.

- GCC >= 6
- sqlite3 >= 3.49.1
- Qt >= 6.8.2


# How to use

You can use the software as a CLI and as a GUI.

To run inside the terminal just enter`mtrack --help` to show all available
commands. Running mtrack without any options will launch the GUI.

```
Usage:
  mtrack [OPTION...] [optional args]

  -s, --show                    Show the content of the libary
  -a, --add <NAME> <RATING> <STATE> <TYPE>
                                Add a new media to your libary
  -r, --remove <id>             Remove a media by id
      --verbose                 Activate log
  -v, --version                 Show version
  -h, --help                    Show this page
```


# Misc

I used [CXXOPTS](https://github.com/jarro2783/cxxopts) to handle command line 
options easily, [nlohmann/json](https://github.com/nlohmann/json) for
json handling and [fmt](https://github.com/fmtlib/fmt) for formatting.
