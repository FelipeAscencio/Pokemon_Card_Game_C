#include "juego.h"
#include "ataque.h"
#include "lista.h"
#include "pokemon.h"
#include "tipo.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CANTIDAD_POKEMONES 3
#define MINIMO_POKEMONES 6
#define POSICION_UNO 0
#define POSICION_DOS 1
#define POSICION_TRES 2
#define CERO 0
#define TRES 3
#define MAXIMO_TURNOS 9
#define MAX_ATAQUES 3
#define MAX_NOMBRE 20
#define MULTIPLICADOR_REGULAR 1
#define MULTIPLICADOR_EFECTIVO 3
#define DIVISOR_INEFECTIVO 2

typedef struct equipo {
  pokemon_t *pokemon;
  char nombre_pokemon[MAX_NOMBRE];
  char ataque_usado_1[MAX_NOMBRE];
  char ataque_usado_2[MAX_NOMBRE];
  char ataque_usado_3[MAX_NOMBRE];
} equipo_t;

typedef struct jugador {
  equipo_t *equipo[CANTIDAD_POKEMONES];
  int puntaje;
} jugador_t;

struct juego {
  int turnos_jugados;
  jugador_t *j1;
  jugador_t *j2;
  informacion_pokemon_t *info_pokemon;
  lista_t *lista_pokemon;
};

// PRE: El pokemon y la lista recibidos deben ser validos.
// POST: Inserta el pokemon dentro de la lista.
void funcion_de_insercion(pokemon_t *pokemon, void *aux);

// PRE: El iterador debe estar inicializado.
// POST: Devuelve el pokemon si lo encuentra, o NULL en caso de que no exista.
pokemon_t *buscar_pokemon_por_nombre(lista_iterador_t *iterador,
                                     const char *nombre);

// PRE: - .
// POST: Devuelve true en caso de haber pokemon duplicados, o false en caso
// contrario.
bool verificar_nombres_pokemon(const char *nombre1, const char *nombre2,
                               const char *nombre3);

// PRE: El juego debe estar inicializado correctamente.
// POST: Guarda los pokemones correspondientes en cada jugador.
juego_t *guardar_pokemones(juego_t *juego, JUGADOR jugador,
                           pokemon_t *pokemon_1, pokemon_t *pokemon_2,
                           pokemon_t *pokemon_3);

// PRE: - .
// POST: Libera la memoria reservada para el jugador.
void liberar_jugador(jugador_t *jugador);

// PRE: - .
// POST: Inicializa el jugador y todos sus campos.
jugador_t *inicializar_jugador();

// PRE: La jugada recibida debe ser valida.
// POST: Devuelve el equipo en caso de ser jugada valida, o NULL en caso
// contrario.
equipo_t *jugada_pokemon_valida(jugador_t *jugador, jugada_t jugada_jugador,
                                int *indice);

// PRE: El ataque y el tipo recibidos no deben ser nulos.
// POST: Devuelve la eficacia del ataque en relacion al tipo.
RESULTADO_ATAQUE eficacia(struct ataque ataque, enum TIPO tipo_pokemon);

// PRE: El jugador debe ser valido.
// POST: Añade los puntos correspondientes al jugador por su turno.
jugador_t *puntuar_jugada(jugador_t *jugador, struct ataque ataque_usado,
                          RESULTADO_ATAQUE jugada);

// PRE: El jugador, ataque y el equipo recibidos deben ser validos.
// POST: Modifica el vector de ataques usados de cada jugador en el equipo
// correspondiente.
jugador_t *modificar_usos_de_ataques(jugador_t *jugador,
                                     struct ataque ataque_usado, int indice);

void funcion_de_insercion(pokemon_t *pokemon, void *aux) {
  lista_t *lista = (lista_t *)aux;
  lista = lista_insertar(lista, (void *)pokemon);
  aux = (void *)lista;
}

pokemon_t *buscar_pokemon_por_nombre(lista_iterador_t *iterador,
                                     const char *nombre) {
  while (lista_iterador_tiene_siguiente(iterador)) {
    pokemon_t *pokemon_actual =
        (pokemon_t *)lista_iterador_elemento_actual(iterador);
    const char *nombre_pokemon_actual = pokemon_nombre(pokemon_actual);
    if (strcmp(nombre_pokemon_actual, nombre) == CERO) {
      return pokemon_actual;
    }

    lista_iterador_avanzar(iterador);
  }
  return NULL;
}

