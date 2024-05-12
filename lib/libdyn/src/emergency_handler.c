#include <emergency_handler.h>


int run_emergency_handler(int* pipe_fd)
{
    // close read end
    close(pipe_fd[0]);  
    sleep(9999);
    // write to pipe
    char message[] = "Server failure. Emergency notification sent to all connected clients.";
    if (write(pipe_fd[1], message, sizeof(message)) == -1) {
        perror("write");
        return 1;
    }
    // send SIGINT to parent
    kill(getppid(), SIGINT);
    // Close write end
    close(pipe_fd[1]);  
    return 0;
}
