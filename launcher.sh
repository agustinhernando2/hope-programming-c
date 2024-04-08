#!/bin/bash
gnome-terminal --title="server-AF_INET" -- ./build/server 4495

sleep 1

gnome-terminal --title="client-01" -- ./build/client localhost 4495
