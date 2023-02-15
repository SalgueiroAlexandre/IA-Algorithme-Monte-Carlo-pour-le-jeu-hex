#include "Guezmer.hh"
#include <iostream>
#include <fstream>
#include <cmath> 
#include <sstream>


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

    for (const auto& elem : moves) {
        std::istringstream ss(elem);
        std::string id;
        float score;
        int nbPartie;
        std::getline(ss, id, ',');  
        ss >> score; 
        ss.ignore(); 
        ss >> nbPartie; 
        movesStruct.push_back({id, score, nbPartie});
    }
}

// Initialisation de la variable statique
std::string Guezmer::etatPartie = "";

void Guezmer::majEtatPartie(couple coup,int tour) {
    tour == 1 ? etatPartie += std::to_string(coup.first) + std::to_string(coup.second) : etatPartie += "." + std::to_string(coup.first) + std::to_string(coup.second);
    std::cout << "etatPartie : " << etatPartie << std::endl;
}

bool Guezmer::coupEstConnu(couple coup) const {
    for (const auto& elem : movesStruct) {
        if (elem.id == etatPartie + std::to_string(coup.first) + std::to_string(coup.second)) {
            return true;
        }
    }
    return false;
}

void Guezmer::recherche_coup(Jeu j, couple &coup)
{
    bool toutLesCoupsSontConnus = true;
    int taille = j.coups_possibles().size();
    //afficher les coups possibles
    std::cout << "coups possibles" << std::endl;
    for (int i = 0; i < taille; i++) {
        std::cout << j.coups_possibles()[i].first << j.coups_possibles()[i].second << " ; ";
    }
    std::cout << std::endl;
    // boucle sur tous les coups possibles
    for (int i = 0; i < taille; i++) {
        // si le coup n'est pas connu par montecarlo on le joue
        if (!coupEstConnu(j.coups_possibles()[i])) {
            toutLesCoupsSontConnus = false;
            std::cout << "coup non connu : " << j.coups_possibles()[i].first << j.coups_possibles()[i].second << std::endl;
            coup.first=j.coups_possibles()[i].first;
            coup.second=j.coups_possibles()[i].second;
            return;
        }
    }
    // si tous les coups sont connus on utilise le qubc pour connaitre le meilleur coup
    if (toutLesCoupsSontConnus) {
        float max = 0;
        for (int i = 0; i < taille; i++) {
            for (const auto& elem : movesStruct) {
                if (elem.id == etatPartie + std::to_string(j.coups_possibles()[i].first) + std::to_string(j.coups_possibles()[i].second)) {
                    // si vous voulez regarder les qubc de chaque coup
                    //std::cout << "qubc : " << qubc(elem.score, elem.nbPartie, taille) << " pour le coup : " << j.coups_possibles()[i].first << j.coups_possibles()[i].second << std::endl;
                    if (qubc(elem.score, elem.nbPartie, taille) > max) {
                        max = qubc(elem.score, elem.nbPartie, taille);
                        // maj a jour du meilleur coup
                        coup.first=j.coups_possibles()[i].first;
                        coup.second=j.coups_possibles()[i].second;
                    }
                }
            }
        }
    }
}

float Guezmer::qubc(float score, int nbPartiePere, int nbPartieFils) {
    return round(1000 * (score + sqrt(2 * (log(static_cast<float>(nbPartiePere))/static_cast<float>(nbPartieFils)))))/1000;
}



