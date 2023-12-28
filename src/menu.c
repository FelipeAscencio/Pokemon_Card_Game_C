#include "menu.h"
#include "ataque.h"
#include "juego.h"
#include "lista.h"
#include "pokemon.h"
#include "tipo.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DESCRIPCION 50
#define MAX_SIMBOLO 1
#define CANTIDAD_COMANDOS 8
#define CERO 0

typedef struct comando {
  char simbolo_caracteristico;
  char descripcion[MAX_DESCRIPCION];
  bool (*funcion)(void *);
} comando_t;

struct menu {
  comando_t comandos[CANTIDAD_COMANDOS];
};

menu_t *menu_crear() {
  menu_t *menu = calloc(1, sizeof(menu_t));
  return menu;
}

void menu_agregar_comando(menu_t *menu, char simbolo_comando, char *descripcion,
                          bool (*funcion)(void *)) {
  if (menu == NULL) {
    printf("No se puede agregar un comando en un menu inexistente\n");
    return;
  }

  int i = 0;
  while (i < CANTIDAD_COMANDOS && menu->comandos[i].funcion != NULL) {
    i++;
  }

  if (i == CANTIDAD_COMANDOS) {
    printf("El menu ya esta lleno, no se pueden agregar mas comandos\n");
    return;
  }

  menu->comandos[i].simbolo_caracteristico = simbolo_comando;
  strcpy(menu->comandos[i].descripcion, descripcion);
  menu->comandos[i].funcion = funcion;
}

void menu_listar_comandos(menu_t *menu) {
  if (menu == NULL) {
    printf("No se puede listar un menu vacio\n");
    return;
  }

  int i = 0;
  while (i < CANTIDAD_COMANDOS && menu->comandos[i].funcion != NULL) {
    printf("Escriba '%c', para: %s\n", menu->comandos[i].simbolo_caracteristico,
           menu->comandos[i].descripcion);
    i++;
  }
}

MENU_RESULTADO menu_ejecutar_comando(menu_t *menu, char simbolo_comando,
                                     void *contexto) {
  if (menu == NULL) {
    printf("No hay comandos en un menu nulo\n");
    return MENU_INEXISTENTE;
  }

  int i = 0;
  while (i < CANTIDAD_COMANDOS) {
    if (menu->comandos[i].simbolo_caracteristico == simbolo_comando) {
      if (menu->comandos[i].funcion(contexto) == true) {
        return MENU_OK;
      } else {
        return MENU_ERROR;
      }
    }
    i++;
  }

  return MENU_INEXISTENTE;
}

void menu_destruir(menu_t *menu) { free(menu); }