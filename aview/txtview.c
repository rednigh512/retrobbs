/******************************************************************************
* txtview.c for aview
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


#include<curses.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<libgen.h>

#include"dllist.h"
#include"aview.h"
#include"txtview.h"

dnode * load_file(char * f_name, FILE * file_ptr, int * lines)
{

	dnode * first;
	dnode * current;
	int offset;
	int line_no;
	char line[256];

	fseek(file_ptr, 0, SEEK_SET);

	offset=ftell(file_ptr);
	line_no=0;

	first=init_dlist(&offset, sizeof(offset));
	if(first==NULL){
		addstr("memory error\n");
		return NULL;
	}

	while(fgets(line, sizeof(line), file_ptr) != NULL && current != NULL){
		offset=ftell(file_ptr);
		current=add_dtail(first, &offset, sizeof(offset));
		line_no++;
	}

	fseek(file_ptr, 0, SEEK_SET);

	*lines=line_no;


	return first;
}

void print_line(char * line,int start, int width)
{
	int i;
	width+=start; /* add the starting position to width to offset index */

	for(i=start; (i < width) && i < strlen(line); i++){
		switch(line[i]){
			case '\t':
				addstr("        ");
				break;

			case CONTROL('M'):
				break;

			default:
				if(i==width-1){
					if(strlen(line) > width-2){
						attron(COLOR_PAIR(3));
						addch('>');
						addch('\n');
						attron(COLOR_PAIR(1));
					} else {
						addch(line[i]);
						addch('\n');
					}
				} else {
					if(i==start && start >0){
						attron(COLOR_PAIR(3));
						addch('<');
						attron(COLOR_PAIR(1));
						width--;
					}
	
					addch(line[i]);
				}
				break;
		}
	}
}

int txt_view(char * f_name)
{

	int lines;
	int line_no;
	int *offset;
	int cur_line;
	int k;
	int key_ok=1;
	int quit=0;
	char line[1024];
	int start_col=0;

	int max_rows;
	int max_cols;
	int page_len;

	dnode * first;
	dnode * top;
	FILE * file_ptr;

	line_no=0;
	cur_line=0;

	char *base_file=NULL;
	base_file=basename(f_name);

	max_cols=getmaxx(stdscr);
	max_rows=getmaxy(stdscr);
	page_len=max_rows-1;

	if((file_ptr=fopen(f_name, "r"))==NULL){
		endwin();
		perror(f_name);
		exit(1);
        }

	first=load_file(f_name, file_ptr, &lines);


	if(first == NULL){
		endwin();
		printf("error indexing %s\n", f_name);
		exit(1);
	}

	top=first;

	while(!quit){
		if(key_ok && line_no < lines){
			attron(COLOR_PAIR(1));

			while(fgets(line, sizeof(line), file_ptr) != NULL && cur_line < page_len){
				move(cur_line, 0);
				clrtoeol();
				print_line(line, start_col, max_cols);
				//addstr(line);
				cur_line++;
			}


			if(line_no > lines-page_len){
				clrtobot();
				attron(COLOR_PAIR(2));
				addstr(" --- ");
				attron(COLOR_PAIR(3));
				addstr("End of File");
				attron(COLOR_PAIR(2));
				addstr(" ------");
			}
			move(page_len, 0);
			clrtoeol();
			attron(COLOR_PAIR(2));
			addstr(" --"); 
			attron(COLOR_PAIR(3));
			printw(" Page %4d/%-4d", (line_no + 1) / page_len +1, ((lines +1) / page_len) +1 ); 
			attron(COLOR_PAIR(2));
			addstr(" -----");
			pad_line(50 - strlen(base_file));
			attron(COLOR_PAIR(3));
			addch(' ');
			addstr(base_file);
			attron(COLOR_PAIR(2));
			addstr(" -- ");
		}

		key_ok=0;

		k=getch();
		switch(k){
			case 'q':
				quit=2;
				break;
			case 27:
				quit=1;
				break;

			/* page down */
			case CONTROL('c'):
			case ' ':
			case KEY_NPAGE:
				if(top->next != NULL && line_no  < lines){
					key_ok=1;
					for(cur_line=0; cur_line < page_len && top->next != NULL;
						cur_line++, top=top->next){
					}
					offset=get_ddata(top);
					fseek(file_ptr, *offset, SEEK_SET);
					line_no+=cur_line;
				}
				break;

			/* page up */
			case 'b':
			case CONTROL('r'):
			case KEY_PPAGE:
				if(top->previous != NULL && line_no > 0){
					key_ok=1;
					for(cur_line=0; cur_line < page_len && top->previous != NULL;
						cur_line++, top=top->previous){
					}
					offset=get_ddata(top);
					fseek(file_ptr, *offset, SEEK_SET);
					line_no-=cur_line;
				}
                                break;

			/* line down */
			case 'j':
			case CONTROL('z'):
			case CONTROL('m'):
			case KEY_DOWN:
			case CONTROL('x'):
				if(top->next != NULL && line_no  < (lines)){
					key_ok=1;
					top=top->next;
					offset=get_ddata(top);
					fseek(file_ptr, *offset, SEEK_SET);
					line_no++;
				}
				break;

			/* line up */
			case 'k':
			case CONTROL('w'):
			case CONTROL('e'):
			case KEY_UP:
				if(top->previous != NULL && line_no > 0){
					key_ok=1;
					top=top->previous;
					offset=get_ddata(top);
					fseek(file_ptr, *offset, SEEK_SET);
                                        line_no--;
				}
				break;
			/* side scrolling */
			case 'h':
			case KEY_LEFT:
				if(start_col>0){
					start_col--;
					offset=get_ddata(top);
					fseek(file_ptr, *offset, SEEK_SET);
					key_ok=1;
				}
				break;
			case 'l':
			case KEY_RIGHT:
				if(start_col<1024){
					start_col++;
					offset=get_ddata(top);
					fseek(file_ptr, *offset, SEEK_SET);
					key_ok=1;
				}
				break;
			case '?':
				key_ok=1;
				print_txt_help();
				offset=get_ddata(top);
				fseek(file_ptr, *offset, SEEK_SET);
				break;
			default:
				move(0, 0);
				attron(COLOR_PAIR(2));
				addstr(" -- Unrecognized key ---- ? for help ------------------------------------------");
				break;
		}

		cur_line=0;
	}

	fclose(file_ptr);
	free_dlist(first);

	if(quit>1){
		return 1;
	} else {
		return 0;
	}

}

