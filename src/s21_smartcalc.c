#include "s21_smartcalc.h"

void push(lexems** head, double val, int type, int prior) {
  lexems* buff = malloc(sizeof(lexems));
  if (buff == NULL) exit(STACK_OVERFLOW);
  buff->next = *head;
  buff->val = val;
  buff->type = type;
  buff->prior = prior;
  *head = buff;
}

void pushChar(lexems** head, int val_ascii, int prior) {
  lexems* buff = malloc(sizeof(lexems));
  if (buff == NULL) exit(STACK_OVERFLOW);
  buff->next = *head;
  buff->val = val_ascii;
  buff->prior = prior;
  *head = buff;
}

double popDouble(lexems** pList) {
  if (*pList == NULL) exit(STACK_OVERFLOW);
  lexems* head = *pList;
  double val = head->val;
  *pList = head->next;
  free(head);
  return val;
}

double popInt(lexems** pList) {
  if (*pList == NULL) exit(STACK_OVERFLOW);
  lexems* head = *pList;
  int val = head->val;
  *pList = head->next;
  free(head);
  return val;
}

int getPrior(lexems* head) {
  if (head == NULL) return STACK_UNDERFLOW;
  return head->prior;
}

lexems* reverseStack(lexems* list) {
  lexems* reverse = NULL;
  for (lexems *node = list, *next; node != NULL; node = next) {
    next = node->next;
    node->next = reverse;
    reverse = node;
  }
  return reverse;
}

int checkSymbOnLexem(char* str, int symb) {
  int err = 0;
  for (size_t i = 0; i < strlen(str); i++) {
    if (str[i] == symb) {
      err = 1;
      break;
    }
  }
  return err;
}

int findSymbOnLexem(char symb) {
  int err = 0;
  char* str = "cosintaqrlgmd";
  if (checkSymbOnLexem(str, symb)) err = symb;
  return err;
}

int findDigitOnLexem(char symb) {
  int err = 0;
  char* str = ".,0123456789";
  if (checkSymbOnLexem(str, symb)) err = symb;
  return err;
}

int findAriphSymbOnLexem(char symb) {
  int err = 0;
  char* str = "+-*/()^=";
  if (checkSymbOnLexem(str, symb)) err = symb;
  return err;
}

int findPrior(char symb) {
  int err = 109;
  if (findAriphSymbOnLexem(symb)) {
    if (symb == '*' || symb == '/')
      err = 2;
    else if (symb == '-' || symb == '+')
      err = 1;
    else if (symb == '^')
      err = 4;
    else if (symb == '(' || symb == ')')
      err = 6;
  } else if (findSymbOnLexem(symb)) {
    if (symb == 'x')
      err = 0;
    else
      err = 5;
  } else if (findDigitOnLexem(symb)) {
    err = 0;
  }
  return err;
}

int findMathLexem(char* str) {
  int err = 108;
  if (!strcmp(str, "sin"))
    err = SIN;
  else if (!strcmp(str, "cos"))
    err = COS;
  else if (!strcmp(str, "tan"))
    err = TAN;
  else if (!strcmp(str, "asin"))
    err = ASIN;
  else if (!strcmp(str, "acos"))
    err = ACOS;
  else if (!strcmp(str, "atan"))
    err = ATAN;
  else if (!strcmp(str, "sqrt"))
    err = SQRT;
  else if (!strcmp(str, "ln"))
    err = LN;
  else if (!strcmp(str, "log"))
    err = LOG;
  else if (!strcmp(str, "mod"))
    err = MOD;
  return err;
}

int checkMathLexem(char** str) {
  char buff[MAX_CHAR];
  int j = 0;
  while (**str != '(') {
    if (**str < 97) break;
    buff[j] = **str;
    j++;
    ++(*str);
  }
  buff[j] = '\0';
  int err = findMathLexem(buff);
  if (!err || err == 108) err = 0;
  return err;
}

int validSymb(char symb) {
  int err = 0;
  if (findAriphSymbOnLexem(symb) || findDigitOnLexem(symb) ||
      findSymbOnLexem(symb))
    err = 1;
  return err;
}

int validAriphSymb(char* str) {
  int err = 1, symb = 0, curs = 0;
  for (size_t i = 0; i < strlen(str); i++) {
    symb = str[i];
    if (validSymb(symb)) {
      if (symb == '(' && str[i - 1] != ')') curs++;
      if (symb == ')' && !curs) {
        err = 0;
        break;
      } else if (symb == ')' && curs && str[i - 1] != '(') {
        curs--;
      }
      if (symb == 0 && str[i - 1] == '/') {
        err = 0;
        break;
      }
      if (findDigitOnLexem(symb) && str[i - 1] == ')') {
        err = 0;
        break;
      }
    } else {
      err = 0;
      break;
    }
  }
  if (curs) err = 0;
  if (findAriphSymbOnLexem(str[strlen(str) - 1]) && str[strlen(str) - 1] != ')')
    err = 0;
  return err;
}

