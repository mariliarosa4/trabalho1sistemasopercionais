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
sem_t mutex;                //exclusao mutua para regioes cri�ticas

//array para controlar o estado dos filosofos
int  sensores[N][2];

pthread_t thread1, thread2, thread3, thread4, thread5;
//uma thread para cada fil�sofo


// prototipos...
void geraDadosSensores();

void pegaDuplaSensor(int i);
void put_forks(int i);

void test(int i);
void think(int i);

void eat(int i);

/* i: numero do sensor, de 0 a N-1 */

void sensor(int i) {

while (TRUE) {  /* repete eternamente */

 take_forks(i); /* pega dois garfos ou bloqueia */
 eat(i);  /* come espaguete */
 put_forks(i); /* coloca os dois garfos de volta na mesa */

}
}

void take_forks(int i) {

sem_wait(&mutex);//down(&mutex); /* entra na regiao cri�tica */

    state[i] = HUNGRY; /* registra que o filosofo i esta com fome */
printf("sensor %d HUNGRY\n",i);
test(i);  /* tenta pegar 2 garfos */

sem_post (&mutex);//up(&mutex);  /* sai da regiao cri�tica */
    sem_wait(&s[i]);//down(&s[i]); /* bloqueia se os garfos nao foram pegos */

}

void put_forks(i) {
sem_wait(&mutex); //down(&mutex); /* entra na regiao critica */

    state[i] = THINKING;/* o filosofo acabou de comer */
printf("sensor %d THINKING\n",i);

test(LEFT);  /* verifica se o vizinho da esquerda pode comer agora */
test(RIGHT); /* verifica se o vizinho da direita pode comer agora */
sem_post(&mutex);//up(&mutex);  /* sai da regiao cri�tica */

}

void test(i) {  //testa se os filosofos vizinhos podem comer
     if (state[i]==HUNGRY && state[LEFT]!=EATING && state[RIGHT]!=EATING){

   state[i] = EATING;
   printf("sensor %d EATING\n",i);
   sem_post(&s[i]);//up(&s[i]);

     }
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

//inicializa��o dos sem�foros...
int sem_init(sem_t *sem, int pshared, unsigned int value);
     for(i= 0; i < N ;i++ )
{

sem_init(&s[i], 0, 1);
 p[i] = i;
}

sem_init(&mutex, 0, 1);

// cria��o de threads independentes que executarao a funcao...

iret1 = pthread_create( &thread1, NULL,(void *) sensor,
(int*)p[1]);

iret2 = pthread_create( &thread2, NULL,(void *) sensor,
(int*)p[2]);

iret3 = pthread_create( &thread3, NULL,(void *) sensor,
(int*)p[3]);

iret4 = pthread_create( &thread4, NULL,(void *) sensor,
(int*)p[4]);

iret5 = pthread_create( &thread5, NULL,(void *) sensor,
(int*)p[0]);

pthread_join( thread1, NULL);

pthread_join( thread2, NULL);
pthread_join( thread3, NULL);
pthread_join( thread4, NULL);

pthread_join( thread5, NULL);

exit(0);
}

