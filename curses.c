/******************************************************************************
* curses.c for my BBS tools
*
* Copyright (c) 2016, Michael Jensen All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*
*   * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer
*     in the documentation and/or other materials provided with the
*     distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************/

#include<curses.h>
#include<stdio.h>
#include<stdlib.h>

init_curses()
{

        WINDOW * win;

        /* curses setup */
        win = initscr();
        if(win == NULL){
                perror("Curses Initiailization");
                exit(1);
        }

        if(has_colors()){
                start_color();

		/**************************************
		*
		* Customizeing your BBS colors can be done
		* by changeing the below lines.
		* curses defines the following colors:
		*
		* COLOR_BLACK
		* COLOR_WHITE
		* COLOR_BLUE
		* COLOR_CYAN
		* COLOR_GREEN
		* COLOR_MAGENTA
		* COLOR_RED
		* COLOR_YELLOW
		*
		**************************************\

		/***************************************
		* Color pair 1 defines user text, and titles
		***************************************/

                init_pair(1, COLOR_GREEN, COLOR_BLACK);

		/***************************************
		* Color pair 2 defines background text, and decorations
		***************************************/

                init_pair(2, COLOR_RED, COLOR_BLACK);

		/***************************************
		* Color pair 3 defines foreground text, promts, commands
		***************************************/

                init_pair(3, COLOR_YELLOW, COLOR_BLACK);

		/***************************************
		* Color pair 4 defines backgrounds and overlays
		***************************************/

		init_pair(4, COLOR_YELLOW, COLOR_BLACK);
        }

        keypad(stdscr, TRUE);
        raw();
	nonl();
        noecho();

}


