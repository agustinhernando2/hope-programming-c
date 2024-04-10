#!/bin/bash
gnome-terminal --title="server-AF_INET" -- ./build/server/server 4495 1

sleep 1

gnome-terminal --title="client-01" -- ./build/client/client localhost 4495
