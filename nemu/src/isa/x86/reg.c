#include "nemu.h"
#include <stdlib.h>
#include <time.h>

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void reg_test() {
  srand(time(0));
  uint32_t sample[8];
  uint32_t pc_sample = rand();
  cpu.pc = pc_sample;

  int i;
  for (i = R_EAX; i <= R_EDI; i ++) {
    sample[i] = rand();
    reg_l(i) = sample[i];
    assert(reg_w(i) == (sample[i] & 0xffff));
  }

  assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
  assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
  assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
  assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
  assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
  assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
  assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
  assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

  assert(sample[R_EAX] == cpu.eax);
  assert(sample[R_ECX] == cpu.ecx);
  assert(sample[R_EDX] == cpu.edx);
  assert(sample[R_EBX] == cpu.ebx);
  assert(sample[R_ESP] == cpu.esp);
  assert(sample[R_EBP] == cpu.ebp);
  assert(sample[R_ESI] == cpu.esi);
  assert(sample[R_EDI] == cpu.edi);

  assert(pc_sample == cpu.pc);
}

void isa_reg_display() {
  printf("%%eax: 0x%-12x",cpu.eax);
  printf("%%ax: 0x%-12x",cpu.eax & 0xffff);
  printf("%%ah: 0x%-12x",(cpu.eax >> 8) & 0xff);
  printf("%%al: 0x%-12x",cpu.eax & 0xff);
  
  printf("%%ebx: 0x%-8x\t",cpu.ebx);
  printf("%%bx: 0x%-8x\t",cpu.ebx & 0xffff);
  printf("%%bh: 0x%-8x\t",(cpu.ebx >> 8) & 0xff);
  printf("%%bl: 0x%-8x\n",cpu.ebx & 0xff);
  
  printf("%%ecx: 0x%-8x\t",cpu.ecx);
  printf("%%cx: 0x%-8x\t",cpu.ecx & 0xffff);
  printf("%%ch: 0x%-8x\t",(cpu.ecx >> 8) & 0xff);
  printf("%%cl: 0x%-8x\n",cpu.ecx & 0xff);
  
  printf("%%edx: 0x%-8x\t",cpu.edx);
  printf("%%dx: 0x%-8x\t",cpu.edx & 0xffff);
  printf("%%dh: 0x%-8x\t",(cpu.edx >> 8) & 0xff);
  printf("%%dl: 0x%-8x\n",cpu.edx & 0xff);
  
  printf("%%ebp: 0x%-8x\t",cpu.ebp);
  printf("%%bp: 0x%-8x\t",cpu.ebp & 0xffff);
  printf("%%bph: 0x%-8x\t",(cpu.ebp >> 8) & 0xff);
  printf("%%bpl: 0x%-8x\n",cpu.ebp & 0xff);
}

uint32_t isa_reg_str2val(const char *s, bool *success) {
  return 0;
}
