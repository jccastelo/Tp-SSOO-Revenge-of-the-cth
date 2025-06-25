// Incluimos las biblotecas externas:
#include <check.h>
#include <string.h>

// Incluimos las biblotecas internas:
#include "../src/include/memoria-protocols.h"

#include "../src/include/memoria-protocols.h"

bool se_logueo_error = false;
bool se_envio_paquete = false;
char ultima_instruccion[256] = "";

void log_error(void *logger, const char *fmt, ...) {
    se_logueo_error = true;
}

void enviar_paquete(t_paquete *paquete, int socket) {
    se_envio_paquete = true;
}

void agregar_a_paquete_string(t_paquete *paquete, const char *str, size_t size) {
    strncpy(ultima_instruccion, str, sizeof(ultima_instruccion));
}


START_TEST(test_parsear_int) {
    int buffer[] = { 42 }; // Un solo valor entero
    int desplazamiento = 0;
    int valor_leido = 0;

    // Llamada a la función parsear_int
    parsear_int(buffer, &desplazamiento, &valor_leido);

    // Verificamos que el valor leído sea correcto
    ck_assert_int_eq(valor_leido, 42);
    // Verificamos que el desplazamiento se haya actualizado correctamente
    ck_assert_int_eq(desplazamiento, sizeof(int));
}
END_TEST

START_TEST(test_parsear_string) {
    // Buffer con los datos: id_process = 42, tam_process = 100, file_process = "Hello"
    int buffer[] = { 42, 100, 5, 'H', 'e', 'l', 'l', 'o' };
    int desplazamiento = 0;
    int id_process = 0;
    int tam_process = 0;
    char *file_process = NULL;

    // Parseamos los primeros valores enteros
    parsear_int(buffer, &desplazamiento, &id_process);
    parsear_int(buffer, &desplazamiento, &tam_process);

    // Parseamos la cadena
    parsear_string(buffer, &desplazamiento, &file_process);

    // Verificamos que los valores enteros se hayan parseado correctamente
    ck_assert_int_eq(id_process, 42);
    ck_assert_int_eq(tam_process, 100);

    // Verificamos que el string se haya parseado correctamente
    ck_assert_str_eq(file_process, "Hello");

    // Liberamos la memoria al final de la prueba
    free(file_process);
}
END_TEST

START_TEST(test_rcv_setup_to_process) {
    // Buffer con los datos: id_process = 42, tam_process = 100, file_process = "Hello"
    int buffer[] = { 42, 100, 5, 'H', 'e', 'l', 'l', 'o' };
    int desplazamiento = 0;
    int id_process = 0;
    int tam_process = 0;
    char *file_process = NULL;

    // Simulamos un socket con un buffer
    int pipefds[2];
    socketpair(AF_UNIX, SOCK_STREAM, 0, pipefds);
    write(pipefds[1], buffer, sizeof(buffer));

    // Llamada a la función rcv_setup_to_process
    rcv_setup_to_process(pipefds[0], &id_process, &tam_process, &file_process);

    // Verificamos que los valores enteros se hayan parseado correctamente
    ck_assert_int_eq(id_process, 42);
    ck_assert_int_eq(tam_process, 100);

    // Verificamos que el string se haya parseado correctamente
    ck_assert_str_eq(file_process, "Hello");

    // Liberamos la memoria al final de la prueba
    free(file_process);
    close(pipefds[0]);
    close(pipefds[1]);
}
END_TEST

START_TEST(test_rcv_instruction_consumer) { 
    int id_process = 0;
    int program_counter = 0;

    rcv_instruction_consumer(0 , &id_process , &program_counter );

    ck_assert_int_eq(id_process, 42);
    ck_assert_int_eq(program_counter,100);

}
END_TEST

START_TEST(test_rcv_process_to_end) {
    int id_process;
    rcv_process_to_end(42,&id_process);
    ck_assert_int_eq(id_process,1337);
}
END_TEST

START_TEST(test_send_instruction_consumer){
    se_logueo_error = false;
    se_envio_paquete = false;
    send_instruction_consumer(1, 42, 7 , "MOV A, B");
    ck_assert(se_envio_paquete==true);
    ck_assert_str_eq(ultima_instruccion, "MOV A, B");
    ck_assert(se_logueo_error == false);
}
END_TEST
void agregar_tests_protocols(TCase* tc) {
    tcase_add_test(tc, test_parsear_string);
    tcase_add_test(tc, test_parsear_int);
    tcase_add_test(tc,test_rcv_setup_to_process);
    tcase_add_test(tc, test_rcv_instruction_consumer);
    tcase_add_test(tc,test_rcv_process_to_end);
    tcase_add_test(tc, test_send_instruction_consumer);
    


}