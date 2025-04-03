#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	log_info (logger, "Hola! Soy un log");
	
	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	if (config_has_property (config, "IP")){
		ip = config_get_string_value (config, "IP");
		log_info (logger, "La IP leída es: %s", ip);
	} else {
		perror("Error, el archivo no tiene la key IP");
        abort();
	}

	if (config_has_property (config, "Puerto")){
		puerto = config_get_string_value (config, "Puerto");
		log_info (logger, "El Puerto leído es: %s", puerto);
	} else {
		perror("Error, el archivo no tiene la key Puerto");
        abort();
	}

	if (config_has_property (config, "Valor")){
		valor = config_get_string_value (config, "Valor");
		log_info (logger, "El Valor leído es: %s", valor);
	} else {
		perror("Error, el archivo no tiene la key Valor");
        abort();
	}

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor*
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje ?
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	return 0
	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
	
}

t_log* iniciar_logger(void){
	t_log* nuevo_logger = log_create ("tp0.log", "TP0", true, LOG_LEVEL_INFO);

	if (nuevo_logger == NULL) {
        perror("Error al crear el logger");
        abort(); 
    }
	return nuevo_logger;
}

t_config* iniciar_config(void){
	t_config* nuevo_config = config_create ("cliente.config");
	if (nuevo_config == 0){
		perror("¡No se pudo crear el config!");
        abort(); 
	}
	return nuevo_config;
}

void leer_consola(t_log* logger){
	char* leido;

	leido = readline("> ");
	while(leido != NULL && strcmp(leido, "") != 0){
		log_info (logger, ">> %s",leido);
		free(leido);
		leido = readline("> ");
	}
}

void paquete(int conexion){
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete*
	paquete = crear_paquete();

	leido = readline("> ");
	while(leido != NULL && strcmp(leido, "") != 0){
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		free(leido);
		leido = readline("> ");
	}

	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!*
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config){
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy (logger);
	config_destroy (config);
	liberar_conexion(conexion);  
}
