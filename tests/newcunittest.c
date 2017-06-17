/*
 * File:   newcunittest.c
 * Author: Marília
 *
 * Created on 15/06/2017, 00:26:37
 */

#include <stdio.h>
#include <stdlib.h>
#include <CUnit-2.1-3/CUnit/Headers/Basic.h>

/*
 * CUnit Test Suite
 */

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

void gerarDadosSensores(int b);

void testGerarDadosSensores() {
    int b;
    gerarDadosSensores(b);
    if (1 /*check result*/) {
        CU_ASSERT(0);
    }
}

void pegaDuplaSensor(int i);

void testPegaDuplaSensor() {
    int i;
    pegaDuplaSensor(i);
    if (1 /*check result*/) {
        CU_ASSERT(0);
    }
}

void gerarDadoAleatorio(int idSensor);

void testGerarDadoAleatorio() {
    int idSensor;
    gerarDadoAleatorio(idSensor);
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
    pSuite = CU_add_suite("newcunittest", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "testGerarDadosSensores", testGerarDadosSensores)) ||
            (NULL == CU_add_test(pSuite, "testPegaDuplaSensor", testPegaDuplaSensor)) ||
            (NULL == CU_add_test(pSuite, "testGerarDadoAleatorio", testGerarDadoAleatorio))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
