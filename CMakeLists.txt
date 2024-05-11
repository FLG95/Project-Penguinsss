cmake_minimum_required(VERSION 3.27)
project(Project_Pinguinsss C)

set(CMAKE_C_STANDARD 11)


add_executable(Project_Pinguinsss
        vscode.c


)


target_link_libraries(Project_Pinguinsss ncurses)