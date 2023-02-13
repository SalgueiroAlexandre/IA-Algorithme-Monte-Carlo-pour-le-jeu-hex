#include "Guezmer.hh"
#include <iostream>
#include <fstream>
#include <cmath> 

Guezmer::Guezmer(std::string nom, bool joueur, std::string nomFichier)
    :Joueur(nom,joueur)
{
    // remplie le vecteur moves avec les coups joués dans le fichier nomFichier
    std::ifstream readFile(nomFichier);
    if (!readFile.is_open()) {
        std::cout << "Impossible d'ouvrir le fichier." << std::endl;
    }
    // Lire les coups du fichier
    for(std::string line; getline(readFile, line);) {
        moves.push_back(line);
    }
}



void Guezmer::recherche_coup(Jeu j, couple &coup)
{
    int taille = j.coups_possibles().size();
    int num = rand()%(taille - 1);
     coup.first=j.coups_possibles()[num].first;
     coup.second=j.coups_possibles()[num].second;

    
}

float Guezmer::qubc(float score, int nbPartiePere, int nbPartieFils) {
    return round(1000 * (score + sqrt(2 * (log(static_cast<float>(nbPartiePere))/static_cast<float>(nbPartieFils)))))/1000;
}