bool verificar_nombres_pokemon(const char *nombre1, const char *nombre2,
                               const char *nombre3) {
  if (strcmp(nombre1, nombre2) == CERO) {
    return true;
  }
  if (strcmp(nombre1, nombre3) == CERO) {
    return true;
  }
  if (strcmp(nombre2, nombre3) == CERO) {
    return true;
  }

  return false;
}

juego_t *guardar_pokemones(juego_t *juego, JUGADOR jugador,
                           pokemon_t *pokemon_1, pokemon_t *pokemon_2,
                           pokemon_t *pokemon_3) {
  if (jugador == JUGADOR1) {
    juego->j1->equipo[POSICION_UNO]->pokemon = pokemon_1;
    const char *nombre_pokemon_1 = pokemon_nombre(pokemon_1);
    strcpy(juego->j1->equipo[POSICION_UNO]->nombre_pokemon, nombre_pokemon_1);
    juego->j1->equipo[POSICION_DOS]->pokemon = pokemon_2;
    const char *nombre_pokemon_2 = pokemon_nombre(pokemon_2);
    strcpy(juego->j1->equipo[POSICION_DOS]->nombre_pokemon, nombre_pokemon_2);
    juego->j2->equipo[POSICION_TRES]->pokemon = pokemon_3;
    const char *nombre_pokemon_3 = pokemon_nombre(pokemon_3);
    strcpy(juego->j2->equipo[POSICION_TRES]->nombre_pokemon, nombre_pokemon_3);
  } else {
    juego->j2->equipo[POSICION_UNO]->pokemon = pokemon_1;
    const char *nombre_pokemon_1 = pokemon_nombre(pokemon_1);
    strcpy(juego->j2->equipo[POSICION_UNO]->nombre_pokemon, nombre_pokemon_1);
    juego->j2->equipo[POSICION_DOS]->pokemon = pokemon_2;
    const char *nombre_pokemon_2 = pokemon_nombre(pokemon_2);
    strcpy(juego->j2->equipo[POSICION_DOS]->nombre_pokemon, nombre_pokemon_2);
    juego->j1->equipo[POSICION_TRES]->pokemon = pokemon_3;
    const char *nombre_pokemon_3 = pokemon_nombre(pokemon_3);
    strcpy(juego->j1->equipo[POSICION_TRES]->nombre_pokemon, nombre_pokemon_3);
  }

  return juego;
}

void liberar_jugador(jugador_t *jugador) {
  if (jugador == NULL) {
    return;
  }

  for (int i = 0; i < CANTIDAD_POKEMONES; i++) {
    if (jugador->equipo[i] != NULL) {
      free(jugador->equipo[i]);
    }
  }

  free(jugador);
}

jugador_t *inicializar_jugador() {
  jugador_t *jugador = calloc(1, sizeof(jugador_t));
  if (jugador == NULL) {
    return NULL;
  }

  for (int i = 0; i < CANTIDAD_POKEMONES; i++) {
    jugador->equipo[i] = calloc(1, sizeof(equipo_t));
    if (jugador->equipo[i] == NULL) {
      liberar_jugador(jugador);
      return NULL;
    }
  }

  return jugador;
}

equipo_t *jugada_pokemon_valida(jugador_t *jugador, jugada_t jugada_jugador,
                                int *indice) {
  if (jugador == NULL) {
    return NULL;
  }

  int i = 0;
  bool encontrado = false;
  equipo_t *equipo_apto = NULL;
  while (i < CANTIDAD_POKEMONES && encontrado == false) {
    if (strcmp(jugador->equipo[i]->nombre_pokemon, jugada_jugador.pokemon) ==
        CERO) {
      bool ataque_usado = false;
      if (strcmp(jugador->equipo[i]->ataque_usado_1, jugada_jugador.ataque) ==
          CERO) {
        ataque_usado = true;
      }
      if (strcmp(jugador->equipo[i]->ataque_usado_2, jugada_jugador.ataque) ==
          CERO) {
        ataque_usado = true;
      }
      if (strcmp(jugador->equipo[i]->ataque_usado_3, jugada_jugador.ataque) ==
          CERO) {
        ataque_usado = true;
      }

      if (ataque_usado == false) {
        equipo_apto = jugador->equipo[i];
        encontrado = true;
        *indice = i;
        return equipo_apto;
      }
    }
    i++;
  }

  return equipo_apto;
}

