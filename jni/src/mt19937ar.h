#pragma once

extern void init_genrand(unsigned long s);
extern void init_by_array(unsigned long init_key[], int key_length);
extern unsigned long genrand_int32(void);
extern long genrand_int31(void);
extern double genrand_real1(void);
extern double genrand_real2(void);
extern double genrand_real3(void);
extern double genrand_res53(void);
