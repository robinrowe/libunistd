// semaphore_server.cpp: Illustration of simple semaphore passing
// Inspired by: http://users.cs.cf.ac.uk/Dave.Marshall/C/node26.html
/*   Each process communicates via a semaphore.
 *   The respective process can only do its work (not much here)
 *   When it notices that the semaphore track is free when it returns to 0
 *   Each process must modify the semaphore accordingly
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main()
{	int semid; /* semid of semaphore set */
	key_t key = 1234; /* key to pass to semget() */
	int semflg = IPC_CREAT | 0666; /* semflg to pass to semget() */
	int nsems = 1; /* nsems to pass to semget() */
	int nsops; /* number of operations to do */
	sembuf sops;
	/* ptr to operations to perform */
	/* set up semaphore */
	printf("\nsemget: Setting up seamaphore %d: semget(%#lx, %#o)\n",key, nsems, semflg);
	if((semid = semget(key, nsems, semflg)) == -1)
	{	perror("semget: semget failed");
		return 1;
	}
	printf("semget: semget succeeded: semid =%d\n", semid);
	/* allow for 3 semaphore sets */
	for(int i = 0;i < 3;i++)
	{	nsops = 2;
		/* wait for semaphore to reach zero */
		sops[0].sem_num = 0;
		sops[0].sem_op = 0; /* wait for semaphore flag to become zero */
		sops[0].sem_flg = SEM_UNDO; /* take off semaphore asynchronous  */
		sops[1].sem_num = 0;
		sops[1].sem_op = 1; /* increment semaphore -- take control of track */
		sops[1].sem_flg = SEM_UNDO | IPC_NOWAIT; /* take off semaphore */
		/* Recap the call to be made. */
		printf("\nsemop:Parent Calling semop(%d, &sops, %d) with:", semid, nsops);
		int j = 0;
		for(; j < nsops; j++)
		{	printf("\n\tsops[%d].sem_num = %d, ", j, sops[j].sem_num);
			printf("sem_op = %d, ", sops[j].sem_op);
			printf("sem_flg = %#o\n", sops[j].sem_flg);
		}
		/* Make the semop() call and report the results. */
		if((j = semop(semid, sops, nsops)) == -1)
		{	perror("semop: semop failed");
			continue;
		}
		printf("semop: semop returned %d\n", j);
		printf("Parent Process Taking Control of Track: %d/3 times\n", i+1);
		sleep(5); /* Do nothing for 5 seconds */
		nsops = 1;
		/* wait for semaphore to reach zero */
		sops[0].sem_num = 0;
		sops[0].sem_op = -1; /* Give UP COntrol of track */
		sops[0].sem_flg = SEM_UNDO | IPC_NOWAIT; /* take off semaphore, asynchronous  */
		if((j = semop(semid, sops, nsops)) == -1)
		{	perror("semop: semop failed");
			continue;
		}
		printf("Parent Process Giving up Control of Track: %d/3 times\n", i+1);
		sleep(5); /* halt process to allow child to catch semaphor change first */
	}
}