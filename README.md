# mTrack

With this CLI tool you can organize your consumed media.

> [!IMPORTANT]
> Right now this is a VERY simple tool to save your consumed media. I want to
> provide more features in the future.

# Installation

> [!NOTE]
> This will probably NOT work on windows.

1. Clone this repository.
2. Enter the repository.
3. Run ```chmod +x install.h```
4. Run ```install.sh```
5. Now you can run ```mtrack --help``` in your terminal

## Requirements

A C++ compiler and sqlite3 is required to install this software.

- GCC
- sqlite3

GCC >= 6 **should** work and sqlite3 3.49.1 was the only version I tested.

# How to use

Run ```mtrack --help``` to show all available commands.

# Misc

I used [CXXOPTS](https://github.com/jarro2783/cxxopts) to handle command line options easily.
