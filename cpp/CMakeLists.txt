cmake_minimum_required (VERSION 2.6)
#SET(CMAKE_CXX_COMPILER "icc")
#SET(CMAKE_C_COMPILER "icc")
PROJECT (TINYSHELL)

#INCLUDE_DIRECTORIES("${TINYSHELL}/src/")
INCLUDE_DIRECTORIES("${TINYSHELL}/src/command")

#FIND_LIBRARY(NCURSES curses)
FIND_LIBRARY(READLINE readline)
FIND_LIBRARY(BOOST_SYSTEM boost_system)
FIND_LIBRARY(BOOST_FILESYSTEM boost_filesystem)

SET(CMAKE_CXX_FLAGS "-g -O0 -Wall -std=gnu++0x -fno-inline")
#SET(CMAKE_CXX_FLAGS "-O2 -Wall -std=gnu++0x -finline-functions -vec-report2 -xHost -DNDEBUG -fopenmp")
#SET(CMAKE_CXX_FLAGS "-O2 -Wall -std=c++0x -fopenmp -finline-functions -ftree-vectorize -ftree-vectorizer-verbose=2 -ffast-math -march=native -DNDEBUG")

SET(COMMON "src/command/command" "src/command/extcmd.h" "src/command/extcmd.cc"
    "src/command/command.h" "src/command/builtin.h" "src/command/builtin.cc" 
    "src/command/historycmd.h" "src/command/historycmd.cc" 
    "src/command/cmdfactory.h" "src/command/cmdfactory.cc"
    "src/command/chdircmd.h" "src/command/chdircmd.cc"
    )

ADD_EXECUTABLE(tinyshell
    src/tinyshell.cc
    ${COMMON}
  )

ADD_EXECUTABLE(external
    src/external.c
  )

TARGET_LINK_LIBRARIES(tinyshell ${READLINE})
#TARGET_LINK_LIBRARIES(tinyshell ${BOOST_SYSTEM})
#TARGET_LINK_LIBRARIES(tinyshell ${BOOST_FILESYSTEM})
#TARGET_LINK_LIBRARIES(tinyshell ${NCURSES})
