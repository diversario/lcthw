#ifndef _ex22_h
#define _ex22_h

// makes THE_SIZE in ex22.c available to other .c files
extern int THE_SIZE;

// get and sets and internal static var in ex22.c
int get_age();
void set_age(int age);

double update_ratio(double ratio);

void print_size();

#endif
