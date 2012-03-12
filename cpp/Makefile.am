AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = tinyshell 
tinyshell_SOURCES = src/executable/tinyshell.cpp src/command/command \
                src/command/command.h src/command/extcmd.h src/command/extcmd.cpp

#external_SOURCE = src/executable/external.c

dist_noinst_SCRIPTS = autogen.sh
