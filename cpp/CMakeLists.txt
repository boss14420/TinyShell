cmake_minimum_required (VERSION 2.6)
#SET(CMAKE_CXX_COMPILER "icc")
#SET(CMAKE_C_COMPILER "icc")
PROJECT (TINYSHELL)

#INCLUDE_DIRECTORIES("${TINYSHELL}/src/")
INCLUDE_DIRECTORIES("${TINYSHELL}/src/command")
INCLUDE_DIRECTORIES("${TINYSHELL}/src/job")

#FIND_LIBRARY(NCURSES curses)
FIND_LIBRARY(READLINE readline)
#FIND_LIBRARY(BOOST_SYSTEM boost_system)
#FIND_LIBRARY(BOOST_FILESYSTEM boost_filesystem)

SET(TINYSHELL_VERSION "0.1")

SET(CMAKE_CXX_FLAGS "-g -O0 -Wall -std=c++0x -fno-inline -pedantic-errors -D_TINYSHELL_VERSION_=${TINYSHELL_VERSION}")
#SET(CMAKE_CXX_FLAGS "-O2 -Wall -std=gnu++0x -finline-functions -vec-report2 -xHost -DNDEBUG -fopenmp")
#SET(CMAKE_CXX_FLAGS "-O2 -Wall -std=c++0x -fopenmp -finline-functions -ftree-vectorize -DNDEBUG -D_TINYSHELL_VERSION_=${TINYSHELL_VERSION}")

SET(COMMON "src/command/command" "src/command/extcmd.h" "src/command/extcmd.cc"
    "src/command/command.h" "src/command/builtin.h" "src/command/builtin.cc" 
    "src/command/historycmd.h" "src/command/historycmd.cc" 
    "src/command/cmdfactory.h" "src/command/cmdfactory.cc"
    "src/command/chdircmd.h" "src/command/chdircmd.cc"
    "src/command/helpcmd.h" "src/command/helpcmd.cc"
    "src/job/shell.h" "src/job/shell.cc"
    "src/job/jobcmd.h"
    "src/job/fgcmd.h" "src/job/fgcmd.cc"
    "src/job/bgcmd.h" "src/job/bgcmd.cc"
    "src/job/kill.h" "src/job/kill.cc"
    )

ADD_EXECUTABLE(tinyshell
    src/tinyshell.cc
    ${COMMON}
  )

ADD_EXECUTABLE(external
    src/external.c
  )

TARGET_LINK_LIBRARIES(tinyshell ${READLINE})

INSTALL(TARGETS tinyshell
    RUNTIME DESTINATION "/usr/bin"
    )

