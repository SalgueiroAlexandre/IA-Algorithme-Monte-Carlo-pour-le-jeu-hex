#include <iostream>
#include "ArbitreENT.hh"
#include "Arbitre.hh"
#include "./joueurs/Guezmer.hh"
#include "jeu.hh"



int main()
{
    int nb_parties = 10000;
    Guezmer::initMoves();
    std::srand(std::time(nullptr));
    Arbitre a (player::RAND2,player::M_1,nb_parties);
    // ArbitreENT a (player::RAND2,player::M_1,nb_parties); => arbitre utiliser pour l'entrainement du monte carlo
    int r = a.challenge();
    return r;
}
