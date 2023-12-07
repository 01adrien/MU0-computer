#include "src/includes/cpu.h"
#include "src/includes/utils.h"
#include <stdio.h>

int main(int argc, char const* argv[])
{
  cpu_t cpu;
  init_cpu(&cpu);
  char* source = read_file(argv[1]);
  load_ram(source, &cpu);
  // print_ram(&cpu);
  run_cpu(&cpu);
  printf("\n\n");
  debug_cpu(&cpu);
  print_ram(&cpu);
  free(source);
  return 0;
}
