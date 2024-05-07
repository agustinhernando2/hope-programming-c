#!/bin/bash
port=44756
if [ -n "$2" ]; then
    port="$2"
fi

if [ "$1" = "debug" ]; then
    # Debug ex
    gnome-terminal --title="server - gdb" -- bash -c "gdb ./build/server/server -ex 'set follow-fork-mode child' -ex 'run $port unix'; exec bash" &
    sleep 1
    gnome-terminal --title="client - gdb" -- bash -c "gdb ./build/client/client -ex 'set follow-fork-mode child' -ex 'run localhost $port'; exec bash" &
elif [ "$1" = "valgrind" ]; then
    # Valgrind ex
    gnome-terminal --title="server - valgrind" -- bash -c "valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./build/server/server $port unix; exec bash" &
    sleep 1
    gnome-terminal --title="client - valgrind" -- bash -c "valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./build/client/client localhost $port; exec bash" &
else
    # Normal ex
    gnome-terminal --title="server" -- ./build/server/server $port unix
    sleep 1
    gnome-terminal --title="client" -- ./build/client/client localhost $port
fi
