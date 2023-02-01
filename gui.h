#ifndef __GUI_H__
#define __GUI_H__
#include <curses.h>
#include <stdlib.h>

#include "chip8.h"

#define GAP 5

#define GRAPHIC_WINDOW_SIZE_HORIZONTAL 66
#define GRAPHIC_WINDOW_SIZE_VERTICAL 34
#define GRAPHIC_WINDOW_HORIZONTAL_OFFSET 76 //graphic window horizontal + 2 gap (used for ao, memory, command offsets)

#define REGISTER_WINDOW_SIZE_HORIZONTAL 88//176
#define REGISTER_WINDOW_SIZE_VERTICAL 4
#define REGISTER_WINDOW_VERTICAL_OFFSET 44 //graphic window vertical + 2 gap

#define MEMORY_WINDOW_SIZE_HORIZONTAL 9
#define MEMORY_WINDOW_SIZE_VERTICAL 13


#define AO_WINDOW_SIZE_HORIZONTAL 9
#define AO_WINDOW_SIZE_VERTICAL 3
#define AO_WINDOW_VERTICAL_OFFSET 23 // memory vertical + 2 gap

#define COMMAND_WINDOW_SIZE_HORIZONTAL 20
#define COMMAND_WINDOW_SIZE_VERTICAL 10
#define COMMAND_WINDOW_VERTICAL_OFFSET 28 // ao offset + 1 gap



struct GUI{
  WINDOW *graphic_window;
  WINDOW *register_window;
  WINDOW *memory_window;
  WINDOW *active_operation_window;
  WINDOW *command_window;


};

void initGui(struct GUI* gui);

void print_screen(struct CHIP_8 CPU, struct GUI* gui);

#endif //__GUI_H_
