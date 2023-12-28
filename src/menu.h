#ifndef __MENU_H__
#define __MENU_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct menu menu_t;

typedef enum { MENU_OK, MENU_INEXISTENTE, MENU_ERROR } MENU_RESULTADO;

// PRE: - .
// POST: Inicializa el menu.
menu_t *menu_crear();

// PRE: El comando, la descripcion y la funcion recibidas deben ser validos.
// POST: Agrega un nuevo comando al menu.
void menu_agregar_comando(menu_t *menu, char simbolo_comando, char *descripcion,
                          bool (*funcion)(void *));

// PRE: - .
// POST: Lista todos los comandos actuales del menu.
void menu_listar_comandos(menu_t *menu);

// PRE: El comando y el contexto recibidos deben ser validos.
// POST: Ejecuta, de ser posible y existir, el comando dentro del menu.
MENU_RESULTADO menu_ejecutar_comando(menu_t *menu, char simbolo_comando,
                                     void *contexto);

// PRE: - .
// POST: Libera la memoria reservada por el menu.
void menu_destruir(menu_t *menu);

#endif /* __MENU_H__ */