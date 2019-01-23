/******************************************************************************
* main.c for aview
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
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER I COTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<limits.h>
#include<curses.h>

#include"dllist.h"
#include"main.h"
#include"dir.h"

main(int argc, char **argv)
{
	struct stat f_stat;
	dnode *first;
	dnode *selected;
	char *bg_file;
	char *fg_file;
	char *f_name[PATH_MAX];
	char arg;
	int usr_back=0;
	int usr_fore=0;
	char full_file[PATH_MAX];

	int quit=0;

        while((arg = getopt(argc, argv, "b:f:h")) != -1){
                switch(arg){
                case 'b':
                        usr_back=1;
                        bg_file = optarg;
                        break;
                case 'f':
                        usr_fore=1;
                        fg_file = optarg;
                        break;
                case 'h':
                        print_help();
                        exit(0);
                        break;
                default:
                        break;
                }
        }

        argv += optind;
        argc -= optind;

        if(argc == 1){
                strncpy(f_name,argv[0],sizeof(f_name));
        } else {
		strcpy(f_name,".");
        }

	/* install basic signal handlers to restore terminal from curses */
        signal(SIGSEGV, trap_sig);
        signal(SIGTERM, trap_sig);
        signal(SIGABRT, trap_sig);

	init_curses();

	stat(f_name, &f_stat);

	if(f_stat.st_mode & S_IFDIR){
		/* do direcotory browser */
		first=load_dir(f_name);

		curs_set(0);

		do{
			clear();
			if(usr_back){
				draw_usr_txt(bg_file);
			}

			draw_back();

			if(usr_fore){
				draw_usr_txt(fg_file);
			}

			selected=select_dir(first);
			if(selected !=NULL){
				clear();
				strncpy(full_file, f_name, sizeof(full_file));
				strncat(full_file, "/", sizeof(full_file));
				strncat(full_file, get_ddata(selected), sizeof(full_file));
				quit=txt_view(full_file);

			}
		}while(selected != NULL && !quit);
	} else {
		txt_view(f_name);
	}


	endwin();
}

void print_help()
{
        printf("\n--aview--------------------------------------------------------------------\n");
        printf("generates a selection list of files to view, or views given text file.\n\n");
        printf("aview [-b background_text_file] [-f foreground_text_file] file_or_dir\n");
        printf("       -b -- text file to draw on background\n");
        printf("       -f -- text file to draw on the foreground\n\n");
        printf("       -r -- enable direcory changes\n\n");
}

static void trap_sig(int signo) {
        endwin();
        if(signo == SIGSEGV){
                printf("I'm sorry; I have tried to access memories I am not allowed to.\n");
        }

        if(signo == SIGTERM){
                printf("I've been Terminated\n");
        }

        if(signo == SIGABRT){
                printf("aborted - core dumped\n");
                abort();
        }

        exit(1);
}
