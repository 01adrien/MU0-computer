#include "includes/cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void set_flag(cpu_t* cpu);
char* skip_blank(char* source);
uint32_t sign_extend(uint32_t n);

void init_cpu(cpu_t* cpu)
{
  for (int i = 0; i < MEM_SIZE; i++) {
    cpu->ram[i] = 0;
    if (i < REG_COUNT) {
      cpu->reg[i] = 0;
    }
  }
  cpu->acc = 0;
  cpu->reg[PC] = INSTRUCTION;
}

void print_ram(cpu_t* cpu)
{
  for (int i = 1; i != MEM_SIZE; i++) {
    printf("%08x ", cpu->ram[i - 1]);
    if (i % 10 == 0) {
      printf("\n");
    }
  }
  printf("\n");
}

void load_ram(char* source, cpu_t* cpu)
{
  uint32_t counter = 0;
  while (*source != '\0') {
    char instruction[9];
    source = skip_blank(source);
    strncpy(instruction, source, 8);
    cpu->ram[counter++] = strtol(instruction, NULL, 16);
    source += 9;
  }
}

cpu_status_t run_cpu(cpu_t* cpu)
{
  for (;;) {

    cpu->reg[IR] = cpu->ram[cpu->reg[PC]++];
    uint32_t address = cpu->reg[IR] & 0x0fffffff;
    opcode_t opcode = cpu->reg[IR] >> 28;
    switch (opcode) {
    case LDA:
      cpu->acc = cpu->ram[address];
      break;
    case LEA:
      uint32_t add = cpu->ram[address];
      cpu->acc = cpu->ram[add];
      break;
    case LDI:
      cpu->acc = sign_extend(address);
      set_flag(cpu);
      break;
    case STR:
      cpu->ram[address] = cpu->acc;
      break;
    case STA:
      uint32_t addr = cpu->ram[address];
      cpu->ram[addr] = cpu->acc;
      break;
    case ADD:
      cpu->acc += cpu->ram[address];
      set_flag(cpu);
      break;
    case OR:
      cpu->acc |= cpu->ram[address];
      set_flag(cpu);
      break;
    case NOT:
      cpu->acc = ~cpu->ram[address];
      set_flag(cpu);
      break;
    case SUB:
      cpu->acc += ~cpu->ram[address] + 1;
      set_flag(cpu);
      break;
    case AND:
      cpu->acc &= cpu->ram[address];
      set_flag(cpu);
      break;
    case JMP:
      cpu->reg[PC] = address;
      break;
    case JEZ:
      cpu->reg[PC] = cpu->reg[FLAGS] == Z ? address : cpu->reg[PC];
      break;
    case JGZ:
      cpu->reg[PC] = cpu->reg[FLAGS] == P ? address : cpu->reg[PC];
      break;
    case JNE:
      cpu->reg[PC] = cpu->reg[FLAGS] == N ? address : cpu->reg[PC];
      break;
    case HLT:
      return SUCCES;
    default:
      printf("UNKNOW OPCODE [%d]\n", opcode);
      return ERROR_UNKNOW_OPCODE;
    }
  }
}

void debug_cpu(cpu_t* cpu)
{
  printf("content ACC: %d\n", cpu->acc);
  printf("content IR: %d\n", cpu->reg[IR]);
  printf("content PC: %d\n", cpu->reg[PC]);
  switch (cpu->reg[FLAGS]) {
  case P:
    printf("FLAG: pos");
    break;
  case N:
    printf("FLAG: neg");
    break;
  case Z:
    printf("FLAG: zero");
    break;
  }
  printf("\n\n");
}

void set_flag(cpu_t* cpu)
{
  flag_t flag = P;
  if (cpu->acc == 0) {
    flag = Z;
  }
  else if (cpu->acc >> 31) {
    flag = N;
  }
  cpu->reg[FLAGS] = flag;
}

char* skip_blank(char* source)
{
  while (*source == '\n' || *source == ' ') {
    source++;
  }
  return source;
}

uint32_t sign_extend(uint32_t n)
{
  return n >> 27 ? n |= 0xf0000000 : n;
}
