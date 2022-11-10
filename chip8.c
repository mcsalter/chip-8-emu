#include "chip8.h"
const uint8_t fonts[0x10][5] =
  {
    {0xF0, 0x90, 0x90, 0x90, 0xF0}, // 0
    {0x20, 0x60, 0x20, 0x20, 0x70}, // 1
    {0xF0, 0x10, 0xF0, 0x80, 0xF0}, // 2
    {0xF0, 0x10, 0xF0, 0x10, 0xF0}, // 3
    {0x90, 0x90, 0xF0, 0x10, 0x10}, // 4
    {0xF0, 0x80, 0xF0, 0x10, 0xF0}, // 5
    {0xF0, 0x80, 0xF0, 0x90, 0xF0}, // 6
    {0xF0, 0x10, 0x20, 0x40, 0x40}, // 7
    {0xF0, 0x90, 0xF0, 0x90, 0xF0}, // 8
    {0xF0, 0x90, 0xF0, 0x10, 0xF0}, // 9
    {0xF0, 0x90, 0xF0, 0x90, 0x90}, // A
    {0xE0, 0x90, 0xE0, 0x90, 0xE0}, // B
    {0xF0, 0x80, 0x80, 0x80, 0xF0}, // C
    {0xE0, 0x90, 0x90, 0x90, 0xE0}, // D
    {0xF0, 0x80, 0xF0, 0x80, 0xF0}, // E
    {0xF0, 0x80, 0xF0, 0x80, 0x80}  // F
  };


int bitshift_MMM(uint16_t arguments[0x3]){
  int retval = 0;
  retval += arguments[0] << 0x8;
  retval += arguments[1] << 0x4;
  retval += arguments[2];
  return retval;
}

int bitshift_MM(uint16_t arguments[0x3]){
  int retval = 0;
  retval += arguments[1] << 0x4;
  retval += arguments[2];
  return retval;
}

// initializes the chip_8 cpu
void initialize_chip8(struct CHIP_8 *cpu){
  // zero-initialize the memory
  for(int offset = 0; offset < CHIP_8_MEMORY_SIZE; ++offset){
    cpu->Memory[offset] = 0x0;
  }
  // setting character fonts
  for(int character = 0; character < 0x10; ++character){
    for (int row = 0; row < 5; ++row){
      cpu->Memory[CHIP_8_FONT_OFFSET + (character * 5) + row] = fonts[character][row];
    }
  }
  // zero-initialize the registers
  for(int reg = 0; reg < CHIP_8_RESGISTER_NUM; ++reg){
    cpu->registers[reg] = 0;
  }
  // sets the necessary information to 0/default
  cpu->timer_delay = 0;
  cpu->timer_sound = 0;
  cpu->program_counter = CHIP_8_PROGRAM_OFFSET;
  cpu->stack_pointer = 0;
  cpu->I = 0;
  cpu->keypress = 0;

  
  cpu->Memory[CHIP_8_PROGRAM_OFFSET + 0x0] =  0x00;
  cpu->Memory[CHIP_8_PROGRAM_OFFSET + 0x1] =  0xE0; // clear screen
  cpu->Memory[CHIP_8_PROGRAM_OFFSET + 0x2] =  0xA0;
  cpu->Memory[CHIP_8_PROGRAM_OFFSET + 0x3] =  0x87; // point I addr 087 - B
  cpu->Memory[CHIP_8_PROGRAM_OFFSET + 0x4] =  0x61;
  cpu->Memory[CHIP_8_PROGRAM_OFFSET + 0x5] =  0x05; // set reg 1 = 05
  cpu->Memory[CHIP_8_PROGRAM_OFFSET + 0x6] =  0xD0;
  cpu->Memory[CHIP_8_PROGRAM_OFFSET + 0x7] =  0x05; // draw to 0,0
  cpu->Memory[CHIP_8_PROGRAM_OFFSET + 0x8] =  0xA0;
  cpu->Memory[CHIP_8_PROGRAM_OFFSET + 0x9] =  0x50; // point I addr 050 - 0
  cpu->Memory[CHIP_8_PROGRAM_OFFSET + 0xA] =  0xD1;
  cpu->Memory[CHIP_8_PROGRAM_OFFSET + 0xB] =  0x05; // draw to 0,5
}


