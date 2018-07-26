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
#ifndef _USH_LOW_H_
#define _USH_LOW_H_
#include "ush.h"
/* base.c & lists.c */
ush_cell_t *ush_cell_new(ush_t *ush);
void        ush_cell_del(ush_cell_t *cell);
ush_cell_t *ush_cell_get(ush_t *ush,const char *name);

int   ush_cell_set_empty  (ush_cell_t *cell);
int   ush_cell_set_name   (ush_cell_t *cell,const char *name);
int   ush_cell_set_string (ush_cell_t *cell,const char *value);
int   ush_cell_set_malloc (ush_cell_t *cell,const void *data,int len);
int   ush_cell_set_command(ush_cell_t *cell,ush_cmd_t c);

void  ush_cell_string_to_malloc(ush_cell_t *cell);
const char *ush_cell_get_string (ush_cell_t *cell);
int         ush_cell_get_integer(ush_cell_t *cell);
int         ush_cell_get_count  (ush_cell_t *cell);

void ush_cell_append     (ush_cell_t *cell,ush_cell_t *c2);
void ush_cell_print      (ush_cell_t *cell,FILE *fp);

#define USH_FOREACH_CELL_ALL(C,U) \
     for(ush_cell_t *C = (U)->cells; (C)->type != 'l'; (C)++)
#define USH_FOREACH_CELL_EMPTY(C,U) \
     USH_FOREACH_CELL_ALL(C,U) if((C)->type == 'd')
#define USH_FOREACH_CELL(C,U) \
     USH_FOREACH_CELL_ALL(C,U) if((C)->type != 'd')

/* parser.c */
ush_cell_t *ush_parser_add_chr(ush_t *ush,ush_cell_t *lst,char c,int start);
ush_cell_t *ush_parse(ush_t *ush,const char *str);
void        ush_eval (ush_cell_t **inp);



#endif
