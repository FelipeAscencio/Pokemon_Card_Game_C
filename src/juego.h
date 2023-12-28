#ifndef JUEGO_H_
#define JUEGO_H_

#include "lista.h"
#include <stdbool.h>

typedef enum {
  TODO_OK,
  POKEMON_INSUFICIENTES,
  ERROR_GENERAL,
  POKEMON_INEXISTENTE,
  POKEMON_REPETIDO
} JUEGO_ESTADO;

typedef enum {
  ATAQUE_ERROR,
  ATAQUE_EFECTIVO,
  ATAQUE_INEFECTIVO,
  ATAQUE_REGULAR
} RESULTADO_ATAQUE;

typedef enum { JUGADOR1, JUGADOR2 } JUGADOR;

typedef struct {
  char pokemon[20];
  char ataque[20];
} jugada_t;

typedef struct {
  RESULTADO_ATAQUE jugador1;
  RESULTADO_ATAQUE jugador2;
} resultado_jugada_t;

typedef struct juego juego_t;

/**
 * Crea un juego. Devuelve NULL en caso de error.
 */
juego_t *juego_crear();

/**
 * Carga un archivo de pokemon (con el formato del TP1).
 *
 * Devuelve POKEMON_INSUFICIENTES si no hay pokemon suficientes como para jugar.
 *
 * Devuelve TODO_OK si no hay problemas o ERROR_GENERAL para cualquier otro
 * error.
 */
JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo);

/**
 * Devuelve una lista con todos los pokemon_t* disponibles para jugar. Esta
 * lista no debe ser modificada ni liberada por el usuario (será liberada por el
 * juego cuando este sea destruido).
 *
 * Devuelve la lista o NULL en caso de error.
 */
lista_t *juego_listar_pokemon(juego_t *juego);

/**
 * Informa los 3 pokemon seleccionados por el jugador. Cada jugador puede
 * seleccionar sus pokemon una sola vez.
 *
 * Retorna TODO_OK o ERROR_GENERAL en caso de error.
 *
 * Opcionalmente puede devolver POKEMON_REPETIDO o POKEMON_INEXISTENTE.
 */
JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
                                       const char *nombre1, const char *nombre2,
                                       const char *nombre3);

/**
 * El juego procesa las jugadas de ambos jugadores. Retorna una estructura con
 * el resultado de las jugadas.
 *
 * Los pokemon seleccionados se enfrentan en combate. Los puntos asignados a
 * cada jugador por la jugada dependen del poder del ataque y el tipo del ataque
 * y el pokemon que es atacado. Si el ataque es efectivo se multiplica el poder
 * de ataque por 3. Si el ataque es no efectivo se divide por dos el poder de
 * ataque. Si no es ni una cosa ni la otra se asigna como puntaje el poder de
 * ataque.
 *
 * - Los ataques NORMAL no son efectivos ni inefectivos contra nada.
 * - Los ataques FUEGO son efectivos contra PLANTA e inefectivos contra AGUA.
 * - Los ataques PLANTA son efectivos contra ROCA e inefectivos contra FUEGO.
 * - Los ataques ROCA son efectivos contra ELECTRICO e inefectivos contra PLANTA
 * - Los ataques ELECTRICO son efectivos contra AGUA e inefectivos contra ROCA
 * - Los ataques AGUA son efectivos contra FUEGO e inefectivos contra ELECTRICO
 *
 * Un ataque puede ser utilizado una sola vez a lo largo de todo el juego. El
 * juego finaliza cuando no quedan ataques sin usar. Gana quien sume mas puntos.
 *
 * Si alguno de los resultados es ATAQUE_ERROR la jugada no ha sido procesada
 * por algún error (por ejempo el pokemon o ataque no existe o no estan
 * disponibles).
 */
resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
                                     jugada_t jugada_jugador2);

/**
 * Devuelve el puntaje actual del jugador solicitado o 0 en caso de error.
 */
int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador);

/**
 * Devuelve true si el juego ha finalizado o no existe. Devuelve false si aun
 * quedan movimientos por jugar.
 */
bool juego_finalizado(juego_t *juego);

/**
 * Destruye el juego y toda la memoria asociada.
 */
void juego_destruir(juego_t *juego);

#endif // JUEGO_H_
