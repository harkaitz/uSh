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
#include "ush-low.h"
#include <stdlib.h>
#include <stdarg.h>
#ifndef USH_FREE
#  define USH_FREE(P) free(P)
#endif
#ifndef USH_MALLOC
#  define USH_MALLOC(L) malloc(L)
#endif
/* ------------------------------------------------------------------------
 * USH CONSTRUCTOR AND DESTRUCTOR.
 * ------------------------------------------------------------------------ */
void ush_init  (ush_t *ush) {
     ush->fp[0] = stdin;
     ush->fp[1] = stdout;
     ush->fp[2] = stderr;
     for(int i=0;i<(USH_MAX_VARIABLES-1);i++) {
	  ush->cells[i].type = 'd';
	  ush->cells[i].name[USH_NAME_MAXIMUN_LENGTH-1] = '\0';
	  ush->cells[i].ush  = ush;
     }
     ush->cells[USH_MAX_VARIABLES-1].type = 'l';
     ush->level = 0;
     ush->err[0] = '\0';
     strcpy(ush->flags,"i--------");
     ush->t.start = 1;
     ush->t.lst   = NULL;
}
int  ush_cell_set_empty  (ush_cell_t *cell) {
     if(!cell) return 0;
     if(cell->type=='m') { USH_FREE(cell->d.m); }
     cell->type = 'n';
     return 0;
}
void ush_clean (ush_t *ush) {
     USH_FOREACH_CELL(c,ush) {
	  ush_cell_set_empty(c);
	  c->type = 'd';
     }
}









void ush_printf(ush_t *ush,int num,const char *format,...) {
     if(ush->fp[num]) {
	  va_list args;
	  va_start (args, format);
	  vfprintf(ush->fp[num],format, args);
	  va_end (args);
     }
}
void ush_set_error(ush_t *ush,const char *format,...) {
     va_list args;
     ush_set_flag(ush,'e');
     va_start (args, format);
     vsnprintf(ush->err,sizeof(ush->err)-1,format, args);
     va_end (args);
     if(ush->fp[2]) {
	  fprintf(ush->fp[2],"ush: %s\n",ush->err);
     }
}







char *ush_tst_flag(ush_t *ush,char f) {
     return strchr(ush->flags,f);
}
void ush_set_flag(ush_t *ush,char f) {
     char *n = ush_tst_flag(ush,f); if(n) return;
     n = ush_tst_flag(ush,'-'); if(n) *n=f;
}
void ush_clr_flag(ush_t *ush,char f) {
     char *n = ush_tst_flag(ush,f); if(n) *n='-';
}
char *ush_pop_flag(ush_t *ush,char f) {
     char *n = ush_tst_flag(ush,f); if(!n) return NULL;
     *n = '-'; return n;
}
int  ush_cell_count(ush_t *ush) {
     int C = 0;
     USH_FOREACH_CELL(c,ush) { C++; }
     return C;
}





/* ----------------------------------------------------------------------
 * Create and destroy cells.
 * ---------------------------------------------------------------------- */
void ush_cell_del(ush_cell_t *cell) {
     if(!cell) return;
     ush_cell_set_empty(cell);
     cell->type = 'd';
     return ush_cell_del(cell->next);
}
ush_cell_t *ush_cell_new(ush_t *ush) {
     USH_FOREACH_CELL_EMPTY(c,ush) {
	  c->type  = 'n';
	  c->level = ush->level;
	  c->next  = NULL;
	  return c;
     }
     ush_set_error(ush,"Maximun number of cells reached.");
     return NULL;
}
int ush_cell_set_malloc(ush_cell_t *cell,const void *data,int len) {
     if(!cell) return 0;
     int l = (len==-1)?strlen(data)+1:len;
     void *m = USH_MALLOC(l);
     if(m) {
	  if(data) memcpy(m,data,l);
	  ush_cell_set_empty(cell);
	  cell->type = 'm';
	  cell->d.m  = m;
	  cell->length  = len;
	  return 0;
     } else {
	  ush_set_error(cell->ush,"Maximun memory reached.");
	  return -1;
     }
}




/* ---------------------------------------------------------------
 * Search and name cells.
 * --------------------------------------------------------------- */
ush_cell_t *ush_cell_get(ush_t *ush,const char *name) {
     USH_FOREACH_CELL(c,ush) {
	  if(c->level!=0) if(ush->level!=c->level) continue;
	  if(strcasecmp(c->name,name)) continue;
	  return c;
     }
     return NULL;
}
int ush_cell_set_name(ush_cell_t *cell,const char *name) {
     if(!cell) return 0;
     strncpy(cell->name,name,USH_NAME_MAXIMUN_LENGTH-1);
     return 0;
}
