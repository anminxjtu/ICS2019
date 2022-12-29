#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int bit_convert(char *hex_bit);

int hex2dec(char *input_hex)
{
   // input_hex = "0x1234";
   int hex_len = strlen(input_hex);
   int dec = 0;
   int pow = 1;
   for (int i = 0; i < hex_len-2; i++){
     dec += pow * bit_convert(input_hex + hex_len -1 - i);
     pow *= 16;
     * (input_hex + hex_len -1 - i) = '\0';
   }
   printf("dec is-------:%d\n",dec);
   return dec;
}

int bit_convert(char *hex_bit){
  if (*hex_bit >= 48 && *hex_bit <=57) // ascii code 0:48; 9:57
    return atoi(hex_bit);
  switch (*hex_bit){
    case 'a': return 10;
    case 'b': return 11;
    case 'c': return 12;
    case 'd': return 13;
    case 'e': return 14;
    case 'f': return 15;
  }
  assert(0);
  return -1;
}
