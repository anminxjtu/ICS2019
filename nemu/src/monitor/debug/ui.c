#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
void isa_reg_display();
uint32_t instr_fetch(vaddr_t *pc, int len);
uint32_t paddr_read(paddr_t addr, int len);
char* hex_dec(char *input);
uint32_t expr(char *e, bool *success);
int new_wp(char *expression);
void free_wp(int no);
void display_wp();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args){
  if(args == NULL){
    cpu_exec(1);
  }
  else{
    cpu_exec(atoi(args));
  }
  return 0;
}

static int cmd_info(char *args){
  if(strcmp(args, "r") == 0){
    isa_reg_display();
  }
  else if(strcmp(args, "w") == 0){
    display_wp();
    return 0;
  }
  else{
    printf("No such command!\n");
  }
  return 0;
}

static int cmd_x(char *args){
  char *N = strtok(args, " ");
  if (N == NULL) { printf("Error occurs when anlysis cmd_x!\n");
    return 0; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *expr = N + strlen(N) + 1;
    /*if (expr >= str_end) {
      expr = NULL;
    }*/
    //char *a = '\0';
    //(expr>=a){
   // printf("expr:%s\n",expr);}
    /* cpu.pc = atoi(expr);
    for(int i = 1;i <= atoi(N);i++){
      uint32_t opcode = instr_fetch(cpu.pc, 1);
      decinfo.opcode = opcode;
      cpu.pc++;
      printf("inst:%x",cpu.pc);
    }*/
    //printf("atoi_expr:%d\n",atoi(expr));
    int expr_value = 0;
    char *expr_decimal = expr+2;
    int addr_index = atoi(expr_decimal);
    // printf("%s\n",expr_decimal);
    
    sscanf(expr,"%x",&expr_value);
     printf("expr_value:%d\n",expr_value);
    // printf("atoi_N:%d\n",atoi(N));
    for(int i = 1;i <=atoi(N);i++){
    	printf("0x%d -> 0x%d:\t",addr_index + 4,addr_index);
    	printf("%08x\n",paddr_read(expr_value, 4));
    	expr_value = expr_value + 4;
    	addr_index = addr_index + 4;
    	//hex_dec(expr_decimal);
    }
    
  return 0;
}

static int cmd_p(char *args){
  /* extract the first argument */
  //char *arg = strtok(NULL, " ");
  printf("%s\n",args);
  bool *success = false;
  expr(args, success);
  
  return 0;
}

static int cmd_w(char *args){
  printf("args:%s\n", args);
  new_wp(args);
  return 0;
}

static int cmd_d(char *args){
  free_wp(atoi(args));
  return 0;
}

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Continue N step", cmd_si },
  { "info", "Query the program", cmd_info},
  { "x", "Scan DRAM", cmd_x},
  { "p", "Evaluate the EXPR", cmd_p},
  { "w", "Set the watchpoint", cmd_w},
  { "d", "Delete the watchpoint", cmd_d},

  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
