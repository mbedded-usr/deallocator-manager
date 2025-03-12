#include <stdio.h>
#include <dealloc_manager.h>
#include <stdlib.h>

int main()
{
   Mem_bundle int_mem_bundle = new_mem_bundle();
   int_mem_bundle.dealloc_func = free;

   int int_count = 8;
   int* x = (int*)Malloc(int_count * sizeof(int), &int_mem_bundle);
   for (int i = 0; i < int_count; ++i)
   {
      x[i] = i;
   }
   for (int i = 0; i < int_count; ++i)
   {
      printf("x[%d]=%d\n", i, x[i]);
   }
   
   float* f_arr = (float*)Malloc(12 * sizeof(float), &int_mem_bundle);
   f_arr[8] = 45.32;
   f_arr[2] = -7.0;

   printf("f_arr[2]= %f  f_arr[8]= %f\n", f_arr[2], f_arr[8]);

   Mem_bundle double_mem_bundle = new_mem_bundle();
   double_mem_bundle.dealloc_func = free;
   double* d_arr = (double*)Malloc(55 * sizeof(double), &double_mem_bundle);
   d_arr[23] = 45.2131321323;
   d_arr[32] = -7777.11111111104;

   printf("d_arr[23]= %f  d_arr[32]= %f\n", d_arr[23], d_arr[32]);
   
   Mem_bundle bundle[12];
   for (int i = 0; i < 7; ++i)
   {
      bundle[i] = new_mem_bundle();
      bundle[i].dealloc_func = free;
   }

   char* y = (char*)Malloc(788 * sizeof(char), &bundle[3]);
   y[424] = 'a';
   y[23] = 'f';
   y[532] = '9';

   printf("y[23]= %c  y[424]= %c  y[532]=%c\n", y[23], y[424], y[532]);
   
   return 0;
}
