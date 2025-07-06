// Incluyo las bibliotecas externas necesarias: 
#include <check.h>

// Incluyo las bibliotecas internas necesarias:
#include "../src/include/memoria-pages-tables.h"
#include "../src/include/memoria-state.h"

void count_nodes_per_level(t_list* current_level_nodes, int level, int* level_counts, int max_levels) {
    if (!current_level_nodes || level > max_levels) return;

    level_counts[level - 1] += list_size(current_level_nodes);

    for (int i = 0; i < list_size(current_level_nodes); i++) {
        t_list* node = list_get(current_level_nodes, i);

        if (node) count_nodes_per_level(node, level + 1, level_counts, max_levels);
    }
}

START_TEST(test_populate_page_table) {
    log_info(logger, "[TEST] Comenzando: test_populate_page_table");
    int entries_per_level = 3;
    int total_levels = 4;

    // Asignamos memoria e inicializamos la configuración de memoria con valores definidos por defecto
    config_memoria = malloc(sizeof(t_config_memoria));
    config_memoria->ENTRADAS_POR_TABLA = entries_per_level;
    config_memoria->CANTIDAD_NIVELES = total_levels;

    // Creamos el nodo raíz de la tabla de páginas del proceso y generamos un mock de los marcos libres asociados
    t_list *page_table_root = list_create();
    t_list *mock_free_frames = list_create();

    // Agregamos un marco libre de ejemplo y populamos la tabla de páginas raíz
    list_add(mock_free_frames, 7);
    list_add(mock_free_frames, 8);
    list_add(mock_free_frames, 9);
    populate_page_table(mock_free_frames, page_table_root);

    // Inicializamos los arreglos para contar y validar la cantidad de nodos por nivel
    int level_counts[] = {0, 0 , 0 ,0};
    int expected_values[] = {1, 1, 1, 3};

    // Comenza el conteo de nodos por nivel para cada entrada de la tabla
    count_nodes_per_level(page_table_root, 1, level_counts, config_memoria->CANTIDAD_NIVELES);

    for(int i = 0; i < config_memoria->CANTIDAD_NIVELES; i++) {
        log_info(logger, "[STATUS] Estamos en el nivel %i de la tabla", i + 1);    
        ck_assert_int_eq(level_counts[i], expected_values[i]);
    }
    
    log_info(logger, "[TEST] Finalizó con éxito: test_populate_page_table");
}
END_TEST 

void agregar_tests_page_tables(TCase* tc){     
    tcase_add_test(tc, test_populate_page_table);
}
