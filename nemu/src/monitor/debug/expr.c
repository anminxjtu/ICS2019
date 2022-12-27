#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

bool check_parentheses(int p, int q);

enum {
  TK_NOTYPE = 256, TK_EQ, LETTER, NUM, BRA, KET

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {"[\\[d]]", 0},
  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"\\-", '-'},
  {"\\*", '*'},
  {"/",'/'},
  {"==", TK_EQ},        // equal
 
  {"[(]", BRA},
  {"[)]", KET},
  {"[d]", 5},
  {"[a-z]+", LETTER},
  {"[\\d+]", 8},
  {"[0-9]+", NUM}

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
  //char *str;
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
         
        tokens[nr_token].type = rules[i].token_type;
	printf("%d\n",tokens[nr_token].type);
        switch (rules[i].token_type) {
          //default: TODO();
          case NUM:
          	//char *p_temp;
          	//p_temp = &tokens[nr_token].str
          	//tokens[nr_token].str = *substr_start;
          	//tokens[nr_token].str = substr_start;
          	//printf("str:%.*s\n",substr_len,tokens[nr_token].str);	// printf 神奇用法
          	
          	for (i=0; i < substr_len; i++){
                  //tokens[nr_token].str[i] = substr_start[i];
                  tokens[nr_token].str[i] = * (substr_start+i);
                  //printf("%c",tokens[nr_token].str[i]);
          	}
          	//printf("\n");
          	break;
        }
	nr_token++;
        break;
      }
      
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
  
  check_parentheses(0,nr_token);
  printf("------------------\n");

  return true;
}

bool check_parentheses(int p, int q){

	char tokens_containers[q-p+1];
	int par_num = 0;
	printf("in the function\n");
	for (int i = 0; i < q-p+1; i++){
	    printf("tokens[i+p].type:%c\n",tokens[i+p].type);
	  if (tokens[i+p].type == '(' || tokens[i+p].type == ')'){
	    tokens_containers[par_num] = tokens[i+p].type;
	    par_num++;
	    printf("%d\n",par_num);
	  }
          //tokens_containers[i] = tokens[i+p].type; 
          
          }
          for (int j = 0; j <par_num; j++){
             printf("%c",tokens_containers[j]);
          }
          printf("\n");
          if (par_num >0){
            return true;
	  }
	  return false;
	
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  //TODO();

  return 0;
}
