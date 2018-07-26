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
#include <stdlib.h>
void USH_COMMAND_ECHO (ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     if(!(*inp)) return;
     const char *s = ush_cell_get_string(*inp);
     if(s) ush_printf(u,1,"%s%c",s,((*inp)->next)?' ':'\n');
     return USH_COMMAND_ECHO(u,&((*inp)->next),out);
}
void USH_COMMAND_EXIT (ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     ush_set_flag(u,'q'); ush_set_flag(u,'b');
}
void USH_COMMAND_EXPAND (ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     *out = *inp;
     *inp = NULL;
}
void USH_COMMAND_SET (ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     ush_cell_t *fnd = NULL;
     const char *var = ush_cell_get_string (*inp);
     const char *val = ush_cell_get_string ((*inp)->next);
     if(!var) { // 0 args
	  ush_set_error(u,"set VAR [VAL]");
     } else if(!val) { // 1 args
	  val = ush_cell_get_string(ush_cell_get(u,var));
	  if(val) {
	       *out = ush_cell_new(u);
	       ush_cell_set_malloc(*out,val,-1);
	  } else {
	       ush_set_error(u,"Variable `%s` not found.",var);
	  }
     } else if((fnd = ush_cell_get(u,var))) { // 2 args, already exists.
	  ush_cell_set_malloc(fnd,val,-1);
     } else { // 2 args, new.
	  ush_cell_set_name((*inp)->next,var);
	  (*inp)->next = (*inp)->next->next;
     }
}
void USH_COMMAND_CAT (ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     int l = 0;
     /* Calculate size. */
     for(ush_cell_t *i=*inp;i;i=i->next) {
	  const char *s = ush_cell_get_string(i);
	  l += (s)?strlen(s):0;
     }
     /* Create new cell. */
     //// TODO.
}

void USH_COMMAND_EVAL(ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     ush_eval_str(u,ush_cell_get_string(*inp),out);
}
void USH_COMMAND_BLOCK(ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     ush_eval_str(u,ush_cell_get_string(*inp),out);
}

void USH_COMMAND_CMD(ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     ush_eval(inp);
     *out = *inp;
     *inp = NULL;
}

void USH_COMMAND_IF (ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     ush_cell_t *p = *inp;
     if(ush_cell_get_integer(p)) {
	  p = p->next;
	  return USH_COMMAND_EVAL(u,&p,out);
     } else if((p = p->next)) {
	  return USH_COMMAND_CMD(u,&(p->next),out);
     }
}
void USH_COMMAND_FOREACH(ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     const char *var = ush_cell_get_string(*inp); if(!var) return;
     ush_cell_t *lst = (*inp)->next; if(!lst) return;
     ush_cell_t *blk; for(blk=lst->next;blk->next;blk=blk->next) {} // Get last.
     for(ush_cell_t *e=lst;e!=blk;e=e->next) {
	  ush_cell_set_name(e,var);
	  ush_cell_t *ign = NULL;
	  USH_COMMAND_BLOCK(u,&blk,&ign); ush_cell_del(ign);
	  ush_cell_set_name(e,"");
     }
}
void USH_COMMAND_FOR(ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     int c = ush_cell_get_count(*inp);
     if(c!=4) { ush_set_error(u,"For needs 4 arguments."); return; }
     const char *st =ush_cell_get_string((*inp));
     const char *ck =ush_cell_get_string((*inp)->next);
     const char *nx =ush_cell_get_string((*inp)->next->next);
     ush_cell_t *bd =(*inp)->next->next->next;
     /* Execute first. */
     if(st) {
	  ush_eval_str(u,st,out); ush_cell_del(*out); *out = NULL;
     }
     while(1) {
	  /* Check conditional. */
	  if(ck) {
	       ush_cell_t *nc = ush_parse(u,ck);
	       c = ush_integer(&nc);
	       ush_cell_del(nc);
	       if(!c) break;
	  }
	  /* Evaluate body. */
	  USH_COMMAND_BLOCK(u,&bd,out); ush_cell_del(*out); *out = NULL;
	  ush_clr_flag(u,'c');
	  if(ush_pop_flag(u,'b')) break;
          /* Evaluate next. */
	  if(nx) {
	       ush_eval_str(u,nx,out); ush_cell_del(*out); *out = NULL;
	  }
     }
}
void USH_COMMAND_WHILE(ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     int c = ush_cell_get_count(*inp);
     if(c!=2) { ush_set_error(u,"While needs 2 arguments."); return; }
     ush_cell_t *ck = (*inp);
     ush_cell_t *bd = (*inp)->next;
     (*inp) = ush_cell_new(u);
     (*inp)->next = ck;
     (*inp)->next->next = ush_cell_new(u);
     (*inp)->next->next->next = bd;
     return USH_COMMAND_FOR(u,inp,out);
}
void USH_COMMAND_CONTINUE(ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     ush_set_flag(u,'c');
}
void USH_COMMAND_BREAK(ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     ush_set_flag(u,'b');
}
void USH_COMMAND_PROC(ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     int c = ush_cell_get_count(*inp);
     if(c<3) { ush_set_error(u,"Proc at least needs 3 arguments."); return; }
     const char *cmd_name = ush_cell_get_string(*inp);
     if(cmd_name) {
	  ush_cell_t *body = (*inp)->next->next;
	  ush_cell_del(ush_cell_get(u,cmd_name));
	  ush_cell_set_name(body,cmd_name);
	  *out = *inp; *inp= (*out)->next;
	  (*inp)->next = NULL;
     }
}
void USH_COMMAND_EXEC(ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     int c = ush_cell_get_count(*inp);
     if(c<2) { ush_set_error(u,"Exec at least needs 2 arguments."); return; }
     /* Get cmd. */
     const char *cmd_name = ush_cell_get_string((*inp));
     ush_cell_t *cmd = ush_cell_get(u,cmd_name);
     if(!cmd) { ush_set_error(u,"Command `%s` not found.",(cmd_name)?cmd_name:"-"); return; }
     /* Get body. */
     u->level++;
     int i = 1;
     for(ush_cell_t *var=(*inp)->next;var;var=var->next) {
	  snprintf(var->name,sizeof(var->name)-1,"%i",i++);
     }
     ush_eval_str(u,ush_cell_get_string(cmd),out);
     u->level--;
}



