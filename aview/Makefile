###############################################################################
# Makefile for aview
# 
# Copyright (c) 2016, Michael Jensen All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
#   * Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#
#   * Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer
#     in the documentation and/or other materials provided with the
#     distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
###############################################################################

CFLAGS=-g
OBJS=main.o dir.o dllist.o curses.o txtview.o
LIBS=-lcurses
PROGS=aview


all: ${PROGS}

aview: ${OBJS}
	${CC} ${CFLAGS} -o aview ${OBJS} ${LIBS}

dir.o:
	${CC} ${CFLAGS} -c dir.c

dllist.o:
	${CC} ${CFLAGS} -c dllist.c

main.o:
	${CC} ${CFLAGS} -c main.c

txtview.o:
	${CC} ${CFLAGS} -c txtview.c

curses.o:
	${CC} ${CFLAGS} -c ../curses.c


clean:
	rm -f ${OBJS}

binclean:
	rm -f ${PROGS}

