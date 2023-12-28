#include "src/adversario.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"
#include "src/menu.h"
#include "src/pokemon.h"
#include "src/tipo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCION 20
#define MAX_NOMBRE 20
#define CARACTERES_EXTRA 13
#define CERO 0
#define ATACAR "atacar"

typedef struct estado_juego {
  bool iniciado;
  bool cargado;
  bool elegidos;
  bool terminado;
  juego_t *juego;
  menu_t *menu;
  adversario_t *adversario;
  char pokemon_j1_1[MAX_ACCION];
  char pokemon_j1_2[MAX_ACCION];
  char pokemon_j1_3[MAX_ACCION];
} estado_t;

// PRE; El string recibido debe estar inicializado de forma valida.
// POST: Modifica el string recibido por parametro segun la entrada del usuario.
void obtener_accion(char *nombre_accion);

// PRE: El menu debe estar inicializado de forma valida.
// POST: Inicia el juego siempre y cuando todo este listo.
bool comenzar_juego(void *contexto);

// PRE: El menu debe estar inicializado de forma valida.
// POST: Imprime le ayuda necesaria al usuario para entender el juego.
bool imprimir_ayuda(void *contexto);

// PRE: El menu debe estar inicializado de forma valida.
// POST: Imprime el tutorial de juego al usuario
bool tutorial_juego(void *contexto);

// PRE: El menu debe estar inicializado de forma valida.
// POST: Permite salir del juego al usuario siempre que este lo requiera.
bool salir_juego(void *contexto);

// PRE: El menu debe estar inicializado de forma valida.
// POST: Carga el archivo de pokemones, siempre y cuando este exista.
bool cargar_archivo(void *contexto);

// PRE: El menu debe estar inicializado de forma valida.
// POST: Lista los pokemones disponibles para elegir.
bool listar_pokemones(void *contexto);

// PRE: Los pokemones recibidos deben ser validos.
// POST: Devuelve true si la eleccion de los pokemon del usuario fue exitosa.
bool usuario_elegir_pokemones(estado_t *estado, char *pokemon1, char *pokemon2,
                              char *pokemon3);

// PRE: Los pokemones recibidos deben ser validos.
// POST: Imprime los 2 equipos pokemones.
void imprimir_equipos(char *p1, char *p2, char *p3, char *p4, char *p5,
                      char *p6);

// PRE: - .
// POST: Devuelve true si las elecciones fueron validas, o false en caso
// contrario.
bool eleccion_pokemones(estado_t *estado);

// PRE: El menu debe estar inicializado de forma valida.
// POST: Realiza la eleccion de los pokemones, siempre y cuando todo sea
// correcto.
bool elegir_pokemones(void *contexto);

// PRE: El nombre recibido debe ser valido.
// POST: Devuelve el pokemon con el mismo nombre al recibido.
pokemon_t *obtener_el_pokemon(lista_t *lista, char *nombre);

// PRE: El ataque recibido debe ser valido.
// POST: Imprimir el nombre del ataque recibido.
void imprimir_nombre_ataque(const struct ataque *ataque, void *aux);

// PRE: El menu debe estar inicializado de forma valida.
// POST: Muestra por pantalla los ataques que dispone el usuario.
bool listar_ataques(void *contexto);

void obtener_accion(char *nombre_accion) {
  if (fgets(nombre_accion, MAX_ACCION, stdin) != NULL) {
    size_t len = strlen(nombre_accion);
    if (len > 0 && nombre_accion[len - 1] == '\n') {
      nombre_accion[len - 1] = '\0';
    }
  }
}

bool comenzar_juego(void *contexto) {
  estado_t *estado = (estado_t *)contexto;
  if (estado->iniciado == true) {
    return false;
  }
  if (estado->cargado == false) {
    printf(
        "\nAntes de iniciar el juego debes cargar el archivo de pokemones.\n");
    return false;
  }
  if (estado->elegidos == false) {
    printf("\nDebes elegir tus pokemones antes de iniciar el juego.\n");
    return false;
  }
  printf("\nEl juego va a iniciar, recuerda que el menu de ayuda y tutorial "
         "siguen activos.\n");
  printf("Algunas funciones del menu se verán deshabilitadas hasta el final "
         "del juego.\n");
  estado->iniciado = true;
  return true;
}

