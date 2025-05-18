# Building

When you want to build this software there are somethings you need to consider.
When build via `build.sh <BUILD_TYPE>` you can't access mTrack via the terminal. Then you need
to launch the application directly or via `run.sh` in the root directory of the
project.
When using the build script, the appdata (the database and logs) will be in the
subfolder `dev/mtrack` and `dev/mtrack/log`.

When using `install.sh` you will be able to access mTrack via the terminal but
only for your user only. The appdata will located at `$HOME/.local/share/mtrack`.


## Development

This is a note for other developer. When you running mTrack locally and develop
at the same time you should always use the database inside the projects directory
for testing unless you know what you do.

### Testing

You can build test with `build.sh <BUILD_TYPE> test` and run them with
`run.sh test`.
