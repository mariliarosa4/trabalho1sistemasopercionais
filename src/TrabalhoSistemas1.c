	#include <stdlib.h>
	#include <stdio.h>
	#include <pthread.h>
	#include <semaphore.h>
	
	#define N 5  /* qtdade de  sensores */
	
	sem_t semaforoSensor[N];        //um semaforo por sensor
	sem_t escreveFila;                //exclusao mutua para regioes de acesso a fila cheia
	sem_t leFila; 				//exclusão mutua para fila vazia
	sem_t mutex;
	sem_t paraExame;
	
	int   state[N];
	struct Fila fila;
	//array para controlar o estado dos filosofos
	//coluna 0=> valor aleatorio
	//coluna 1=> numero do sensor ou nome
	//coluna 2=> dupla do sensor
	int  sensores[N][3];
	int i;
	//uma thread para cada filósofo
	
	
	// prototipos...
	void geraDadosSensores();
	
	void pegaDuplaSensor(int i);
	
	
////////////////////////////fila//////////////////////////////////////////
	 struct sSensor{
        int sensor1;
        int dadoSensor1;
		int sensor2;
		int dadoSensor2;
    };
	int maxTam = 2;
	struct Fila {
		int capacidade;
		struct sSensor dados[2];
		int primeiro;
		int ultimo;
		int nItens; 
	};
	
	struct Dados{
		int desvioPadrao;
		int media;
		int variancia;
		int quantidadeDados;
		int soma;
	}DADOS[5];
	
void criarFila( struct Fila *f) { 
	f->capacidade = 2;
	f->primeiro = 0;
	f->ultimo = -1;
	f->nItens = 0; 
}

void inserir(struct Fila *f, int sensor1, int dadoSensor1, int sensor2, int dadoSensor2) {
	if(f->ultimo == f->capacidade-1)
		f->ultimo = -1;
	f->ultimo++;
	f->dados[f->ultimo].sensor1 = sensor1; // incrementa ultimo e insere
	f->dados[f->ultimo].dadoSensor1 = dadoSensor1;
	f->dados[f->ultimo].sensor2 = sensor2;
	f->dados[f->ultimo].dadoSensor2 = dadoSensor2;
	f->nItens++; 
	mostrarFila(&fila);
}

struct sSensor remover( struct Fila *f ) { // pega o primeiro da fila
	struct sSensor temp = f->dados[f->primeiro++]; 
	if(f->primeiro == f->capacidade)
		f->primeiro = 0;
	f->nItens--;  
	return temp;
}

int estaVazia( struct Fila *f ) { // retorna verdadeiro se a fila estÃ¡ vazia
	return (f->nItens==0);
}

int estaCheia( struct Fila *f ) { // retorna verdadeiro se a fila estÃ¡ cheia
	return (f->nItens == f->capacidade);
}

void mostrarFila(struct Fila *f){
	int cont, i; 
	for ( cont=0, i= f->primeiro; cont < f->nItens; cont++){
		printf("Sensor 1 %d - dadoSensor1 %d - Sensor2 %d - dadoSensor2 %d ",f->dados[i].sensor1, f->dados[i].dadoSensor1, f->dados[i].sensor2,f->dados[i].dadoSensor2);
		if (i == f->capacidade)
			i=0;
	}
	printf("\n\n");
}
//////////////////////////fim das funcoes da fila/////////////////////////////////////////
	
	
	

	/* i: numero do sensor, de 0 a N-1 */
	void geraDadosSensores(int b){
		
		srand(time(NULL));
	
		int numeroAleatorio=(rand())%50;//pega o resto da divisão do numero aleatorio por 50 (um numero menor q 50)
		printf("numero aleatorio %d \n", numeroAleatorio);
		sensores[b][0]=numeroAleatorio;
	 	sensores[b][1]=b;//colocar  o nome do sensor talvez (?)	
	
		
	}
	void iniciaSensores(){
		int b;
		srand(time(NULL));
	  for(b= 0; b < N ;b++ ){
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
		while (1) { 
			sleep(1);
			geraDadosSensores(i);
			pegaSensor(i);
			escrita(i);
			visualizador();
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
				sleep(1);
				sensores[sensorAleatorio][2]=i;
				sensores[i][2]=sensorAleatorio;
				printf("\n ================ %d é a dupla de %d ============== \n", sensorAleatorio, i);
				achou= 1;
				break;		
			}	
		}	
	}
	
	void escrita(int i){
		sem_wait(&escreveFila);
		if (sensores[i][2]!=-1){
			if (!estaCheia(&fila)){
				
			int semaforoDupla=sensores[i][2];
	
			sensores[i][2]=-1;
			sensores[semaforoDupla][2]=-1;
			sleep(1);
			printf("\n ----->>>>escrevendo e liberando sensores \n");
			inserir(&fila,i, sensores[i][0], semaforoDupla,sensores[semaforoDupla][0] );
			mostrarFila(&fila);
			sem_post(&semaforoSensor[i]);
			sem_post(&escreveFila);
			}else{
				printf("\n\n\n----------------------------------------fila cheia");
			//bloqueia todos os sensores
		
		}
	}
}

	void visualizador(){
		
		sem_wait(&leFila);
		if (!estaVazia(&fila)){
			struct sSensor ParFila = remover(&fila);
			sumarizarDados(ParFila);
			sem_post(&escreveFila);
		}
			sem_post(&leFila);
	}
	
	void sumarizarDados(struct sSensor ParFila){
		
		printf("\n\n--------------->%d %d", ParFila.sensor1, ParFila.sensor2);
		if(ParFila.sensor1==0 || ParFila.sensor2==0){
			printf("\n\n\n 00");
			DADOS[0].soma+=ParFila.dadoSensor1;
				DADOS[0].quantidadeDados+=1;
		}
		if(ParFila.sensor1==1 || ParFila.sensor2==1){
			printf("\n\n\n 11");
			DADOS[1].soma+=ParFila.dadoSensor1;
			DADOS[1].quantidadeDados+=1;
		}
		if(ParFila.sensor1==2 || ParFila.sensor2==2){
			printf("\n\n\n 22");
			DADOS[2].soma+=ParFila.dadoSensor1;
			DADOS[2].quantidadeDados+=1;
		}		
		if(ParFila.sensor1==3 || ParFila.sensor2==3){
			DADOS[3].soma+=ParFila.dadoSensor1;
			DADOS[3].quantidadeDados+=1;
			printf("\n\n\n 33");
		}
		if(ParFila.sensor1==4 || ParFila.sensor2==4){
			DADOS[4].soma+=ParFila.dadoSensor1;
			DADOS[4].quantidadeDados+=1;
			printf("\n\n\n 44");
		}
		int cont;		
		for(cont=0;cont<5;cont++){
			
			if (DADOS[cont].quantidadeDados>0){
				int media = DADOS[cont].soma/DADOS[cont].quantidadeDados;
					printf("\n===================================");
			printf("\n=================================== %d ------ sensor %d", media, cont);
			}else{
					printf("\n===================================");
			printf("\n=================================== %d ------ sensor %d", DADOS[cont].soma, cont);
			}
		
		}
	}
	
	//////MAIN///////////
	main(){
	criarFila(&fila);
	iniciaSensores(); //inclui os valores de duplas -1 iniciais
	void *thread_result;
	pthread_t thread[N];
	
	sem_init(&mutex, 0, 1);
	sem_init(&escreveFila, 0, 1);
	sem_init(&leFila, 0, 1);
	sem_init(&paraExame, 0, 1);
	
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

