#!/bin/sh
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
export PATH="`pwd`/bin:$PATH"
## -------------- Generate from templates ----------------------
if test ! -f hrknew.txt;then
    true
elif which hrknew 2>/dev/null >/dev/null;then
    hrknew -a
else
    echo "(*) Skipping regenerating from templates ..."
fi
## -------------- Generate README.md from README.md.in ---------
if test ! -f README.md.in;then
    true
elif which hrkreadme 2>/dev/null >/dev/null;then
    echo "Generating new readme from README.md.in ..."
    hrkreadme -o README.md README.md.in || echo "New README.md not generated."
else
    echo "(*) Skipping regenerating README.md ..."
fi
## -------------- Print file licenses --------------------------
if ! which hrklicense 2>/dev/null >/dev/null;then
    true
elif ! hrklicense -s 2>/dev/null;then
    echo "No license found, creating \`LICENSE.txt\` (MIT)..."
    hrklicense -p mit.txt > /tmp/LICENSE.txt.tmp
    mv /tmp/LICENSE.txt.tmp LICENSE.txt
    hrklicense -cl
else
    hrklicense -cl
fi
