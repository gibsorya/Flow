#include "error_macros.hpp"
#include "windows.h" //TODO: remove this dependency when using ansi characters
#include <stdio.h>
#include <iostream>

//TODO: Currently this print message will only work for Windows machines. Need to use ANSI for UNIX devs
void print_error(const char *error, const char *message) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(console, 12);
    std::cout << error << " " << message << std::endl;

    // printf(u)
}

