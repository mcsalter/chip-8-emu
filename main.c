#include "main.h"


void help(){
  
}

/* load function
 * Takes in a file name, checks that the file exists, and loads the file
 * return codes:
 * 0  - executed correctly
 * 1  - file extension is not '.ch8'
 * 2  - file does not exist OR cannot be read
 * 3  - file exists but could not be opened (race condition?)
 */
int load_chip8_cart(struct CHIP_8* cpu, const char cart_file_pointer[]){
  errno = 0;
  char* extension = strrchr(cart_file_pointer, '.');
  if(strcmp(extension, ".ch8") != 0){
    fprintf(stderr, "WARN: file extension is wrong, %s should be .ch8", extension);
    return 1;
    }
  if(access(cart_file_pointer, R_OK) == -1){
    perror("access");
    fprintf(stderr, "%s", cart_file_pointer);
    return 2;
  }

  FILE *file_pointer;
  if((file_pointer = fopen(cart_file_pointer, "rb")) == NULL){
    perror("fopen");
    return 3;
  }
  int c;
  for(int index = CHIP_8_PROGRAM_OFFSET; index < CHIP_8_MEMORY_SIZE && (c = fgetc(file_pointer)) != EOF; ++index){
    cpu->Memory[index] = c;
  }
  fclose(file_pointer);
  return 0;
}




int main (int argc, char *argv[]) {

  // getting the option flags from input
  int file_flag = 0;
  int opt = 0;
  int ncurses_flag = 0;
  char* file_handle = "./IBM-Logo.ch8";
  int ch = 0;

  while ((opt = getopt(argc, argv, "f:nhl")) != -1){
    switch(opt){
    case 'f':
      file_flag = 1;
      file_handle = optarg;
      break;
    case 'h':
      help();
      break;
    case 'n':
      ncurses_flag = 1;
      break;
    default:
      help();
    }
  }
  
  struct CHIP_8 CPU;
  struct GUI GUI;
  initialize_chip8(&CPU);
  int retval;
  if(file_flag){
    if ((retval = load_chip8_cart(&CPU, file_handle)) != 0){
      endwin();
    }
  }
  initGui(&GUI);
  while(CPU.program_counter < CHIP_8_MEMORY_SIZE){
    cpu_tick(&CPU);
    if(ncurses_flag){
      print_screen(CPU, &GUI);

      
      ch = getch();
      if(ch == 'q'){
	break;
      }
    }
  }

  endwin();
  return 0;
}
