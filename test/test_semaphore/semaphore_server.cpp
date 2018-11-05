// sem_server.c
// Inspired by https://gist.github.com/aspyct/3118858

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char *argv[])
{	sembuf sop;
	key_t sem_key = 1234; //ftok("./sem_server.c", 42);
	// Create the semaphore
	int sem_id = semget(sem_key, 1, IPC_CREAT | IPC_EXCL | 0600);
	if(semctl(sem_id, 0, SETVAL, 0) < 0)
	{	perror("Could not set value of semaphore");
		return 1;
	}
	puts("Now create some clients");
	sleep(5);
	printf("Increasing sem count\n");
	sop.sem_num = 0;
	sop.sem_op = 1;
	sop.sem_flg = 0;
	if(semop(sem_id, &sop, 1))
	{	perror("Could not increment semaphore");
		return 2;
	}
	sleep(30);
	if(semctl(sem_id, 0, IPC_RMID) < 0)
	{	perror("Could not delete semaphore");
	}
	return 0;
}