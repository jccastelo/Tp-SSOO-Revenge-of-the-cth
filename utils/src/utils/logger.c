#include "logger.h"

t_log *logger;

char* concatenar_parametros(char** argv, int argc) {
    char* resultado = string_new();
    for (int i = 1; i < argc; i++) {
        string_append_with_format(&resultado, "%s", argv[i]);
        if (i < argc - 1)
            string_append(&resultado, " ");
    }
    return resultado;
}