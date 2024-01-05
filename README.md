# Pokemon Card Game

- To compile and run the tests in your own console:

```bash
bash .tests/test.sh
```

- To play:

```bash
make
```

---

## Introduction

This project is my second game, the game is about a "pokemon card game", you will battle against the computer in a turn battle with differents pokemons depending the file you load to play.

In particular, what was sought with the development of this game was to practice the operating logic of a rival controlled by the computer automatically and implement good interaction with the user.

What the end user is given is a "main.c", with all the corresponding ".h" and ".o" for the "libraries" they use, with the game deployed and ready to play.

Although several of these "libraries" can be reused for other implementations that the user encounters, for example, the "ADT MENU".

Additionally, in the repository there is my own test library, with which in the example file I test the operation of the game.

I use this library with the idea of ​​visually making it easier for anyone who wants to see how it works to read the tests and whether they work correctly or incorrectly.

## Functioning

###  Explanation of "Implementation and design"

First of all, this was a project for the university, so the code is written in spanish.

With that clarified, we move on to the explanation.

Several "libraries" were used to implement the game:

- "adversario": Contains the necessary implementation for the logic of the adversary controlled by the machine, this is handled with a turn-based selection system in a "semi-automatic" way, since, although it will perform the same actions each game, both the Pokémon available to play, as well as the order of choice, will make each confrontation a unique and totally different encounter from the others, without much implementation complexity.
  
- "juego": It contains all the logic of the game itself, it is responsible for verifying the state of the game, who is winning or losing, the number of turns, etc.
  This library is perhaps the least reusable of all, because it has a very marked sense of operation due to the game implemented.

- "lista": For part of the game I reused my own list implementation, if you want more information you can find it in "ADT_SQL".

- "menu": To handle all interaction with the user in the most user-friendly way, I implemented an "ADT MENU" that is responsible for having all the commands that the player has access to.
  This "ADT" is completely reusable, and can be very useful for many areas, and not just for creating games.
  This menu allows you to save a description about the type of command, a representative character of the command and a characteristic function of the command, giving us a very wide range of possibilities when using it.

- "pokemon": This library was one of the first jobs we did in the quarter, but mainly for the game we are interested in the fact that it allows us to "parse" the pokemon file that we are going to use, and it gives us many useful functions when managing the pokemons with those of us who are going to play.

- There are some extra "libraries" like "type" or "attack", but they are simply definitions of "enums" or "structs" that we use for the game implementation.

### Implemented structures

Next I will explain the structures that I used in "juego.c" and "adversario.c", along with the justification and explanation of each of them.

### JUEGO

```c

typedef struct equipo {
	pokemon_t *pokemon; // Save the player's pokemon.
	char nombre_pokemon[MAX_NOMBRE]; // It makes it easier to access when we only need the pokemon's name.
	char ataque_usado_1[MAX_NOMBRE]; // Save used attacks.
	char ataque_usado_2[MAX_NOMBRE]; // Save used attacks.
	char ataque_usado_3[MAX_NOMBRE]; // Save used attacks.
}equipo_t;

typedef struct jugador {
	equipo_t *equipo[CANTIDAD_POKEMONES]; // Structure that stores all the player's Pokémon.
	int puntaje; // Player Score.
}jugador_t;

struct juego {
	int turnos_jugados; // Number of turns played.
	jugador_t *j1; // Player 1 data.
	jugador_t *j2; // Player 2 data.
	informacion_pokemon_t *info_pokemon; // Used to later list the pokemon.
	lista_t *lista_pokemon; // List to access the pokemon.
};

```

### ADVERSARIO

```c

typedef struct ataques{
    char nombre[MAX_NOMBRE]; // Attack name.
} ataques_t;

typedef struct pokemones{
	char nombre[MAX_NOMBRE]; // Pokemon name.
    ataques_t *ataques[MAX_ATAQUES]; // All pokemon attacks.
} pokemones_t;

struct adversario {
	lista_t *lista; // List to access the pokemon.
	pokemones_t *pokemones[MAX_POKEMONES]; // Structure to store the opponent's pokemon.
	int turnos; // To know what move you have to make for each turn.
};

```

