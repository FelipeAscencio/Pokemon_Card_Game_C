#include "pokemon.h"
#include "ataque.h"
#include "tipo.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOMBRE 50
#define MAX_ATAQUES 3
#define MAX_LINEA 255
#define MAX_POKEMONES 700

struct pokemon {
  char nombre[MAX_NOMBRE];
  enum TIPO tipo;
  struct ataque *ataques[MAX_ATAQUES];
};

struct info_pokemon {
  struct pokemon *pokemones[MAX_POKEMONES];
  int cantidad_pokemones;
};

// PRE: - .
// POST: Ordena por orden alfabetico creciente los pokemon del puntero
// informacion.
void ordenar_informacion(struct info_pokemon *informacion);

// PRE: - .
// POST: Devuelve true en el caso de que haya podido asignar la memoria
// necesaria.
bool validar_memoria_ataques(struct pokemon **pokemon_temporal,
                             char nombre_pokemon[MAX_NOMBRE],
                             enum TIPO tipo_pokemon);

// PRE: Todas las variables en el argumento deben estar cargadas correctamente.
// POST: Carga los ataques al nuevo pokemon.
void cargar_datos_ataques(struct pokemon **pokemon_temporal,
                          struct ataque pre_ataques[MAX_ATAQUES]);

// PRE: Todas las variables en el argumento deben estar cargadas correctamente.
// POST: Carga al nuevo pokemon y devuelve true, en caso de no poder cargarlo
// devuelve false.
bool cargar_datos_pokemon(struct info_pokemon *informacion,
                          char nombre_pokemon[MAX_NOMBRE],
                          enum TIPO tipo_pokemon,
                          struct ataque pre_ataques[MAX_ATAQUES]);

// PRE: enum TIPO debe estar definido correctamente.
// POST: Devuelve el tipo correspondiente a cada caso en formato "enum TIPO".
enum TIPO asignar_tipo(char letra_tipo);

// PRE: - .
// POST: Modifica el pokemon en caso de poder haces los cambios, sino corta.
void leer_archivo(FILE *archivo, struct info_pokemon *informacion);

void ordenar_informacion(struct info_pokemon *informacion) {
  int cantidad_a_ordenar = informacion->cantidad_pokemones;
  struct pokemon *auxiliar;
  int i = 0;
  int j = 0;

  if (cantidad_a_ordenar <= 1) {
    return;
  } else {
    for (i = 0; i < cantidad_a_ordenar - 1; i++) {
      for (j = 0; j < cantidad_a_ordenar - i - 1; j++) {
        if (strcmp(informacion->pokemones[j]->nombre,
                   informacion->pokemones[j + 1]->nombre) >= 0) {
          auxiliar = informacion->pokemones[j];
          informacion->pokemones[j] = informacion->pokemones[j + 1];
          informacion->pokemones[j + 1] = auxiliar;
        }
      }
    }
  }
}

bool validar_memoria_ataques(struct pokemon **pokemon_temporal,
                             char nombre_pokemon[MAX_NOMBRE],
                             enum TIPO tipo_pokemon) {
  int j = 0;
  int k = 0;
  for (k = 0; k < MAX_ATAQUES; k++) {
    (*pokemon_temporal)->ataques[k] =
        (struct ataque *)calloc(1, sizeof(struct ataque));
    if ((*pokemon_temporal)->ataques[k] == NULL) {
      for (j = 0; j < k; j++) {
        free((*pokemon_temporal)->ataques[j]);
      }
      free(pokemon_temporal);
      return false;
    }
  }
  return true;
}

void cargar_datos_ataques(struct pokemon **pokemon_temporal,
                          struct ataque pre_ataques[MAX_ATAQUES]) {
  int j = 0;
  for (j = 0; j < MAX_ATAQUES; j++) {
    strcpy((*pokemon_temporal)->ataques[j]->nombre, pre_ataques[j].nombre);
    (*pokemon_temporal)->ataques[j]->poder = pre_ataques[j].poder;
    (*pokemon_temporal)->ataques[j]->tipo = pre_ataques[j].tipo;
  }
}

bool cargar_datos_pokemon(struct info_pokemon *informacion,
                          char nombre_pokemon[MAX_NOMBRE],
                          enum TIPO tipo_pokemon,
                          struct ataque pre_ataques[MAX_ATAQUES]) {
  struct pokemon *pokemon_temporal =
      (struct pokemon *)calloc(1, sizeof(struct pokemon));
  if (pokemon_temporal == NULL) {
    return false;
  }
  strcpy(pokemon_temporal->nombre, nombre_pokemon);
  pokemon_temporal->tipo = tipo_pokemon;

  bool espacio_valido =
      validar_memoria_ataques(&pokemon_temporal, nombre_pokemon, tipo_pokemon);

  if (espacio_valido == true) {
    cargar_datos_ataques(&pokemon_temporal, pre_ataques);
    int indice_pokemon = informacion->cantidad_pokemones;
    informacion->cantidad_pokemones++;
    informacion->pokemones[indice_pokemon] = pokemon_temporal;
    return true;
  }
  return false;
}

enum TIPO asignar_tipo(char letra_tipo) {
  if (letra_tipo == 'N') {
    return NORMAL;
  } else if (letra_tipo == 'F') {
    return FUEGO;
  } else if (letra_tipo == 'A') {
    return AGUA;
  } else if (letra_tipo == 'P') {
    return PLANTA;
  } else if (letra_tipo == 'E') {
    return ELECTRICO;
  } else if (letra_tipo == 'R') {
    return ROCA;
  } else {
    return -1;
  }
}

