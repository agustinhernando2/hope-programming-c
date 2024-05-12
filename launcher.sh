#!/bin/bash
port=44756
if [ -n "$2" ]; then
    port="$2"
fi

if [ "$1" = "debug" ]; then
    # Debug ex
    #gnome-terminal --title="server - gdb" -- bash -c "gdb ./build/server/server -ex 'set follow-fork-mode parent' -ex 'br server.c:28' -ex 'run'; exec bash" &
    gnome-terminal --title="server - gdb" -- bash -c "gdb ./build/server/server -ex 'run' -ex 'set follow-fork-mode child'; exec bash" &
    sleep 1
    # gnome-terminal --title="client6 - gdb" -- bash -c "gdb ./build/client/client -ex 'set follow-fork-mode child' -ex 'br client.c:48' -ex 'run localhost 4'; exec bash" &
    gnome-terminal --title="client6 - gdb" -- bash -c "gdb ./build/client/client -ex 'run localhost 4'; exec bash" &
elif [ "$1" = "valgrind" ]; then
    # Valgrind ex
    gnome-terminal --title="server - valgrind" -- bash -c "valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./build/server/server; exec bash" &
    sleep 1
    gnome-terminal --title="client4 - valgrind" -- bash -c "valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./build/client/client localhost 4; exec bash" &
    sleep 1
    gnome-terminal --title="client6 - valgrind" -- bash -c "valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./build/client/client localhost 6; exec bash" &
else
    # Normal ex
    gnome-terminal --title="server" -- ./build/server/server
    sleep 1
    gnome-terminal --title="client4" -- ./build/client/client localhost 4
    sleep 1
    gnome-terminal --title="client6" -- ./build/client/client localhost 6
fi
