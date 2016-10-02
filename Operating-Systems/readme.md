# My Shell

My Shell, was a small university assignment, at the operating systems course. 
This ~200 line shell supports execution of simple, as well as, multiple 
pipped commands with many arguments. Furthermore it supports absolute and 
relative paths and has integrated `cd` and `exit` commands.

## Build

Building is as easy as any C program.

```sh
> gcc myshell.c -o myshell
```

## Usage
```sh
> ./myshell
$cut -f 2 myfile.txt | sort -n | uniq | wc
      1       4      30
$
```

License
----
MIT