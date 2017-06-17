/*
 * File:   newcunittest2.c
 * Author: Marília
 *
 * Created on 17/06/2017, 17:43:05
 */

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

/*
 * CUnit Test Suite
 */

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

void criarFila(Fila* f);

void testCriarFila() {
    Fila* f;
    criarFila(f);
    if (1 /*check result*/) {
        CU_ASSERT(0);
    }
}

void inserirNodoDaFila(Fila* f, int sensor1, int dadoSensor1, int sensor2, int dadoSensor2);

void testInserirNodoDaFila() {
    Fila* f;
    int sensor1;
    int dadoSensor1;
    int sensor2;
    int dadoSensor2;
    inserirNodoDaFila(f, sensor1, dadoSensor1, sensor2, dadoSensor2);
    if (1 /*check result*/) {
        CU_ASSERT(0);
    }
}

int filaVazia(Fila* fila);

void testFilaVazia() {
    Fila* fila;
    int result = filaVazia(fila);
    if (1 /*check result*/) {
        CU_ASSERT(0);
    }
}

void test_filaVazia();

void testTest_filaVazia() {
    test_filaVazia();
    if (1 /*check result*/) {
        CU_ASSERT(0);
    }
}

int filaCheia(Fila* fila);

void testFilaCheia() {
    Fila* fila;
    int result = filaCheia(fila);
    if (1 /*check result*/) {
        CU_ASSERT(0);
    }
}

int main() {
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("newcunittest2", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "testCriarFila", testCriarFila)) ||
            (NULL == CU_add_test(pSuite, "testInserirNodoDaFila", testInserirNodoDaFila)) ||
            (NULL == CU_add_test(pSuite, "testFilaVazia", testFilaVazia)) ||
            (NULL == CU_add_test(pSuite, "testTest_filaVazia", testTest_filaVazia)) ||
            (NULL == CU_add_test(pSuite, "testFilaCheia", testFilaCheia))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
