//sem_client.c
// Inspired by https://gist.github.com/aspyct/3118858

#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{	key_t sem_key = 1234;
	int sem_id = semget(sem_key, 0, 0);
	if(sem_id < 0)
	{	perror("Could not obtain semaphore");
		return 1;
	}
	sembuf sop;
	for(int i = 0; i < 5; ++i)
	{	sop.sem_num = 0;
		sop.sem_op = -1;
		sop.sem_flg = SEM_UNDO;
		printf("Client #%d waiting\n", getpid());
		semop(sem_id, &sop, 1);
		printf("Client #%d acquired. Sleeping\n", getpid());
		sleep(1);
		printf("Client #%d releasing\n", getpid());
		sop.sem_op = 1;
		semop(sem_id, &sop, 1);
	}
	return 0;
}
