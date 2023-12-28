#include "adversario.h"
#include "ataque.h"
#include "juego.h"
#include "lista.h"
#include "pokemon.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_POKEMONES 3
#define MAX_NOMBRE 20
#define MAX_ATAQUES 3
#define CERO 0
#define UNO 1
#define DOS 2
#define PRIMER_POKEMON 0
#define SEGUNDO_POKEMON 1
#define TERCER_POKEMON 2
#define PRIMER_ATAQUE 0
#define SEGUNDO_ATAQUE 1
#define TERCER_ATAQUE 2
#define PRIMER_TURNO 0
#define SEGUNDO_TURNO 1
#define TERCER_TURNO 2
#define CUARTO_TURNO 3
#define QUINTO_TURNO 4
#define SEXTO_TURNO 5
#define SEPTIMO_TURNO 6
#define OCTAVO_TURNO 7

typedef struct ataques {
  char nombre[MAX_NOMBRE];
} ataques_t;

typedef struct pokemones {
  char nombre[MAX_NOMBRE];
  ataques_t *ataques[MAX_ATAQUES];
} pokemones_t;

struct adversario {
  lista_t *lista;
  pokemones_t *pokemones[MAX_POKEMONES];
  int turnos;
};

// PRE: EL adversario y los punteros a strings no deben ser nulos.
// POST: Guarda 3 pokemones aleatorios listados en los punteros a string.
void obtener_pokemones_listados(adversario_t *adversario, char *nombre_1,
                                char *nombre_2, char *nombre_3);

// PRE: El adversario debe existir y el pokemon con el nombre recibido debe
// estar listado. POST: Devuelve el pokemon con el nombre correspondiente.
pokemon_t *pokemon_por_nombre(adversario_t *adversario, char *nombre);

// PRE: El ataque recibido debe ser valido.
// POST: Guarda en nombre del ataque en el puntero auxiliar;
void funcion_copia(const struct ataque *ataque, void *auxiliar);

// PRE: El pokemon y el pokemon deben ser validos.
// POST: Devuelve el pokemon con los ataques disponibles cargados.
pokemones_t *guardar_ataques(pokemones_t *pokemon_adversario,
                             pokemon_t *pokemon);

// PRE: El adversario recibido debe tener pokemones y ataques cargados
// correctamente. POST: Devuelve la jugada respectiva al turno correspondiente.
jugada_t turno(adversario_t *adversario, int indice_pokemon, int indice_ataque);

void obtener_pokemones_listados(adversario_t *adversario, char *nombre_1,
                                char *nombre_2, char *nombre_3) {
  srand((unsigned int)time(NULL));
  size_t largo_lista = lista_tamanio(adversario->lista);
  int valor_1 = rand() % (int)largo_lista;
  int valor_2;
  int valor_3;

  do {
    valor_2 = rand() % (int)largo_lista;
  } while (valor_2 == valor_1);
  do {
    valor_3 = rand() % (int)largo_lista;
  } while (valor_3 == valor_1 || valor_3 == valor_2);

  lista_iterador_t *iterador = lista_iterador_crear(adversario->lista);
  int i = 0;
  while (lista_iterador_tiene_siguiente(iterador)) {
    pokemon_t *pokemon_actual =
        (pokemon_t *)lista_iterador_elemento_actual(iterador);
    const char *nombre_pokemon_actual = pokemon_nombre(pokemon_actual);
    if (i == valor_1) {
      strcpy(nombre_1, nombre_pokemon_actual);
    } else if (i == valor_2) {
      strcpy(nombre_2, nombre_pokemon_actual);
    } else if (i == valor_3) {
      strcpy(nombre_3, nombre_pokemon_actual);
    }
    lista_iterador_avanzar(iterador);
    i++;
  }
  lista_iterador_destruir(iterador);
}

pokemon_t *pokemon_por_nombre(adversario_t *adversario, char *nombre) {
  lista_iterador_t *iterador = lista_iterador_crear(adversario->lista);
  while (lista_iterador_tiene_siguiente(iterador)) {
    pokemon_t *pokemon_actual =
        (pokemon_t *)lista_iterador_elemento_actual(iterador);
    const char *nombre_pokemon_actual = pokemon_nombre(pokemon_actual);
    if (strcmp(nombre_pokemon_actual, nombre) == CERO) {
      lista_iterador_destruir(iterador);
      return pokemon_actual;
    }

    lista_iterador_avanzar(iterador);
  }
  lista_iterador_destruir(iterador);
  return NULL;
}

void funcion_copia(const struct ataque *ataque, void *auxiliar) {
  strcpy((char *)auxiliar, ataque->nombre);
}

pokemones_t *guardar_ataques(pokemones_t *pokemon_adversario,
                             pokemon_t *pokemon) {
  char nombre_ataque_1[MAX_NOMBRE];
  char nombre_ataque_2[MAX_NOMBRE];
  char nombre_ataque_3[MAX_NOMBRE];

  con_cada_ataque(pokemon, funcion_copia, nombre_ataque_1);
  con_cada_ataque(pokemon, funcion_copia, nombre_ataque_2);
  con_cada_ataque(pokemon, funcion_copia, nombre_ataque_3);

  strcpy(pokemon_adversario->ataques[PRIMER_ATAQUE]->nombre, nombre_ataque_1);
  strcpy(pokemon_adversario->ataques[SEGUNDO_ATAQUE]->nombre, nombre_ataque_2);
  strcpy(pokemon_adversario->ataques[TERCER_ATAQUE]->nombre, nombre_ataque_3);

  return pokemon_adversario;
}

