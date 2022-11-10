#include "main.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

/* void init_gui(struct gui* gui){ */
/*     if(SDL_Init(SDL_INIT_EVERYTHING) != 0){ */
/*         SDL_Log("error intializing SDL %s", SDL_GetError()); */
/*     } */
/*     gui->window = NULL; */
/*      gui->surface = NULL; */
/*      //SDL_Surface* imageSurface = NULL; //nullptr; */
/*      //SDL_Renderer* renderer = NULL; //nullptr; */
/*     gui->window = SDL_CreateWindow("chip 8 emu", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); */
/*     if (gui->window == NULL){ */
/*         SDL_Log("window creation error: %s", SDL_GetError()); */
/*     } */
/*     else { */
/*         gui->surface = SDL_GetWindowSurface(gui->window); */
/*         SDL_FillRect(gui->surface, NULL, SDL_MapRGB(gui->surface->format, 0x00, 0x00, 0x00)); */
/*         SDL_UpdateWindowSurface(gui->window); */
/*     } */
/* } */


/* void shutdown_gui(struct gui *gui){ */
/*     //Destroy window */
/*     SDL_DestroyWindow( gui->window ); */
/*     //Quit SDL subsystems */
/*     SDL_Quit(); */
/*} */

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
  for(int index = 0; index < (CHIP_8_MEMORY_SIZE - CHIP_8_PROGRAM_OFFSET) && (c = fgetc(file_pointer)) != EOF; ++index){
    cpu->Memory[index + CHIP_8_PROGRAM_OFFSET] = c;
  }
  fclose(file_pointer);
  return 0;
}

void print_screen(struct CHIP_8 CPU){
  int val = ' ';
  for(int boarder = 0; boarder < CHIP_8_SCREEN_WIDTH; boarder++){
    fprintf(stdout, "|-");
  }
  fprintf(stdout, "\n");
  for(int y = 0; y < CHIP_8_SCREEN_HEIGHT; y++){
    for(int x = 0; x < CHIP_8_SCREEN_WIDTH; x++){
      if(CPU.graphics[y][x] == 1){
	val = 35;
      } if(CPU.graphics[y][x] == 0){
	val = 32;
      }
      fprintf(stdout, "|%c", val);
    }
    fprintf(stdout, "|%x\n", y);
  }
  
  for(int boarder = 0; boarder < CHIP_8_SCREEN_WIDTH; boarder++){
    fprintf(stdout, "|-");
  }
  fprintf(stdout, "\n");
}


int main (int argc, char *argv[]) {

  // getting the option flags from input
  int file_flag = 0;
  int print_flag = 0;
  int step_flag = 0;
  int list_op_flag = 0;
  int opt = 0;
  char* file_handle = "./IBM-Logo.ch8";

  while ((opt = getopt(argc, argv, "f:sphl")) != -1){
    switch(opt){
    case 'f':
      file_flag = 1;
      file_handle = optarg;
      break;
    case 's':
      step_flag = 1;
      break;
    case 'l':
      list_op_flag = 1;
      break;
    case 'h':
      help();
      break;
    case 'p':
      print_flag = 1;
      break;
    default:
      help();
    }
  }
  
  struct CHIP_8 CPU;
  initialize_chip8(&CPU);
  int retval;
  if(file_flag){
    if ((retval = load_chip8_cart(&CPU, file_handle)) != 0){
      return retval;
    }
  }
  //struct gui GUI;
  //init_gui(&GUI);
  while(CPU.program_counter < CHIP_8_MEMORY_SIZE ){
    cpu_tick(&CPU);
    if(print_flag){
      print_screen(CPU);
    }
    if(list_op_flag){
      fprintf(stderr, "| %x %x %x %x  | ", CPU.current_instruction[0], CPU.current_instruction[1], CPU.current_instruction[2], CPU.current_instruction[3]);
      for(int itr = 0; itr < 0x10; ++itr){
	fprintf(stderr, "reg %x: %2x|", itr, CPU.registers[itr]);
      }
      fprintf(stderr, "\n");
    }
    if(step_flag){
      getchar();
    }
  }

  //print_screen(CPU);

  //  shutdown_gui(&GUI);
  return 0;
}