void fetch(struct CHIP_8 *cpu){
  cpu->current_instruction[0] = 0;
  cpu->current_instruction[1] = 0;
  cpu->current_instruction[2] = 0;
  cpu->current_instruction[3] = 0;
  int command_first_half = 0;
  int command_second_half = 0;
  int bitmask = 0b00001111;
  if(cpu->program_counter < 0){
    assert(false);
  }
  if(cpu->program_counter > CHIP_8_MEMORY_SIZE ){
    assert(false);
  }
  
  command_first_half = cpu->Memory[cpu->program_counter];
  cpu->program_counter++;
  command_second_half = cpu->Memory[cpu->program_counter];
  cpu->program_counter++;

  cpu->current_instruction[0] = (command_first_half >> 4) & bitmask;
  cpu->current_instruction[1] = (command_first_half) & bitmask;
  cpu->current_instruction[2] = (command_second_half >> 4) & bitmask;
  cpu->current_instruction[3] = (command_second_half) & bitmask;
  
}

/*
 * parses the opcodes into an operation struct
 * uses 0xFFFF to specify unused argumet in Op
 * returns 0 if success.
 */
int lexer(struct CHIP_8 *cpu, struct Operation *Op){
  int op_type = cpu->current_instruction[0];
  int arg_1 = cpu->current_instruction[1];
  int arg_2 = cpu->current_instruction[2];
  int arg_3 = cpu->current_instruction[3];
  
  Op->arguments[0] = 0xFFFF;
  Op->arguments[1] = 0xFFFF;
  Op->arguments[2] = 0xFFFF;

  assert(INSTRUCTION_SET_LENGTH == CHIP_8_INSTRUCTION_LENGTH);
  // time for nested switch cases, yay!
  switch(op_type){
  case 0:
    if (arg_1 == 0 && arg_2 == 0 && arg_3 == 0){
      Op->instruction = NOP;			// 0000
    } else if(arg_1 == 0 && arg_2 == 0xE && arg_3 == 0){
      Op->instruction = ERASE;			// 00E0
    } else if (arg_1 == 0 && arg_2 == 0xE && arg_3 == 0xE){
      Op->instruction = RETURN;			// 00EE
    } else {
      fprintf(stderr,"WARN: unknown instruction %d %d %d %d, mem: %d\n", cpu->current_instruction[0], cpu->current_instruction[1], cpu->current_instruction[2], cpu->current_instruction[3], cpu->program_counter);
      assert(false);
    }
    break;
  case 1:
    Op->instruction = GOTO_MEMORY;		// 1MMM
    Op->arguments[0] = arg_1;
    Op->arguments[1] = arg_2;
    Op->arguments[2] = arg_3;
    break;
  case 2:
    Op->instruction = EXECUTE_SUBROUTINE;	// 2MMM
    Op->arguments[0] = arg_1;
    Op->arguments[1] = arg_2;
    Op->arguments[2] = arg_3;
    break;
  case 3:
    Op->instruction = SKIP_EQ_HEX;		// 3XKK
    Op->arguments[0] = arg_1;
    Op->arguments[1] = arg_2;
    Op->arguments[2] = arg_3;
    break;
  case 4: 
    Op->instruction = SKIP_NEQ_HEX;		// 4XKK
    Op->arguments[0] = arg_1;
    Op->arguments[1] = arg_2;
    Op->arguments[2] = arg_3;
    break;
  case 5:
    Op->instruction = SKIP_EQ_REG;		// 5XY0
    Op->arguments[0] = arg_1;
    Op->arguments[1] = arg_2;
    break;
  case 6:
    Op->instruction = SET_REG_HEX;		// 6XKK
    Op->arguments[0] = arg_1;
    Op->arguments[1] = arg_2;
    Op->arguments[2] = arg_3;
    break;
  case 7:
    Op->instruction = SUM_REG_HEX;		// 7XKK
    Op->arguments[0] = arg_1;
    Op->arguments[1] = arg_2;
    Op->arguments[2] = arg_3;
    break;
  case 8:
    // the following commands depend on the
    // value of arg_3, will use a switch case to determine which
    // command to use
      Op->arguments[0] = arg_1;
      Op->arguments[1] = arg_2;
    switch (arg_3) {
    case 0:
      Op->instruction = COPY_REG_REG;		// 8XY0
      break;
    case 1:
      Op->instruction = LOGIC_OR_REG_REG;	// 8XY1
      break;
    case 2:
      Op->instruction = LOGIC_AND_REG_REG;	// 8XY2
      break;
    case 3:
      Op->instruction = LOGIC_XOR_REG_REG;	// 8XY3
      break;
    case 4:
      Op->instruction = SUM_REG_REG;		// 8XY4
      break;
    case 5:
      Op->instruction = SUB_REG_REG;		// 8XY5
      break;
    case 6:
      Op->instruction = STOR_REG_REG;		// 8XY6
      break;
    case 7:
      Op->instruction = SET_SUB_REG;		// 8XY7
      break;
    default:
      fprintf(stderr,"WARN: unknown instruction %d %d %d %d, mem: %d\n", cpu->current_instruction[0], cpu->current_instruction[1], cpu->current_instruction[2], cpu->current_instruction[3], cpu->program_counter);
      assert(false);
    }
    break;
  case 9:
    Op->instruction = SKIP_NEQ_REG;		// 9XY0
    Op->arguments[0] = arg_1;
    Op->arguments[1] = arg_2;
    break;
  case 0xA:
    Op->instruction = SET_I_ADDR;		// AMMM
    Op->arguments[0] = arg_1;
    Op->arguments[1] = arg_2;
    Op->arguments[2] = arg_3;
    break;
  case 0xB:
    Op->instruction = JMP_ADDR_REG;		// BMMM
    Op->arguments[0] = arg_1;
    Op->arguments[1] = arg_2;
    Op->arguments[2] = arg_3;
    break;
  case 0xC:
    Op->instruction = LOGIC_AND_RAND_HEX;	// CXNN
    Op->arguments[0] = arg_1;
    Op->arguments[1] = arg_2;
    Op->arguments[2] = arg_3;
    break;
  case 0xD:
    Op->instruction = DISPLAY_XY_N;		// DXYN
    Op->arguments[0] = arg_1;
    Op->arguments[1] = arg_2;
    Op->arguments[2] = arg_3; 
    break;
  case 0xE:
      Op->arguments[0] = arg_1;
    if (arg_2 == 9 && arg_3 == 0xE){
      Op->instruction = SKIP_EQ_KEY_REG;	// EX9E
    }else if (arg_2 == 0xA && arg_3 == 1) {
      Op->instruction = SKIP_NEQ_KEY_REG;	// EXA1
    }else{
      fprintf(stderr,"WARN: unknown instruction %d %d %d %d, mem: %d\n", cpu->current_instruction[0], cpu->current_instruction[1], cpu->current_instruction[2], cpu->current_instruction[3], cpu->program_counter);
      assert(false);
    }
    break;
  case 0xF:
    Op->arguments[0] = arg_1;
    if (arg_2 == 0 && arg_3 == 7){
      Op->instruction = SET_REG_TIMER;		// FX07
    } else if (arg_2 == 0 && arg_3 == 0xA){
      Op->instruction = SET_REG_KEYDOWN;	// FX0A
    } else if (arg_2 == 1 && arg_3 == 5){
      Op->instruction = SET_TIMER_REG;		// FX15
    } else if (arg_2 == 1 && arg_3 == 7){
      Op->instruction = SET_SOUND_REG;		// FX17
    } else if (arg_2 == 1 && arg_3 == 0xE){
      Op->instruction = SUM_I_REG;		// FX1E
    } else if (arg_2 == 2 && arg_3 == 9){
      Op->instruction = SET_I_DSP_REG_LSDIGIT;	// FX29
    } else if (arg_2 == 3 && arg_3 == 3){
      Op->instruction = STOR_REG_BCD;		// FX33
    } else if (arg_2 == 5 && arg_3 == 5){
      Op->instruction = STOR_REG_I;		// FX55
    } else if (arg_2 == 6 && arg_3 == 5){
      Op->instruction = LOAD_REG_I;		// FX65
    } else {
      fprintf(stderr,"WARN: unknown instruction %d %d %d %d, mem: %d\n", cpu->current_instruction[0], cpu->current_instruction[1], cpu->current_instruction[2], cpu->current_instruction[3], cpu->program_counter);
      assert(false);
    }
    break;
  default:      fprintf(stderr,"WARN: unknown instruction %d %d %d %d, mem: %d\n", cpu->current_instruction[0], cpu->current_instruction[1], cpu->current_instruction[2], cpu->current_instruction[3], cpu->program_counter);
    assert(false);
  }
  return 0;
}

