# Na2na2a:

## Q1 : List the available shells in your system.
```
cat /etc/shells 

OUTPUT:

/bin/sh
/bin/bash
/usr/bin/bash
/bin/rbash
/usr/bin/rbash
/usr/bin/sh
/bin/dash
/usr/bin/dash
```
## Q2 : List the environment variables in your current shell.

`env   || printenv`

## Q3 : Display your current shell name.
`echo $SHELL`

## Q4 : Create a Bash shell alias named PrintPath for the “echo $PATH” command
`alias PrintPath='echo $PATH'`