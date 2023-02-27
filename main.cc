
#include <iostream>
#include "ArbitreENT.hh"
#include "jeu.hh"




int main()
{
    //initialise la graine du générateur aléatoire
    std::srand(std::time(nullptr));

    //création de l'Arbitre (joueur jouant en 1er la premiere partie, joueur jouant en 2eme celle-ci , nombre de parties)
    ArbitreENT a (player::M_1,player::M_2,50000,"../coups.csv","../coups.csv");
     //commence le challenge
    int r= a.challenge();

   return r;

}
