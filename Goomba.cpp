#include "Goomba.h"
#include "Niveau.h"

Goomba::Goomba(unsigned _x, unsigned _y, bool _orientation):
    Personnage(_x, _y, GOOMBA, _orientation)
{ }

Goomba::~Goomba()
{ }

void Goomba::update()
{
    if (dead)
    {
        tileSet.setAnimation(2);
        return;
    }

    float speed = 0.5f;

    /// Mort ?
    if (position.y/TAILLE_BLOC > niveau->height)
    {
        dead = true;
        return;
    }

    /// Horizontal
    if (orientation == LEFT)
    {
        position.x -= speed;
        tileSet.setAnimation(0);
    }
    else if (orientation == RIGHT)
    {
        position.x += speed;
        tileSet.setAnimation(1);
    }

    /// Collisions
    std::vector<Hit> hits = niveau->getHits(getAABB());

    for (Hit& hit: hits)
    {
        if (hit.sides && hit.bloc->solid)
        {
            position.x = round(position.x/TAILLE_BLOC) * TAILLE_BLOC;
            orientation = !orientation;
        }
    }

    /// Vertical
    position.y += 2.0f;

    /// Collisions
    hits = niveau->getHits(getAABB());

    for (Hit& hit: hits)
    {
        if (!hit.sides && hit.bloc->solid)
            position.y = round(position.y/TAILLE_BLOC) * TAILLE_BLOC;
    }
}
