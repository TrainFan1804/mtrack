# Adding a new media section

When adding a new media section theses things need to be done (It shouldn't
be added any integer types because it's so anoying to add checks on BOTH the
backend and the frontend (If they added just code them hard in the code)):

- In `globals.h` add the type and name to `Media`. Modify the return values of
    `Media::attributesToSql` and `Media::valuesToSql`
    Also add the new section in the constructor and modify the different SQL
    statements.
- In `optionparser.cpp` add the new media section in the created media object. 
    (Don't forget to update the `--help` window)
- In `MTrack.py` add the name for the treeview column in `MTrack.TREE_COL_LIST` 
    and the name for the add window label and entry in `AddTopLevel.NAME_LIST`
