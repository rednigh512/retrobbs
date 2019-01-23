/******************************************************************************
 * llist.h for llist.c linked list implementaion
 *
 * Copyright (c) 2013, Michael Jensen All rights reserved.
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
 * ------------------------------------------------------------------
 *  
 * Datas structures and function prototypes header.
 *
 ****************************************************************************/

/*****************************************************************************
 * Data structures
 ***************************************************************************/

typedef struct dnode {
	struct dnode * next; /* next list element pointer */
	struct dnode * previous;
	void * dptr;        /* Generic data pointer */
	size_t dsize;       /* size of data held */
}dnode;

/*****************************************************************************
 * Function prototypes
 ***************************************************************************/

/* initiializes the list return a pointer for the first element */
dnode * init_dlist(void *dptr,size_t dsize);

/* returns the last element of the list */
dnode * find_tail(dnode *first);



/* add the data pointed at by dptr of the size dsize to the end of the list */
dnode * add_dtail(dnode *first,void *dptr,size_t dsize);

/* inserts the data pointed at by dptr of the size dsize as the next item after element */
dnode * add_delement(dnode * element,void * dptr,size_t dsize);



/* free last element does not free head or first element */
dnode * free_dtail(dnode *first);

/* removes and frees memory of element pointed to by element */
dnode * free_delement(dnode *first,dnode *element);

/* frees the entire list */
dnode * free_dlist(dnode *first);



/* return next list element */
dnode * next_delement(dnode *current);

/* return previous list element */
dnode * previous_delement(dnode *current);

/* returns the data pointer of the current element */
void * get_ddata(dnode *current);

/* returns the size of the data of the current element */
size_t get_dsize(dnode *current);


dnode * disconnect_delement(dnode *element);

dnode * connect_delement(dnode * element, dnode * connect_me);

