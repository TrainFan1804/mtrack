# Adding a new media section

When adding a new media section theses things need to be done:
- In `globals.h` add the type and name to `media.Media`. 
    Also add the new section in the constructor.
- In `databasemanager.cpp` modifiy the sql statement in `addMedia` 
    so the function can actually add the new section. Don't forget to modify the
    INSERT statment too!
- In `communication.cpp` add the new media section in the created media object.
    **Important**: When the new section is a INTEGER type add via `int_col_vals`
- In `optionparser.cpp` add the new media section in the created media object. 
    (Don't forget to update the `--help` window)
- In `build.sh` add the new section into the created files that are created at building.
- IN `MTrack.py` add a check if the string is INTEGER
