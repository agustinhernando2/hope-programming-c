#!/bin/bash
gnome-terminal --title="server-AF_INET" -- ./build/server/server 4486 unix

sleep 1

gnome-terminal --title="client-01" -- ./build/client/client localhost 4486
