#include "client.h"


int main(void) {
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	log_info(logger, "Hola! Soy un log");

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	valor = config_get_string_value(config, "CLAVE");
	puerto = config_get_string_value(config, "PUERTO");
	ip = config_get_string_value(config, "IP");

	// Loggeamos el valor de config
	log_info(logger, "%s", valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion); 

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
	return 0;
}

t_log* iniciar_logger(void) {
	t_log* nuevo_log;
	nuevo_log = log_create("tp0.log", "tp0", true, LOG_LEVEL_INFO);
	if(nuevo_log == NULL){
		printf("No pude crear el logger\n");
		abort();
	}
	return nuevo_log;
}

t_config* iniciar_config(void) {
	t_config* nuevo_config;
	nuevo_config = config_create("cliente.config");
	if(nuevo_config == NULL) {
		printf("No pude leer la config\n");
		abort();
	}
	return nuevo_config;
}

void leer_consola(t_log* logger) {
	char* leido;

	leido = readline("> ");
	while(leido != NULL && strcmp(leido, "") != 0) {
		log_info(logger,"%s", leido);
		free(leido);
		leido = readline("> ");
	}

	if(leido != NULL) {
		free(leido);
	}
	
}

void paquete(int conexion) {
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");
	while(leido != NULL && strcmp(leido, "") != 0) {
		agregar_a_paquete(paquete, leido, strlen(leido) +1);
		free(leido);
		leido = readline("> ");
	}

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	if(leido != NULL) {
		free(leido);
	}
	enviar_paquete(paquete, conexion);
    eliminar_paquete(paquete);
	
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config) {
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	
	liberar_conexion(conexion);

	if(logger != NULL) {
		log_destroy(logger);
	}

	if(config != NULL) {
		config_destroy(config);
	}

}
