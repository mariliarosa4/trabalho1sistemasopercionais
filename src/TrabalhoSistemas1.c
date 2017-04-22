//Bibliotecas utilizadas...
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include <semaphore.h>


#define N 5  /* qtdade de  sensores */
#define THINKING 0 /* filosofo pensando */
#define HUNGRY 1 /* filosofo tentando pegar garfos */
#define EATING 2 /* filosofo comendo */
#define TRUE 1

sem_t s[N];                 //um semaforo por sensor
sem_t mutex;                //exclusao mutua para regioes cri­ticas
int   state[N];
//array para controlar o estado dos filosofos
int  sensores[N][2];
int numeroAleatorio;

pthread_t thread[N];
//uma thread para cada filósofo


// prototipos...
void geraDadosSensores();

void pegaDuplaSensor(int i);
void put_forks(int i);

void test(int i);
void think(int i);

void eat(int i);

/* i: numero do sensor, de 0 a N-1 */
void geraDadosSensores(){
	int b;
  for(b= 0; b < N ;b++ ){
	srand(time(NULL));
	numeroAleatorio=(rand())%50;//pega o resto da divisão do numero aleatorio por 50 (um numero menor q 50)
	
		sensores[b][0]=numeroAleatorio;
 	sensores[b][1]=b;//colocar  o nome do sensor talvez (?)
}
}


void sensor(void *i) {

while (TRUE) {  /* repete eternamente */


 eat(i);  /* come espaguete */
 put_forks(i); /* coloca os dois garfos de volta na mesa */

}
}


void put_forks(i) {
sem_wait(&mutex); //down(&mutex); /* entra na regiao critica */

state[i] = THINKING;/* o filosofo acabou de comer */
printf("sensor %d THINKING\n",i);

sem_post(&mutex);//up(&mutex);  /* sai da regiao cri­tica */

}


void think(int i) {
/*Filosofo esta pensando...*/
	sleep(1);
	return;
}

void eat(int i) {
/*Filosofo esta comendo...*/
	sleep(1);
	return;
}

//////////////MAIN FUNCTION////////////////////////
main()
{

int  iret1, iret2, iret3, iret4, iret5;

int i;
int p[N] ;

void *thread_result;

//inicialização dos semáforos...
int sem_init(sem_t *sem, int pshared, unsigned int value);
     for(i= 0; i < N ;i++ ){
		sem_init(&s[i], 0, 1);
 		p[i] = i;//velho
}

sem_init(&mutex, 0, 1);
geraDadosSensores(); //inclui os valores aleatórios para os sensores
// criação de threads independentes que executarao a funcao...

for(i=0;i<N;i++){
	pthread_create(&thread[i],NULL, sensor, &i);
}

for(i=0;i<N;i++){
	pthread_join(&thread[i],&thread_result);
}
return 0;
}

