#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char expr[32]; // char *expr 会出现奇怪错误  expr[32] and strcpy
  int expr_value;

} WP;

#endif
