#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
//#include <CUnit/CUnit.h>

#define NUMEROSENSORES 5  
#define TAMANHOFILA 1
#define ATRIBUTOS 3
#define INICIALIZADOR -1
#define MAXIMOALEATORIO 50
#define ATRIBUTO_VALORALEATORIO 0
#define ATRIBUTO_NUMEROSENSOR 1
#define ATRIBUTO_ID_DUPLA 2
#define TRUE 1

sem_t semaforoSensor[NUMEROSENSORES]; //um semaforo por sensor
sem_t escreverNaFila; //exclusao mutua para regioes de acesso a fila cheia
sem_t lerFila; //exclusão mutua para fila vazia
sem_t sessaoCritica;



int sensores[NUMEROSENSORES][ATRIBUTOS];


time_t rawtime;
struct tm * timeinfo;
void gerarDadosSensores(int b);
void pegaDuplaSensor(int i);
void gerarDadoAleatorio(int idSensor);

void coletor(int idSensor);
void monitor(int i);
void visualizador();
////////////////////////////fila//////////////////////////////////////////

struct parSensor {
    int sensor1;
    int dadoSensor1;
    int sensor2;
    int dadoSensor2;
};

struct Fila {
    int capacidade;
    struct parSensor nodoParSensor[TAMANHOFILA];
    int primeiro;
    int ultimo;
    int numeroItensNaFila;
};


struct fila *get_default_foo()
{
    static int foo_calculated = 0;
    static struct fila default_foo;
    if (!foo_calculated) /* assuming single-threaded access */
    {
        foo_calculated = 1;
        default_foo.payload = calculate_payload();
    }
    return &default_foo;
}
///////////////// Struct dos dados Sumarizados /////////////////////////

struct DadosSumarizados {
    int desvioPadrao;
    int media;
    int variancia;
    int quantidadeDados;
    int soma;
    int valorMinimo;
    int valorMaximo;
} DADOS_SUMARIZADOS[NUMEROSENSORES];

void criarFila(struct Fila *f) {
    f->capacidade = TAMANHOFILA;
    f->primeiro = 0;
    f->ultimo = -1;
    f->numeroItensNaFila = 0;
}

void inserirNodoDaFila(struct Fila *f, int sensor1, int dadoSensor1, int sensor2, int dadoSensor2) {
    if (f->ultimo == f->capacidade - 1)
        f->ultimo = -1;
    f->ultimo++;
    f->nodoParSensor[f->ultimo].sensor1 = sensor1; // incrementa ultimo e insere
    f->nodoParSensor[f->ultimo].dadoSensor1 = dadoSensor1;
    f->nodoParSensor[f->ultimo].sensor2 = sensor2;
    f->nodoParSensor[f->ultimo].dadoSensor2 = dadoSensor2;
    f->numeroItensNaFila++;

}

struct parSensor removerNodoDaFila(struct Fila *f) {
    struct parSensor primeiroDaFila = f->nodoParSensor[f->primeiro++];
    if (f->primeiro == f->capacidade)
        f->primeiro = 0;
    f->numeroItensNaFila--;
    return primeiroDaFila;
}

int filaVazia(struct Fila *fila) { // retorna verdadeiro se a fila estÃ¡ vazia
    return (fila->numeroItensNaFila == 0);
}

void test_filaVazia(void){
	  CU_ASSERT_TRUE_FATAL(Fila);
}

int filaCheia(struct Fila *fila) { // retorna verdadeiro se a fila estÃ¡ cheia
    return (fila->numeroItensNaFila == fila->capacidade);
}


//////////////////////////fim das funcoes da fila/////////////////////////////////////////

void gerarDadosSensores(int idSensor) {
    int contadorIdSensor;

    if (idSensor == INICIALIZADOR) {
        for (contadorIdSensor = 0; contadorIdSensor < NUMEROSENSORES; contadorIdSensor++) {
            gerarDadoAleatorio(contadorIdSensor);
        }
    } else {
        gerarDadoAleatorio(idSensor);
    }

}

void gerarDadoAleatorio(int idSensor) {

    srand(time(NULL));
    int numeroAleatorio = 1 + (rand()) % MAXIMOALEATORIO;
    sensores[idSensor][ATRIBUTO_VALORALEATORIO] = numeroAleatorio;
    sensores[idSensor][ATRIBUTO_NUMEROSENSOR] = idSensor;
}

void iniciarSensores() {
    srand(time(NULL));
    int idSensor;
    for (idSensor = 0; idSensor < NUMEROSENSORES; idSensor++) {
        sensores[idSensor][ATRIBUTO_ID_DUPLA] = INICIALIZADOR;
    }
}

void *sensor(void *j) {
    int idSensor = *(int *) j;
    while (TRUE) {
        coletor(idSensor);
        monitor(idSensor);
        visualizador();
    }
}

void coletor(int idSensor) {
    sem_wait(&sessaoCritica);
    printf("\n EXAME EM EXECUCAO...");
    if (sensores[idSensor][ATRIBUTO_ID_DUPLA] == INICIALIZADOR) {
        procurarDuplaSensor(idSensor);
    }
    sem_post(&sessaoCritica);
    sem_wait(&semaforoSensor[idSensor]);
}

