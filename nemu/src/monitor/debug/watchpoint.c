#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

int new_wp(char *expression) {
  WP *new_head = NULL;
  WP *new_free = NULL;
  new_free = free_ -> next;
  free_ -> next = head;
  new_head = free_;
  new_head -> expr = expression;
  head = new_head;
  free_ = new_free;
  //head -> expr = expression;
  printf("%s\n",head -> expr);
  printf("watchpoint %d has been set!\n",head -> NO);
  return head -> NO;
  
  
  /*printf("------------------\n");
  WP *temp = NULL;
  temp = head;
  head = free_; // new wp is set as head
  printf("head:%p,free_:%p\n",head,free_);
  //assert(0);
  printf("head -> next:%p\ttemp:%p\n",head -> next,temp);
  WP *temp2 = NULL;
  temp2 = free_ -> next; // allocate the first point of free
  head -> next = temp; // head free_ : same !!!!!
  printf("temp:%p\n",temp2);
  free_ = temp2;
  printf("temp2:%p\n",free_);
  printf("watchpoint %d has been set!\n",head -> NO);
  return head -> NO;*/
}

void free_wp(int no) {
  WP *wp = NULL; // p to no
  for (int i = 0; i < NR_WP; i ++) {
    wp = &wp_pool[i];  // only & , not (wp_pool + i)
    if ((wp -> NO) == no)
      break;
  }

  //WP *temp = NULL;
  WP *p2wp = NULL;
  p2wp = head;
  if (head == wp){ // special situation 
    WP *new_head = NULL;
    WP *new_free = NULL;
    new_head = head -> next;
    new_free = head;
    new_free -> next = free_;
    free_ = new_free;
    head = new_head;
    printf("watchpoint %d has been deleted!\n",no);
  }
  else{
    while (true){
      printf("p2wp -> next:%p\twp:%p\n",p2wp -> next,wp);
      if ((p2wp -> next) == wp){
         break;
      }
      else {
       p2wp = p2wp -> next;  // find the p to wp
      }
    }
    p2wp -> next = wp -> next; // delete the wp from head
    wp -> next = free_;
    free_ = wp;  // set wp as first of free
    printf("watchpoint %d has been deleted!\n",no);
  }
}

void display_wp(){
  WP *read_wp = NULL;
  read_wp = head;
  if (read_wp == NULL){
    printf("No watching point!\n");
    return;
  }
  printf("Num\t\tExpress\t\tValue\n");
  while (read_wp -> next != NULL){
    printf("%d\t%s\n", read_wp -> NO, read_wp -> expr);
    read_wp = read_wp -> next;
  }
  printf("%d\t%s\n", read_wp -> NO, read_wp -> expr);
} 
  
  
 
