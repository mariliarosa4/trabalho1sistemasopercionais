#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include<time.h>
#define N 5  /* qtdade de  sensores */
#define TAMANHOFILA 1
sem_t semaforoSensor[N]; //um semaforo por sensor
sem_t escreveFila; //exclusao mutua para regioes de acesso a fila cheia
sem_t leFila; //exclusão mutua para fila vazia
sem_t mutex;
sem_t mutex2;
struct Fila fila;
int sensores[N][3];
int i;
int contDuplas=0;
//array para controlar o estado dos filosofos
//coluna 0=> valor aleatorio
//coluna 1=> numero do sensor 
//coluna 2=> dupla do sensor
time_t rawtime;
struct tm * timeinfo; //informações de tempo
void geraDadosSensores(int b);
void pegaDuplaSensor(int i);

////////////////////////////fila//////////////////////////////////////////

struct sSensor {
    int sensor1;
    int dadoSensor1;
    int sensor2;
    int dadoSensor2;
};

struct Fila {
    int capacidade;
    struct sSensor dados[TAMANHOFILA];
    int primeiro;
    int ultimo;
    int nItens;
};
///////////////// Struct dos dados Sumarizados /////////////////////////
struct Dados {
    int desvioPadrao;
    int media;
    int variancia;
    int quantidadeDados;
    int soma;
    int valorMinimo;
    int valorMaximo;
} DADOS[5];

void criarFila(struct Fila *f) {
    f->capacidade = TAMANHOFILA;
    f->primeiro = 0;
    f->ultimo = -1;
    f->nItens = 0;
}

void inserir(struct Fila *f, int sensor1, int dadoSensor1, int sensor2, int dadoSensor2) {
    if (f->ultimo == f->capacidade - 1)
        f->ultimo = -1;
    f->ultimo++;
    f->dados[f->ultimo].sensor1 = sensor1; // incrementa ultimo e insere
    f->dados[f->ultimo].dadoSensor1 = dadoSensor1;
    f->dados[f->ultimo].sensor2 = sensor2;
    f->dados[f->ultimo].dadoSensor2 = dadoSensor2;
    f->nItens++;

}
struct sSensor remover(struct Fila *f) { // pega o primeiro da fila
    struct sSensor temp = f->dados[f->primeiro++];
    if (f->primeiro == f->capacidade)
        f->primeiro = 0;
    f->nItens--;
    return temp;
}
int estaVazia(struct Fila *f) { // retorna verdadeiro se a fila estÃ¡ vazia
    return (f->nItens == 0);
}
int estaCheia(struct Fila *f) { // retorna verdadeiro se a fila estÃ¡ cheia
    return (f->nItens == f->capacidade);
}
void mostrarFila(struct Fila *f) {
    int cont, i;
    i = f->primeiro;
    while (i <= f->ultimo) {
      ///  printf("Sensor1: %d - dadoSensor1: %d - Sensor2: %d - dadoSensor2: %d ", f->dados[i].sensor1, f->dados[i].dadoSensor1, f->dados[i].sensor2, f->dados[i].dadoSensor2);
        i++;
    }


    printf("\n\n");

}
//////////////////////////fim das funcoes da fila/////////////////////////////////////////

/* i: numero do sensor, de 0 a N-1 */
void geraDadosSensores(int b) {
    int c;
    int numeroAleatorio;
    srand(time(NULL));
    if (b == -1) {//pela primeira vez
        for (c = 0; c < 5; c++) {
            numeroAleatorio = 1 + (rand()) % 50; 
           // printf("numero aleatorio %d \n", numeroAleatorio);
            sensores[b][0] = numeroAleatorio;
            sensores[b][1] = b; 	
        }
    } else {
        numeroAleatorio = 1 + (rand()) % 50; 
       // printf("numero aleatorio %d  do id: %d \n", numeroAleatorio, b);
        sensores[b][0] = numeroAleatorio;
        sensores[b][1] = b; 	
    }

}

void iniciaSensores() {
    int b;
    srand(time(NULL));
    for (b = 0; b < N; b++) {
        sensores[b][2] = -1; //-1 para nao tem dupla, e  outro valor com o i da dupla
    }
}

void mostraDadoSensores(i) {
    int b;
    sleep(2);
    printf("\n");
    for (b = 0; b < N; b++) {
      //  printf("sensor %d - dupla %d  \n", b, sensores[b][2]);
    }
    printf("\n");

}

void *sensor(void *j) {
    int i = *(int *) j;
    while (1) {
    
        pegaSensor(i);
        
        escrita(i);
        visualizador();
    }
}

void pegaSensor(int i) {
    sem_wait(&mutex);
		printf("\n EXAME EM EXECUCAO...");
    int dupla;
    if (sensores[i][2] == -1) {//Não tem dupla, procura dupla
        procuraDupla(i);
    }

    sem_post(&mutex);
    sem_wait(&semaforoSensor[i]);
}

