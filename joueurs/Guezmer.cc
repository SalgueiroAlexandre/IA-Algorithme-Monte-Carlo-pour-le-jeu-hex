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
        // si etatPartie est inferieur ou egal a 1
        if(etatPartie.size() <= 1){
            // on verifie que le coup est dans le vecteur movesStruct
            if (elem.id == std::to_string(coup.first) + std::to_string(coup.second)) {
                return true;
            }
        }else{
            // on verifie que le coup est dans le vecteur movesStruct
            if (elem.id == etatPartie + "." + std::to_string(coup.first) + std::to_string(coup.second)) {
                return true;
            }
        }
    }
    return false;
}



void Guezmer::recherche_coup(Jeu j, couple &coup)
{
    bool toutLesCoupsSontConnus = true;
    int taille = j.coups_possibles().size();
    std::cout << std::endl;
    std::vector<couple> coupsInconnus;
    // boucle sur tous les coups possibles
    for (int i = 0; i < taille; i++) {
        // si le coup n'est pas connu par montecarlo on le joue
        if (!coupEstConnu(j.coups_possibles()[i])) {
            toutLesCoupsSontConnus = false;
            coupsInconnus.push_back(j.coups_possibles()[i]);
        }
    }

    if(!toutLesCoupsSontConnus){
        // on joue un coup inconnu aleatoirement
        int random = rand() % (coupsInconnus.size());
        coup.first = coupsInconnus[random].first;
        coup.second = coupsInconnus[random].second;
        std::cout<< "coup inconnu  aleatoirement jouer : " << coup.first << coup.second << std::endl;
        return;
    }

    // si tous les coups sont connus on utilise le qubc pour connaitre le meilleur coup
    if (toutLesCoupsSontConnus) {
        float max = 0;
        int nbPartiePere = 0;
        for (int i = 0; i < taille; i++) {
            for (const auto& elem : movesStruct) {
                // on prend en compte que l'etatPartie est l'idPere 
                //pour rceuperer le nombre de partie du pere
                if(elem.id == etatPartie){
                    nbPartiePere = elem.nbPartie;
                }
                std::string comparateur;
                // si etatPartie est inferieur ou egal a 1
                if(etatPartie.size() <= 1){
                    // on verifie que le coup est dans le vecteur movesStruct
                    comparateur = std::to_string(j.coups_possibles()[i].first) + std::to_string(j.coups_possibles()[i].second);
                }else{
                    // on verifie que le coup est dans le vecteur movesStruct
                    comparateur = etatPartie + "." + std::to_string(j.coups_possibles()[i].first) + std::to_string(j.coups_possibles()[i].second);
                }
                if (elem.id == comparateur) {
                    // si vous voulez regarder les qubc de chaque coup
                    //std::cout << "qubc : " << qubc(elem.score, elem.nbPartie, taille) << " pour le coup : " << j.coups_possibles()[i].first << j.coups_possibles()[i].second << std::endl;
                    std::cout<<"indice qubc : "<<elem.id <<" "<<elem.score<<" "<<nbPartiePere<<" "<<elem.nbPartie<<std::endl;
                    std::cout<<"qubc : "<<qubc(elem.score,nbPartiePere,elem.nbPartie)<<std::endl;
                    // si le joueur est premier inversé le score
                    int score = elem.score;
                    if (etatPartie.size() %2==1)
                    {
                        std::cout<<"inversion du score"<<std::endl;
                        score = -elem.score;
                    }

                    if (qubc(score,nbPartiePere,elem.nbPartie) > max) {
                        max = qubc(elem.score,nbPartiePere,elem.nbPartie);
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
    if(nbPartiePere == 0){
        nbPartiePere = nbPartieFils;
    }
    return round(1000 * ((score / nbPartieFils) + sqrt(2 * (log(static_cast<float>(nbPartiePere))/static_cast<float>(nbPartieFils)))))/1000;
}



