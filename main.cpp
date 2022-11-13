#include <iostream>
#include "args.h"
#include "compile/compile.h"

int main(int argc, char **argv) {
    compile_data data;
    compile_config config = parse_args(argc, argv);

    data = compile(config);
    config.exec(data);
    return 0;
}