equipo_t *encontrar_equipo(jugador_t *jugador, jugada_t jugada_jugador,
                           int *indice) {
  int i = 0;
  bool encontrado = false;
  equipo_t *equipo_apto = NULL;
  while (i < CANTIDAD_POKEMONES && encontrado == false) {
    if (strcmp(jugador->equipo[i]->nombre_pokemon, jugada_jugador.pokemon) ==
        CERO) {
      equipo_apto = jugador->equipo[i];
      encontrado = true;
      *indice = i;
    }
    i++;
  }
  return equipo_apto;
}

RESULTADO_ATAQUE eficacia(struct ataque ataque, enum TIPO tipo_pokemon) {
  enum TIPO ataque_tipo = ataque.tipo;
  if (ataque_tipo == FUEGO) {
    if (tipo_pokemon == PLANTA) {
      return ATAQUE_EFECTIVO;
    } else if (tipo_pokemon == AGUA) {
      return ATAQUE_INEFECTIVO;
    }
  } else if (ataque_tipo == PLANTA) {
    if (tipo_pokemon == ROCA) {
      return ATAQUE_EFECTIVO;
    } else if (tipo_pokemon == FUEGO) {
      return ATAQUE_INEFECTIVO;
    }
  } else if (ataque_tipo == ROCA) {
    if (tipo_pokemon == ELECTRICO) {
      return ATAQUE_EFECTIVO;
    } else if (tipo_pokemon == PLANTA) {
      return ATAQUE_INEFECTIVO;
    }
  } else if (ataque_tipo == ELECTRICO) {
    if (tipo_pokemon == AGUA) {
      return ATAQUE_EFECTIVO;
    } else if (tipo_pokemon == ROCA) {
      return ATAQUE_INEFECTIVO;
    }
  } else if (ataque_tipo == AGUA) {
    if (tipo_pokemon == FUEGO) {
      return ATAQUE_EFECTIVO;
    } else if (tipo_pokemon == ELECTRICO) {
      return ATAQUE_INEFECTIVO;
    }
  }

  return ATAQUE_REGULAR;
}

jugador_t *puntuar_jugada(jugador_t *jugador, struct ataque ataque_usado,
                          RESULTADO_ATAQUE jugada) {
  int puntos_turno = (int)ataque_usado.poder;

  if (jugada == ATAQUE_EFECTIVO) {
    puntos_turno = (puntos_turno * MULTIPLICADOR_EFECTIVO);
  } else if (jugada == ATAQUE_INEFECTIVO) {
    if ((puntos_turno % DIVISOR_INEFECTIVO) != CERO) {
      puntos_turno = ((puntos_turno / DIVISOR_INEFECTIVO) + 1);
    } else {
      puntos_turno = (puntos_turno / DIVISOR_INEFECTIVO);
    }
  }

  jugador->puntaje = jugador->puntaje + puntos_turno;
  return jugador;
}

jugador_t *modificar_usos_de_ataques(jugador_t *jugador,
                                     struct ataque ataque_usado, int indice) {
  if (jugador->equipo[indice]->ataque_usado_1 != NULL) {
    if (jugador->equipo[indice]->ataque_usado_2 != NULL) {
      strcpy(jugador->equipo[indice]->ataque_usado_3, ataque_usado.nombre);
    }
    strcpy(jugador->equipo[indice]->ataque_usado_2, ataque_usado.nombre);
  }
  strcpy(jugador->equipo[indice]->ataque_usado_1, ataque_usado.nombre);
  return jugador;
}

juego_t *juego_crear() {
  juego_t *juego = calloc(1, sizeof(juego_t));
  if (juego == NULL) {
    return NULL;
  }

  juego->j1 = inicializar_jugador();
  if (juego->j1 == NULL) {
    free(juego);
  }

  juego->j2 = inicializar_jugador();
  if (juego->j2 == NULL) {
    liberar_jugador(juego->j1);
    free(juego);
  }

  return juego;
}

JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo) {
  informacion_pokemon_t *ip = pokemon_cargar_archivo(archivo);
  if (ip == NULL) {
    return ERROR_GENERAL;
  }

  int cantidad_pokemones = pokemon_cantidad(ip);
  if (cantidad_pokemones < MINIMO_POKEMONES) {
    pokemon_destruir_todo(ip);
    return POKEMON_INSUFICIENTES;
  } else {
    informacion_pokemon_t *aux = juego->info_pokemon;
    juego->info_pokemon = ip;
    ip = aux;
    pokemon_destruir_todo(ip);
    return TODO_OK;
  }
}

