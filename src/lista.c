#include "lista.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define PRIMERA_POSICION 0
#define NO_HAY_NODOS 0
#define NO_APLICADO_A_ELEMENTOS 0

typedef struct nodo {
  void *elemento;
  struct nodo *siguiente;
} nodo_t;

struct lista {
  nodo_t *nodo_inicio;
  nodo_t *nodo_final;
};

struct lista_iterador {
  lista_t *lista;
  nodo_t *nodo_actual;
};

// PRE: La lista no debe ser nula.
// POST: Devuelve un nodo auxiliar en la ultima posicion.
void buscar_final(nodo_t **nodo_anterior, nodo_t **nodo_actual, lista_t *lista);

// PRE: La lista y la posicion no deben ser nulas.
// POST: Devuelve un nodo auxiliar en la posicion solicitada.
void buscar_posicion(nodo_t **nodo_anterior, nodo_t **nodo_actual,
                     size_t posicion, lista_t *lista);

void buscar_final(nodo_t **nodo_anterior, nodo_t **nodo_actual,
                  lista_t *lista) {
  while ((*nodo_actual)->siguiente != NULL) {
    *nodo_anterior = *nodo_actual;
    *nodo_actual = (*nodo_actual)->siguiente;
  }
}

void buscar_posicion(nodo_t **nodo_anterior, nodo_t **nodo_actual,
                     size_t posicion, lista_t *lista) {
  size_t i = 0;
  while (i < posicion && *nodo_actual != NULL) {
    *nodo_anterior = *nodo_actual;
    *nodo_actual = (*nodo_actual)->siguiente;
    i++;
  }
}

lista_t *lista_crear() {
  struct lista *nueva_lista = (struct lista *)calloc(1, sizeof(struct lista));
  if (nueva_lista == NULL) {
    return NULL;
  }
  return nueva_lista;
}

lista_t *lista_insertar(lista_t *lista, void *elemento) {
  if (lista == NULL) {
    return NULL;
  }

  nodo_t *nuevo_nodo = (nodo_t *)calloc(1, sizeof(nodo_t));
  if (nuevo_nodo == NULL) {
    return NULL;
  }

  nuevo_nodo->elemento = elemento;

  if (lista->nodo_inicio == NULL) {
    lista->nodo_inicio = nuevo_nodo;
    lista->nodo_final = nuevo_nodo;
  } else {
    lista->nodo_final->siguiente = nuevo_nodo;
    lista->nodo_final = nuevo_nodo;
  }

  return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
                                    size_t posicion) {
  if (lista == NULL) {
    return NULL;
  }

  nodo_t *nuevo_nodo = (nodo_t *)calloc(1, sizeof(nodo_t));
  if (nuevo_nodo == NULL) {
    return NULL;
  }
  nuevo_nodo->elemento = elemento;

  if (posicion == PRIMERA_POSICION || lista->nodo_inicio == NULL) {
    nuevo_nodo->siguiente = lista->nodo_inicio;
    lista->nodo_inicio = nuevo_nodo;
    if (lista->nodo_final == NULL) {
      lista->nodo_final = nuevo_nodo;
    }
  } else {
    nodo_t *nodo_anterior = NULL;
    nodo_t *nodo_actual = lista->nodo_inicio;
    buscar_posicion(&nodo_anterior, &nodo_actual, posicion, lista);

    if (nodo_actual == NULL) {
      free(nuevo_nodo);
      lista_insertar(lista, elemento);
    } else {
      nuevo_nodo->siguiente = nodo_actual;
      nodo_anterior->siguiente = nuevo_nodo;
    }
  }
  return lista;
}

