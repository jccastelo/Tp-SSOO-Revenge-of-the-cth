#ifndef TEST_PROTOCOLS_H
#define TEST_PROTOCOLS_H

#include <check.h>
extern bool se_logueo_error;
extern bool se_envio_paquete;
extern char ultima_instruccion[];

void agregar_tests_protocols(TCase* tc);
#endif
