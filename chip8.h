#ifndef __CHIP_8__
#define __CHIP_8__

#include <stddef.h>

enum chip8_opcodes { 
  c8_noop,		// 0NNN
  c8_screen_clear,	// 00E0
  c8_return,		// 00EE
  c8_goto,		// 1NNN
  c8_call_subroutine,	// 2NNN
  c8_jump_eq_mem,	// 3XNN
  c8_jump_neq_mem,	// 4XNN
  c8_jump_eq_reg,	// 5XY0
  c8_set_reg_mem,	// 6XNN
  c8_add_reg_mem,	// 7XNN
  c8_set_reg_reg,	// 8XY0
  c8_or_reg_reg,	// 8XY1
  c8_and_reg_reg,	// 8XY2
  c8_xor_reg_reg,	// 8XY3
  c8_add_reg_reg,	// 8XY4
  c8_sub_reg_reg,	// 8XY5
  c8_store_lsb,	// 8XY6
  c8_sub_same_reg_reg,	// 8XY7
  c8_store_msb,	// 8XYE
  c8_jump_neq_reg,	// 9XY0
  c8_set_inst_mem,	// ANNN
  c8_set_pc_mem_offset, // BNNN
  c8_set_reg_rand,	// CXNN
  c8_screen_draw,	// DXYN
  c8_jump_eq_key,	// EX8E
  c8_jump_neq_key,	// EXA1
  c8_set_reg_timer,	// FX07
  c8_set_reg_key,	// FX0A
  c8_set_timer_reg,	// FX15
  c8_set_sound_reg,	// FX18
  c8_add_inst_reg,	// FX1E
  c8_set_inst_sprite,	// FX29
  c8_store_bcd,	// FX33
  c8_store_regs,	// FX55
  c8_load_regs		// FX65
};

typedef struct chip8{
  char memory [0x1000];  // 4096 bytes (4KB) of memory
  char registers[0x10];  // 16 1 byte registers
  char display[0x800];   // 2048 bytes (2KB) of display memory
  size_t I;              // instruction pointer
  size_t stack[0x10];    // stack for return pointers, giving it 16 spaces
  char delay_timer;      // 1 byte delay timer
  char sound_timer;      // 1 byte sound timer
} chip8;

#endif //__CHIP_8__
