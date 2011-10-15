/*
 * Copyright 2011 Peter Stuifzand
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

void start(struct parser* p, struct buffer* b, char* filename)
{
    printf("Loading %s\n", filename);
    p->buf = b;

    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Couldn't open filename: %s\n", filename);
        exit(1);
    }

    if (fread(p->buf->program, sizeof(char), 4095, fp) > 0) {
        p->buf->begin = p->buf->program;
    }
    else {
        printf("ERROR");
        exit(1);
    }
}

int identifier(struct parser* p)
{
    int n = 0;

    whitespace(p);

    if (!isalpha(*(p->buf->begin))) {
        return 0;
    }

    while (isalnum(*(p->buf->begin))) {
        p->id[n++] = *(p->buf->begin);
        p->buf->begin++;
    }

    p->id[n]=0;
    return 1;
}

int character(struct parser* p, char c) 
{
    whitespace(p);

    if (*(p->buf->begin) == c) {
        p->buf->begin++;
        return 1;
    }

    printf("Error while trying to match: [%c]\n", c);
    exit(1);
    return 0;
}

int lstring(struct parser* p, char* lit)
{
    whitespace(p);

    size_t l = strlen(lit);

    if (strncmp(p->buf->begin, lit, l) == 0) {
        p->buf->begin += l;
        return 1;
    }
    return 0;
}

int string(struct parser* p) 
{
    int n = 0;

    whitespace(p);

    char* s = p->buf->begin;

    if (*s != '\'') {
        return 0;
    }

    p->id[n++] = *s++;

    while (*s != '\'') {
        p->id[n++] = *s;
        s++;
    }

    p->id[n++] = *s++;

    p->buf->begin += n;

    p->id[n] = 0;

    return 1;
}

int whitespace(struct parser* p)
{
    while (isspace(*(p->buf->begin))) {
        p->buf->begin++;
    }
    return 1;
}

int number(struct parser* p)
{
    whitespace(p);

    if (!isdigit(*(p->buf->begin))) {
        printf("Not a number\n");
        return 0;
    }

    p->num = 0;
    while (isdigit(*(p->buf->begin))) {
        p->num += (p->num*10) + *(p->buf->begin)-'0';
        p->buf->begin++;
    }
    return 1;
}

