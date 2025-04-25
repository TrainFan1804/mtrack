# appdata specific paths (needed at runtime)
APPDATA_PATH="$HOME/.local/share/mtrack"
LOG_PATH="$APPDATA_PATH/log"
PYTHON_PATH="$PWD/dist/pyenv/bin/python3"
GUI_PY="$PWD/gui/main.py"

if [ ! -d $PWD/dist/pyenv ]; then
    echo "Creating python enviroment..."
    python3 -m venv $PWD/dist/pyenv
fi

mkdir -p $APPDATA_PATH
mkdir -p $LOG_PATH

# Because I am to lazy to define the path in the C++ code twice I came up with
# this crazy stuff...
# Maybe I should rewrite the entire programm in just a big script that execute a big cat command...
cat << EOF > include/buildenv.h
#ifndef BUILDENV_H
#define BUILDENV_H

/*
    mTrack use a different location for the appdata itself. Right now
    it's just a simple sqlite database and a (optional) log.
*/
#define APPDATA_DIR_PATH    "$APPDATA_PATH"
#define LOG_DIR_PATH        "$LOG_PATH"
#define PYTHON_PATH         "$PYTHON_PATH"
#define GUI_PY              "$GUI_PY"

// response codes for the communication between frontend and backend
#define ASK_DATA            "100"
#define ADD_RESPONSE        "101"
#define RM_RESPONSE         "102"
#define SEND_RESPONSE       "200"
#define TRN_END             "300"
#define SEND_ID             "301"
#define WRG_FORMAT          "400"
#define DB_ERROR            "500"

#define RESPONSE_CODE_SIZE  4   // prefix = 3, . = 1

// macros for SQL queries
#define TABLE_NAME          "MEDIA"
#define TABLE_INT_COL       "rating"    // all table columns that are integer (important for casting from json to int)
#define TABLE_STR_COL       "name", "state", "type"      // all table columns that are string
#define TABLE_ALL_COL       "ID", "NAME", "RATING", "STATE", "TYPE"

const std::string SQL_JSON_SELECT_ALL = "SELECT json_group_array(json_object('id', ID, 'name', NAME, 'rating', RATING, 'state', STATE, 'type', TYPE)) AS json_result FROM " + std::string(TABLE_NAME) + ";";

const std::string SQL_CREATE = "CREATE TABLE IF NOT EXISTS " + std::string(TABLE_NAME) + "(ID INTEGER PRIMARY KEY, NAME TEXT NOT NULL, RATING INT NOT NULL, STATE TEXT NOT NULL, TYPE TEXT NOT NULL);";

#endif
EOF

# pls forgive me for this...
cat << EOF > gui/config/responsecodes.py
ASK_DATA        =   "100"
ADD_RESPONSE    =   "101"
RM_RESPONSE     =   "102"
SEND_RESPONSE   =   "200"
TRN_END         =   "300"
SEND_ID         =   "301"
WRG_FORMAT      =   "400"
DB_ERROR        =   "500"

RESPONSE_CODE_SIZE  =   3 + 1
EOF

cat << EOF > gui/config/mediasection.py
JSON_ID             =   'id'
NAME_LIST           =   ['name', 'rating', 'state', 'type']
EOF
