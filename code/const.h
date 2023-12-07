#ifndef _CONST_H_
#define _CONST_H_

#define MEM_SIZE    300
#define MAX_FILE    MEM_SIZE
#define STACK_P     0
#define LOCAL_P     1
#define ARGS_P      2
#define THIS_P      3
#define THAT_P      4
#define TEMP        5   // address first temp
#define R0          13
#define R1          14
#define R2          15
#define ZERO        16
#define ONE         17
#define STATIC      18    // address first static
#define LOCAL       30    // address first local
#define ARGS        50    // address first args
#define STACK       70    // address first stack
#define INSTRUCTION 100   // address first instruction

// temp segment:   5  => 12
// static segment: 18 => 29

#endif