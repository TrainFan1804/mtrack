#include "db/custom_callbacks.h"

#include <string>

int jsonSelectCallback(
    void *head, int num_cols, char **row_data, char **NOT_USED
)
{
    if (num_cols > 0 && row_data[0])
    {
        std::string *result = static_cast<std::string *>(head);
        *result             = row_data[0];
    }
    else
    {
        std::string *result = static_cast<std::string *>(head);
        *result             = "[]";
    }
    return 0;
}
