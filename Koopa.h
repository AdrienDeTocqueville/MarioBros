#ifndef KOOPA_H
#define KOOPA_H

#include "Personnage.h"

class Koopa : public Personnage
{
    public:
        Koopa(unsigned _x, unsigned _y, bool _orientation);
        virtual ~Koopa();

        virtual void update();

    protected:

    private:
};

#endif // KOOPA_H
