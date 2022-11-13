#include "stdio.h"
#include "string.h"

int main(argc: int, argv: char**) {
    [[ default=30 ]] test: int = 40;

    printf("Hello, World! %d\n", test);
}