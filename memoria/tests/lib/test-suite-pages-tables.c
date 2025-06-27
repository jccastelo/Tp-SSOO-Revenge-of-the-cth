#include <check.h>

#include "../src/include/memoria-pages-tables.h"

void contar_nodos_por_nivel(t_list *nodo, int nivel, int *niveles, int total_levels) {
    if (!nodo) return;

    

    niveles[nivel] += list_size(nodo);

    if (nivel < total_levels - 1) {
        for (int i = 0; i < list_size(nodo); i++) {
            t_list *hijo = list_get(nodo, i);
            if (hijo) {
                contar_nodos_por_nivel(hijo, nivel + 1, niveles,total_levels);
            }
        }
    }
}



START_TEST(test_populate_page_table){
    int total_levels = config_memoria->CANTIDAD_NIVELES;
    int niveles[total_levels];

    for (int i = 0; i < total_levels; i++) {
        niveles[i] = 0;
    }

    int size_process = 40;
    int valores_esperados[]={3,10,40};

    t_list *frames_libres = is_memory_sufficient(size_process);
    t_list *page_table_root = list_create(); 

    populate_page_table(frames_libres, page_table_root);
    contar_nodos_por_nivel(page_table_root, 0, niveles,total_levels);
    
    for(int i= 0; i < config_memoria -> CANTIDAD_NIVELES; i++){
        ck_assert_int_eq(niveles[i],valores_esperados[i]);
    }
}
END_TEST 


void agregar_test_suite_pages_tables(TCase* tc){     
    tcase_add_test(tc, test_populate_page_table);
}
