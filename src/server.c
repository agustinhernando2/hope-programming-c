#include "server.h"
#include <stdio.h>

#include <emergency_handler.h>
#include <critical_handler.h>

int main(int argc, char const *argv[])
{
    printf("Hello, World \n");


    printf("Calling shared lib from main program. I get '%d'.\n", shared_lib_function("Hello, OS II"));
    printf("Calling static lib from main program. I get '%d'.\n", static_lib_function("Bye, OS II"));

    return 0;
}


