#ifndef DEF_CONSTANTES
#define DEF_CONSTANTES

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>

#define TAILLE_BLOC 16

#define NB_BLOC_LARGEUR 16
#define NB_BLOC_HAUTEUR 12

class Bloc;
using Map = std::vector<std::vector<Bloc>>;

enum class BLOC {CIEL = 46, SOL = 95, MYSTERE = 63, BRIQUE = 50, BRIQUEVIDE = 51, PIECE = 79, CUBE = 53, GOOMBA = 71, KOOPA = 75, MARIO = 77, VIDE = 86};
///ASCII:               .         _             ?            2                3           0          5            G           K           M          V

enum {LEFT = 0, RIGHT = 1, DOWN = 2, UP = 3};

std::string toString(int _number);
std::string toStringF(float _number);

BLOC getBLOC(char _type);
char getChar(BLOC _type);

#endif
