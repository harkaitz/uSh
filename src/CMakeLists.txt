#l:
#l:  Copyright (c) 2018 Harkaitz Agirre Ezama, harkaitzv@gmail.com
#l:
#l:  Permission is hereby granted, free of charge, to any person obtaining
#l:  a copy of this software and associated documentation files (the
#l:  "Software"), to deal in the Software without restriction, including
#l:  without limitation the rights to use, copy, modify, merge, publish,
#l:  distribute, sublicense, and/or sell copies of the Software, and to
#l:  permit persons to whom the Software is furnished to do so, subject to
#l:  the following conditions:
#l:
#l:  The above copyright notice and this permission notice shall be
#l:  included in all copies or substantial portions of the Software.
#l:
#l:  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#l:  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#l:  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#l:  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
#l:  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
#l:  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
#l:  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#l:
set(ush_SOURCES
  library.c numbers.c term.c parser.c lists.c base.c
  )
set(ush_HEADERS
  ush.h ush-low.h
  )

add_library(ush STATIC ${ush_SOURCES} ${ush_HEADERS})
install(TARGETS ush DESTINATION lib)
install(FILES ${ush_HEADERS} DESTINATION include/ush)

add_executable(ushc main.c)
target_link_libraries(ushc ush)
target_compile_definitions(ushc PRIVATE -DDEBUG)
install(TARGETS ushc DESTINATION bin)
install(
  FILES ushc-test
  DESTINATION bin
  PERMISSIONS
  OWNER_READ    OWNER_WRITE OWNER_EXECUTE
  GROUP_READ    GROUP_WRITE GROUP_EXECUTE
  WORLD_EXECUTE             WORLD_READ)