void pad_line(int count)
{
	while(count > 0){
		addch('-');
		count--;
	}
}

dnode * page_up(dnode * top)
{
	int i=0;

	for(i=0; i < 24 && top->previous != NULL; i++){
		top=top->previous;
	}

	return top;
}

void print_txt_help()
{

	move(0,0);
	clear();
	attron(COLOR_PAIR(2));
	addstr(" +----------------------------------------------------------------------------+\n");
	addstr(" |                             TEXT VIEW HELP                                 |\n");
	addstr(" |----------------------------------------------------------------------------|\n");
	addstr(" |                                                                            |\n");
	addstr(" |  ");
	attron(COLOR_PAIR(3));
	addstr("line up: Control W, Control E, Up Arrow key");
	attron(COLOR_PAIR(2));
	addstr("                               |\n");
	addstr(" |                                                                            |\n");
	addstr(" |  ");
	attron(COLOR_PAIR(3));
	addstr("line down: Control Z, Control X, Down Arrow key");
	attron(COLOR_PAIR(2));
	addstr("                           |\n");
	addstr(" |                                                                            |\n");
	addstr(" |  ");
	attron(COLOR_PAIR(3));
	addstr("Page Down: Control C, Space, Page Down key");
	attron(COLOR_PAIR(2));
	addstr("                                |\n");
	addstr(" |                                                                            |\n");
	addstr(" |  ");
	attron(COLOR_PAIR(3));
	addstr("Page Up: Control R, Page Up key");
	attron(COLOR_PAIR(2));
	addstr("                                           |\n");
	addstr(" |                                                                            |\n");
	addstr(" |                                                                            |\n");
	addstr(" |  ");
	attron(COLOR_PAIR(3));
	addstr("Quit to list: Escape");
	attron(COLOR_PAIR(2));
	addstr("                                                      |\n");
	addstr(" |                                                                            |\n");
	addstr(" |  ");
	attron(COLOR_PAIR(3));
	addstr("Exit program: q");
	attron(COLOR_PAIR(2));
	addstr("                                                           |\n");
	addstr(" |                                                                            |\n");
	addstr(" |                                                                            |\n");
	addstr(" |                                                                            |\n");
	addstr(" |                                                                            |\n");
	addstr(" |                                                                            |\n");
	addstr(" |                                                                            |\n");
	addstr(" | ");
	attron(COLOR_PAIR(3));
	addstr("Any key to continue");
	attron(COLOR_PAIR(2));
	addstr("                                                        |\n");
	addstr(" +----------------------------------------------------------------------------+\n");
	getch();
}

