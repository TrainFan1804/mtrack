# Building

To build to software you can use the `build.sh` script that will build **mtrack** directly.

When using `install.sh` you will be able to access mTrack via the terminal but
only for your user only. The appdata will located at `$HOME/.local/share/mtrack`.

## Development

When build via `build.sh --dev` you can't access mTrack via the terminal. You need
to launch the application directly or via `run.sh` in the root directory of the
project.

Building in developer mode the softwares appdata (logs, backups and databases) will be in the
project subfolder `dev/mtrack`.

Using `build.sh clean <BUILD_OPTION>` will build the entire project from the
beginning. May take a few moments to finish. Running `clean` without any build
option will just remove all build artefacts.


> [!TIP]
> When you running mTrack locally and develop at the same time you should always use the
> database inside `dev/mtrack` for testing unless you know what you do.
> It would be best to have two seperate repos. One for development and one for
> running private.

> [!TIP]
> If you want to develop this software further it may be helpfull to use the
> Qt-Designer to design the GUI.

### Testing

You can build test with `build.sh --dev test` and run them with
`run.sh test`.
