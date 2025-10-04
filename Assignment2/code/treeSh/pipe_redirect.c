#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define PR_BUFFER_SIZE 256

int main() {
    int pipe_fds[2]; // pipe_fds[0] for read (a la stdin), pipe_fds[1] for write (a la stdout)
    pid_t pid;
    char messages[] = "Yellow submarine\n is the bluest\n of them all\n"; // what will happen if you remove the trailing '\n'??
    char buffer[PR_BUFFER_SIZE];
    char *s; // helper pointer for fgets return value
    int line_count;
    
    // Create a pipe
    if (pipe(pipe_fds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        // Close the write end of the pipe as the child will only read
        close(pipe_fds[1]);

	// Duplicate the pipe "stream" onto stdin
	// The macro expands into just the number 0 (zero)
	dup2(pipe_fds[0], STDIN_FILENO); 
	// Now if we do "read(...) or scanf(...) they will access the same stream
	// This is because there are two structures "accessing" the same "stream"
	// We don't want that, so...
	close(pipe_fds[0]); // because we will be using a standard C lib to read

	// no let's read what the parent sends us. --- this next bit will look very familiar once you have a look at treeSh.c 

	line_count = 0;
	while(1){
	  s = fgets(buffer, PR_BUFFER_SIZE, stdin); // please notice that we using the stdin "pointer", but it now points to the pipe !

	  if (s == NULL ) {
	    printf("\n Soooo much talking...\n");
	    fflush(stdout);
	    exit(EXIT_SUCCESS);
	  }
	  printf("This is line %d: %s",line_count++,buffer);	  
	}
	  
        exit(EXIT_SUCCESS); // we should never reach this line, but just in case
    } else { // Parent process
        // Close the read end of the pipe as the parent will only write
        close(pipe_fds[0]);

        // Write data to the pipe -- notice that we are using a system call here, rather than a standard C library function
	// Because of it, we are using a file descriptor (an integer), rather than FILE*
        if (write(pipe_fds[1], messages, sizeof(messages)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        printf("Parent sent: %s\nEND OF MESSAGES\n", messages);
	fflush(stdout);
        // Close the write end of the pipe
        close(pipe_fds[1]);

        // Wait for the child process to terminate
        wait(NULL);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
