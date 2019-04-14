#include <stdio.h>
#include <string.h>         
#include <pthread.h>  // For Creating Thread.........!!
#include <stdlib.h>  
#include <time.h>     // To wait for a particular time
#include <semaphore.h>  

#define totalseats 3
#define SLEEP_MAX 5


//Using lock mutex;

pthread_mutex_t mutex;

int chairCount[3];
int counter = 0;        //Waiting students
int st2 = 0;
int tch2 = 0;

/* Using semaphores */

sem_t stdt;
sem_t assistant;
sem_t ta_teaching;

// Functions Declaration
void sleepfun(void);
void* stdbusy(void* stu_id);
void* teacher();

void sleepfun(void){
	int time = rand() % SLEEP_MAX + 1;
	sleep(time);
}

void* teacher()
{
	while(1)
	{
		sem_wait(&stdt);	
		
		//sem_wait(&mutex);
		pthread_mutex_lock(&mutex);
		
		printf("Teaching Assistant is teaching student %d\n",chairCount[tch2]);	
		chairCount[tch2]=0;
		counter--;
		
		printf("Students In Waiting Hall : [1] %d [2] %d [3] %d\n",chairCount[0],chairCount[1],chairCount[2]);
		tch2 = (tch2 + 1) % totalseats;
		
		sleepfun();

		printf("		 Teaching finish.\n");	

		//sem_post(&mutex);
		pthread_mutex_unlock(&mutex);

		sem_post(&assistant);
	}	
}


void* stdbusy  (void* stu_id)
{
	int id = *(int*)stu_id;

	printf("Student %d is programming\n",id);		
	
	while(1)
	{
		sleepfun();

		//sem_wait(&mutex);
		pthread_mutex_lock(&mutex);

		if(counter < totalseats)	
		{
			chairCount[st2] = id;
			counter++;
			
			printf("	Student %d is in Waiting Hall\n",id);
			printf("Students in Waiting Hall : [1] %d [2] %d [3] %d\n",chairCount[0],chairCount[1],chairCount[2]);
			st2 = (st2+1) % totalseats;
			
			//sem_post(&mutex);
			pthread_mutex_unlock(&mutex);
	
			//wakeup ta
			sem_post(&stdt);
			sem_wait(&assistant);
		}
		else // All Chairs are Filled
		{
			//sem_post(&mutex);
			pthread_mutex_unlock(&mutex);
			
			printf("No more chairs. student %d is programming\n",id);		
		}
	}				
}
int main(int argc, char **argv){

	//thread
	pthread_t *Stud;
	pthread_t assist;
	
	int* id;
	int stnum;
	
	//position
	int i;

	//Input the number of students
	printf("Enter the Number Of Students : ");
	scanf("%d", &stnum);

	//Initialization
	Stud = (pthread_t*)malloc(sizeof(pthread_t) * stnum);
	id = (int*)malloc(sizeof(int) * stnum);

	memset(id, 0, stnum);

	sem_init(&stdt,0,0);
	sem_init(&assistant,0,1);
	
	
	srand(time(NULL));


	pthread_mutex_init(&mutex,NULL);

	
	pthread_create(&assist,NULL,teacher,NULL);  // creating thread

	
	for(i=0; i<stnum; i++)
	{
		
		/* creating thread */
		id[i] = i+1;
		pthread_create(&Stud[i], NULL, stdbusy, (void*) &id[i]);
	} 
	
	pthread_join(assist, NULL);
	
	for(i=0;i<stnum;i++)
	{
		pthread_join(Stud[i],NULL);
	}	
	
	return 0;
}
