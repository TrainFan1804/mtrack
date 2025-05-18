# Adding a new media section

When adding a new media section theses things need to be done (It shouldn't
be added any integer types because it's so anoying to add checks on BOTH the
backend and the frontend (If they added just code them hard in the code)):

- In `globals/sql_globals.h` update the SQL stuff.
- In `Media.h` add the type and name to `Media`. Modify the return values of
    `Media::attributesToSql` and `Media::valuesToSql`
    Also add the new section in the constructor and modify the different SQL
    statements.
- In `optionparser.cpp` add the new media section in the created media object. 
    (Don't forget to update the `--help` window)
- In `gui/QMedia.h` add a function that has the same name as the new attribute and
    return the correct value. Also update `attributeAmount` in the same header.
- In `gui/wrapper/MediaViewModel.cpp` update the switch case's in the methods
    `data`and `headerData` to update the TableView in the GUI correctly.
- Ofc update the Qt itself with Qt Designer.