void procurarDuplaSensor(int idSensor) {
    srand(time(NULL));
    int idSensorAleatorio;
    int achouDupla = 0;
    while (achouDupla == 0) {
        idSensorAleatorio = (rand()) % NUMEROSENSORES;
        if (sensores[idSensorAleatorio][ATRIBUTO_ID_DUPLA] == INICIALIZADOR && idSensorAleatorio != idSensor) {
            sem_post(&semaforoSensor[idSensor]);
            sleep(3);
            sensores[idSensorAleatorio][ATRIBUTO_ID_DUPLA] = idSensor;
            sensores[idSensor][ATRIBUTO_ID_DUPLA] = idSensorAleatorio;
            gerarDadosSensores(idSensor);
            sleep(1);
            gerarDadosSensores(idSensorAleatorio);
            achouDupla = 1;
            break;
        }
    }
}

void monitor(int idSensor) {
    sem_wait(&escreverNaFila);
    if (sensores[idSensor][ATRIBUTO_ID_DUPLA] != INICIALIZADOR) {
        if (!filaCheia(&fila)) {
            int idDuplaIdSensorAtual = sensores[idSensor][ATRIBUTO_ID_DUPLA];
            sensores[idSensor][ATRIBUTO_ID_DUPLA] = INICIALIZADOR;
            sensores[idDuplaIdSensorAtual][ATRIBUTO_ID_DUPLA] = INICIALIZADOR;
            sleep(3);
            inserirNodoDaFila(&fila, idSensor, sensores[idSensor][ATRIBUTO_VALORALEATORIO], idDuplaIdSensorAtual, sensores[idDuplaIdSensorAtual][ATRIBUTO_VALORALEATORIO]);
            sem_post(&semaforoSensor[idSensor]);
            sem_post(&escreverNaFila);
        }
    }

}

void visualizador() {
    sem_wait(&lerFila);
    if (!filaVazia(&fila)) {
        struct parSensor ParFila = removerNodoDaFila(&fila);
        sumarizarDados(ParFila);
        sem_post(&escreverNaFila);
    }
    sem_post(&lerFila);
}

void sumarizarDados(struct parSensor ParFila) {
    sumarizaSensor(ParFila.sensor1, ParFila.dadoSensor1);
    sleep(1);
    sumarizaSensor(ParFila.sensor2, ParFila.dadoSensor2);
    sleep(1);
    int idSensor;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    puts("\a"); //emite som do windows simulando um bipe cardiaco
    printf("\n \n==================================== %s\n", asctime(timeinfo));
    for (idSensor = 0; idSensor < NUMEROSENSORES; idSensor++) {
        switch (idSensor) {
            case 0:
                printf(" (i) ritmo cardiaco: ");
                break;
            case 1:
                printf(" (ii) suprimento sanguineo: ");
                break;
            case 2:
                printf(" (iii) primento de oxigenio: ");
                break;
            case 3:
                printf(" (iv) despolarização atrial: ");
                break;
            case 4:
                printf(" (v) repolarização ventricular: ");
                break;
            default:
                printf("Algum erro ocorreu!");
        }
        printf("Media:  %d --- sensor: %d --- quantidade: %d ---valor MAXIMO: %d --- valor MINIMO: %d \n \n", DADOS_SUMARIZADOS[idSensor].media, idSensor, DADOS_SUMARIZADOS[idSensor].quantidadeDados, DADOS_SUMARIZADOS[idSensor].valorMaximo, DADOS_SUMARIZADOS[idSensor].valorMinimo);
    }
    sleep(2);
}

void sumarizaSensor(int idSensor, int dadoSensor) {

    DADOS_SUMARIZADOS[idSensor].soma += dadoSensor;
    DADOS_SUMARIZADOS[idSensor].quantidadeDados += 1;
    if (DADOS_SUMARIZADOS[idSensor].quantidadeDados > 0) {
        DADOS_SUMARIZADOS[idSensor].media = DADOS_SUMARIZADOS[idSensor].soma / DADOS_SUMARIZADOS[idSensor].quantidadeDados;
    } else {
        DADOS_SUMARIZADOS[idSensor].media = 0;
    }
    definirMaiorMenor(idSensor, dadoSensor);

}

void definirMaiorMenor(int idSensor, int dadoSensor) {
    if (dadoSensor > 0) {
        if (DADOS_SUMARIZADOS[idSensor].quantidadeDados == 1) {
            DADOS_SUMARIZADOS[idSensor].valorMinimo = dadoSensor;
            DADOS_SUMARIZADOS[idSensor].valorMaximo = dadoSensor;
        } else {
            if (dadoSensor > DADOS_SUMARIZADOS[idSensor].valorMaximo) {
                DADOS_SUMARIZADOS[idSensor].valorMaximo = dadoSensor;
            }
            if (dadoSensor < DADOS_SUMARIZADOS[idSensor].valorMinimo) {
                DADOS_SUMARIZADOS[idSensor].valorMinimo = dadoSensor;
            }
        }

    }

}

void iniciarSemaforos() {
    sem_init(&sessaoCritica, 0, 1);
    sem_init(&escreverNaFila, 0, 1);
    sem_init(&lerFila, 0, 1);
}

main() {
    criarFila(&fila);
    iniciarSensores();
    void *thread_result;
    pthread_t thread[NUMEROSENSORES];
    iniciarSemaforos();
    gerarDadosSensores(INICIALIZADOR);
    int idSensor;
    for (idSensor = 0; idSensor < NUMEROSENSORES; idSensor++) {
        sem_init(&semaforoSensor[idSensor], 0, 1);

        pthread_create(&thread[idSensor], NULL, sensor, &idSensor);
    }
    for (idSensor = 0; idSensor < NUMEROSENSORES; idSensor++) {
        pthread_join(thread[idSensor], &thread_result);
    }

}

