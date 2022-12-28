#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

bool check_parentheses(int p, int q);
int eval(int p, int q);

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
  printf("check_parentheses(0,nr_token-1):%d\n",check_parentheses(0,nr_token-1));
  printf("begin evaling-------------\n");
  eval(0,nr_token-1);
  printf("eval(0,nr_token-1):%d\n",eval(0,nr_token-1));
  //eval(0,0);
  printf("------------------\n");

  return true;
}

bool check_parentheses(int p, int q){

	int tokens_containers[q-p+1];
	int braket_containers[q-p+1];
	int par_num = 0;
	
	//printf("in the function\n");
	//printf("%d\n",tokens[0].type);
	
	for (int i = 0; i < q-p+1; i++){
	    tokens_containers[i] = tokens[i+p].type;
	    
	    //printf("tokens[i+p].type:%d\n",tokens[i+p].type);
	    //printf("tokens_containers[i]:%d\n",tokens_containers[i]);
	    /*printf("tokens[i+p%d].type:%c\n",i+p,tokens[i+p].type);
	  if (tokens[i+p].type == '(' || tokens[i+p].type == ')'){
	    tokens_containers[par_num] = tokens[i+p].type;
	    par_num++;
	    printf("%d\n",par_num);
	  }*/
          //tokens_containers[i] = tokens[i+p].type; 
          
          }
         // for (int j = 0; j < q-p+1; j++){
         //    printf("%c",tokens_containers[j]);
          //}
          //printf("\n");
          
        for (int i = 0; i <q-p+1; i++){
           if (tokens_containers[i] == BRA || tokens_containers[i] == KET){
              braket_containers[par_num] =  tokens_containers[i];
              par_num ++;
           }
        }
        
        printf("-------braket_containers-------:%d\n",par_num);
        for (int j = 0; j < par_num; j ++){
            printf("%d\n",braket_containers[j]);
        }
        printf("-------braket_containers-------\n");
        // check the legality of EXPRESSION
          if (par_num >0){
            if (par_num % 2 == 0){
               int *begin = braket_containers;
               int *end = braket_containers + par_num - 1;
               //printf("begin:%p,end:%p\n",begin, end);
               while (begin < end){
              	      if (*begin == BRA && *end == KET){
              	         printf("begin:%p,end:%p\n",begin, end);
              	         begin ++;
              	         end --;
              	      }
              	      else
              	       return false;
               }
               // check the surrounding braket
               if (tokens_containers[0] == BRA && tokens_containers[q-p] == KET){
                 return true;
               }
               else
                 return false;
               
            }
            
	  }
	  return false;
	
}

int eval(int p, int q){
	Token *begin = tokens + p;
	Token *end = tokens +q;
	printf("tokens begin:%p;\tend:%p;\t%ld\n", begin, end, end - begin + 1);
	if (p > q) {
	  printf("bad expression!\n");
	  return -1;
	}
	else if (p == q && tokens[p].type != TK_NOTYPE){
	  printf("NUM:%d\n",atoi(tokens[p].str));
	  assert(0);
	  return atoi(tokens[p].str);
	}
	else if (check_parentheses(p,q) == true){
	  //printf("p+1:%d\tq-1:%d", p+1, q-1);
	  //assert(0);
	  return eval(p + 1, q - 1);
	}
	else{
	  //assert(0);
	  int op = 0;
	  int op_index = 0;
	  int op_f = 0;
	  int op_b = 0;
	  int front_flag = 1;
	  int back_flag = 1;
	  int rank[q-p+1];
	  for (int i = 0; i < q- p + 1; i++){
	      rank[i] = 0;
	  }
	  // find the main operator
	  for (int i = 0; i < q - p + 1; i ++){
	    if (tokens[p+i].type == '('){
	       front_flag = 0;
	    }
	    if (front_flag){
	      if (tokens[p+i].type != NUM){
	        op_f = tokens[p+i].type;
	        switch (op_f){
	          case '+': rank[i] = 2; break;
	          case '-': rank[i] = 2; break;
	          case '*': rank[i] = 1; break;
	          case '/': rank[i] = 1; break;
	        }
	      }
	    }
	    
	    if (tokens[q-i].type == ')'){
	       back_flag = 0;
	    }
	    if (back_flag){
	      if (tokens[q-i].type != NUM){
	        op_b = tokens[q+i].type;
	        switch (op_b){
	          case '+': rank[q-p-i] = 2; break;
	          case '-': rank[q-p-i] = 2; break;
	          case '*': rank[q-p-i] = 1; break;
	          case '/': rank[q-p-i] = 1; break;
	        }
	      }
	    }
	    
	      
	    
	  }
	  printf("----rank-------\n");
	  for (int i = 0; i < q-p+1; i++){
	      printf("%d\n",rank[i]);
	  }
	  for (int i = 0; i < q-p+1; i++){
	      if (rank[q-p-i] > op){
	      	op = rank[q-p-i];
	      	op_index = q-p-i+p;
	      }
	  }
	  printf("op_index:%d\n",op_index);
	  //assert(0);
	  int val1 = eval(p, op_index - 1);
	  //assert(0);
	  int val2 = eval(op_index + 1, q);
	  //assert(0);
	  switch (tokens[op_index].type){
	    case '+': return val1 + val2;
	    case '-': return val1 - val2;
	    case '*': return val1 * val2;
	    case '/': return val1 / val2;
	  }
	}
	return 0;
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
