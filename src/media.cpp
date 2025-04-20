#include <iostream>

#include "media.h"

namespace
{
    static int media_amount = 0;
}

Media::Media()
{
    _rating = 1;
    _media_id = media_amount++;
    _name = "";
}