jugada_t turno(adversario_t *adversario, int indice_pokemon,
               int indice_ataque) {
  jugada_t nueva_jugada = {.ataque = "", .pokemon = ""};

  strcpy(nueva_jugada.pokemon, adversario->pokemones[indice_pokemon]->nombre);
  printf("Pokemon elegido por el adversario %s\n", nueva_jugada.pokemon);
  strcpy(nueva_jugada.ataque,
         adversario->pokemones[indice_pokemon]->ataques[indice_ataque]->nombre);
  printf("Ataque elegido por el adversario %s\n", nueva_jugada.ataque);

  return nueva_jugada;
}

adversario_t *adversario_crear(lista_t *pokemon) {
  if (pokemon == NULL) {
    return NULL;
  }

  adversario_t *nuevo_adversario = calloc(1, sizeof(adversario_t));
  if (nuevo_adversario == NULL) {
    return NULL;
  }
  nuevo_adversario->lista = pokemon;
  nuevo_adversario->turnos = CERO;

  for (int i = 0; i < MAX_POKEMONES; i++) {
    nuevo_adversario->pokemones[i] = calloc(1, sizeof(pokemones_t));
    if (nuevo_adversario->pokemones[i] == NULL) {
      for (int j = 0; j < i; j++) {
        free(nuevo_adversario->pokemones[j]);
      }
      free(nuevo_adversario);
      return NULL;
    }
    for (int k = 0; k < MAX_ATAQUES; k++) {
      nuevo_adversario->pokemones[i]->ataques[k] = calloc(1, sizeof(ataques_t));
      if (nuevo_adversario->pokemones[i]->ataques[k] == NULL) {
        for (int l = 0; l < k; l++) {
          free(nuevo_adversario->pokemones[i]->ataques[l]);
        }
        free(nuevo_adversario->pokemones[i]);
        for (int m = 0; m < i; m++) {
          free(nuevo_adversario->pokemones[m]);
        }
        free(nuevo_adversario);
        return NULL;
      }
    }
  }
  return nuevo_adversario;
}

bool adversario_seleccionar_pokemon(adversario_t *adversario, char **nombre1,
                                    char **nombre2, char **nombre3) {
  if (adversario == NULL) {
    return false;
  }
  if (nombre1 == NULL) {
    return false;
  }
  if (nombre2 == NULL) {
    return false;
  }
  if (nombre3 == NULL) {
    return false;
  }

  char nombre_1[MAX_NOMBRE];
  char nombre_2[MAX_NOMBRE];
  char nombre_3[MAX_NOMBRE];

  obtener_pokemones_listados(adversario, nombre_1, nombre_2, nombre_3);
  strcpy(*nombre1, nombre_1);
  strcpy(*nombre2, nombre_2);
  strcpy(*nombre3, nombre_3);

  return true;
}

bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1,
                                     char *nombre2, char *nombre3) {
  if (adversario == NULL) {
    return false;
  }

  strcpy(adversario->pokemones[PRIMER_POKEMON]->nombre, nombre1);
  strcpy(adversario->pokemones[SEGUNDO_POKEMON]->nombre, nombre2);
  strcpy(adversario->pokemones[TERCER_POKEMON]->nombre, nombre3);

  pokemon_t *pokemon1 = pokemon_por_nombre(adversario, nombre1);
  pokemon_t *pokemon2 = pokemon_por_nombre(adversario, nombre2);
  pokemon_t *pokemon3 = pokemon_por_nombre(adversario, nombre3);

  adversario->pokemones[PRIMER_POKEMON] =
      guardar_ataques(adversario->pokemones[PRIMER_POKEMON], pokemon1);
  adversario->pokemones[SEGUNDO_POKEMON] =
      guardar_ataques(adversario->pokemones[SEGUNDO_POKEMON], pokemon2);
  adversario->pokemones[TERCER_POKEMON] =
      guardar_ataques(adversario->pokemones[TERCER_POKEMON], pokemon3);

  return true;
}

jugada_t adversario_proxima_jugada(adversario_t *adversario) {
  jugada_t jugada = {.ataque = "", .pokemon = ""};
  if (adversario == NULL) {
    return jugada;
  }

  if (adversario->turnos == PRIMER_TURNO) {
    jugada = turno(adversario, DOS, CERO);
  } else if (adversario->turnos == SEGUNDO_TURNO) {
    jugada = turno(adversario, UNO, UNO);
  } else if (adversario->turnos == TERCER_TURNO) {
    jugada = turno(adversario, CERO, DOS);
  } else if (adversario->turnos == CUARTO_TURNO) {
    jugada = turno(adversario, DOS, DOS);
  } else if (adversario->turnos == QUINTO_TURNO) {
    jugada = turno(adversario, UNO, CERO);
  } else if (adversario->turnos == SEXTO_TURNO) {
    jugada = turno(adversario, CERO, UNO);
  } else if (adversario->turnos == SEPTIMO_TURNO) {
    jugada = turno(adversario, DOS, UNO);
  } else if (adversario->turnos == OCTAVO_TURNO) {
    jugada = turno(adversario, UNO, DOS);
  } else {
    jugada = turno(adversario, CERO, CERO);
  }
  return jugada;
}

void adversario_informar_jugada(adversario_t *a, jugada_t j) { a->turnos++; }

void adversario_destruir(adversario_t *adversario) {
  if (adversario == NULL) {
    return;
  }

  for (int i = 0; i < MAX_POKEMONES; i++) {
    for (int j = 0; j < MAX_ATAQUES; j++) {
      free(adversario->pokemones[i]->ataques[j]);
    }
    free(adversario->pokemones[i]);
  }

  free(adversario);
}
