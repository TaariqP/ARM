cmake_minimum_required(VERSION 3.10)
project(arm11_11)

set(CMAKE_CXX_STANDARD 11)

add_executable(arm11_11
        src/assemble.c
        src/emulate.c
        src/Makefile src/emulate_utils/utils.c src/emulate_utils/utils.h src/emulate_utils/defs.h src/emulate_utils/decode.c src/emulate_utils/execute.c src/emulate_utils/execute.h src/emulate_utils/decode.h src/assemble_utils/utils.c src/assemble_utils/utils.h src/assemble_utils/defs.h)
