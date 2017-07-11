#ifndef DEF_JOUEUR
#define DEF_JOUEUR

#include "Personnage.h"

class Joueur : public Personnage
{
/// -----   METHODES   -----
    public:
        Joueur(unsigned _x, unsigned _y, bool orientation = RIGHT);
        virtual ~Joueur();

        virtual void update();

//    private:
/// -----   ATTRIBUTS   -----
        bool enSaut, canJump;
        unsigned jumpTime;

        int state;
};

#endif