void leer_archivo(FILE *archivo, struct info_pokemon *informacion) {
  char linea[MAX_LINEA];
  char nombre_pokemon[MAX_NOMBRE];
  char letra_tipo;
  enum TIPO tipo_pokemon;
  struct ataque pre_ataques[MAX_ATAQUES];
  int i = 0;
  int estado = 0;

  while (fgets(linea, sizeof(linea), archivo) != NULL) {
    if (estado == 0) {
      if (sscanf(linea, "%49[^;];%c", nombre_pokemon, &letra_tipo) == 2) {
        tipo_pokemon = asignar_tipo(letra_tipo);
        if (tipo_pokemon == -1) {
          return;
        }
        estado++;
      } else {
        return;
      }
    } else if (estado >= 1 && estado <= 3) {
      if (sscanf(linea, "%49[^;];%c;%u", pre_ataques[i].nombre, &letra_tipo,
                 &pre_ataques[i].poder) == 3) {
        pre_ataques[i].tipo = asignar_tipo(letra_tipo);
        if (pre_ataques[i].tipo == -1) {
          return;
        }
        estado++;
        i++;
      } else {
        return;
      }
    }
    if (estado >= 4) {
      bool carga_exitosa = cargar_datos_pokemon(informacion, nombre_pokemon,
                                                tipo_pokemon, pre_ataques);
      if (carga_exitosa == true) {
        i = 0;
        estado = 0;
      } else {
        return;
      }
    }
  }
}

informacion_pokemon_t *pokemon_cargar_archivo(const char *path) {
  if (path == NULL) {
    return NULL;
  }
  FILE *archivo = fopen(path, "r");
  if (archivo == NULL) {
    return NULL;
  }

  struct info_pokemon *informacion =
      (struct info_pokemon *)calloc(1, sizeof(struct info_pokemon));
  if (informacion == NULL) {
    fclose(archivo);
    return NULL;
  }

  informacion->cantidad_pokemones = 0;
  leer_archivo(archivo, informacion);
  fclose(archivo);

  if (informacion->cantidad_pokemones >= 1) {
    ordenar_informacion(informacion);
    return informacion;
  } else {
    free(informacion);
    return NULL;
  }
}

pokemon_t *pokemon_buscar(informacion_pokemon_t *ip, const char *nombre) {
  if (nombre == NULL) {
    return NULL;
  }
  if (ip == NULL) {
    return NULL;
  }

  int i = 0;
  for (i = 0; i < ip->cantidad_pokemones; i++) {
    if (strcmp(ip->pokemones[i]->nombre, nombre) == 0) {
      return ip->pokemones[i];
    }
  }
  return NULL;
}

int pokemon_cantidad(informacion_pokemon_t *ip) {
  if (ip == NULL) {
    return 0;
  }

  return ip->cantidad_pokemones;
}

const char *pokemon_nombre(pokemon_t *pokemon) {
  if (pokemon == NULL) {
    return NULL;
  }

  return pokemon->nombre;
}

enum TIPO pokemon_tipo(pokemon_t *pokemon) {
  if (pokemon == NULL) {
    return NORMAL;
  }

  return pokemon->tipo;
}

const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon,
                                           const char *nombre) {
  if (pokemon == NULL) {
    return NULL;
  }
  if (nombre == NULL) {
    return NULL;
  }

  int i = 0;
  for (i = 0; i < MAX_ATAQUES; i++) {
    if (pokemon->ataques[i] != NULL) {
      if (strcmp(pokemon->ataques[i]->nombre, nombre) == 0) {
        return pokemon->ataques[i];
      }
    }
  }

  return NULL;
}

int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
                     void *aux) {
  if (ip == NULL) {
    return 0;
  }
  if (f == NULL) {
    return 0;
  }

  ordenar_informacion(ip);

  int cantidad_aplicaciones = 0;
  int i = 0;

  for (i = 0; i < ip->cantidad_pokemones; i++) {
    if (ip->pokemones[i] != NULL) {
      f(ip->pokemones[i], aux);
      cantidad_aplicaciones++;
    }
  }

  return cantidad_aplicaciones;
}

int con_cada_ataque(pokemon_t *pokemon,
                    void (*f)(const struct ataque *, void *), void *aux) {
  if (pokemon == NULL) {
    return 0;
  }
  if (f == NULL) {
    return 0;
  }

  int cantidad_ataques = 0;
  int i = 0;

  for (i = 0; i < MAX_ATAQUES; i++) {
    if (pokemon->ataques[i] != NULL) {
      f(pokemon->ataques[i], aux);
      cantidad_ataques++;
    }
  }

  return cantidad_ataques;
}

void pokemon_destruir_todo(informacion_pokemon_t *ip) {
  int i = 0;
  int j = 0;

  if (ip == NULL) {
    return;
  } else {
    for (i = 0; i < ip->cantidad_pokemones; i++) {
      if (ip->pokemones[i] != NULL) {
        for (j = 0; j < MAX_ATAQUES; j++) {
          if (ip->pokemones[i]->ataques[j] != NULL) {
            free(ip->pokemones[i]->ataques[j]);
          }
        }
        free(ip->pokemones[i]);
      }
    }
    free(ip);
  }
}
