#ifndef _CPU_H_
#define _CPU_H

#include "../../../const.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
  IR,
  PC,
  FLAGS,
  REG_COUNT
} register_t;

typedef enum cpu_status {
  ERROR_UNKNOW_OPCODE,
  SUCCES
} cpu_status_t;

typedef enum flag {
  P,
  Z,
  N
} flag_t;

typedef enum opcode {
  LDA,
  STR,
  ADD,
  SUB,
  JMP,
  LDI,
  JEZ,
  OR,
  NOT,
  JGZ,
  JNE,
  LEA,
  STA,
  AND,
  HLT = 0b1111
} opcode_t;

typedef struct cpu {
  uint32_t ram[MEM_SIZE];
  uint32_t reg[REG_COUNT];
  uint32_t acc;
} cpu_t;

void init_cpu(cpu_t* cpu);
void print_ram(cpu_t* cpu);
void load_ram(char* source, cpu_t* cpu);
cpu_status_t run_cpu(cpu_t* cpu);
void debug_cpu(cpu_t* cpu);

#endif	 // !