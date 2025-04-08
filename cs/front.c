/* front.c - a lexical analyzer system for simple arithmetic expressions */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <ctype.h>

#include <string.h>


/* Global declarations */

/* Variables */

int charClass;

char lexeme[100];

char nextChar;

int lexLen;

int token;

int nextToken;

FILE* in_fp;


/* Function declarations */

void addChar();

void getChar();

void getNonBlank();

int lex();

int lookup(char ch);


/* Character classes */

#define LETTER 0

#define DIGIT 1

#define UNKNOWN 99


/* Token codes */

#define INT_LIT 14

#define IDENT 13

#define ASSIGN_OP 5

#define ADD_OP 8

#define SUB_OP 10

#define MULT_OP 12

#define DIV_OP 15

#define LEFT_PAREN 17

#define RIGHT_PAREN 19

#define EOF_TOKEN -2

#define IF_ 11

#define ELSE_ 4

#define LBRACE 3

#define RBRACE 22


//

/* main driver */

int main() {

    /* Open the input data file and process its contents */

    if ((in_fp = fopen("front.in", "r")) == NULL)

        printf("ERROR - cannot open front.in\n");

    else {

        getChar();

        do {

            lex();

        } while (nextToken != EOF_TOKEN);

    }

    return 0;

}


//

/* lookup - a function to lookup operators and parentheses and return the token */

int lookup(char ch) {

    switch (ch) {

    case '(':

        addChar();

        nextToken = LEFT_PAREN;

        break;

    case ')':

        addChar();

        nextToken = RIGHT_PAREN;

        break;

    case '+':

        addChar();

        nextToken = ADD_OP;

        break;

    case '-':

        addChar();

        nextToken = SUB_OP;

        break;

    case '*':

        addChar();

        nextToken = MULT_OP;

        break;

    case '/':

        addChar();

        nextToken = DIV_OP;

        break;



    case '=':

        addChar();

        nextToken = ASSIGN_OP;

        break;


    case '{':

        addChar();

        nextToken = LBRACE;

        break;

    case '}':

        addChar();

        nextToken = RBRACE;

        break;


    default:

        addChar();

        nextToken = EOF_TOKEN;

        break;

    }

    return nextToken;

}


//

/* addChar - a function to add nextChar to lexeme */

void addChar() {

    if (lexLen <= 98) {

        lexeme[lexLen++] = nextChar;

        lexeme[lexLen] = '\0';

    }
    else {

        printf("Error - lexeme is too long\n");

    }

}


//

/* getChar - a function to get the next character of input and determine its character class */

void getChar() {

    if ((nextChar = getc(in_fp)) != EOF) {

        if (isalpha(nextChar))

            charClass = LETTER;

        else if (isdigit(nextChar))

            charClass = DIGIT;

        else

            charClass = UNKNOWN;

    }
    else {

        charClass = EOF_TOKEN;

    }

}


//

/* getNonBlank - a function to call getChar until it returns a non-whitespace character */

void getNonBlank() {

    while (isspace(nextChar))

        getChar();

}


//

/* lex - a simple lexical analyzer for arithmetic expressions */

int lex() {

    lexLen = 0;

    getNonBlank();

    switch (charClass) {

        /* Parse identifiers */

    case LETTER:

        addChar();

        getChar();

        while (charClass == LETTER || charClass == DIGIT) {

            addChar();

            getChar();

        }

        nextToken = IDENT;


        if (strcmp(lexeme, "if") == 0) {

            nextToken = IF_;

        }
        else if (strcmp(lexeme, "else") == 0) {

            nextToken = ELSE_;

        }
        else {

            nextToken = IDENT;

        }


        break;


        /* Parse integer literals */

    case DIGIT:

        addChar();

        getChar();

        while (charClass == DIGIT) {

            addChar();

            getChar();

        }

        nextToken = INT_LIT;

        break;


        /* Parentheses and operators */

    case UNKNOWN:

        lookup(nextChar);

        getChar();

        break;


        /* EOF */

    case EOF_TOKEN:

        nextToken = EOF_TOKEN;

        strcpy(lexeme, "EOF");

        break;

    }


    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);

    return nextToken;

}