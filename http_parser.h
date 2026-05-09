#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

void parse_request(const char *buffer, char *method, char *path);

#endif
