# Creating a new Qt .ui file

When you want to add a new `.ui` file you need to follow these steps to make sure the build script will
compile correctly.

1. Put the new `.ui` file into the directory: `./forms`
2. Run `create_header.sh` in `./forms` once
3. Create a wrapper class for the new created form. The wrapper class header should
    be put in `include/gui/wrapper` and the src file in `src/gui/wrapper`