bool imprimir_ayuda(void *contexto) {
  printf("\n\n\nEntraste en el menu de ayuda.\n");
  printf("Tranquilo, empezar a jugar puede ser algo dificil, pero con el "
         "tiempo te encantara.\n");
  printf("En el juego cuentas con muchos comandos para usar.\n");
  printf("Este es un listado de todos los comandos que puedes utilizar.\n");
  estado_t *estado = (estado_t *)contexto;
  menu_listar_comandos(estado->menu);
  printf("Recuerda que si ya estas jugando, puede ser que algunos esten "
         "deshabilitados.\n");
  printf("Espero que te haya sido de mucha ayuda.\n");
  printf("Animo, vas a convertirte en el mejor duelista pokemon de todos los "
         "tiempos.\n\n");
  return true;
}

bool tutorial_juego(void *contexto) {
  printf("\n\n\nBienvenido al tutorial de juego, aqui vas a aprender todo lo "
         "necesario acerca de como jugar.\n");
  printf("Primero que nada, vas a tener que enviar el nombre del archivo de "
         "pokemones con el que quieres jugar.\n");
  printf("Este debe encontrarse dentro de la carpeta 'ejemplos' del juego.\n");
  printf("Recuerda que el archivo tiene que respetar el orden acordado.\n");
  printf("Si no tienes tu propio archivo simplemente escribe 'correcto' cuando "
         "vayas al menu de cargar archivo.\n");
  printf("Si tienes tu propio archivo escribe su nombre para cargarlo.\n");
  printf("Una vez cargado el archivo, puedes listarlo (tip: si no recuerdas "
         "algun comando entra al menu de ayuda).\n");
  printf("Y con eso ya podras elegir tus pokemones, recuerda que los 2 "
         "primeros iran a tu equipo, \n");
  printf("y el tercero al equipo del rival (ayuda: trata de elegir el peor "
         "para tu rival, a no ser que quieras un verdadero reto).\n");
  printf("Una vez elegidos tus pokemones, ya puedes iniciar el juego.\n");
  printf("Con el juego ya iniciado, puedes listar los ataques para ver cuales "
         "son tus opciones.\n");
  printf("En cada uno de los 9 turnos, deberas hacer un ataque con el pokemon "
         "seleccionado.\n");
  printf(
      "Recuerda que no puedes repetir ataques ya utilizados con un pokemon.\n");
  printf("En base al ataque que elijas, y el tipo del pokemon rival, se te "
         "asignarán unos puntos.\n");
  printf("Para realizar la jugada deberas escribir primero el pokemon que "
         "elijes, y segundo el ataque.\n");
  printf("No te preocupes si escribes algo mal o repites ataques, yo te "
         "notificare cuando eso pase y podras repetir el proceso.\n");
  printf("Mientras mas fuerte sea el ataque, y mas eficaz contra el tipo del "
         "rival, mas puntos obtendras.\n");
  printf("El entrenador que llegue al final del juego con mas puntos gana, no "
         "te desanimes si no\n");
  printf("lo consiges a la primera, con practica llegaras a ser el mejor "
         "entrenador de todos los tiempos.\n");
  printf("Con esto ya tienes todo lo necesario para ser el mejor, disfruta "
         "mucho el juego, animooo!!\n\n");
  return true;
}

bool salir_juego(void *contexto) {
  estado_t *estado = (estado_t *)contexto;
  estado->terminado = true;
  printf("\n\n\nYa te vas? que pena. Nos vemos la proxima!\n\n");
  return true;
}

bool cargar_archivo(void *contexto) {
  estado_t *estado = (estado_t *)contexto;
  if (estado->iniciado == true) {
    return false;
  }
  if (estado->cargado == true) {
    return false;
  }

  printf("\nEscribe a continuación el nombre del archivo de pokemones:\n");
  char nombre_archivo[MAX_ACCION];
  obtener_accion(nombre_archivo);
  char nombre_final[MAX_ACCION + CARACTERES_EXTRA];
  snprintf(nombre_final, sizeof(nombre_final), "ejemplos/%s.txt",
           nombre_archivo);

  JUEGO_ESTADO carga = juego_cargar_pokemon(estado->juego, nombre_final);
  if (carga == ERROR_GENERAL) {
    printf("\nEl nombre del archivo brindado es incorrecto.\n");
    return false;
  } else if (carga == POKEMON_INSUFICIENTES) {
    printf("\nEl archivo de pokemon brindado no tiene los suficientes "
           "pokemones para jugar.\n");
    return false;
  }

  estado->cargado = true;
  return true;
}

