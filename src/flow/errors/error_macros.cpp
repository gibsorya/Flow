#include <error_macros.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  #include "windows.h"
#endif

#include <stdio.h>
#include <iostream>

// Prints an error message.
void print_error(const char *error, const char *message) {
  #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(console, 12);

    std::cout << error << ": " << message << std::endl;
  #elif __APPLE__
    std::cout << "\033[0;31m" << error << ": " << message << "\033[0m" << std::endl;
  #endif
}