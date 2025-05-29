#pragma once

/**
 * For a simple explanation how the callback works see:
 * https://stackoverflow.com/questions/31146713/sqlite3-exec-callback-function-clarification
 *
 * This function is called for every row in the table.
 *
 * This is some crazy sh*t here
 */
int jsonSelectCallback(
    void *head, int num_cols, char **row_data, char **NOT_USED
);
