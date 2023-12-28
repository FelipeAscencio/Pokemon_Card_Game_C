#ifndef ATAQUE_H_
#define ATAQUE_H_

#include "tipo.h"

struct ataque {
  char nombre[20];
  enum TIPO tipo;
  unsigned int poder;
};

#endif // ATAQUE_H_
