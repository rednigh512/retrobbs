/******************************************************************************
 * llist - linked list implementation
 *
 * Copyright (c) 2014, Michael Jensen All rights reserved.
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
 ****************************************************************************/

#include<stdlib.h>
#include<strings.h>

#include"dllist.h"


dnode * init_dlist(void * dptr, size_t dsize)
{
	dnode * first;	

	first=(dnode *)malloc(sizeof(dnode));
	if(first==NULL){
		return NULL;
	} else {
		first->next=NULL;
		first->previous=NULL;
		first->dptr=malloc(dsize);
		first->dsize=dsize;
		if(first->dptr!=NULL){
			bcopy(dptr, first->dptr, dsize);
		}
		return first;
	}
}

dnode * find_dtail(dnode * first)
{
	for(; first->next!=NULL; first=first->next);
	return first;
}

dnode * add_dtail(dnode * first, void * dptr, size_t dsize)
{
	dnode *current;
	dnode *previous;

	current=find_dtail(first);
	current->next=(dnode *)malloc(sizeof(dnode));
	previous=current;
	current=current->next;
	if(current==NULL){
		return NULL;
	} else {
		current->next=NULL;
		current->previous=previous;
                current->dptr=malloc(dsize);
                current->dsize=dsize;
                if(current->dptr!=NULL){
                        bcopy(dptr, current->dptr, dsize);
                }
		return current;
	}
}

dnode * free_dtail(dnode *first)
{
	dnode *current;
	dnode *previous;

	if(first->next==NULL){
		return NULL;
	}	

	for(current=first; current->next!=NULL; current=current->next){
		previous=current;
	}

	if(current->dptr!=NULL){
		bzero(current->dptr, current->dsize);
		free(current->dptr);
	}
	bzero(current,sizeof(dnode));
	previous->next=NULL;
	free(current);
	return previous;
}

dnode * next_delement(dnode * current)
{
	return current->next;
}

dnode * previous_delement(dnode * current)
{
	return current->previous;
}

dnode * free_dlist(dnode * first)
{
       while(free_dtail(first)!=NULL){
                free_dtail(first);
        }
        if(first!=NULL){
		if(first->dptr!=NULL){
			bzero(first->dptr, first->dsize);
			free(first->dptr);
		}
		bzero(first, sizeof(dnode));
                free(first);
        }
	return NULL;
}


dnode * free_delement(dnode *first, dnode *element)
{

	dnode *previous;

	previous=element->previous;

	if(element->next!=NULL){
		previous->next=element->next;
		element->next->previous=previous;
		if(element->dptr!=NULL){
			bzero(element->dptr,element->dsize);
			free(element->dptr);
		}
		bzero(element, sizeof(dnode));
		free(element);
	} else {
		element=free_dtail(first);	
	}

	return previous;
}

dnode * add_delement(dnode * element, void * dptr, size_t dsize)
{
	dnode *next_element;
	dnode *current;

	if(element->next==NULL){
		return add_dtail(element, dptr, dsize);
	} 
	current=(dnode *)malloc(sizeof(dnode));
	if(current==NULL){
		return NULL;
	}

	next_element=element->next;
	element->next=current;
	current->next=next_element;
	current->previous=element;
	current->dptr=malloc(dsize);
	current->dsize=dsize;
	if(current->dptr!=NULL){
		bcopy(dptr, current->dptr, dsize);
	}
	return current;
}	

void * get_ddata(dnode *current)
{
	return current->dptr;
}

size_t get_dsize(dnode *current)
{
	return current->dsize;
}

dnode * disconnect_delement(dnode *element)
{

        dnode *previous;
	dnode *next;

        previous=element->previous;
	next=element->next;

	/* don't relink if we are removing the head */
	if(element->previous != NULL){
		/* relink if not at tail */
		if(element->next!=NULL){
			previous->next=next;
			next->previous=previous;
		} else {
			previous->next=NULL;
		}
	}

	element->next=NULL;
	element->previous=NULL;

	return next;
}


dnode * connect_delement(dnode * first, dnode * element)
{
        dnode *tail;

	tail=find_dtail(first);

	tail->next=element;
	element->previous=tail;
	tail=tail->next;

        return tail;
}

