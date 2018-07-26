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
#include <stdio.h>
#include "ush.h"
int main(int argc,char *argv[]) {
     ush_t ush; int ret = 1;
     ush_init(&ush);
     ush_library_init(&ush);
     if(argc==1) {
	  ush_set_string(&ush,"MESSAGE",
			 "\n"
			 "uSh - A small, extensible interpreter."                    "\n"
			 "Copyright (c) 2018, Harkaitz Agirre, All rights reserved." "\n"
			 ""                                                          "\n"
			 "It's not finished, but you can test it. Try;"              "\n"
			 "--------------------------------\n"
                         "  if (== (+ 2 2) 5) {       \n"
                         "      echo \"No No\"        \n"
                         "  } else {                  \n"
                         "      echo Hurray!!         \n"
                         "  }                         \n"
			 "-------------------------------\n"
			 "Type `exit` without quotes to quit.\n"
			 ,0);
	  ush_loop(&ush,(void*)getc,stdin);
	  ret = 0;
     }
#    if defined(__unix__) || defined(_WIN32) 
     else {
	  FILE *fp = fopen(argv[1],"rb");
	  if(fp) {
	       ush_clr_flag(&ush,'i');
	       ush_loop(&ush,(void*)getc,fp);
	       fclose(fp);
	       ret = 0;
	  } else {
	       ush_set_error(&ush,"Can't open `%s`.",argv[1]);
	       ret = 1;
	  }
     }
#    endif
     ush_clean(&ush);
     return ret;
}
