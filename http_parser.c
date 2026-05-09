#include <stdio.h>
#include <string.h>
#include "http_parser.h"

void parse_request(const char *buffer, char *method, char *path)
{
    sscanf(buffer, "%s %s", method, path);
}
