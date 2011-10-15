#ifndef LIB_H
#define LIB_H

struct parser {
    int current;
    FILE* fp;

    char id[100];
    int  num;
    struct buffer* buf;
};

struct buffer {
    char* program;
    char* begin;
};

void start(struct parser* p, struct buffer* b, char* filename);
int nextchar(struct parser* p);
int identifier(struct parser* p);
int character(struct parser* p, char c);
int lstring(struct parser* p, char* lit);
int string(struct parser* p);
int whitespace(struct parser* p);
int number(struct parser* p);

#endif
