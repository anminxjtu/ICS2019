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

int new_wp() {
  printf("------------------\n");
  WP *temp = NULL;
  temp = head;
  head = free_; // new wp is set as head
  printf("head:%p,free_:%p\n",head,free_);
  //assert(0);
  (head -> next) = temp;
  printf("temp:%p\n",temp);
  WP *temp2 = NULL;
  temp2 = free_ -> next; // allocate the first point of free
  printf("temp:%p\n",temp2);
  free_ = temp2;
  printf("temp:%p\n",free_);
  printf("watchpoint %d has been set!\n",head -> NO);
  return head -> NO;
}

void free_wp(int no) {
  WP *wp = NULL; // p to no
  for (int i = 0; i < NR_WP; i ++) {
    wp = wp_pool + i;
    if ((wp -> NO) == no)
      break;
    else
      assert(0);
  }

  //WP *temp = NULL;
  WP *p2wp = head;
  while (true){
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
