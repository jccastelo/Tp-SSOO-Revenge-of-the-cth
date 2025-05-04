// Incluyo las bibliotecas externas necesarias:
#include <check.h>
#include <math.h>

// Incluyo las bibliotecas internas necesarias:
#include "../src/include/memoria-instructions.h"
#include "../src/include/memoria-state.h"

START_TEST(test_read_pseudocode_file) {
    log_info(logger, "[TEST] Comenzando: test_read_pseudocode_file");

    FILE *pseudocode_file = fopen("tests/mock-instructions.txt", "r");
    ck_assert_ptr_nonnull(pseudocode_file);

    t_list *process_instructions = read_pseudocode_file(pseudocode_file);
    ck_assert_int_eq(list_size(process_instructions), 4);

    ck_assert_str_eq(list_get(process_instructions, 0), "NO_OP 3");
    ck_assert_str_eq(list_get(process_instructions, 1), "IO PANTALLA 5");
    ck_assert_str_eq(list_get(process_instructions, 2), "READ 0");
    ck_assert_str_eq(list_get(process_instructions, 3), "EXIT");

    list_destroy_and_destroy_elements(process_instructions, free);
    log_info(logger, "[TEST] Finalizó con éxito: test_read_pseudocode_file");
}
END_TEST

START_TEST(test_associate_instructions_folder_path) {
    log_info(logger, "[TEST] Comenzando: test_associate_instructions_folder_path");

    config_memoria = malloc(sizeof(t_config_memoria));
    config_memoria->PATH_INSTRUCCIONES = "/home/utnso/scripts/";

    char *file_process = "programa.txt";
    associate_instructions_folder_path(&file_process);

    ck_assert_str_eq(file_process, "/home/utnso/scripts/programa.txt");
    free(file_process);
    free(config_memoria);
    log_info(logger, "[TEST] Finalizó con éxito: test_associate_instructions_folder_path");
}
END_TEST

Suite *instrucciones_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Instructions");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_read_pseudocode_file);
    tcase_add_test(tc_core, test_associate_instructions_folder_path);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    // Inicializamos un logger:
    logger = log_create("tests.log","Tests", true, LOG_LEVEL_INFO);

    s = instrucciones_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}