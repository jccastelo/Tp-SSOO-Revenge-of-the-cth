#include <check.h>
#include <stdlib.h>
#include <commons/log.h>

#include "include/test-instruction.h"
#include "include/test-protocols.h"

// Declaración de logger global:
t_log* logger;

Suite* suite_instructions(void);
Suite* suite_protocols(void);

int main(void) {
    int failed_total = 0;
    SRunner *sr = srunner_create(suite_instructions());

    logger = log_create("tests.log", "Tests", true, LOG_LEVEL_INFO);

    srunner_add_suite(sr, suite_protocols());

    srunner_run_all(sr, CK_NORMAL);
    failed_total = srunner_ntests_failed(sr);

    srunner_free(sr);
    log_destroy(logger);

    return (failed_total == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

Suite *suite_instructions(void) {
    Suite *s = suite_create("Instructions");
    TCase *tc_core = tcase_create("Core");

    agregar_tests_instructions(tc_core);  // << Usamos función auxiliar

    suite_add_tcase(s, tc_core);
    return s;
}

Suite *suite_protocols(void) {
    Suite *s = suite_create("Protocols");
    TCase *tc_core = tcase_create("Core");

    agregar_tests_protocols(tc_core);  // << Usamos función auxiliar

    suite_add_tcase(s, tc_core);
    return s;
}
