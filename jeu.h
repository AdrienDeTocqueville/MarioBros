#pragma once

#include "Joueur.h"
#include "Input.h"

#include "NEAT/Network.h"

void jouer(unsigned world, std::string level);
float fitnessEvaluation(Network& net, bool& _stop);

vector<float> getInputs(sf::Vector2f pos, const Niveau& niveau, bool addBias = true);
