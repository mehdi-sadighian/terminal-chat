# terminal-chat
c++ chat server and client multithreaded

this chat server is written on UNIX environment (FREEBSD 10.3). its an exprimental program to send and recive
message simultaneously on a single socket with threads.

Note: if you want to compile in linux, remove this line:
#define _BSD_SOURCE


# Compilation:


on server side:
g++ chatserver.cpp -lpthread -o chatserver

on client side:
g++ chatclient.cpp -lpthread -o chatclient


# Usage:

on server side:
./chatserver <port>

example:
./chatserver 2017


on client side:
./chatclient <host name> <port>

example:
./chatclient 192.168.10.1 2017


# License

This software is released with no warranties and is under public domain.

mehdi.sadighian@hotmail.com

