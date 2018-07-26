/**
 *l:
 *l:  Copyright (c) 2018 Harkaitz Agirre Ezama, harkaitzv@gmail.com
 *l:
 *l:  Permission is hereby granted, free of charge, to any person obtaining
 *l:  a copy of this software and associated documentation files (the
 *l:  "Software"), to deal in the Software without restriction, including
 *l:  without limitation the rights to use, copy, modify, merge, publish,
 *l:  distribute, sublicense, and/or sell copies of the Software, and to
 *l:  permit persons to whom the Software is furnished to do so, subject to
 *l:  the following conditions:
 *l:
 *l:  The above copyright notice and this permission notice shall be
 *l:  included in all copies or substantial portions of the Software.
 *l:
 *l:  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *l:  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *l:  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *l:  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *l:  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *l:  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *l:  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *l:
 */
#ifndef _USH_H_
#define _USH_H_
#include <stdio.h>
#include <string.h>
/* ----------------- BASE.C ----------------------------- */
#define USH_NAME_MAXIMUN_LENGTH 20
#define USH_ERR_MAXIMUN_LENGTH  256
#define USH_MAX_VARIABLES       80

typedef struct ush_s      ush_t;
typedef struct ush_cell_s ush_cell_t;


/* Command type. */
typedef void (*ush_cmd_t) (ush_t *u,ush_cell_t **inp,ush_cell_t **out);

/* A cell. */
struct ush_cell_s {
     char        type; // l:Last d:Deleted n:None m:Malloc s:String c:Command.
     char        name[USH_NAME_MAXIMUN_LENGTH];
     int         level;
     ush_cell_t *next;
     int         length;
     ush_t      *ush;
     union {
	  const char *s;
	  char       *m;
	  ush_cmd_t   c;
     } d;
};

/* The main object. */
struct ush_s {
     FILE       *fp[3];
     int         level;
     char        flags[10];
     ush_cell_t  cells[USH_MAX_VARIABLES];
     char        err[USH_ERR_MAXIMUN_LENGTH];
     /* Terminal. */
     struct {
	  int         start;
	  ush_cell_t *lst;
     } t;
     /* Parser. */
     struct {
	  int   escape;
	  int   level;
	  char  level_up;
	  char  level_dw;
	  int   point;
	  char  buffer[256];
     } p;
};

/* base.c & lists.c */
void ush_init      (ush_t *ush);
void ush_clean     (ush_t *ush);
void ush_set_error (ush_t *ush,const char *format,...);
void ush_printf    (ush_t *ush,int num,const char *format,...);

char *ush_tst_flag (ush_t *ush,char f);
void  ush_set_flag (ush_t *ush,char f);
void  ush_clr_flag (ush_t *ush,char f);
char *ush_pop_flag(ush_t *ush,char f);

int   ush_cell_count(ush_t *ush);

int         ush_add_command  (ush_t *ush,const char *name,ush_cmd_t c);
const char *ush_get_string   (ush_t *ush,const char *name);
const char *ush_set_string   (ush_t *ush,const char *name,const char *str,int m);


/* term.c */
void ush_print_prompt(ush_t *ush);
void ush_loop  (ush_t *ush,int (*gc) (void *),void *u);
int  ush_eat(ush_t *ush,char c);
void ush_eval_str(ush_t *ush,const char *str,ush_cell_t **out);
/* library.c */
void ush_library_init(ush_t *ush);

/* numbers.c */
int ush_integer(ush_cell_t **inp);
void USH_COMMAND_INTEGER (ush_t *u,ush_cell_t **inp,ush_cell_t **out);

#endif
