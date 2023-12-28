#ifndef ADVERSARIO_H_
#define ADVERSARIO_H_

#include "juego.h"
#include "lista.h"

typedef struct adversario adversario_t;

/**
 * Crea un adversario asignandole un listado de pokemon_t que puede utilizar
 * durante el juego.
 *
 * Devuelve NULL en caso de error.
 */
adversario_t *adversario_crear(lista_t *);

/**
 * El adversario selecciona los pokemon para utilizar durante el juego. Modifica
 * los 3 punteros pasados para hacerlos apuntar a los nombres de los 3 pokemon
 * que quiere seleccionar.
 *
 * Los primeros 2 pokemon pertenecen al adversario, el tercer pokemon es para el
 * jugador.
 *
 * Devuelve true si pudo seleccionar o false en caso de error,
 */
bool adversario_seleccionar_pokemon(adversario_t *, char **nombre1,
                                    char **nombre2, char **nombre3);

/**
 * Informa al adversario los pokemon seleccionados por el jugador. Los primeros
 * 2 pokemon pertenecen al jugador, el tercer pokemon completa los pokemon del
 * adversario.
 *
 * Devuelve true en caso de exito o false en caso de error.
 */
bool adversario_pokemon_seleccionado(adversario_t *, char *nombre1,
                                     char *nombre2, char *nombre3);

/**
 * Devuelve la próxima jugada del adversario para el turno actual. La jugada
 * debe tener en cuenta las reglas del juego y adherirse a ellas (no se puede
 * utilizar el mismo movimientos dos veces, etc).
 */
jugada_t adversario_proxima_jugada(adversario_t *);

/**
 * Le informa al adversario cuál fue la última jugada del jugador.
 * Al llamar a esta funcion el adversario sabe que el turno fue exitoso.
 */
void adversario_informar_jugada(adversario_t *, jugada_t);

/**
 * Destruye el adversario y libera la memoria reservada.
 */
void adversario_destruir(adversario_t *);

#endif // ADVERSARIO_H_
