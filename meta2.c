/*
 * Copyright 2011 Peter Stuifzand
 */
#include <stdio.h>
#include <string.h>
#include "lib.h"
#include <stdlib.h>

int label_count = 0;

int ex1(struct parser* p);

void generate_label(int lbl) 
{
    
    int c = lbl % 10;
    lbl -= c;
    lbl /= 10;
    int b = lbl % 10;
    lbl -= b;
    lbl /= 10;
    int a = lbl % 26;
    printf("%c%c%c\n", a+'A', b+'0', c+'0');
}

void label(char* s) 
{
    printf("%s\n",s);
}

void out1(char* p)
{
    printf("        %-3s\n",p);
}

void out2(char* p, char* s)
{
    printf("        %-3s ",p);
    if (s) {
        printf("%s", s);
    }
    printf("\n");
}

void out3(char* p, int lbl)
{
    printf("        %-3s ",p);
    generate_label(lbl);
}


int output1(struct parser* p)
{
    whitespace(p);

    if (lstring(p, "*1")) {
        out1("GN1");
        return 1;
    }
    else if (lstring(p, "*2")) {
        out1("GN2");
        return 1;
    }
    else if (lstring(p, "*")) {
        out1("CI");
        return 1;
    }
    else if (string(p)) {
        out2("CL", p->id);
        return 1;
    }
    return 0;
}

int output(struct parser* p)
{
    if (lstring(p, ".OUT")) {
        lstring(p, "(");
        while (output1(p)) {}
        lstring(p, ")");
        out1("OUT");
    }
    else if (lstring(p, ".LABEL")) {
        out1("LB");
        output1(p);
        out1("OUT");
    }
    else {
        //out1("OUT");
        return 0;
    }
    
    return 1;
}


int ex3(struct parser* p)
{
    int label_1 = label_count++;
    
    whitespace(p);

    if (lstring(p, ".ID")) {
        out1("ID");
        return 1;
    }
    else if (lstring(p, ".NUMBER")) {
        out1("NUM");
        return 1;
    }
    else if (lstring(p, ".STRING")) {
        out1("SR");
        return 1;
    }
    else if (lstring(p, ".EMPTY")) {
        out1("SET");
        return 1;
    }
    else if (lstring(p, "(")) {
        ex1(p);
        lstring(p, ")");
        return 1;
    }
    else if (*(p->buf->begin) == '$') {
        p->buf->begin++;
        generate_label(label_1);
        ex3(p);
        out3("BT", label_1);
        out1("SET");
        return 1;
    }
    else if (identifier(p)) {
        out2("CLL", p->id);
        return 1;
    }
    else if (string(p)) {
        out2("TST", p->id);
        return 1;
    }
    return 0;
}

int ex2(struct parser* p)
{
    int label_1 = label_count++;
    int stop = 0;

    if (ex3(p)) {
        out3("BF", label_1);
    }
    else if (output(p)) {
    }
    else {
        stop = 1;
    }

    if (stop) return 0;

    while (!stop) {
        if (ex3(p)) {
            out1("BE");
        }
        else if (output(p)) {
        }
        else {
            stop = 1;
        }
    }

    generate_label(label_1);

    return 1;
}

int ex1(struct parser* p)
{
    int alt_count=1;
    int label_1 = label_count++;

    printf("                              ; alternation %d\n", alt_count);
    ex2(p);

    while (lstring(p, "/")) {
        alt_count++;
        printf("                              ; alternation %d\n", alt_count);

        out3("BT", label_1);

        if (!ex2(p)) break;
    }
    generate_label(label_1);

    return 1;
}

int st(struct parser* p)
{
    if (!identifier(p)) {
        return 0;
    }

    label(p->id);

    character(p, '=');
    ex1(p);
    lstring(p, ".,");
    out1("R");

    return 1;
}

int program(struct parser* p)
{
    lstring(p, ".SYNTAX");
    identifier(p);
    out2("ADR", p->id);

    while (st(p)) {}

    if (lstring(p, ".END")) {
        out1("END");
        return 1;
    }
    return 1;
}

int main(int argc,char** argv)
{
    struct buffer b;
    b.program = (char*)malloc(sizeof(char)*4096);
    struct parser p;
    if (argc == 2) {
        start(&p, &b, argv[1]);
        program(&p);
    }
    free(b.program);
    return 0;
}
