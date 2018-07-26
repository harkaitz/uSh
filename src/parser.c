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
#include <string.h>
/* ----------------------------------------------------------------------- */
ush_cell_t *ush_parser_add_chr(ush_t *u,ush_cell_t *lst,char cc,int start) {
     if(start) {
	  u->p.escape  = 0;
	  u->p.level  = 0;
	  u->p.point  = 0;
     }
     
     char c = (u->p.level == 0 && strchr("\n;",cc))?'\0':cc;
     if(u->p.level == 0) {
	  /* The command ended, return the list. */
	  if(c=='\0') { return lst; }
	  /* Spaces, in level 0, are ignored. */
	  else if(u->p.level == 0 && strchr(" \t",c)) { return NULL; }
	  /* A word starts. */
	  u->p.level++;
	  u->p.point = 0;
	  if(c=='{') {
	       u->p.level_up = '{';
	       u->p.level_dw = '}';
	  } else if(c=='(') {
	       u->p.level_up = '(';
	       u->p.level_dw = ')';
	  } else if(c=='[') {
	       u->p.level_up = '[';
	       u->p.level_dw = ']';
	  } else if(c=='\'') {
	       u->p.level_up = '\'';
	       u->p.level_dw = '\'';
	  } else if(c=='"') {
	       u->p.level_up = '"';
	       u->p.level_dw = '"';
	  } else if(c=='$') {
	       u->p.level_up = '$';
	       u->p.level_dw = ' ';     
	  } else {
	       u->p.level_up = ' ';
	       u->p.level_dw = ' ';
	       return ush_parser_add_chr(u,lst,c,0);
	  }
	  return NULL;
     }
     
     /* The second in escape sequence. */
     else if (u->p.escape) {
	  switch(c) {
	  case 'n' : c = '\n'; break;
	  case 't' : c = '\t'; break;
	  case '\\': c = '\\'; break;
	  }
	  u->p.buffer[u->p.point++] = c;
	  u->p.escape = 0;
     }

     /* The first in escape sequence. */
     else if (strchr(" \"",u->p.level_dw) && c == '\\') {
	  u->p.escape = 1;
     }

     
     /* Word end. */
     else if (c == '\0' || u->p.level_dw == c || (u->p.level_dw == ' ' && strchr("\n;",c))) {
	  u->p.level = (c=='\0')?0:u->p.level-1;
	  if(u->p.level) {
	       u->p.buffer[u->p.point++] = c;
	  } else {
	       u->p.buffer[u->p.point++] = '\0';
	       ush_cell_t *nc = NULL;
	       if(strchr("[",u->p.level_up)) {
		    nc = ush_parse(u,u->p.buffer);
                    ush_eval(&nc);
	       } else if(strchr("$",u->p.level_up)) {
		    nc = ush_cell_new(u);
		    const char *v  = ush_cell_get_string(ush_cell_get(u,u->p.buffer));
		    if(!v) {
			 ush_set_error(u,"Variable `%s` not found.",u->p.buffer);
			 ush_cell_set_string(nc,"");
		    } else {
			 ush_cell_set_string(nc,v);
			 ush_cell_string_to_malloc(nc);
		    }
	       } else {
		    if(strchr("(",u->p.level_up)) {
			 nc = ush_parse(u,u->p.buffer);
			 int n = ush_integer(&nc);
			 ush_cell_del(nc);
			 snprintf(u->p.buffer,sizeof(u->p.buffer)-1,"%i",n);
		    }
		    nc = ush_cell_new(u);
		    ush_cell_set_string(nc,u->p.buffer);
		    ush_cell_string_to_malloc(nc);
	       }
	       if(nc) ush_cell_append(lst,nc);
	  }
	  if(c=='\0' || (u->p.level_dw == ' ' && strchr("\n;",c)))
	       return lst;
     }

     /* Up in the structure. */
     else if (u->p.level_up == c && u->p.level_up != '$') {
	  u->p.level++;
	  u->p.buffer[u->p.point++] = c;
     }

     /* A simple character. */
     else {
	  u->p.buffer[u->p.point++] = c;
     }
     return NULL;
}

ush_cell_t *ush_parse(ush_t *ush,const char *str) {
     ush_cell_t  *lst = ush_cell_new(ush);
     ush_cell_set_malloc(lst,str,-1);
     
     
     for(int i=0;1;i++) {
	  ush_cell_t *ret = ush_parser_add_chr(ush,lst,lst->d.m[i],!i);
	  if(ret) break;
	  if(!lst->d.m[i]) break;
     }
     
     if(lst->next) {
	  ush_cell_t *ret = lst->next;
	  lst->next = NULL;
	  ush_cell_del(lst);
	  ush_cell_string_to_malloc(ret);
	  return ret;
     } else {
	  return NULL;
     }
}
void ush_eval(ush_cell_t **pinp) {
     /* Split command and arguments. */
     ush_cell_t *cmd = *pinp; if(!cmd) return;
     ush_cell_t *arg = cmd->next; cmd->next = NULL;
     /* The return value to NULL, by default. */
     *pinp = NULL;
     /* Get first string. */
     const char *cmdname = ush_cell_get_string(cmd);
     if(!cmdname) {
	  ush_set_error(cmd->ush,"First word must be a command.");
	  goto cleanup;
     }
     /* Discard comments. */    
     if(cmdname[0]=='#') { goto cleanup; } 
     
     
     /* Search command. */
     ush_cell_t *c = ush_cell_get(cmd->ush,cmdname);
     
     if(!c) {
	  ush_set_error(cmd->ush,"Command `%s` not found.",cmdname);
	  goto cleanup;
     }
     /* If the first is not a command. */
     if(c->type!='c') {
	  ush_set_error(cmd->ush,"`%s` is not a command.",cmdname);
	  goto cleanup;
     }
     /* Execute command. */
     c->d.c(cmd->ush,&arg,pinp);
cleanup:
     ush_cell_del(cmd);
     if(arg) { ush_cell_del(arg); }
}










void ush_test_parser(void) {
#    ifdef DEBUG
     ush_t ush;
     ush_init(&ush);
     ush_cell_t *c = ush_parse(&ush,"echo kaixo [echo a] $b$b {muindua\nadsd}\n");
     ush_cell_print(c,stdout);
     ush_clean(&ush);
#    endif
}


