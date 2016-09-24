# Vaiable definitions.



PLATFORM	= $(shell uname)
CC		= gcc
OBJDIR		= ./objs
TESTS		= ./tests
SRC         = ./src
INCLUDES	= ./include
INCFLAGS	:= -I${INCLUDES}
#OPTFLAGS	:= -Os -DNDEBUG
OPTFLAGS	:= -g
CCFLAGS		:= -c ${OPTFLAGS} -Wall -std=c99

# Used to generate the .o files
LIBEIOBJDIR :=  ${OBJDIR}/libeiobj
SRC_FILES=$(wildcard $(SRC)/*.c)

# The list of objects to include in the library
LIBEIOBJS=$(patsubst $(SRC)/%.c, $(LIBEIOBJDIR)/%.o, $(SRC_FILES))

# Platform specific definitions (OS X, Linux, Windows)

ifeq (${PLATFORM},Darwin)

	# Building for Mac OS X
	
	PLATDIR		= _osx
	INCFLAGS	:= ${INCFLAGS} -I/opt/local/include -I/opt/local/include/SDL
	LINK		= ${CC}
	LIBEI		= ${OBJDIR}/libei.a
	LIBEIBASE	= ${PLATDIR}/libeibase.a
	LIBS		= ${LIBEIBASE} -L/opt/local/lib -lfreeimage -lSDL -lSDL_ttf -lSDL_gfx -framework AppKit

else
	ifeq (${PLATFORM},Linux)

	# Building for Linux
	
	PLATDIR		= _x11
	INCFLAGS	:= ${INCFLAGS} -I/usr/include/SDL
	LINK		= ${CC}
	LIBEI		= ${OBJDIR}/libei.a
#	ARCH	        = 32
	ARCH	        = 64
	LIBEIBASE	= ${PLATDIR}/libeibase${ARCH}.a
	LIBS		= ${LIBEIBASE} -L${PLATDIR} -lSDL -lSDL_ttf -lSDL_gfx${ARCH} -lfreeimage${ARCH} -lm -lstdc++

	else

	# Building for Windows
	
	PLATDIR		= _win
	INCFLAGS	:= ${INCFLAGS} -I/usr/include -I/usr/include/SDL -I/usr/local/include/SDL
	LINK		= ${CC} -mwindows -mno-cygwin
	LIBEI		= ${OBJDIR}/libei.lib
	LIBEIBASE	= ${PLATDIR}/libeibase.lib
	LIBS		= -lmingw32 ${LIBEIBASE} /usr/lib/SDL_ttf.lib /usr/local/lib/libSDL_gfx.a -L/usr/lib -lSDL -lFreeImage

	endif
endif



# Main target of the makefile. To build specific targets, call "make <target_name>"

TARGETS		=	${LIBEI} \
			minimal frame frame_test button button_test hello_world\
			puzzle minesweeper two048 pong

all : ${TARGETS}



########## Test-programs

# minimal

minimal : ${OBJDIR}/minimal.o ${LIBEIBASE}
	${LINK} -o minimal ${OBJDIR}/minimal.o ${LIBS}

${OBJDIR}/minimal.o : ${TESTS}/minimal.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/minimal.c -o ${OBJDIR}/minimal.o


# Building test for valgrind
.PHONY: valgrind-init_minimal valgrind_minimal

valgrind-init_minimal:
	@rm -f my.supp sdl.supp
	valgrind --leak-check=full --gen-suppressions=all --log-file=my.supp --show-possibly-lost=yes --show-reachable=yes ./minimal
	@sed '/==/d' my.supp > sdl.supp

valgrind_minimal:
	valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --track-origins=yes --suppressions=sdl.supp ./minimal 

# frame

frame : ${OBJDIR}/frame.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o frame ${OBJDIR}/frame.o ${LIBEI} ${LIBS} 

${OBJDIR}/frame.o : ${TESTS}/frame.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/frame.c -o ${OBJDIR}/frame.o

# Building test for valgrind
.PHONY: valgrind-init_frame valgrind_frame

valgrind-init_frame:
	@rm -f my.supp sdl.supp
	valgrind --leak-check=full --gen-suppressions=all --log-file=my.supp --show-possibly-lost=yes --show-reachable=yes ./frame
	@sed '/==/d' my.supp > sdl.supp

valgrind_frame:
	valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --track-origins=yes --suppressions=sdl.supp ./frame

# frame_test

frame_test : ${OBJDIR}/frame_test.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o frame_test ${OBJDIR}/frame_test.o ${LIBEI} ${LIBS} 

${OBJDIR}/frame_test.o : ${TESTS}/frame_test.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/frame_test.c -o ${OBJDIR}/frame_test.o

# Building test for valgrind
.PHONY: valgrind-init_frame_test valgrind_frame_test

valgrind-init_frame_test:
	@rm -f my.supp sdl.supp
	valgrind --leak-check=full --gen-suppressions=all --log-file=my.supp --show-possibly-lost=yes --show-reachable=yes ./frame_test
	@sed '/==/d' my.supp > sdl.supp

valgrind_frame_test:
	valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --track-origins=yes --suppressions=sdl.supp ./frame_test

# button

button : ${OBJDIR}/button.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o button ${OBJDIR}/button.o ${LIBEI} ${LIBS}

${OBJDIR}/button.o : ${LIBEIOBJS} ${TESTS}/button.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/button.c -o ${OBJDIR}/button.o


# Building test for valgrind
.PHONY: valgrind-init_button valgrind_button

valgrind-init_button:
	@rm -f my.supp sdl.supp
	valgrind --leak-check=full --gen-suppressions=all --log-file=my.supp --show-possibly-lost=yes --show-reachable=yes ./button 
	@sed '/==/d' my.supp > sdl.supp

valgrind_button:
	valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --track-origins=yes --suppressions=sdl.supp ./button 

# button_test

button_test : ${OBJDIR}/button_test.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o button_test ${OBJDIR}/button_test.o ${LIBEI} ${LIBS}

${OBJDIR}/button_test.o : ${LIBEIOBJS} ${TESTS}/button_test.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/button_test.c -o ${OBJDIR}/button_test.o


# Building test for valgrind
.PHONY: valgrind-init_button_test valgrind_button_test

valgrind-init_button_test:
	@rm -f my.supp sdl.supp
	valgrind --leak-check=full --gen-suppressions=all --log-file=my.supp --show-possibly-lost=yes --show-reachable=yes ./button_test
	@sed '/==/d' my.supp > sdl.supp

valgrind_button_test:
	valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --track-origins=yes --suppressions=sdl.supp ./button_test


# hello_world

hello_world : ${OBJDIR}/hello_world.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o hello_world ${OBJDIR}/hello_world.o ${LIBEI} ${LIBS}

${OBJDIR}/hello_world.o : ${LIBEIOBJS} ${TESTS}/hello_world.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/hello_world.c -o ${OBJDIR}/hello_world.o

# Building test for valgrind
.PHONY: valgrind-init_hello_world valgrind_hello_world

valgrind-init_hello_world:
	@rm -f my.supp sdl.supp
	valgrind --leak-check=full --gen-suppressions=all --log-file=my.supp --show-possibly-lost=yes --show-reachable=yes ./hello_world
	@sed '/==/d' my.supp > sdl.supp

valgrind_hello_world:
	valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --track-origins=yes --suppressions=sdl.supp ./hello_world


# top_level_test

top_level_test : ${OBJDIR}/top_level_test.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o top_level_test ${OBJDIR}/top_level_test.o ${LIBEI} ${LIBS}

${OBJDIR}/top_level_test.o : ${LIBEIOBJS} ${TESTS}/top_level_test.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/top_level_test.c -o ${OBJDIR}/top_level_test.o

# Building test for valgrind
.PHONY: valgrind-init_top_level_test valgrind_top_level_test

valgrind-init_top_level_test:
	@rm -f my.supp sdl.supp
	valgrind --leak-check=full --gen-suppressions=all --log-file=my.supp --show-possibly-lost=yes --show-reachable=yes ./top_level_test
	@sed '/==/d' my.supp > sdl.supp

valgrind_top_level_test:
	valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --track-origins=yes --suppressions=sdl.supp ./top_level_test

# parsing

parsing : ${OBJDIR}/parsing.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o parsing ${LDFLAGS} ${OBJDIR}/parsing.o ${LIBEI} ${LIBS}

${OBJDIR}/parsing.o : ${LIBEIOBJS} ${TESTS}/parsing.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/parsing.c -o ${OBJDIR}/parsing.o

# Building test for valgrind
.PHONY: valgrind-init_parsing valgrind_parsing

valgrind-init_parsing :
	@rm -f my.supp sdl.supp
	valgrind --leak-check=full --gen-suppressions=all --log-file=my.supp --show-possibly-lost=yes --show-reachable=yes ./parsing
	@sed '/==/d' my.supp > sdl.supp

valgrind_parsing:
	valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --track-origins=yes --suppressions=sdl.supp i ./parsing 

# puzzle

puzzle : ${OBJDIR}/puzzle.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o puzzle ${OBJDIR}/puzzle.o ${LIBEI} ${LIBS}

${OBJDIR}/puzzle.o : ${LIBEIOBJS} ${TESTS}/puzzle.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/puzzle.c -o ${OBJDIR}/puzzle.o

# Building test for valgrind
.PHONY: valgrind-init_puzzle valgrind_puzzle

valgrind-init_puzzle :
	@rm -f my.supp sdl.supp
	valgrind --leak-check=full --gen-suppressions=all --log-file=my.supp --show-possibly-lost=yes --show-reachable=yes ./puzzle
	@sed '/==/d' my.supp > sdl.supp

valgrind_puzzle:
	valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --track-origins=yes --suppressions=sdl.supp ./puzzle

# two048

two048 : ${OBJDIR}/two048.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o two048 ${OBJDIR}/two048.o ${LIBEI} ${LIBS}

${OBJDIR}/two048.o : ${LIBEIOBJS} ${TESTS}/two048.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/two048.c -o ${OBJDIR}/two048.o

# Building test for valgrind
.PHONY: valgrind-init_two048 valgrind_two048

valgrind-init_two048:
	@rm -f my.supp sdl.supp
	valgrind --leak-check=full --gen-suppressions=all --log-file=my.supp --show-possibly-lost=yes --show-reachable=yes ./two048
	@sed '/==/d' my.supp > sdl.supp

valgrind_two048:
	valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --track-origins=yes --suppressions=sdl.supp ./two048

# minesweeper

minesweeper : ${OBJDIR}/minesweeper.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o minesweeper ${OBJDIR}/minesweeper.o ${LIBEI} ${LIBS}

${OBJDIR}/minesweeper.o : ${LIBEIOBJS} ${TESTS}/minesweeper.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/minesweeper.c -o ${OBJDIR}/minesweeper.o

# pong
#
pong : ${OBJDIR}/pong.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o pong ${OBJDIR}/pong.o ${LIBEI} ${LIBS}

${OBJDIR}/pong.o : ${LIBEIOBJS} ${TESTS}/pong.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/pong.c -o ${OBJDIR}/pong.o

# Building test for valgrind
.PHONY: valgrind-init_minesweeper valgrind_minesweeper

valgrind-init_minesweeper:
	@rm -f my.supp sdl.supp
	valgrind --leak-check=full --gen-suppressions=all --log-file=my.supp --show-possibly-lost=yes --show-reachable=yes ./minesweeper 
	@sed '/==/d' my.supp > sdl.supp

valgrind_minesweeper:
	valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --track-origins=yes --suppressions=sdl.supp ./minesweeper 

# test_linked_rects

test_linked_rects : ${OBJDIR}/test_linked_rects.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_linked_rects ${OBJDIR}/test_linked_rects.o ${LIBEI} ${LIBS}

${OBJDIR}/test_linked_rects.o : ${LIBEIOBJS} ${TESTS}/test_linked_rects.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_linked_rects.c -o ${OBJDIR}/test_linked_rects.o

# Building test for valgrind
.PHONY: valgrind-init_test_linked_rects valgrind_test_linked_rects

valgrind-init_test_linked_rects:
	@rm -f my.supp sdl.supp
	valgrind --leak-check=full --gen-suppressions=all --log-file=my.supp --show-possibly-lost=yes --show-reachable=yes ./test_linked_rects
	@sed '/==/d' my.supp > sdl.supp

valgrind_test_linked_rects:
	valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --track-origins=yes --suppressions=sdl.supp ./test_linked_rects 

# Building of the library libei

${LIBEI} : ${LIBEIOBJS}
	ar rcs ${LIBEI} ${LIBEIOBJS}

${LIBEIOBJDIR}/%.o : ${SRC}/%.c
	${CC} ${CCFLAGS} ${INCFLAGS} $< -o $@

# Building of the doxygen documentation.

doc :
	doxygen docs/doxygen.cfg



# Removing all built files.

clean:
	rm -f ${TARGETS}
	rm -f *.exe
	rm -f ${OBJDIR}/*.o
	rm -f ${LIBEIOBJDIR}/*.o