bool listar_pokemones(void *contexto) {
  estado_t *estado = (estado_t *)contexto;
  if (estado->iniciado == true) {
    return false;
  }
  if (estado->cargado == false) {
    printf("\nAntes de listar los pokemones, debes cargar el archivo.\n");
    return false;
  }
  if (estado->elegidos == true) {
    printf("\nYa elegiste tus pokemon, solo te falta empezar el juego.\n");
    return false;
  }

  lista_t *lista = juego_listar_pokemon(estado->juego);
  lista_iterador_t *iterador = lista_iterador_crear(lista);
  printf("\n\n\nLos pokemon disponibles son: \n");
  while (lista_iterador_tiene_siguiente(iterador)) {
    pokemon_t *pokemon_actual =
        (pokemon_t *)lista_iterador_elemento_actual(iterador);
    const char *nombre_pokemon_actual = pokemon_nombre(pokemon_actual);
    printf("%s\n", nombre_pokemon_actual);
    lista_iterador_avanzar(iterador);
  }
  lista_iterador_destruir(iterador);

  return true;
}

bool usuario_elegir_pokemones(estado_t *estado, char *pokemon1, char *pokemon2,
                              char *pokemon3) {
  JUEGO_ESTADO elecciones_usuario = juego_seleccionar_pokemon(
      estado->juego, JUGADOR1, pokemon1, pokemon2, pokemon3);

  if (elecciones_usuario == ERROR_GENERAL) {
    printf("\nUps, hubo un error al realizar la carga, pofavor, vuelva a "
           "internarlo.\n");
    return false;
  } else if (elecciones_usuario == POKEMON_REPETIDO) {
    printf("\nNo se puede elegir 2 veces al mismo pokemon.\n");
    return false;
  } else if (elecciones_usuario == POKEMON_INEXISTENTE) {
    printf("\nUn pokemon o mas de los seleccionados no existe entre los "
           "posibles.\n");
    return false;
  }

  printf("\nFelicitaciones, tus elecciones son validas.\n");
  return true;
}

void imprimir_equipos(char *p1, char *p2, char *p3, char *p4, char *p5,
                      char *p6) {
  printf("\nLos equipos elegidos son asi: \n");
  printf("\nEquipo 1: %s, %s, %s\n", p1, p2, p3);
  printf("\nEquipo 2: %s, %s, %s\n", p4, p5, p6);
}

bool eleccion_pokemones(estado_t *estado) {
  printf("\nSelecciona tu primer pokemon: \n");
  char pokemon1[MAX_ACCION];
  obtener_accion(pokemon1);
  printf("\nSelecciona tu segundo pokemon: \n");
  char pokemon2[MAX_ACCION];
  obtener_accion(pokemon2);
  printf("\nSelecciona el pokemon para tu rival: \n");
  char pokemon3[MAX_ACCION];
  obtener_accion(pokemon3);
  bool eleccion_exitosa =
      usuario_elegir_pokemones(estado, pokemon1, pokemon2, pokemon3);

  if (eleccion_exitosa == true) {
    strcpy(estado->pokemon_j1_1, pokemon1);
    strcpy(estado->pokemon_j1_2, pokemon2);
    char *pokemon1_adversario = calloc(MAX_NOMBRE, sizeof(char));
    char *pokemon2_adversario = calloc(MAX_NOMBRE, sizeof(char));
    char *pokemon3_adversario = calloc(MAX_NOMBRE, sizeof(char));
    adversario_seleccionar_pokemon(estado->adversario, &pokemon1_adversario,
                                   &pokemon2_adversario, &pokemon3_adversario);
    juego_seleccionar_pokemon(estado->juego, JUGADOR2, pokemon1_adversario,
                              pokemon2_adversario, pokemon3_adversario);
    adversario_pokemon_seleccionado(estado->adversario, pokemon1_adversario,
                                    pokemon2_adversario, pokemon3);
    strcpy(estado->pokemon_j1_3, pokemon3_adversario);
    imprimir_equipos(pokemon1, pokemon2, pokemon3_adversario,
                     pokemon1_adversario, pokemon2_adversario, pokemon3);
    free(pokemon1_adversario);
    free(pokemon2_adversario);
    free(pokemon3_adversario);
    return true;
  }

  return false;
}

