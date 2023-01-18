#pragma once

#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <locale.h>

#include "chip8.h"
#include "gui.h"



int load_chip8_cart(struct CHIP_8* cpu, const char cart_file_pointer[]);

void help();

int main(int argc, char *argv[]);
