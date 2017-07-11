#ifndef GOOMBA_H
#define GOOMBA_H

#include "Personnage.h"

class Goomba : public Personnage
{
    public:
        Goomba(unsigned _x, unsigned _y, bool _orientation);
        virtual ~Goomba();

        virtual void update();

    protected:

    private:
};

#endif // GOOMBA_H