bool elegir_pokemones(void *contexto) {
  estado_t *estado = (estado_t *)contexto;
  if (estado->iniciado == true) {
    return false;
  }
  if (estado->cargado == false) {
    printf("\nAntes de elegir los pokemones, debes cargar el archivo.\n");
    return false;
  }
  if (estado->elegidos == true) {
    printf("\nYa has seleccionado los pokemones para tu combate.\n");
    return false;
  }
  lista_t *lista = juego_listar_pokemon(estado->juego);
  estado->adversario = adversario_crear(lista);

  bool eleccion_exitosa = eleccion_pokemones(estado);
  if (eleccion_exitosa == true) {
    estado->elegidos = true;
    return true;
  }

  return false;
}

pokemon_t *obtener_el_pokemon(lista_t *lista, char *nombre) {
  lista_iterador_t *iterador = lista_iterador_crear(lista);
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

void imprimir_nombre_ataque(const struct ataque *ataque, void *aux) {
  printf("%s, ", ataque->nombre);
}

bool listar_ataques(void *contexto) {
  estado_t *estado = (estado_t *)contexto;
  if (estado->elegidos == false) {
    printf("\nDebes elegir los pokemones, antes de ver sus ataques.\n");
    return false;
  }

  lista_t *lista = juego_listar_pokemon(estado->juego);
  pokemon_t *pokemon1 = obtener_el_pokemon(lista, estado->pokemon_j1_1);
  pokemon_t *pokemon2 = obtener_el_pokemon(lista, estado->pokemon_j1_2);
  pokemon_t *pokemon3 = obtener_el_pokemon(lista, estado->pokemon_j1_3);

  printf("\n\n\nLos ataques de cada pokemon de tu equipo son: \n");
  printf("%s : \n", estado->pokemon_j1_1);
  con_cada_ataque(pokemon1, imprimir_nombre_ataque, NULL);
  printf("\n%s : \n", estado->pokemon_j1_2);
  con_cada_ataque(pokemon2, imprimir_nombre_ataque, NULL);
  printf("\n%s : \n", estado->pokemon_j1_3);
  con_cada_ataque(pokemon3, imprimir_nombre_ataque, NULL);

  return true;
}

int main(int argc, char *argv[]) {
  estado_t *estado = calloc(1, sizeof(estado_t));
  if (estado == NULL) {
    printf("\nHubo un error al inicializar el juego, intentelo de nuevo.\n");
  }

  printf("\nBienvenido al mejor juego de todo Algo 2 Mendez.\n");
  printf("\nRecuerda que ante cualquier inconveniente puedes escribir 'a' para "
         "recibir ayuda.\n");
  estado->juego = juego_crear();
  estado->menu = menu_crear();

  menu_agregar_comando(estado->menu, 'j', "Comenzar el juego", comenzar_juego);
  menu_agregar_comando(estado->menu, 'a', "Ayuda", imprimir_ayuda);
  menu_agregar_comando(estado->menu, 't', "Tutorial de juego", tutorial_juego);
  menu_agregar_comando(estado->menu, 'q', "Salir del juego", salir_juego);
  menu_agregar_comando(estado->menu, 'c', "Cargar archivo", cargar_archivo);
  menu_agregar_comando(estado->menu, 'l', "Listar pokemones cargados",
                       listar_pokemones);
  menu_agregar_comando(estado->menu, 'e', "Elegir pokemones", elegir_pokemones);
  menu_agregar_comando(estado->menu, 'x', "Listar ataques", listar_ataques);

  while (estado->iniciado == false && estado->terminado == false) {
    printf("\n\nIngrese su proxima accion:\n");
    char proxima_accion[MAX_ACCION];
    obtener_accion(proxima_accion);
    MENU_RESULTADO resultado = menu_ejecutar_comando(
        estado->menu, proxima_accion[CERO], (void *)estado);
    if (resultado == MENU_OK) {
      if (estado->terminado == false) {
        printf("\nLa accion fue exitosa.\n");
      }
    } else if (resultado == MENU_ERROR) {
      printf("\nLa accion no se pudo completar con exito.\n");
    } else {
      printf("\nLa accion recibida no es un comando valido, escriba 'a' para "
             "recibir ayuda.\n");
    }
  }

  while (estado->terminado == false) {
    bool juego_acabado = juego_finalizado(estado->juego);
    if (juego_acabado == true) {
      int puntos_jugador = juego_obtener_puntaje(estado->juego, JUGADOR1);
      int puntos_adversario = juego_obtener_puntaje(estado->juego, JUGADOR2);
      if (puntos_jugador > puntos_adversario) {
        printf("\n\n\nFelicitaciones, ganaste este combate!\n");
        printf("Eres un gran entrenador pokemon, vuelve pronto para otro "
               "combate.\n\n");
      } else if (puntos_jugador == puntos_adversario) {
        printf(
            "\n\n\nEsto si que no me lo esperaba, que empate mas increible.\n");
        printf("Espero verte pronto para desempatar el resultado.\n\n");
      } else {
        printf("\n\n\nWow eso estuvo increible, tu rival lo hizo increible.\n");
        printf("No importa que hayas perdido, puedes volver a intentarlo "
               "siempre que quieras.\n\n");
      }
      estado->terminado = true;
    } else {
      printf("\n\nSigue el combate, si quieres seguir atacando ingresa "
             "'atacar'.\n");
      printf("Sino, puedes seguir utilizando las opciones del menu activas.\n");
      char proxima_accion[MAX_ACCION];
      obtener_accion(proxima_accion);
      if (strcmp(proxima_accion, ATACAR) == CERO) {
        printf("\n\nUn nuevo turno empieza, ingresa tu jugada:\n");
        printf("Con que pokemon quieres atacar?\n");
        char proximo_pokemon[MAX_ACCION];
        obtener_accion(proximo_pokemon);
        printf("Que ataque quieres realizar?\n");
        char proximo_ataque[MAX_ACCION];
        obtener_accion(proximo_ataque);
        jugada_t jugada_jugador;
        strcpy(jugada_jugador.pokemon, proximo_pokemon);
        strcpy(jugada_jugador.ataque, proximo_ataque);
        jugada_t jugada_adversario =
            adversario_proxima_jugada(estado->adversario);

        resultado_jugada_t resultado =
            juego_jugar_turno(estado->juego, jugada_jugador, jugada_adversario);
        if (resultado.jugador1 == ATAQUE_ERROR) {
          printf("\nUps, parece que el pokemon o el ataque efectuado no fue "
                 "valido.\n");
          printf("Recuerda escribir con el fomato correcto todo y no repetir "
                 "ataques.\n");
          printf("No pasa nada, intentalo de nuevo, tu puedes!");
        } else {
          adversario_informar_jugada(estado->adversario, jugada_adversario);
          printf("\nWow, vaya turno, estos son los puntajes hasta ahora.\n");
          int puntos_jugador = juego_obtener_puntaje(estado->juego, JUGADOR1);
          int puntos_adversario =
              juego_obtener_puntaje(estado->juego, JUGADOR2);
          printf("Tu puntaje es de: %i \n", puntos_jugador);
          printf("El puntaje del adversario es: %i \n", puntos_adversario);
          if (puntos_jugador < puntos_adversario) {
            printf("\nOh oh, vas perdiendo, pero no te preocupes, todavia "
                   "falta!\n");
          } else if (puntos_jugador == puntos_adversario) {
            printf("\nVaya combate, esto esta muy parejo!\n");
          } else {
            printf("\nVamos, vas ganando, sigue asi!\n");
          }
        }
      } else {
        MENU_RESULTADO resultado = menu_ejecutar_comando(
            estado->menu, proxima_accion[CERO], (void *)estado);
        if (resultado == MENU_OK) {
          if (estado->terminado == false) {
            printf("\nLa accion fue exitosa.\n");
          }
        } else if (resultado == MENU_ERROR) {
          printf("\nLa accion no se pudo completar con exito.\n");
        } else {
          printf("\nLa accion recibida no es un comando valido, escriba 'a' "
                 "para recibir ayuda.\n");
        }
      }
    }
  }

  adversario_destruir(estado->adversario);
  menu_destruir(estado->menu);
  juego_destruir(estado->juego);
  free(estado);
}