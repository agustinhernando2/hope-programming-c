#!/bin/bash
gnome-terminal --title="server-AF_INET" -- ./build/server/server 4432 unix

sleep 1

gnome-terminal --title="client-01" -- ./build/client/client localhost 4432

#gdb ./build/client/client -ex "set follow-fork-mode child" -ex "br client.c:29" -ex "run localhost 4478"
#gdb ./build/server/server -ex "set follow-fork-mode child" -ex "br server.c:116" -ex "run 4478 unix"
