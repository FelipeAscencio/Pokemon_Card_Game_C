#ifndef __PRUEBAS_H_
#define __PRUEBAS_H_

#include <stdio.h>

#define BLANCO "\x1b[37;1m"
#define VERDE "\x1b[32;1m"
#define ROJO "\x1b[31;1m"
#define AMARILLO "\x1b[33;1m"
#define NORMAL "\x1b[0m"

#define TILDE "✓"
#define CRUZ "✗"

int cantidad_de_pruebas_corridas = 0;
int cantidad_de_pruebas_fallidas = 0;

void pruebas_afirmar(int afirmacion, const char *descripcion)
{
	if (afirmacion) {
		printf(VERDE TILDE " ");
	} else {
		cantidad_de_pruebas_fallidas++;
		printf(ROJO CRUZ " ");
	}
	printf(BLANCO "%s\n", descripcion);
	fflush(stdout);
	cantidad_de_pruebas_corridas++;
}

void pruebas_nuevo_grupo(const char *descripcion)
{
	printf(AMARILLO "\n%s\n", descripcion);
	while (*(descripcion++))
		printf("=");
	printf(BLANCO "\n");
}

int pruebas_mostrar_reporte()
{
	printf("\n---------------------------------\n"
	       "%i pruebas corridas, %i errores - %s\n" NORMAL,
	       cantidad_de_pruebas_corridas,
	       cantidad_de_pruebas_fallidas,
	       cantidad_de_pruebas_fallidas == 0 ? "OK" : "D:");
	return cantidad_de_pruebas_fallidas;
}

#endif