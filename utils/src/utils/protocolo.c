#include "protocolo.h"

t_paquete* crear_paquete(op_code operacion) {
	t_paquete* paquete = malloc(sizeof(t_paquete));
    paquete->codigo_operacion = operacion;
    
	crear_buffer(paquete);
	return paquete;
}

void crear_buffer(t_paquete* paquete) {
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio) {
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio);
	memcpy(paquete->buffer->stream + paquete->buffer->size, valor, tamanio);
	paquete->buffer->size += tamanio;
}

void agregar_a_paquete_string(t_paquete* paquete, char* cadena, int tamanio) {
    int cadena_length = strlen(cadena);
    size_t size = sizeof(int);  // Tamaño en bytes de un entero
    
    // Expandir el tamaño del buffer del paquete para acomodar la longitud de la cadena
    paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + size);
    memcpy(paquete->buffer->stream + paquete->buffer->size, &cadena_length, size);
    paquete->buffer->size += size;

    // Expandir el tamaño del buffer para acomodar la cadena
    paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio);
    memcpy(paquete->buffer->stream + paquete->buffer->size, cadena, tamanio);
    paquete->buffer->size += tamanio;
}

void enviar_paquete(t_paquete* paquete, int socket_cliente) {
	int bytes = paquete->buffer->size + 2 *sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);
    free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete) {
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void* recibir_buffer(int* size, int socket_cliente) {
	void *buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

int recibir_operacion(int socket_cliente) {
	int cod_op;
	if (recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else{
		close(socket_cliente);
		return -1;
	}
}

void* serializar_paquete(t_paquete* paquete, int bytes) {
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(op_code));
	desplazamiento += sizeof(op_code);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);

	return magic;
}

void recibir_handshake(int socket) {
	int32_t handshake;
	int32_t resultOk = 0;
	int32_t resultError = -1;

	recv(socket, &handshake, sizeof(int32_t), MSG_WAITALL);
	
	if (handshake == 1) 
		send(socket, &resultOk, sizeof(int32_t), 0);
	else 
		send(socket, &resultError, sizeof(int32_t), 0);
}

void generar_handshake(int socket, char *server_name) {
    int32_t handshake = 1;
    int32_t result;

	op_code cod_op = HANDSHAKE; 
	send(socket, &cod_op, sizeof(op_code), 0);
    send(socket, &handshake, sizeof(int32_t), 0);

	recv(socket, &result, sizeof(int32_t), MSG_WAITALL);

    if(result == 0) 
        log_info(logger, "Handshake exitoso con %s", server_name);
    else {
        log_error(logger, "Error en el handshake con %s", server_name);
        exit(EXIT_FAILURE);
    }
}

void parsear_int(void* buffer ,int* desplazamiento, void* ubicacion) {
    memcpy(ubicacion, buffer + *desplazamiento, sizeof(int));
    *desplazamiento += sizeof(int);
}

void parsear_string(void *buffer, int *desplazamiento, void *dest) {
    int longitud;
    memcpy(&longitud, buffer + *desplazamiento, sizeof(int));
    *desplazamiento += sizeof(int);

    char **string_ptr = (char **)dest;
    *string_ptr = malloc(longitud + 1);
    if (*string_ptr == NULL) {
        perror("Error al asignar memoria para la cadena");
        exit(1);
    }

    memcpy(*string_ptr, buffer + *desplazamiento, longitud);
    (*string_ptr)[longitud] = '\0';

    *desplazamiento += longitud;
}

t_list *parsear_ints(void *buffer, int *desplazamiento, int size) {
    t_list *integers = list_create();
    parse_values_and_add_to_list(integers, (parse_func_t)parsear_int, buffer, desplazamiento, size, sizeof(int));
    return integers;
}

void parse_values_and_add_to_list(t_list *list, parse_func_t parse_func, void *buffer, int *offset, int buffer_size, size_t elem_size) {
    while (*offset < buffer_size) {
        void *element = malloc(elem_size);
        if (!element) {
            perror("Error allocating memory");
            exit(1);
        }
        parse_func(buffer, offset, element);
        list_add(list, element);
    }
}

void noop_parse_entry(void *buffer, int *desplazamiento, void *extra_data) {
    // No realiza ninguna acción; función de parseo vacía
}
