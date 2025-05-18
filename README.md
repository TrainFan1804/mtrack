# mTrack

> [!NOTE]
> This software is **NOT** supported for windows!

With this software you can organize your watched movies, read books or any other
media you consumed. You can use a simple CLI tool or a simple GUI.


# Installation

1. Clone this repository.
2. Enter the repository.
3. Run `chmod +x install.h`
4. Run `./install.sh`
    
## Requirements

A C++ compiler and sqlite3 is required to install this software.

- GCC >= 6
- sqlite3 >= 3.49.1
- python3 >= 3.13.2


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
options easily and [nlohmann/json](https://github.com/nlohmann/json) for
json handling.
