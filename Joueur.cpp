#include "Joueur.h"
#include "Niveau.h"
#include "Input.h"

#include <cmath>

float g(float x)
{
    return 144.0f * x - 72.0f * (x*x);
}

float f(int x)
{
    if (x < 17)
        return 0.0f;

    float X = x / 650.0f;
    float dt = 17.0f / 650.0f;

    return g(X) - g(X-dt);

    return 2.0f;

    return 2.0f * x - 10.0f;
}

/// Constructeur
Joueur::Joueur(unsigned _x, unsigned _y, bool _orientation):
    Personnage(_x, _y, Type::MARIO, _orientation),
    enSaut(0), canJump(true), jumpTime(0.0f),
    state(IDLE)
{ blocSize = 2; }

/// Destructeur

Joueur::~Joueur()
{ }

/// Methodes
void Joueur::update()
{
    float speed = 1.5f;
    if (Input::getKeyDown(sf::Keyboard::LShift))
        speed *= 2.0f;


    /// Mort
    if (position.y/TAILLE_BLOC > niveau->height)
        dead = true;

    /// Vertical
    jumpTime += 17;

    if (Input::getKeyDown(sf::Keyboard::Up))
    {
        if (canJump)
        {
            jumpTime = 0;
            enSaut = 1;
            canJump = false;
        }
        else if (jumpTime > 650)
            enSaut = 2;
    }
    else if (jumpTime > 250)
        enSaut = 2;

    if (enSaut == 1)
    {
        position.y -= f(jumpTime);
        state = JUMP;
    }
    else if (enSaut == 2)
    {
        position.y += 2.0f;
        state = FALL;
    }

    /// Collisions
    std::vector<Hit> hits = niveau->getHits(getAABB());

    for (Hit& hit: hits)
    {
        if (!hit.sides)
        {
            if (hit.bloc->solid)
            {
                position.y = round(position.y/TAILLE_BLOC) * TAILLE_BLOC;

                enSaut = 2*hit.top;
                if (!hit.top)
                    canJump = true;
            }

            if (hit.top)
            {
                if (hit.bloc->type == BLOC::MYSTERE)
                    hit.bloc->setType(BLOC::VIDE);

                else if (hit.bloc->type == BLOC::BRIQUE)
                    hit.bloc->setType(BLOC::CIEL);

                else if (hit.bloc->type == BLOC::BRIQUEVIDE)
                    hit.bloc->setType(BLOC::VIDE);
            }
        }
    }

    /// Horizontal
    if (Input::getKeyDown(sf::Keyboard::Left))
    {
        position.x -= speed;
        orientation = LEFT;
        state = WALK;
    }
    else if (Input::getKeyDown(sf::Keyboard::Right))
    {
        position.x += speed;
        orientation = RIGHT;
        state = WALK;
    }
    else if (!enSaut && canJump)
    {
        tileSet.setAnimation(orientation);
        state = IDLE;
    }

    /// Collisions
    hits = niveau->getHits(getAABB());

    for (Hit& hit: hits)
    {
        if (hit.bloc->type == BLOC::PIECE)
        {
            hit.bloc->setType(BLOC::CIEL);
            continue;
        }

        if (hit.sides && hit.bloc->solid)
            position.x = round(position.x/TAILLE_BLOC) * TAILLE_BLOC;
    }


    /// Personnages
    sf::FloatRect aabb(getAABB()), intersection;

    for (Personnage* p: niveau->personnages)
    {
        if (p->dead)
            continue;

        bool col = true, topcol = false;

        float xRight = p->position.x - (position.x + TAILLE_BLOC);
        float xLeft = position.x - (p->position.x + TAILLE_BLOC);
        float yTop = position.y - (p->position.y + p->blocSize*TAILLE_BLOC);
        float yBot = p->position.y - (position.y + blocSize*TAILLE_BLOC);

        if (xLeft > 0.0f || xRight > 0.0f)
            col = false;

        if (yTop > 0.0f || yBot > 0.0f)
            col = false;

        if (yBot > -TAILLE_BLOC/2)
            topcol = true;

        if (col)
        {
            if (topcol)
            {
                p->takeDamage();

                jumpTime = 0;
                enSaut = 1;
                canJump = false;
            }
            else
            {
                takeDamage();
            }
        }

//        if (aabb.intersects(p->getAABB(), intersection))
//        {
//            if (intersection.width < intersection.height || // Sides
//                intersection.top == aabb.top)   // Top
//            {
//                takeDamage();
//            }
//            else // Bottom
//            {
//                p->takeDamage();
//
//                jumpTime = 0;
//                enSaut = 1;
//                canJump = false;
//            }
//        }
    }

    /// Move map
    if (position.x - niveau->cameraOffset.x < 4*TAILLE_BLOC)
        niveau->setXOffset(position.x - 4*TAILLE_BLOC);

    else if (position.x - niveau->cameraOffset.x > (NB_BLOC_LARGEUR-7)*TAILLE_BLOC)
        niveau->setXOffset(position.x - (NB_BLOC_LARGEUR-7)*TAILLE_BLOC);

    if (position.y+2*TAILLE_BLOC - niveau->cameraOffset.y < 6*TAILLE_BLOC)
        niveau->setYOffset(position.y - 4*TAILLE_BLOC);

    else if (position.y+2*TAILLE_BLOC - niveau->cameraOffset.y > (NB_BLOC_HAUTEUR-2)*TAILLE_BLOC)
        niveau->setYOffset(position.y - (NB_BLOC_HAUTEUR-4)*TAILLE_BLOC);

    /// Set animation
    switch (state)
    {
        case IDLE:
            tileSet.setAnimation(0+orientation);
            break;

        case WALK:
            tileSet.setAnimation(2+orientation);
            break;

        case JUMP:
            tileSet.setAnimation(4+orientation);
            break;

        case FALL:
            tileSet.setAnimation(0+orientation); // TODO: create falling animation
            break;
    }
}
