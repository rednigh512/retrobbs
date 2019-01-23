/******************************************************************************
* dir.c for aview
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
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER I CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/


#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<limits.h>
#include<curses.h>
#include<string.h>

#include"dllist.h"
#include"dir.h"
#include"aview.h"

#define START_LINE 5

dnode * load_dir(char * path)
{

	dnode * first=NULL;
	dnode * current=NULL;
	struct dirent *dir;
	DIR *dir_ptr;
	struct stat f_stat;
	char f_name[NAME_MAX];
	char full_path[PATH_MAX];

	if((dir_ptr=opendir(path)) == NULL){
		endwin();
		perror("couldn't open directory");
		exit(1);
	}


	while((dir=readdir(dir_ptr)) != NULL){
		strncpy(full_path, path, PATH_MAX);
		if(full_path[strlen(full_path)-1] != '/'){
			strncat(full_path, "/", PATH_MAX);
		}

		strncat(full_path, dir->d_name, PATH_MAX);

		strncpy(f_name, dir->d_name, NAME_MAX);

		if(stat(full_path, &f_stat) == -1){
			continue;
		}

		/* skip directories */
		if(f_stat.st_mode & S_IFDIR){
			continue;
		}

		if(first == NULL){
			first=init_dlist(&f_name, NAME_MAX);
		} else {
			current=add_dtail(first, &f_name, NAME_MAX);
		}
	}

	closedir(dir_ptr);

	return first;
}

void draw_usr_txt(const char * f_name)
{

        FILE * fp;
        char line[256];
        int row=0;

        fp=fopen(f_name, "r");
        if(fp==NULL){
                endwin();
                perror(f_name);
                exit(1);
        }

        move(0, 0);

        attron(COLOR_PAIR(4));

        while(fgets(line, sizeof(line), fp) != NULL){
                if(line[strlen(line)-1] == '\n'){
                        row++;
                        line[strlen(line)-1]='\0';
                }
                addstr(line);
                move(row, 0);
        }

        move(0 ,0);

        fclose(fp);

}

void draw_back()
{

        int row;
	int max_rows;
	
	max_rows=getmaxy(stdscr)-1;

        move(4,3);

        attron(COLOR_PAIR(2));

        addstr("+-------------------------------------------+");

        for(row=START_LINE; row < max_rows; row++){
                move(row, 3);
                addstr("|                                           |");
        }
        move(max_rows, 3);
        addstr("+------------------------------------?-help-+");

}


void draw_dir(dnode * current, dnode * selected)
{
	int row=START_LINE;
	int max_rows;
	int pad;
	char * f_name;

	max_rows=getmaxy(stdscr)-1;

        move( START_LINE, 7);

	attron(COLOR_PAIR(3));
	while(current != NULL && row < max_rows){
		f_name=get_ddata(current);
		/* center name */
                pad=strlen(f_name);
                        if(pad != 0){
                                pad=pad/2;
                                pad=23 - pad;
                        }
                move( row, pad);

		if(current == selected){
                        attron(A_REVERSE);
                } else {

                        attroff(A_REVERSE);
                }

		addstr(f_name);
		row++;
		current=current->next;
	}

}

void clear_list()
{
        int row=START_LINE;
	int max_rows;

	max_rows=getmaxy(stdscr)-1;

        attroff(A_REVERSE);
        while(row < max_rows){
                move(row,4);
                addstr("                                           ");
                row++;
        }
}

dnode * select_dir(dnode * first)
{
	int quit=0;
	int k;
	dnode * current;
	dnode * selected;
	dnode * top;
	int row=0;
	int max_rows;

	max_rows=getmaxy(stdscr)-2; /* subtract box chars */
	max_rows-=START_LINE;

	selected=first;
	top=first;

	while(!quit){
		draw_dir(top, selected);
		k=getch();
		switch(k){
                        case ' ':
                        case CONTROL('m'):
                                quit=1;
                                break;
                        case 'j':
                        case CONTROL('x'):
                        case KEY_DOWN:
                                if(selected->next != NULL){
                                        selected=selected->next;
                                        if(row < max_rows){
                                                row++;
                                        } else {
                                                if(top->next !=NULL){
                                                        top=top->next;
                                                        clear_list();
                                                }
                                        }
                                }
                                break;

                        case 'k':
                        case CONTROL('e'):
                        case KEY_UP:
                                if(selected->previous != NULL){
                                        selected=selected->previous;
					if(row <= 0){
                                                if(top->previous != NULL){
                                                        top=top->previous;
                                                        clear_list();
                                                        row=0;
                                                }
                                        } else {
                                                row--;
                                        }

                                }
                                break;
			case 'q':
			case 27:
				quit=1;
				return NULL;
                        default:
                                break;
                }
	}

	return selected;
}



tolower_str(char * str)
{
	int i;

	for(i=0; i < strlen(str); i++){
		str[i]=tolower(str[i]);
	}

}

