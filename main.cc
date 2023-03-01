
#include <iostream>
#include "ArbitreENT.hh"
#include "jeu.hh"
#include "lecteur.hh"
#include <cmath> // Ajout nécessaire



int main()
{
    int nb_parties = 1000000;
    //initialise la graine du générateur aléatoire
    std::srand(std::time(nullptr));
    
    //création de l'Arbitre (joueur jouant en 1er la premiere partie, joueur jouant en 2eme celle-ci , nombre de parties)
    ArbitreENT a (player::M_1,player::M_2,nb_parties);
     //mesure du temps d'execution
    //mesure du temps d'execution
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    int r = a.challenge();
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    std::cout << "Temps d'execution : " << std::floor(elapsed_seconds.count() / 60) << " minutes "
            << elapsed_seconds.count() - std::floor(elapsed_seconds.count() / 60) * 60 << " secondes" << std::endl;



   return r;

}
