#!/bin/bash

# Prompt user to enter the username
read -p "Enter the username to add: " username

# Check if the user exists
if getent passwd "$username" &>/dev/null; 
    then
        echo "User $username exists."
else
        sudo adduser $username
      
fi

# Prompt user to enter the group name
read -p "Enter the group name you want to add $username to : " groupname

# Check if the group exists
if getent group "$groupname" &>/dev/null; 
    then
        sudo usermod -a -G $groupname $username
        echo " $username has been added to $groupname successfully"
else
       echo "group doesn't exist."
      
fi