void USH_COMMAND_SEQ(ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     ush_set_error(u,"TODO SEQ.");
}
void USH_COMMAND_EXISTS(ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     ush_set_error(u,"TODO EXISTS.");
}
void USH_COMMAND_EQUAL(ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     ush_set_error(u,"TODO EQUAL.");
}
void USH_COMMAND_ERROR(ush_t *u,ush_cell_t **inp,ush_cell_t **out) {
     char buffer[sizeof(u->err)]; buffer[0]='\0';
     for(ush_cell_t *c=*inp;c;c=c->next) {
	  const char *s = ush_cell_get_string(c);
	  if(s) {
	       strncat(buffer,s,sizeof(buffer)-1);
	       strncat(buffer,(c->next)?" ":"",sizeof(buffer)-1);
	  }
     }
     ush_set_error(u,"%s",buffer);
}




void ush_library_init(ush_t *ush) {
     ush_add_command(ush,"echo",USH_COMMAND_ECHO);
     ush_add_command(ush,"exit",USH_COMMAND_EXIT);
     ush_add_command(ush,"quit",USH_COMMAND_EXIT);
     ush_add_command(ush,"%"   ,USH_COMMAND_EXPAND);
     ush_add_command(ush,"set" ,USH_COMMAND_SET);

     
     ush_add_command(ush,"eval"   ,USH_COMMAND_EVAL);
     ush_add_command(ush,"if"     ,USH_COMMAND_IF);
     ush_add_command(ush,"elseif" ,USH_COMMAND_IF);
     ush_add_command(ush,"elif"   ,USH_COMMAND_IF);
     ush_add_command(ush,"else"   ,USH_COMMAND_EVAL);
     
     ush_add_command(ush,"foreach" ,USH_COMMAND_FOREACH);
     ush_add_command(ush,"for"     ,USH_COMMAND_FOR);
     ush_add_command(ush,"while"   ,USH_COMMAND_WHILE);
     ush_add_command(ush,"continue",USH_COMMAND_CONTINUE);
     ush_add_command(ush,"break"   ,USH_COMMAND_BREAK);

     ush_add_command(ush,"proc"    ,USH_COMMAND_PROC);
     ush_add_command(ush,"exec"    ,USH_COMMAND_EXEC);

     ush_add_command(ush,"error"    ,USH_COMMAND_ERROR);
}