void procuraDupla(int i) {
    srand(time(NULL));
    int sensorAleatorio;
    int achou = 0;
    while (achou == 0) {
        sensorAleatorio = (rand()) % 5; // aleatorio de 0 a 4	
        if (sensores[sensorAleatorio][2] == -1 && sensorAleatorio != i) {
            sem_post(&semaforoSensor[i]);
            sleep(1);
            sensores[sensorAleatorio][2] = i;
            sensores[i][2] = sensorAleatorio;
            geraDadosSensores(i);
            sleep(1);
            geraDadosSensores(sensorAleatorio);
          //  printf("\n ================ %d é a dupla de %d ============== \n", sensorAleatorio, i);
            achou = 1;
            break;
        }
    }
}

void escrita(int i) {
sem_wait(&escreveFila);
    if (sensores[i][2] != -1) {
        if (!estaCheia(&fila)) {
			contDuplas=0;
			sleep(2);
            int semaforoDupla = sensores[i][2];

            sensores[i][2] = -1;
            sensores[semaforoDupla][2] = -1;
            sleep(1);
          //  printf("\n ----->>>>escrevendo e liberando sensores \n");
            sleep(2);
            inserir(&fila, i, sensores[i][0], semaforoDupla, sensores[semaforoDupla][0]);
            mostrarFila(&fila);
            sem_post(&semaforoSensor[i]);
           sem_post(&escreveFila);
        } else {
           // printf("\n\n\n----------------------------------------fila cheia");
			//não libera o semaforo
        }
    }
//}	

}

void visualizador() {

    sem_wait(&leFila);
    if (!estaVazia(&fila)) {
        struct sSensor ParFila = remover(&fila);
        sumarizarDados(ParFila);
        sem_post(&escreveFila);
    }
    sem_post(&leFila);
}

void sumarizarDados(struct sSensor ParFila) {
    sumarizaSensor(ParFila.sensor1, ParFila.dadoSensor1);
    sleep(1);
    sumarizaSensor(ParFila.sensor2, ParFila.dadoSensor2);
    sleep(1);
    int cont;
    time ( &rawtime );
  	timeinfo = localtime ( &rawtime );
  	puts("\a");
    printf("\n \n============= %s =============\n",asctime(timeinfo) );
    for (cont = 0; cont < 5; cont++) {
    	switch (cont){
    			case 0:
    				printf("(i) ritmo cardíaco: ");
    				break;
    			case 1:
    				printf("(ii) suprimento sanguíneo: ");
    				break;
    			case 2:
    				printf("(iii) primento de oxigênio: ");
    				break;
    			case 3:
    				printf("(iv) despolarização atrial: ");
    				break;
    			case 4: 
    				printf("(v) repolarização ventricular: ");
    				break;
    			default:
    				printf("outro sensor");			
		}
        printf("Média:  %d --- sensor: %d --- quantidade: %d ---valor MAXIMO: %d --- valor MINIMO: %d \n \n", DADOS[cont].media, cont, DADOS[cont].quantidadeDados, DADOS[cont].valorMaximo, DADOS[cont].valorMinimo);
    }
    sleep(2);
}

void sumarizaSensor(int idSensor, int dadoSensor) {

    DADOS[idSensor].soma += dadoSensor;
    DADOS[idSensor].quantidadeDados += 1;
    if (DADOS[idSensor].quantidadeDados > 0) {
        DADOS[idSensor].media = DADOS[idSensor].soma / DADOS[idSensor].quantidadeDados;
    } else {
        DADOS[idSensor].media = 0;
    }
    definirMaiorMenor(idSensor, dadoSensor);

}

void definirMaiorMenor(int idSensor, int dadoSensor) {
    if (dadoSensor > 0) {
        if (DADOS[idSensor].quantidadeDados == 1) {
            DADOS[idSensor].valorMinimo = dadoSensor;
            DADOS[idSensor].valorMaximo = dadoSensor;
        } else {
            if (dadoSensor > DADOS[idSensor].valorMaximo) {
                DADOS[idSensor].valorMaximo = dadoSensor;
            }
            if (dadoSensor < DADOS[idSensor].valorMinimo) {
                DADOS[idSensor].valorMinimo = dadoSensor;
            }
        }

    }

}
//////MAIN///////////

main() {
    criarFila(&fila);
    iniciaSensores(); //inclui os valores de duplas -1 iniciais
    void *thread_result;
    pthread_t thread[N];
    sem_init(&mutex, 0, 1);
    sem_init(&mutex2, 0, 1);
    sem_init(&escreveFila, 0, 1);
    sem_init(&leFila, 0, 1);
    geraDadosSensores(-1);
    for (i = 0; i < N; i++) {
        sem_init(&semaforoSensor[i], 0, 1);
    }
    for (i = 0; i < N; i++) {
        pthread_create(&thread[i], NULL, sensor, &i);
    }
    for (i = 0; i < N; i++) {
        pthread_join(thread[i], &thread_result);
    }

}

