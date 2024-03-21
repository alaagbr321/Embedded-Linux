# Section 1

## Q1 : List three Linux Distributions
* Ubuntu
* Debian 
* RedHat

## Q2 : From the slides what is the man command used for?
`man` command in Linux is used to display the user manual of any command that we can run on the terminal

## Q3 : What is the difference between rm and rmdir using man command?
* `rm` command is used to remove both files and non-empty directories
* `rmdir` command is used only to remove empty directories

## Q4 : Create the following hierarchy under your home directory
![hierarchy](https://github.com/alaagbr321/Embedded-Linux/assets/145403129/4c04af0e-0d66-4482-92f5-a3c06a307f0d)


### Solution :


```
mkdir ~/dir1
mkdir ~/dir1/dir11 ~/dir1/dir12

mkdir ~/docs
touch mycv

touch ~/Documents/OldFiles
```
### a . Remove dir11 with rmdir in one-step. What did you notice? And how did you overcome that?


`rmdir ~/dir11 ` won`t work as dir11 is a non-empty directory .

To overcome this issue  we use `rm -r `.

### b . Then remove OldFiles using `rmdir –p` command. State what happened to the hierarchy
Nothing happens as `rmdir -p` only works on directories .

### c . The output of the command pwd was /home/user. Write the absolute and relative path for the file mycv 


* Absolute Path : `/home/user/Documents/mycv`.
* Relative Path : `Documents/mycv`.


## Q5 : Copy the /etc/passwd file to your home directory making its name is mypasswd
`cp /etc/passwd ~/ `.

## Q6 : Rename this new file to be oldpasswd .
`mv ~/passwd ~/oldpasswd`

## Q7 : You are in /usr/bin, list four ways to go to your home directory

```
1. cd ~
2. cd /home/
3. cd ../../home/
4. cd
```
## Q8 : List Linux commands in /usr/bin that starts with letter w
` ls /usr/bin/w* `

## Q9 : What command `type` is used for?
`type` command is used to know if a certain command is a built-in command or external command and the path of its binary if it is an external command.

## Q10 : Show 2 types of command file in /usr/bin that start with letter c
```
cd : shell Built-in command
cp : /usr/bin/cp
```
## Q11 : Using man command find the command to read file.

Using `man -k` we can search for any pattern in the man pages.

## Q12 : What is the usage of `apropos` command?
`apropos` is a command to search the manual page names and descriptions.

# Section 2
Write a bash script to:
1. Create a directory called "myDirectory" in your home folder.
2. Inside "myDirectory," create another directory named "secondDirectory."
3. Within "secondDirectory," create a file named "myNotePaper."
4. Copy "myNotePaper" from "secondDirectory" to "myDirectory."
5. Rename the copied file in "myDirectory" to "myOldNotePaper."
## Solution :
```
#!/bin/bash
mkdir myDirectory
mkdir myDirectory/secondDirectory
touch myDirectory/secondDirectory/myNotePaper
cp    myDirectory/secondDirectory/myNotePaper myDirectory/
mv    myDirectory/myNotePaper myDirectory/myOldNotePaper
```