### MENU

```c

typedef struct comando {
	char simbolo_caracteristico; // Symbol that identifies each menu option/command.
	char descripcion[MAX_DESCRIPCION]; // Specific description of each menu option/command.
	bool (*funcion)(void *); // Characteristic function of each menu option/command.
} comando_t;

struct menu {
	comando_t comandos[CANTIDAD_COMANDOS]; // Array that saves all menu options.
};

```

---

## Testing

For test the code, i implemented my own tests.

In the repository actions you can see the status of the executed tests.

Also add the analysis with valgrind to see if you have memory leaks, since the program uses dynamic memory.

---

## ADT MENU

Finally, in case you want to understand/reuse the "ADT MENU", I leave a brief explanation of the computational complexity and functions.

menu_t *menu_crear() es O(1): Reserve the necessary memory for the menu.

void menu_agregar_comando(menu_t *menu, char simbolo_comando, char *descripcion, bool (*funcion)(void *)) es O(n): This function adds an option/command to the "ADT MENU", assigning it its characteristic description, symbol and function.

void menu_listar_comandos(menu_t *menu) es O(n): This function lists all the options/commands available in our "ADT MENU".

MENU_RESULTADO menu_ejecutar_comando(menu_t *menu, char simbolo_comando, void *contexto) es O(n): This function looks for the option/command within the array, and if it finds it, it executes its associated function with the "void *" received by parameter as "context".

void menu_destruir(menu_t *menu) es O(1): Release the memory reserved for the menu.

---

## Rules

- The game requires 2 players.
- At the beginning of the game, players are presented with a list of available Pokémon.
- Each player selects 3 pokemon from those available.
  - The 3 pokemon must be different, pokemon cannot be repeated.
  - Both players make their choice without knowing the other player's choice.
- Once the Pokémon have been selected by both users, the choices are reported.
  - The first 2 pokemon selected by the user are their own pokemon while the third pokemon must be assigned as the other player's third pokemon.
  - For example, player 1 selects A, B, and C; Player 2 selects D, E, and F. Player 1's final team is A, B, and F, while Player 2's team is D, E, and C.
  - Although each player must select 3 different pokemon, it is possible that duplicate pokemon remain on the final team (since the third pokemon is secretly chosen by the other player).
- The game takes place in rounds. Since each player has 3 pokemon and each pokemon has 3 attacks, the number of rounds of the game is always 9.
  - In each round each player decides a pokemon and an attack for that pokemon.
  - The selected pokemon face each other.
  - Each player is assigned a score that is calculated based on the power of the attack and the type of the attack and the opposing Pokémon.
  - Once a Pokémon's attack has been used, it can no longer be used during the game.
- At the end of the game, the player with the most points wins.

### Score

The score is calculated based on the type of the attack and the type of the pokemon receiving the attack. The type of the attacking pokemon does not influence the score. The possible combinations and expected logic are documented in `juego_jugar_turno`.

### Required text format

As explained before, the game can be played with any type of "valid" pokemon file, but to create our own file, we must understand what we take as valid.
This is very easy, the format is the same that we can find in the 2 files in the "examples" folder.
This format is based on separating with ";" the terms.
And the structure is based on:
pokemon;type
attack1;type;power
attack2;type;power
attack3;type;power
The only rules to follow within this structure are:
- Do not exceed the maximum number of characters in names.
- The power of attacks is always a positive number.
- The type of attacks/pokemons must be one of the possible ones (in order to simplify the type table, not all of the types from the original saga are included).

---

## How to play?

Playing is very easy, simply download the "repository", enter the folder that contains the files, run the "make" command in the console and then just type the letter "a" to enter the help menu.
The implemented menu will be responsible for teaching you everything you need to play.
Enjoy them!