lista_t *juego_listar_pokemon(juego_t *juego) {
  if (juego == NULL) {
    return NULL;
  }
  if (juego->info_pokemon == NULL) {
    return NULL;
  }

  lista_t *lista = lista_crear();
  con_cada_pokemon(juego->info_pokemon, funcion_de_insercion, (void *)lista);
  lista_t *aux = juego->lista_pokemon;
  juego->lista_pokemon = lista;
  lista = aux;
  lista_destruir(lista);
  return juego->lista_pokemon;
}

JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
                                       const char *nombre1, const char *nombre2,
                                       const char *nombre3) {
  if (juego == NULL) {
    return ERROR_GENERAL;
  }

  bool pokemon_duplicado = verificar_nombres_pokemon(nombre1, nombre2, nombre3);
  if (pokemon_duplicado == true) {
    return POKEMON_REPETIDO;
  }

  lista_iterador_t *iterador1 = lista_iterador_crear(juego->lista_pokemon);
  pokemon_t *pokemon_1 = buscar_pokemon_por_nombre(iterador1, nombre1);
  lista_iterador_destruir(iterador1);
  lista_iterador_t *iterador2 = lista_iterador_crear(juego->lista_pokemon);
  pokemon_t *pokemon_2 = buscar_pokemon_por_nombre(iterador2, nombre2);
  lista_iterador_destruir(iterador2);
  lista_iterador_t *iterador3 = lista_iterador_crear(juego->lista_pokemon);
  pokemon_t *pokemon_3 = buscar_pokemon_por_nombre(iterador3, nombre3);
  lista_iterador_destruir(iterador3);
  if (pokemon_1 != NULL) {
    if (pokemon_2 != NULL) {
      if (pokemon_3 != NULL) {
        juego =
            guardar_pokemones(juego, jugador, pokemon_1, pokemon_2, pokemon_3);
        return TODO_OK;
      }
    }
  }
  return POKEMON_INEXISTENTE;
}

resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
                                     jugada_t jugada_jugador2) {
  resultado_jugada_t resultado;
  resultado.jugador1 = ATAQUE_ERROR;
  resultado.jugador2 = ATAQUE_ERROR;

  int indice_1 = 0;
  int indice_2 = 0;
  equipo_t *equipo_1 =
      jugada_pokemon_valida(juego->j1, jugada_jugador1, &indice_1);
  if (equipo_1 == NULL) {
    return resultado;
  }

  equipo_t *equipo_2;
  if (juego->turnos_jugados < TRES) {
    equipo_2 = jugada_pokemon_valida(juego->j2, jugada_jugador2, &indice_2);
    if (equipo_2 == NULL) {
      return resultado;
    }
  } else {
    equipo_2 = encontrar_equipo(juego->j2, jugada_jugador2, &indice_2);
  }

  enum TIPO tipo_pokemon_1 = pokemon_tipo(equipo_1->pokemon);
  enum TIPO tipo_pokemon_2 = pokemon_tipo(equipo_2->pokemon);

  const struct ataque *ataque_1 =
      pokemon_buscar_ataque(equipo_1->pokemon, jugada_jugador1.ataque);
  const struct ataque *ataque_2 =
      pokemon_buscar_ataque(equipo_2->pokemon, jugada_jugador2.ataque);

  RESULTADO_ATAQUE jugada_1 = eficacia(*ataque_1, tipo_pokemon_2);
  RESULTADO_ATAQUE jugada_2 = eficacia(*ataque_2, tipo_pokemon_1);
  resultado.jugador1 = jugada_1;
  resultado.jugador2 = jugada_2;

  juego->j1 = puntuar_jugada(juego->j1, *ataque_1, jugada_1);
  juego->j2 = puntuar_jugada(juego->j2, *ataque_2, jugada_2);

  juego->j1 = modificar_usos_de_ataques(juego->j1, *ataque_1, indice_1);
  juego->j2 = modificar_usos_de_ataques(juego->j2, *ataque_2, indice_2);

  juego->turnos_jugados++;
  return resultado;
}

int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador) {
  if (juego == NULL) {
    return CERO;
  }

  if (jugador == JUGADOR1) {
    return juego->j1->puntaje;
  }
  return juego->j2->puntaje;
}

bool juego_finalizado(juego_t *juego) {
  if (juego == NULL) {
    return true;
  }

  if (juego->turnos_jugados == MAXIMO_TURNOS) {
    return true;
  }

  return false;
}

void juego_destruir(juego_t *juego) {
  pokemon_destruir_todo(juego->info_pokemon);
  lista_destruir(juego->lista_pokemon);
  liberar_jugador(juego->j1);
  liberar_jugador(juego->j2);
  free(juego);
}
