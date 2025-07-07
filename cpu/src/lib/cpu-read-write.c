#include "include/cpu-read-write.h"


// TRADUCCIONES

int obtener_direccion_fisica(int direccion_logica) {
    t_traduccion *traduccion = traducir_direccion_logica(direccion_logica);

    int pagina = traduccion->nro_pagina;
    int marco = -1;

    if (tlb_habilitada()) {
        marco = buscar_frame_tlb(pagina);
    }

    if(marco < 0) {
        marco = pedir_marco_a_memoria(traduccion);
        if(tlb_habilitada())
            agregar_a_tlb(pagina, marco);
    }

    int dir_fisica = marco * TAM_PAGINA + traduccion->desplazamiento;
    free(traduccion);
    return dir_fisica;
}



// INSTRUCCIONES READ Y WRITE

void leer(int direccion_logica, int tamanio) {
    t_traduccion *traduccion = traducir_direccion_logica(direccion_logica);
    char* resultado = malloc(tamanio + 1);

    if(cache_habilitada()) {
        int entrada = busqueda_cache(traduccion->nro_pagina);

        char* contenido = leer_pagina_cache_parcial(entrada, traduccion->desplazamiento, tamanio);
        memcpy(resultado, contenido, tamanio);
        free(contenido);
    }
    else {
        int dir_fisica = obtener_direccion_fisica(direccion_logica);
        char* contenido = leer_en_memoria_desde(dir_fisica, tamanio);
        memcpy(resultado, &contenido, tamanio);
        free(contenido);
    }
    
    resultado[tamanio] = '\0';
    // TODO -> LOG DE RESULTADO

    free(resultado);
    free(traduccion);
}

void escribir(int direccion_logica, char* contenido) {
    t_traduccion *traduccion = traducir_direccion_logica(direccion_logica);

    if(cache_habilitada()) {
        int entrada = busqueda_cache(traduccion->nro_pagina);

        escribir_pagina_cache(entrada, traduccion->desplazamiento, contenido);

    }
    else {
        int dir_fisica = obtener_direccion_fisica(direccion_logica);
        escribir_en_memoria_desde(dir_fisica, contenido);
    }

    free(traduccion);
}


int busqueda_cache(int nro_pagina) {
    int entrada = buscar_pagina_cache(nro_pagina);

        if (entrada == -1) {
            entrada = conseguir_entrada_libre();

            // DESALOJO

            if (cache[entrada].bit_modificado) {
                int dir_fisica = obtener_direccion_fisica(cache[entrada].pagina * TAM_PAGINA);
                escribir_pagina_en_memoria(dir_fisica, cache[entrada].contenido);
            }

            // ACTUALIZO LA CACHE

            char* contenido_frame = leer_frame_memoria(nro_pagina);
            agregar_en_entrada_cache(entrada, nro_pagina, contenido_frame);
        }

    return entrada;
}

char* leer_frame_memoria(int nro_pagina) {
    int dir_logica = nro_pagina * TAM_PAGINA;
    t_traduccion* traduccion = traducir_direccion_logica(dir_logica);

    int frame = pedir_marco_a_memoria(traduccion);
    free(traduccion);

    char* contenido = conseguir_contenido_frame(frame);
    return contenido;
}


void guardado_cache_por_desalojo() {
    for (int entrada=0; entrada < config_cpu->ENTRADAS_CACHE; entrada++) {
        guardado_de_cache(entrada);
    }
}

void guardado_de_cache(int entrada) {
    if (cache[entrada].bit_modificado) {
            int dir_fisica = obtener_direccion_fisica(cache[entrada].pagina * TAM_PAGINA);
            int frame = dir_fisica / TAM_PAGINA;
            escribir_pagina_en_memoria(frame, cache[entrada].contenido);
            log_info(logger, "PID: %d - Memory Update - Página: %d - Frame: %d", contexto->pid, cache[entrada].pagina, frame);
        }
}