#include <iostream>
#include <string>

const std::string LIB_PATH = "\
..\\..\\dependencies \
";

const std::string LIB = "\
-lglew32s \
-lglu32 \
-lopengl32 \
-lglfw3 \
-lsoil2 \
"; 

const std::string MCHN_OPT = "\
-mwindows \
-mconsole";

const std::string SRC =  "\
main.cpp \
";

const std::string MACROS = "\
GLEW_STATIC \
";

int main(int argc, char const *argv[])
{
    //  -o2 -Wall
    const std::string CMD = " g++ " + SRC + " -o main " +
                            " -L " + LIB_PATH + " " +  LIB + " " + MCHN_OPT +
                            " -D " + MACROS ;
    const std::string CMD_beau = " g++ -o2 -Wall \n [SRC] " + SRC +
                            "\n [-L] " + LIB_PATH + LIB + MCHN_OPT +
                            "\n [-D] " + MACROS +
                            "\n [EXE] -o main ";

    std::cout<<" ---------------------------"<<std::endl;
    std::cout<<" [COMMAND] "<<CMD_beau<<std::endl;
    std::cout<<" ---------------------------"<<std::endl;

    system(CMD.c_str());

    return 0;
}

/*
g++  p_2_2.cpp -o p_2_2 -LC:\Users\rogrp\Desktop\computer_graphics\Dependencies -lglu32 -lopengl32 -lglfw3 -lglew32 -mwindows
*/