#ifndef TESTS_H
#define TESTS_H

#include <check.h>
#include "test-instruction.h"
#include "test-protocols.h"

/**
 * @brief Crea una suite completa que agrupa todos los casos de prueba de memoria.
 * 
 * @return Suite* Puntero a la suite de tests combinada.
 */
Suite* memoria_test_suite(void);

/**
 * @brief Crea la suite de tests correspondiente a las instrucciones de pseudocódigo.
 * 
 * Esta suite contiene tests relacionados con la lectura de instrucciones desde archivos
 * y la asociación de rutas de instrucciones.
 * 
 * @return Un puntero a la suite de tests de instrucciones.
 */
Suite* suite_instructions(void);
/**
 * @brief Crea la suite de tests correspondiente a los protocolos de comunicación.
 * 
 * Esta suite valida la correcta interpretación de paquetes de setup y otras funciones 
 * relacionadas con la deserialización de mensajes del cliente.
 * 
 * @return Un puntero a la suite de tests de protocolos.
 */
Suite *suite_protocols(void);

#endif // TESTS_H
