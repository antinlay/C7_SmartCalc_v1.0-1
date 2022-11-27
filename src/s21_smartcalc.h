#ifndef SRC_S21_SMART_CALC_H_
#define SRC_S21_SMART_CALC_H_

#define MAX_CHAR 255

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INTG 1
#define ARPH 3

#define SIN 11
#define COS 12
#define TAN 13

#define ASIN 14
#define ACOS 15
#define ATAN 16
#define SQRT 17

#define LN 18
#define LOG 19
#define MOD 2

// errors
#define STACK_OVERFLOW -100
#define STACK_UNDERFLOW -101

typedef struct lexem {
  double val;
  int type;
  int prior;
  struct lexem* next;
} lexems;

// STACK FUNC
void push(lexems** head, double val, int type, int prior);
void pushChar(lexems** head, int val_ascii, int prior);
double popDouble(lexems** pList);
double popInt(lexems** pList);
int getPrior(lexems* head);
lexems* reverseStack(lexems* list);
// VALIDATION FUNC
int checkSymbOnLexem(char* str, int symb);
int findSymbOnLexem(char symb);
int findDigitOnLexem(char symb);
int findAriphSymbOnLexem(char symb);
int findPrior(char symb);
int findMathLexem(char* str);
// CHECK FUNC
int validSymb(char symb);
int validAriphSymb(char* str);
void removeDuplicate(char* input, char* output);
// PARSE FUNC
int checkMathLexem(char** str);
int parser(lexems** head, char* str);
// CALCULATE
double calcLogic(lexems** numList, lexems** list);
double calculate(char* str);
#endif  // SRC_S21_SMART_CALC_H_
