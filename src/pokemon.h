#ifndef POKEMON_H_
#define POKEMON_H_

#include "tipo.h"
typedef struct pokemon pokemon_t;
typedef struct info_pokemon informacion_pokemon_t;

/**
 * Lee un archivo con pokemones (con el formato especificado en el enunciado) y
 * devuelve un puntero a la información leida.
 *
 * En caso de encontrarse un error con uno de los pokemon leidos, se debe
 * descartar dicho pokemon y mantener los pokemon validos. No se debe continuar
 * leyendo el archivo.
 *
 * En caso de no poder leer ningún pokemon correctamente, devuelve NULL.
 */
informacion_pokemon_t *pokemon_cargar_archivo(const char *path);

/**
 * Busca un pokemon por nombre. Devuelve el pokemon encontrado o NULL si no se
 * encuentra.
 */
pokemon_t *pokemon_buscar(informacion_pokemon_t *ip, const char *nombre);

/**
 * Devuelve la cantidad de pokemon disponibles.
 */
int pokemon_cantidad(informacion_pokemon_t *ip);

/**
 *Devuevle el nombre del pokemon
 */
const char *pokemon_nombre(pokemon_t *pokemon);

/**
 *Devuevle el tipo del pokemon. Por defecto devuelve NORMAL.
 */
enum TIPO pokemon_tipo(pokemon_t *pokemon);

/**
 * Busca un ataque del pokemon por nombre. Devuelve el ataque encontrado o NULL
 * si no se encuentra.
 */
const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon,
                                           const char *nombre);

/**
 * Aplica la función f a cada uno de los pokemon (por orden alfabetico). El
 * parámetro auxiliar debe pasarse como segundo parámetro a la función.
 *
 * Devuelve la cantidad de pokemon al que se les aplicó la función.
 */
int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
                     void *aux);

/**
 * Aplica la función f a cada uno de los ataques de un pokemon (con el mismo
 * orden de aparicion original dentro del archivo). El parámetro auxiliar debe
 * pasarse como segundo parámetro a la función.
 *
 * Devuelve la cantidad de ataques al que se les aplicó la función.
 */
int con_cada_ataque(pokemon_t *pokemon,
                    void (*f)(const struct ataque *, void *), void *aux);

/**
 *  Libera la información de los pokemon
 */
void pokemon_destruir_todo(informacion_pokemon_t *ip);

#endif // POKEMON_H_
