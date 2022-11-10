#pragma once
//#include <SDL2/SDL.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <locale.h>

#include "chip8.h"

//struct gui {
//  SDL_Window* window;
//  SDL_Surface* surface;
//};

//void init_gui(struct gui *gui);
//void shutdown_gui(struct gui *gui);

int load_chip8_cart(struct CHIP_8* cpu, const char cart_file_pointer[]);

void help();

void print_screen(struct CHIP_8 CPU);
int main(int argc, char *argv[]);