void execute(struct CHIP_8 *cpu, struct Operation *op){
  assert(INSTRUCTION_SET_LENGTH == CHIP_8_INSTRUCTION_LENGTH);
  switch(op->instruction){
  case NOP:
    break;
  case ERASE:
    for(int x = 0; x < CHIP_8_SCREEN_WIDTH; ++x){
      for(int y = 0; y < CHIP_8_SCREEN_HEIGHT; ++y){
	cpu->graphics[y][x] = 0x0;
      }
    }
    break;
  case RETURN:
    if(cpu->stack_pointer == 0){
      fprintf(stderr, "WARN: Stack Underflow");
      assert(false);
    }
    cpu->program_counter =
      (cpu->Memory[CHIP_8_STACK_OFFSET + ((cpu->stack_pointer * 2) + 1)]) |
      (cpu->Memory[CHIP_8_STACK_OFFSET + (cpu->stack_pointer * 2)] << 8);
    cpu->stack_pointer--;
    break;
  case GOTO_MEMORY:
    cpu->program_counter = bitshift_MMM(op->arguments);
    break;
  case EXECUTE_SUBROUTINE:
    if(cpu->stack_pointer == CHIP_8_STACK_MAX){
      fprintf(stderr, "WARN: Stack Overflow");
      assert(false);
    }
    cpu->stack_pointer++;
    cpu->Memory[CHIP_8_STACK_OFFSET + (cpu->stack_pointer * 2)] = (cpu->program_counter >> 8);
    cpu->Memory[CHIP_8_STACK_OFFSET + ((cpu->stack_pointer * 2) + 1)] = (cpu->program_counter & 0xff);
    cpu->program_counter = bitshift_MMM(op->arguments);
    break;
  case SKIP_EQ_HEX:
    assert(false);
    break;
  case SKIP_NEQ_HEX:
    assert(false);
    break;
  case SKIP_EQ_REG:
    assert(false);
    break;
  case SET_REG_HEX:
    cpu->registers[op->arguments[0]] = bitshift_MM(op->arguments);
    break;
  case SUM_REG_HEX:
    cpu->registers[op->arguments[0]] += bitshift_MM(op->arguments);
    break;
  case COPY_REG_REG:
    assert(false);
    break;
  case LOGIC_OR_REG_REG:
    assert(false);
    break;
  case LOGIC_AND_REG_REG:
    assert(false);
    break;
  case LOGIC_XOR_REG_REG:
    assert(false);
    break;
  case SUM_REG_REG:
    assert(false);
    break;
  case SUB_REG_REG:
    assert(false);
    break;
  case STOR_REG_REG:
    assert(false);
    break;
  case SET_SUB_REG:
    assert(false);
    break;
  case SKIP_NEQ_REG:
    assert(false);
    break;
  case SET_I_ADDR:
    cpu->I = bitshift_MM(op->arguments);
    break;
  case JMP_ADDR_REG:
    assert(false);
    break;
  case LOGIC_AND_RAND_HEX:
    assert(false);
    break;
  case DISPLAY_XY_N:
    {
      uint8_t temp_I = cpu->I;
      // fprintf(stderr, "%d, %d, %d\n", cpu->registers[op->arguments[0]],cpu->registers[op->arguments[1]], temp_I);
      uint8_t x_pos = cpu->registers[op->arguments[0]] & 63;
      uint8_t y_pos = cpu->registers[op->arguments[1]] & 31;
      cpu->registers[0xF] = 0x0;
      for (int y = 0; y < op->arguments[2]; ++y){ 
	int value_to_draw = cpu->Memory[temp_I];
	// fprintf(stderr, "%08b\n", value_to_draw);
	for(int x = 0; x < 8; ++x){ 
	  if ((x >= CHIP_8_SCREEN_WIDTH) || (y >= CHIP_8_SCREEN_HEIGHT)){
	    continue;
	  }
	  if((cpu->graphics[y_pos + y][x_pos + x] & (0x01)) == 1 && (value_to_draw >> (8 - x) & (0x01)) == 0x1){
	    cpu->registers[0xf] = 0x1;
	  }
	  //uint8_t temp_val = (cpu->graphics[y_pos + y][x_pos + x] & (0x01)) ^ ((value_to_draw >>  (7 - x)) & (0x01));
	  cpu->graphics[y_pos+y][x_pos+x] ^= ((value_to_draw >> (8 - x)) & 0x01) ;//temp_val;

	}
	temp_I++;
      }
      break;
    }
  case SKIP_EQ_KEY_REG:
    assert(false);
    break;
  case SKIP_NEQ_KEY_REG:
    assert(false);
    break;
   case SET_REG_TIMER:
    assert(false);
    break;
  case SET_REG_KEYDOWN:
    assert(false);
    break;
  case SET_TIMER_REG:
    assert(false);
    break;
   case SET_SOUND_REG:
    assert(false);
    break;
  case SUM_I_REG:
    assert(false);
    break;
  case SET_I_DSP_REG_LSDIGIT:
    assert(false);
    break;
  case STOR_REG_BCD:
    assert(false);
    break;
  case STOR_REG_I:
    assert(false);
    break;
  case LOAD_REG_I:
    assert(false);
    break;
  case INSTRUCTION_SET_LENGTH:
    fprintf(stderr, "WARN: not a valid operator: %d\n", op->instruction);
    assert(false);
    break;
  default:
    fprintf(stderr,"WARN: unknown operation: %d\n", op->instruction);
    assert(false);
  }
}


void cpu_tick(struct CHIP_8 *cpu){
  struct Operation op;
  fetch(cpu);
  lexer(cpu, &op);
  execute(cpu, &op);   
}
