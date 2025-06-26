#include <check.h>
#include <commons/collections/list.h> 


#include "../src/include/memoria-user-spaces.h"
#include  "../src/include/memoria-config.h"
#include  "../src/include/memoria-state.h"

START_TEST(test_memory_sufficient){
    memoria_config_init();
    memoria_state_init() ;
    int tamanio_proceso = 40;
    t_list* frames_libres = is_memory_sufficient(tamanio_proceso);
    ck_assert_int_eq(list_size(frames_libres),1);
}
END_TEST 

void agregar_tests_page_tables(TCase* tc){
    tcase_add_test(tc, test_memory_sufficient);
}