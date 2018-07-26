# uSh

uSh (Micro Shell), is a small interpreter with a syntax similar to Tcl/Tk. Is
written in pure ANSI C and can be easily extended. I wrote this program to
improve my skills programming in C.

[Source Code in Github](https://github.com/harkaitz/uSh)

The prebuilt binaries can be found in [dist](dist) directory.

## Features

uSh has a very simple syntax. You can check it's syntax reading the example
files; `src/ushc-test` 

- It supports flow control; `if,else,elif,while,foreach,for`
- Basic arithmetic;         +,-,...
- Custom functions;         `proc,exec`
- Variables;                `set`

### Building

Build as any other `cmake` project.

### Running

```
> ushc
> ushc <filename>
```

## Example code;

```
#!/usr/bin/env ushc
# This is a comment.
echo "1) First line."
if 0 { echo "2) err: if 0 <code> is evaluated." }
if 1 { echo "2) if 1 <code> working well." }
if 0 { } else { echo "3) else working well." }
if 0 { } elseif 1 { echo "4) elseif working well." }
# foreach.
foreach a 1 2 3 4 {
    echo "5) foreach" $a
}
# for.
for {set i 0} {<= $i 2} {set i (+ $i 1)} {
    for {set j 0} {<= $j 2} {set j (+ $j 1)} {
	if (== $i $j) {
	    continue
	}
	echo "5) for" $i $j
    }
}
# while.
set i 0
while {< $i 4} {
    echo "6) while" $i
    set i (+ $i 1)
}
# proc,exec.
proc kaixo {} {
    echo "7) proc,exec:" 1 $1 2 $2
}
exec kaixo a b
# error
error "8) error, The program should end here"
echo "E) error not working."
```
```
ush: 8) error, The program should end here
1) First line.
2) if 1 <code> working well.
3) else working well.
4) elseif working well.
5) foreach 1
5) foreach 2
5) foreach 3
5) foreach 4
5) for 0 1
5) for 0 2
5) for 1 0
5) for 1 2
5) for 2 0
5) for 2 1
6) while 0
6) while 1
6) while 2
6) while 3
7) proc,exec: 1 a 2 b
```

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md)

## Versioning

We use [SemVer](http://semver.org/) for versioning.

## Authors

* Harkaitz Agirre Ezama.

## License

Read [LICENSE](LICENSE) file for details