void *lista_quitar(lista_t *lista) {
  if (lista == NULL) {
    return NULL;
  }
  if (lista->nodo_inicio == NULL) {
    return NULL;
  }

  void *elemento_removido = lista->nodo_final->elemento;

  if (lista->nodo_inicio == lista->nodo_final) {
    nodo_t *nodo_eliminado = lista->nodo_inicio;
    free(nodo_eliminado);
    lista->nodo_inicio = NULL;
    lista->nodo_final = NULL;
    return elemento_removido;
  }

  nodo_t *nodo_anterior = NULL;
  nodo_t *nodo_actual = lista->nodo_inicio;
  buscar_final(&nodo_anterior, &nodo_actual, lista);

  lista->nodo_final = nodo_anterior;
  lista->nodo_final->siguiente = NULL;
  free(nodo_actual);
  return elemento_removido;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion) {
  if (lista == NULL) {
    return NULL;
  }
  if (lista->nodo_inicio == NULL) {
    return NULL;
  }

  nodo_t *nodo_anterior = NULL;
  nodo_t *nodo_actual = lista->nodo_inicio;
  buscar_posicion(&nodo_anterior, &nodo_actual, posicion, lista);

  if (nodo_actual == NULL) {
    return lista_quitar(lista);
  }

  void *elemento_removido = nodo_actual->elemento;
  if (nodo_anterior != NULL) {
    nodo_anterior->siguiente = nodo_actual->siguiente;
  } else {
    lista->nodo_inicio = nodo_actual->siguiente;
  }

  if (nodo_actual == lista->nodo_final) {
    lista->nodo_final = nodo_anterior;
  }

  free(nodo_actual);
  return elemento_removido;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion) {
  if (lista == NULL) {
    return NULL;
  }

  nodo_t *nodo_actual = lista->nodo_inicio;
  size_t indice_actual = 0;

  while (nodo_actual != NULL && indice_actual < posicion) {
    nodo_actual = nodo_actual->siguiente;
    indice_actual++;
  }

  if (nodo_actual != NULL && indice_actual == posicion) {
    return nodo_actual->elemento;
  }

  return NULL;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
                            void *contexto) {
  if (lista == NULL) {
    return NULL;
  }
  if (comparador == NULL) {
    return NULL;
  }

  nodo_t *nodo_actual = lista->nodo_inicio;

  while (nodo_actual != NULL) {
    if (comparador(nodo_actual->elemento, contexto) == 0) {
      return nodo_actual->elemento;
    }
    nodo_actual = nodo_actual->siguiente;
  }

  return NULL;
}

void *lista_primero(lista_t *lista) {
  if (lista == NULL) {
    return NULL;
  }
  if (lista->nodo_inicio == NULL) {
    return NULL;
  }

  return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista) {
  if (lista == NULL) {
    return NULL;
  }
  if (lista->nodo_final == NULL) {
    return NULL;
  }

  return lista->nodo_final->elemento;
}

bool lista_vacia(lista_t *lista) {
  if (lista == NULL) {
    return true;
  }
  if (lista->nodo_inicio == NULL) {
    return true;
  }
  return false;
}

size_t lista_tamanio(lista_t *lista) {
  if (lista == NULL) {
    return NO_HAY_NODOS;
  }

  size_t contador = 0;
  nodo_t *nodo_actual = lista->nodo_inicio;

  while (nodo_actual != NULL) {
    contador++;
    nodo_actual = nodo_actual->siguiente;
  }

  return contador;
}

void lista_destruir(lista_t *lista) {
  if (lista == NULL) {
    return;
  }

  nodo_t *nodo_actual = lista->nodo_inicio;
  nodo_t *nodo_siguiente = NULL;

  while (nodo_actual != NULL) {
    nodo_siguiente = nodo_actual->siguiente;
    free(nodo_actual);
    nodo_actual = nodo_siguiente;
  }

  free(lista);
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *)) {
  if (lista == NULL) {
    return;
  }

  nodo_t *nodo_actual = lista->nodo_inicio;
  nodo_t *nodo_siguiente = NULL;

  while (nodo_actual != NULL) {
    nodo_siguiente = nodo_actual->siguiente;
    if (funcion != NULL) {
      funcion(nodo_actual->elemento);
    }
    free(nodo_actual);
    nodo_actual = nodo_siguiente;
  }

  free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista) {
  if (lista == NULL) {
    return NULL;
  }

  lista_iterador_t *iterador = malloc(sizeof(lista_iterador_t));
  if (iterador == NULL) {
    return NULL;
  }

  iterador->lista = lista;
  iterador->nodo_actual = lista->nodo_inicio;

  return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador) {
  if (iterador == NULL) {
    return false;
  }
  if (iterador->nodo_actual == NULL) {
    return false;
  }

  return true;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador) {
  if (iterador == NULL) {
    return false;
  }
  if (iterador->nodo_actual == NULL) {
    return false;
  }

  iterador->nodo_actual = iterador->nodo_actual->siguiente;

  if (iterador->nodo_actual != NULL) {
    return true;
  }

  return false;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador) {
  if (iterador == NULL) {
    return NULL;
  }
  if (iterador->nodo_actual == NULL) {
    return NULL;
  }

  return iterador->nodo_actual->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador) {
  if (iterador != NULL) {
    free(iterador);
  }
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
                               void *contexto) {
  if (lista == NULL) {
    return NO_APLICADO_A_ELEMENTOS;
  }
  if (funcion == NULL) {
    return NO_APLICADO_A_ELEMENTOS;
  }

  size_t elementos_iterados = 0;
  nodo_t *nodo_actual = lista->nodo_inicio;
  bool seguir_iterando = true;

  while (nodo_actual != NULL && seguir_iterando == true) {
    seguir_iterando = funcion(nodo_actual->elemento, contexto);
    elementos_iterados++;
    nodo_actual = nodo_actual->siguiente;
  }

  return elementos_iterados;
}