project(C)
include_directories (./cgen)
add_executable(prog main.c cgen/all)