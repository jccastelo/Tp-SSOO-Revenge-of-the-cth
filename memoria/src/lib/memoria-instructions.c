#include "../include/memoria-instructions.h"

void loading_process_instructions(int id_process, char *file_procces) {
    // Completa la ruta del archivo pseudocódigo asociándola al directorio correspondiente
    associate_instructions_folder_path(&file_procces);

    // Abre el archivo en modo lectura ("r")
    FILE *pseudocode_file = fopen(file_procces, "r");

    // Verifica si el archivo fue abierto correctamente
    if (!pseudocode_file) {
        log_error(logger, "No se pudo abrir el archivo %s", file_procces);
        exit(1); // TODO: Evaluar si corresponde finalizar el programa ante este error
    }

    // Lee e interpreta las instrucciones contenidas en el archivo
    t_list *process_instructions = read_pseudocode_file(pseudocode_file);
    load_process_instructions_in_instrucciones_por_procesos(id_process, process_instructions);
}

void associate_instructions_folder_path(char **file_procces) {
    char *new_path = string_from_format("%s%s", config_memoria->PATH_INSTRUCCIONES, *file_procces);
    *file_procces = new_path;
}

t_list *read_pseudocode_file(FILE *pseudocode_file) {
    // Inicializa las variables necesarias para la lectura de líneas
    char *instruction = NULL;
    size_t len_buffer = 0;
    ssize_t read;

    // Crea una lista para almacenar las instrucciones del proceso
    t_list *process_instructions = list_create();

    // Leemos el archivo y cada instrucción es un elemento de la lista (process_instructions) 
    while ((read = getline(&instruction, &len_buffer, pseudocode_file)) != -1) {
        int len_instruction = string_length(instruction);

        // Si la instrucción termina con un salto de línea ('\n'), lo reemplazamos por '\0'.
        if (instruction[len_instruction - 1] == '\n') 
            instruction[len_instruction - 1] = '\0'; 

        list_add(process_instructions, strdup(instruction));
    }
    
    // Liberamos memoria:
    fclose(pseudocode_file);

    return process_instructions;
}

void load_process_instructions_in_instrucciones_por_procesos(int id_process, t_list *process_instructions) {
    char *key_procces = string_itoa(id_process);
    dictionary_put(instrucciones_por_procesos, key_procces, process_instructions);
}

void get_instruction(int client_socket, int id_process, int program_counter, char **instruction) {
    // Convertimos el valor entero id_process en una cadena para usarla como clave.
    char *key_id_process = string_itoa(id_process); 

    // Obtenemos la lista de instrucciones del proceso del diccionario
    t_list *process_instructions = dictionary_get(instrucciones_por_procesos, key_id_process);

    // Verificamos que el proceso tenga instrucciones cargadas.
    if (!process_instructions) {
        log_debug(logger, "El proceso no tiene instrucciones cargadas en memoria.");
        free(key_id_process); 
        return;
    }

    *instruction = list_get(process_instructions, program_counter);

    // Logueamos la instrucción obtenida, y liberamos la cadena creada si es necesaria. 
    log_info(logger, "Obtener instrucción: ## PID: %d - Obtener instrucción: %d - Instrucción: %s", id_process, program_counter, *instruction);
    free(key_id_process); 
}