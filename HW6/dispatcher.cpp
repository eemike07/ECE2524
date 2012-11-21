// ECE 2524 Homework 6: Heard it on the Pipeline
// Michael Surratt

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <iostream>

using namespace std;
 
int main() {
	pid_t gen_pid, con_pid;
	int pipe_1[2], exitcode;

	if (pipe(pipe_1) == -1){
        exit(EXIT_FAILURE);
    }
    // Generator Child Process /////////////////////////////////////////
    gen_pid = fork();
    if(gen_pid == 0){
		close(pipe_1[0]);  // Close the read end. 
		dup2(pipe_1[1],1);
		execv("./generator", NULL);
	}
	// Consumer Child Process //////////////////////////////////////////
	con_pid = fork();
	if(con_pid == 0){
		close(pipe_1[1]);  // Close the write end.
		dup2(pipe_1[0],0);
		execv("./consumer", NULL);
	}
	if(gen_pid == -1 || con_pid == -1){
		exit(EXIT_FAILURE);
	}
	close(pipe_1[0]);
	close(pipe_1[1]);
	
	sleep (1);
	kill(gen_pid, SIGTERM); 
	
	sleep(1);
	waitpid(gen_pid, &exitcode, 0);
	cerr << "child[" << gen_pid << "] exited with status " << exitcode << endl;
	waitpid(con_pid, &exitcode, 0);
	cerr << "child[" << con_pid << "] exited with status " << exitcode << endl;
	
	return 0;
}
