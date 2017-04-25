	#include <stdlib.h>
	#include <stdio.h>
	#include <pthread.h>
	#include <semaphore.h>
	
	#define N 5  /* qtdade de  sensores */
	
	sem_t semaforoSensor[N];        //um semaforo por sensor
	sem_t filaCheia;                //exclusao mutua para regioes de acesso a fila cheia
	sem_t filaVazia; 				//exclusão mutua para fila vazia
	sem_t mutex;
	int   state[N];
	
	//array para controlar o estado dos filosofos
	//coluna 0=> valor aleatorio
	//coluna 1=> numero do sensor ou nome
	//coluna 2=> dupla do sensor
	int  sensores[N][3];
	int numeroAleatorio;
	int i;
	//uma thread para cada filósofo
	
	
	// prototipos...
	void geraDadosSensores();
	
	void pegaDuplaSensor(int i);
	
	/* i: numero do sensor, de 0 a N-1 */
	void geraDadosSensores(){
		int b;
		srand(time(NULL));
	  for(b= 0; b < N ;b++ ){
		numeroAleatorio=(rand())%50;//pega o resto da divisão do numero aleatorio por 50 (um numero menor q 50)
		printf("numero aleatorio %d \n", numeroAleatorio);
		sensores[b][0]=numeroAleatorio;
	 	sensores[b][1]=b;//colocar  o nome do sensor talvez (?)	
		sensores[b][2]=-1;//-1 para nao tem dupla, e  outro valor com o i da dupla
		}
	}
	
	void mostraDadoSensores(){
		int b;
		sleep(2);
		printf("\n");
		for(b= 0; b < N ;b++ ){
			printf("sensor %d - dupla %d  \n", b, sensores[b][2]);	
		}
		printf("\n");
	
	}
	
	
	void *sensor(void *j) {
		int i = *(int *)j;
		while (1) {  /* repete eternamente */
			sleep(1);
			pegaSensor(i);
			mostraDadoSensores();
			escrita(i);
		}
	}
	
	void pegaSensor(int i){
		sem_wait(&mutex);
		
		int dupla;
		if (sensores[i][2]==-1){//Não tem dupla, procura dupla
		 	procuraDupla(i);
		}
	
		sem_post(&mutex);
		sem_wait(&semaforoSensor[i]);	
	}
	
	void procuraDupla(int i){
		srand(time(NULL));
		int sensorAleatorio;
		int achou=0;
		while (achou==0){
			sensorAleatorio=(rand())%5;// aleatorio de 0 a 4	
			if (sensores[sensorAleatorio][2]==-1 && sensorAleatorio!=i){
				sem_post(&semaforoSensor[i]);
				sensores[sensorAleatorio][2]=i;
				sensores[i][2]=sensorAleatorio;
				printf("\n ================ %d é a dupla de %d ============== \n", sensorAleatorio, i);
				achou= 1;
				break;		
			}	
		}	
	}
	
	void escrita(int i){
		
		if (sensores[i][2]!=-1){
			int semaforoDupla=sensores[i][2];
	
			sensores[i][2]=-1;
			sensores[semaforoDupla][2]=-1;
			sleep(1);
			printf("----->>>>escrevendo e liberando sensores \n");
			sem_post(&semaforoSensor[i]);
	
		}
	}
	//////MAIN///////////
	main(){
	
	int  iret1, iret2, iret3, iret4, iret5;
	
	geraDadosSensores(); //inclui os valores aleatórios para os sensores
	void *thread_result;
	pthread_t thread[N];
	sem_init(&mutex, 0, 1);
	
	
	mostraDadoSensores();
	//inicialização dos semáforos por sensor...
	
	    for(i= 0; i < N ;i++ ){
			sem_init(&semaforoSensor[i], 0, 1);
		}
	
		for(i=0;i<N;i++){
			pthread_create(&thread[i],NULL, sensor, &i);
		}	
	
		for(i=0;i<N;i++){
			pthread_join(thread[i],&thread_result);
		
		}	
		
	}

