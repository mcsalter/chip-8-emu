#pragma once

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <assert.h>
#include <stdbool.h>

#define CHIP_8_SCREEN_WIDTH 64 // X
#define CHIP_8_SCREEN_HEIGHT 32 // Y

#define CHIP_8_MEMORY_SIZE 0x1000
#define CHIP_8_RESGISTER_NUM 0x10

#define CHIP_8_FONT_OFFSET 0x50
#define CHIP_8_PROGRAM_OFFSET 0x200
#define CHIP_8_STACK_OFFSET 0x00
#define CHIP_8_STACK_MAX 0x10

#define CHIP_8_INSTRUCTION_LENGTH 34

#define CHP_8_FONT_CHAR_LENGTH 0x5

enum instruction_set{
  NOP,				// 0 -- 0000 ✓
  ERASE,			// 1 -- 00E0 ✓
  RETURN,			// 2 -- 00EE ✓
  GOTO_MEMORY,			// 3 -- 1MMM ✓
  EXECUTE_SUBROUTINE,		// 4 -- 2MMM ✓
  SKIP_EQ_HEX,			// 5 -- 3XNN
  SKIP_NEQ_HEX,			// 6 -- 4XNN
  SKIP_EQ_REG,			// 7 -- 5XY0
  SET_REG_HEX,			// 8 -- 6XNN ✓
  SUM_REG_HEX,			// 9 -- 7XNN ✓
  COPY_REG_REG,			// 10 -- 8XY0
  LOGIC_OR_REG_REG,		// 11 -- 8XY1
  LOGIC_AND_REG_REG,		// 12 -- 8XY2
  LOGIC_XOR_REG_REG,		// 13 -- 8XY3
  SUM_REG_REG,			// 14 -- 8XY4
  SUB_REG_REG,			// 15 -- 8XY5
  STOR_REG_REG,			// 16 -- 8XY6
  SET_SUB_REG,			// 17 -- 8XY7
  SKIP_NEQ_REG,			// 18 -- 9XY0
  SET_I_ADDR,			// 19 -- AMMM ✓
  JMP_ADDR_REG,			// 20 -- BMMM
  LOGIC_AND_RAND_HEX,		// 21 -- CXNN
  DISPLAY_XY_N,		        // 22 -- DXYN ✓
  SKIP_EQ_KEY_REG,		// 23 -- EX9E
  SKIP_NEQ_KEY_REG,		// 24 -- EXA1
  SET_REG_TIMER,		// 25 -- FX07
  SET_REG_KEYDOWN,		// 26 -- FX0A
  SET_TIMER_REG,		// 27 -- FX15
  SET_SOUND_REG,		// 28 -- FX17
  SUM_I_REG,			// 29 -- FX1E
  SET_I_DSP_REG_LSDIGIT,	// 30 -- FX29
  STOR_REG_BCD,			// 31 -- FX33
  STOR_REG_I,			// 32 -- FX55
  LOAD_REG_I,			// 33 -- FX65
  INSTRUCTION_SET_LENGTH,       // 34 -- 34 instructions total
};

struct Operation{
  enum instruction_set instruction;
  uint16_t arguments[0x3];
};

struct CHIP_8{
  // memory related variables:
  
  // the system memory
  uint8_t Memory[CHIP_8_MEMORY_SIZE];

  // stack pointer, where programs are stored when exiting to an earlier program
  uint8_t stack_pointer;

  // program counter, this increments on every instruction
  uint16_t program_counter;
  
  // memory pointer, points to specified locations in memory  
  uint16_t I;

  // registers:
  // 16 registers that the system uses
  uint8_t registers[CHIP_8_RESGISTER_NUM];


  // timers:
  // timers must iterate from their set time down to 0, these decrese at 60 Hz
  
  // delay timer, something happens when timer_delay == 0
  uint8_t timer_delay;
  
  // sound timer, something plays once timer_sound == 0
  uint8_t timer_sound;

  // IO:
  
  // screen data array
  // now that the graphics should be able to support 8 bit colour, it cannot be treated as a boolean array
  uint8_t graphics[CHIP_8_SCREEN_HEIGHT][CHIP_8_SCREEN_WIDTH];
  
  // value that stores the keypress from keyboard
  uint8_t keypress;

  uint16_t current_instruction[4];

};

int bitshift_MMM(uint16_t arguments[0x3]);
int bitshift_MM(uint16_t arguments[0x3]);

int lexer(struct CHIP_8 *cpu, struct Operation *Op);

void initialize_chip8(struct CHIP_8 *cpu);

void fetch(struct CHIP_8 *cpu);

void execute(struct CHIP_8 *cpu, struct Operation *op);

void cpu_tick(struct CHIP_8 *cpu);
