#include "gui.h"

void initGui(struct GUI* gui){
  initscr();
  raw();
  noecho();
  
  if (has_colors())
  {
    start_color();
    
    /*
     * Simple color assignment, often all we need.  Color pair 0 cannot
     * be redefined.  This example uses the same value for the color
     * pair as for the foreground color, though of course that is not
     * necessary:
     */
    init_pair(1, COLOR_RED,     COLOR_BLACK);
    init_pair(2, COLOR_GREEN,   COLOR_BLACK);
    init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
    init_pair(4, COLOR_BLUE,    COLOR_BLACK);
    init_pair(5, COLOR_CYAN,    COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_WHITE,   COLOR_BLACK);
  }
  else{
    assert(false);
  }
  
  gui->graphic_window = newwin(GRAPHIC_WINDOW_SIZE_VERTICAL,
			       GRAPHIC_WINDOW_SIZE_HORIZONTAL,
			       GAP,
			       GAP);
  
  gui->register_window = newwin(REGISTER_WINDOW_SIZE_VERTICAL,
				REGISTER_WINDOW_SIZE_HORIZONTAL,
				REGISTER_WINDOW_VERTICAL_OFFSET,
				GAP);

  gui->memory_window = newwin(MEMORY_WINDOW_SIZE_VERTICAL,
			      MEMORY_WINDOW_SIZE_HORIZONTAL,
			      GAP,
			      GRAPHIC_WINDOW_HORIZONTAL_OFFSET);

  gui->active_operation_window = newwin(AO_WINDOW_SIZE_VERTICAL,
					AO_WINDOW_SIZE_HORIZONTAL,
					AO_WINDOW_VERTICAL_OFFSET,
					GRAPHIC_WINDOW_HORIZONTAL_OFFSET);

  gui->command_window = newwin(COMMAND_WINDOW_SIZE_VERTICAL,
			       COMMAND_WINDOW_SIZE_HORIZONTAL,
			       COMMAND_WINDOW_VERTICAL_OFFSET,
			       GRAPHIC_WINDOW_HORIZONTAL_OFFSET);

  box(gui->graphic_window, 0,0);
  mvwprintw(gui->graphic_window, 0, 1, "SCREEN");
  box(gui->register_window, 0,0);
  mvwprintw(gui->register_window, 0, 1, "REGISTERS");
  box(gui->memory_window, 0,0);
  mvwprintw(gui->memory_window, 0, 1, "MEMORY");
  mvwprintw(gui->memory_window,6,0, ">");
  box(gui->active_operation_window, 0,0);
  mvwprintw(gui->active_operation_window, 0, 1, "CURR OP");
  box(gui->command_window, 0,0);
  mvwprintw(gui->command_window, 0, 1, "COMMANDS");
  refresh();
}

void print_screen(struct CHIP_8 CPU, struct GUI* gui){
  // graphics window
  char val = ' ';
  wattron(gui->graphic_window, COLOR_PAIR(3));
  for(int y = 0; y < CHIP_8_SCREEN_HEIGHT; y++){
    for(int x = 0; x < CHIP_8_SCREEN_WIDTH; x++){
      if(CPU.graphics[y][x] == 1){
	val = '#';
      } if(CPU.graphics[y][x] == 0){
	val = ' ';
      }
      mvwprintw(gui->graphic_window, y + 1, x + 1, "%c", val);
    }
  }

  // active ops window
  mvwprintw(gui->active_operation_window,1, 1, "%x %x %x %x",
	    CPU.current_instruction[0],
	    CPU.current_instruction[1],
	    CPU.current_instruction[2],
	    CPU.current_instruction[3]);

  // memory viewer window
  for(int offset = -5; offset < 6; ++offset){
    if(2*offset+ CPU.program_counter > CHIP_8_MEMORY_SIZE){
      mvwprintw(gui->memory_window,offset+6,1, "END MEM");
      continue;
      }
    mvwprintw(gui->memory_window,offset+6,1, "%x %x %x %x",
	      (CPU.Memory[CPU.program_counter+ (2*offset) - 2] >> 4) & 0b00001111,
	      (CPU.Memory[CPU.program_counter+ (2*offset) - 2]) & 0b00001111,
	      (CPU.Memory[CPU.program_counter+ (2*offset) - 1] >> 4) & 0b00001111,
	      (CPU.Memory[CPU.program_counter+ (2*offset) - 1]) & 0b00001111);
  }

  // registers window
  for(int itr = 0; itr < 0x8; ++itr){
    mvwprintw(gui->register_window, 1, 1 + (itr*11) ,"reg %x: %2x", itr, CPU.registers[itr]);
    mvwprintw(gui->register_window, 2, 1 + (itr*11) ,"reg %x: %2x", itr + 8, CPU.registers[itr+8]);
  }
  // refresh all the windows
  wnoutrefresh(gui->active_operation_window);
  wnoutrefresh(gui->graphic_window);
  wnoutrefresh(gui->memory_window);
  wnoutrefresh(gui->register_window);
  doupdate();
}