void removeDuplicate(char* input, char* output) {
  int j = 0;
  for (size_t i = 0; i < strlen(input); i++) {
    if (!checkSymbOnLexem(output, input[i])) {
      output[j] = (int)input[i];
      j++;
    }
  }
}

int parser(lexems** head, char* str) {
  char *newStr = str, *buff;
  double val = 0;
  int prior = 0;
  int err = validAriphSymb(str);
  if (err) {
    if (newStr[0] == '-') push(head, 0, INTG, 0);
    while (*newStr) {
      buff = newStr;
      if (findDigitOnLexem(*newStr)) {
        prior = findPrior(*newStr);
        val = strtod(newStr, &newStr);
        push(head, val, INTG, prior);
      } else if (findAriphSymbOnLexem(*newStr)) {
        push(head, 0, *newStr, findPrior(*newStr));
        newStr++;
      } else if (findSymbOnLexem(*newStr)) {
        if (*newStr == 'm')
          push(head, 0, ARPH, checkMathLexem(&newStr));
        else
          push(head, 0, 40, checkMathLexem(&newStr));
        if (getPrior(*head) == 2)
          continue;
        else
          newStr++;
      }
      if (val == 0 && buff == newStr) break;
    }
  }
  return err;
}

double calcLogic(lexems** numList, lexems** list) {
  if (*numList == NULL || list == NULL) {
    exit(STACK_UNDERFLOW);
  }
  double firstNum = 0, secondNum = 0, res = 0;
  int mod = 0;
  if (*list != NULL) mod = (*list)->val;
  firstNum = popDouble(numList);
  int symb = popInt(list);
  if (symb == '(' || symb == ')') {
    symb = popInt(list);
    return firstNum;
  } else {
    secondNum = popDouble(numList);
  }
  if (symb == '+')
    res = secondNum + firstNum;
  else if (symb == '-')
    res = secondNum - firstNum;
  else if (symb == '*')
    res = secondNum * firstNum;
  else if (symb == '/')
    res = secondNum / firstNum;
  else if (symb == '^')
    res = pow(secondNum, firstNum);
  else if (mod == 3)
    res = (int)secondNum % (int)firstNum;
  return res;
}

double logicFunc(double x, int symb) {
  double res = 0;
  if (symb == 11) res = sin(x);
  if (symb == 12) res = cos(x);
  if (symb == 13) res = tan(x);
  if (symb == 14) res = asin(x);
  if (symb == 15) res = acos(x);
  if (symb == 16) res = atan(x);
  if (symb == 17) res = sqrt(x);
  if (symb == 18) res = log(x);
  if (symb == 19) res = log10(x);
  return res;
}

double calculate(char* str) {
  lexems *head = NULL, *stack = NULL, *numList = NULL, *list = NULL;
  int prior = 0, resMathLexem = 0, bracket = 0;
  double res = 0;
  if (parser(&head, str)) {
    stack = reverseStack(head);
    while (stack != NULL) {
      if (!stack->prior) {
        push(&numList, stack->val, 0, 0);
      } else if (stack->prior) {
        if (stack->prior > 6) {
          resMathLexem++;
          prior = 0;
        }
        if (stack->prior > prior) {
          if (stack->type == ')') {
            while (list->prior <= prior && list->val != '(') {
              res = calcLogic(&numList, &list);
              push(&numList, res, 0, 0);
              prior = list->prior;
            }
            if (resMathLexem && list->prior > 8) {
              res = logicFunc(popDouble(&numList), list->prior);
              push(&numList, res, 0, 0);
              resMathLexem--;
            }
            prior = 0;
            popInt(&list);
          }
          if (list) {
            if (stack->type == '-' && bracket == '(') push(&numList, 0, 0, 0);
          }
          if (stack->type != ')') pushChar(&list, stack->type, stack->prior);
        } else {
          while (stack->prior <= prior && list->val != '(') {
            res = calcLogic(&numList, &list);
            push(&numList, res, 0, 0);
            if (list == NULL)
              prior = 0;
            else
              prior = list->prior;
          }
          pushChar(&list, stack->type, stack->prior);
        }
        if (stack->type == '(' || (resMathLexem && list->prior > 8)) {
          prior = 0;
        } else {
          if (list != NULL) prior = list->prior;
        }
      }
      bracket = stack->type;
      head = stack;
      free(stack);
      stack = head->next;
    }
    while (list) {
      if (list->prior > 6) {
        res = logicFunc(popDouble(&numList), list->prior);
        popInt(&list);
      } else {
        res = calcLogic(&numList, &list);
      }
      push(&numList, res, 0, 0);
    }
  }
  popDouble(&numList);
  return res;
}
