#include "include/mmu.h"

t_traduccion* traducir_direccion_logica(int dir_logica) {
    t_traduccion *traduccion = malloc(sizeof(t_traduccion));
    traduccion->entradas = malloc(sizeof(int) * CANTIDAD_NIVELES);

    int nro_pagina = obtener_nro_pagina(dir_logica);
    traduccion->nro_pagina = nro_pagina;
    obtener_entradas_por_nivel(traduccion->entradas, nro_pagina);
    traduccion->desplazamiento = obtener_desplazamiento(dir_logica);

    log_debug(logger, "NRO PAGINA: %d", traduccion->nro_pagina);
    for (int i=0; i < CANTIDAD_NIVELES; i++) {
        log_debug(logger, "ENTRADA: %d, NIVEL: %d", i, traduccion->entradas[i]);
    }
    log_debug(logger, "offset: %d", traduccion->desplazamiento);

    return traduccion;
}

int obtener_nro_pagina(int dir_logica) {
    int nro_pagina = dir_logica/TAM_PAGINA;
    return nro_pagina;
}

void obtener_entradas_por_nivel(int *entradas, int nro_pagina) {
    for (int i = 0; i < CANTIDAD_NIVELES; i++) {
        int X = i + 1;
        int divisor = potencia(ENTRADAS_POR_TABLA, CANTIDAD_NIVELES - X);
        entradas[i] = (nro_pagina / divisor) % ENTRADAS_POR_TABLA;
    }
}

int obtener_desplazamiento(int dir_logica) {
    int offset = dir_logica % TAM_PAGINA;
    return offset;
}


//para utils
int potencia(int base, int exponente) {
    int resultado = 1;
    for (int i = 0; i < exponente; i++) {
        resultado *= base;
    }
    return resultado;
}