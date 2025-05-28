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

> [!TIP]
> You may need to change your `$PATH` to use `mtrack` via your terminal.
    
## Requirements

> [!IMPORTANT]
> When using wayland as communication protocol you need to install the `qt6-wayland` 
> package for your distrobution.

A C++ compiler, sqlite3 and Qt6 is required to install this software. All other requirements are shipped
with the software.

- GCC >= 6
- sqlite3 >= 3.49.1
- Qt >= 6.8.2

### Debian

Run this command to install al requirements:

```
sudo apt install build-essential sqlit3 qt6-base-dev
```

> [!WARNING]
> While testing this software on debian the `build.sh` could not find `moc` 
> and `uic` under `/usr/lib/qt6` because the actuall execuables where under
> `/usr/lib/qt6/libexec`. My workaround are two symbolic links in the 
> qt6 directory pointing to the executables in libexec.


# How to use

You can use the software as a CLI and as a GUI.

To run inside the terminal just enter`mtrack --help` to show all available
commands. Running mtrack without any options will launch the GUI.

```
mtrack: A simple tool to track your media consume
Usage:
  ./build/mtrack [OPTION...]

  -s, --show                    Show the content of the libary
  -a, --add <NAME> <RATING> <STATE> <TYPE>
                                Add a new media to your libary
  -r, --remove <id>             Remove a media by id
  -e, --edit <EDITED_ID> <NEW_NAME> <NEW_RATING> <NEW_STATE> <NEW_TYPE>
                                Edit a entry with the new parameter
      --verbose                 Activate log
  -v, --version                 Show version
  -h, --help                    Show this page
```

> [!NOTE]
> When using `--add` or `--edit` you need to enter the values like this:
> ```
> mtrack --add=<NAME>,<RATING>,<STATE>,<TYPE>
> mtrack --edit=<EDITED_ID>,<NEW_NAME>,<NEW_RATING>,<NEW_STATE>,<NEW_TYPE>
> ```


# Misc

I used [CXXOPTS](https://github.com/jarro2783/cxxopts) to handle command line 
options easily, [nlohmann/json](https://github.com/nlohmann/json) for
json handling and [fmt](https://github.com/fmtlib/fmt) for formatting.
