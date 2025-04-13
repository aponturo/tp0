#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* port;
	char* value;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config, "IP");
	port = config_get_string_value(config, "PORT");
	value = config_get_string_value(config, "VALUE");
	
	log_info(logger, "IP:%s - PORT:%s - CLAVE:%s", ip,port,value);

	// Loggeamos el valor de config


	/* ---------------- LEER DE CONSOLA ---------------- */

	// leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, port);
	enviar_mensaje(value,conexion);
	// Enviamos al servidor el valor de CLAVE como mensaje

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
    t_log* logger = log_create(
        "logs/client.log",   
        "CLIENT",           
        true,                    
		LOG_LEVEL_TRACE        
    );
    if (logger == NULL) {
        printf("Error creating logger.\n");
		abort();
    }
	return logger;
}

t_config* iniciar_config(void)
{
	t_config * config = config_create("config/config.cfg");
	if (config == NULL) {
		printf("Error getting config.\n");
		abort();
    }
	return config;
}

void leer_consola(t_log* logger)
{
	// La primera te la dejo de yapa
	char* read;
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	do{
		read = readline("> ");
		log_info(logger, read);
		if(read && *read == '\0'){
			break;
		}
		free(read);
	}while(read);
	free(read);
	// ¡No te olvides de liberar las lineas antes de regresar!
}

void paquete(int fd_conexion)
{
	// Ahora toca lo divertido!
	char* read;
	t_paquete* paquete;
	paquete = crear_paquete();
	// Leemos y esta vez agregamos las lineas al paquete
	do{
		read = readline("> Paquete: ");
		if(read && *read == '\0'){
			break;
		}
		agregar_a_paquete(paquete, read, strlen(read)+1);
		free(read);
	}while(read);
	free(read);
	enviar_paquete(paquete,fd_conexion);
	eliminar_paquete(paquete);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	  liberar_conexion(conexion);
	  log_destroy(logger);
	  config_destroy(config);
}
