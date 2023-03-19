#include "Guezmer.hh"
#include "../Lecteur.hh"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath> 
#include <sstream>
#include <unordered_map>


/**
 * @brief Constreur de la classe Guezmer qui initialise les variables etatPartie à vide pour stocké les coups qui sont joués durant la partie
 * et les coordonnées de départ de la stratégie si le coup n'est pas connu dans le csv
 *
*/
Guezmer::Guezmer(std::string nom, bool joueur)
    :Joueur(nom,joueur)
{
    if (joueur)
    {
        x = 5;
        y = 0;
    }else{
        x = 0;
        y = 5;
    }
    etatPartie = "";
}

// initialisation de la map
std::unordered_map<std::string, coupStruct> Guezmer::movesMap;
std::string Guezmer::etatPartie = "";

/**
 * @brief Fonction qui de récuperer les données de notre csv et les stocks dans une map
 * 
 */
void Guezmer::initMoves(){
    std::vector<std::string> moves;
    // creation du lecteur
    Lecteur l("../coups.csv");
    l.lire(moves);
    for (const auto& elem : moves) {
        std::istringstream ss(elem);
        std::string id;
        float score;
        int nbPartie;
        std::getline(ss, id, ',');  
        ss >> score; 
        ss.ignore(); 
        ss >> nbPartie; 
        movesMap[id] = {id, score, nbPartie};
    }
}

/**
 * @brief Fonction qui met à jour l'était de la partie en stockant les coups joués
 * 
 * @return std::string 
 */
void Guezmer::majEtatPartie(Jeu j) {
    auto grille = j.grille();
    // trouve le coup de la grille qui n'est pas dans etatPartie
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            if (grille[i][j] != 0) {
                std::string id = std::to_string(i) + std::to_string(j);
                if (etatPartie.find(id) == std::string::npos) {
                    if (etatPartie.size() == 0) {
                        etatPartie = id;
                    } else {
                        etatPartie += "." + id;
                    }
                    return;
                }
            }
        }
    }

}

/**
 * @brief Fonction qui permet de choisir le meilleur coup à jouer en fonction de la map
 *  
 */
std::vector<std::string> Guezmer::getMoves() {
    std::vector<std::string> moves;
    for (const auto& [id, elem] : movesMap) {
        moves.push_back(id + "," + std::to_string(elem.score) + "," + std::to_string(elem.nbPartie));
    }
    return moves;
}

/**
 * @brief Fonction qui permet d'effectuer le rollback en cas de victoire ou de défaite sur les coups qui ont était joué durant la partie
 * qui sont déjà présent dans le csv 
 *  
 */
void Guezmer::rollback(int result) {
    std::vector<std::string> outputArray;
    std::string currentSubString;
    for (char c : etatPartie) {
        if (c == '.') {
            outputArray.push_back(currentSubString);
            currentSubString += c;
        } else {
            currentSubString += c;
        }
    }
    etatPartie = "";
    outputArray.push_back(currentSubString);
    for(const auto & elem : outputArray) {
        std::string id = elem;
        auto it = movesMap.find(id);
        if (it != movesMap.end()) {
            it->second.nbPartie++;
            it->second.score += result;
            std::cout << "ROLLBACK sur " << it->first << " avec un score de " << it->second.score << " et un nombre de partie de " << it->second.nbPartie << std::endl;
        } else {
            movesMap[id] = {id, result, 1};
            std::cout << "AJOUT de " << id << " avec un score de " << result << " et un nombre de partie de 1" << std::endl;
            return;
        }
    }
}

/**
 * @brief Fonction qui permet de vérifier si le coup est connu dans le csv
 *  
 */
bool Guezmer::coupEstConnu(couple coup) const {
    std::string coup_id;
    if(etatPartie.size() <= 1) {
        coup_id = std::to_string(coup.first) + std::to_string(coup.second);
    } else {
        coup_id = etatPartie + "." + std::to_string(coup.first) + std::to_string(coup.second);
    }
    return movesMap.find(coup_id) != movesMap.end();
}

/**
 * @brief Fonction qui permet de récuperer le nombre e partie du père d'un coup
 *  @param id : id du coup pour lequelle on veut le nombre de partie du père
 */
int Guezmer::nbPartiePere(std::string id){
    if(etatPartie.size() <= 1) {
        // return la taille de la map
        return movesMap.size();
    }else{
        // return le nombre de partie du pere
        std::string pere = id.substr(0, id.find_last_of("."));
        return movesMap[pere].nbPartie;
    }
}

/**
 * @brief Fonction qui permet de choisir le coup si il n'est pas connu dans le csv avec une stratégie qui consiste à relier 2 bords le plus rapidement possible 
 * sans prendre réellement en compte les coups joués par l'adversaire
 */
void Guezmer::choisirCoupNonConnu(const Jeu & j, couple& coup){
    // si on est joueur 1 (de haut en bas)
    if(joueur()){
        if(j.case_libre(couple(y,x))){
            coup.second = x;
            coup.first = y;
            x = x;
            y = y + 1;
        }else if (j.case_libre(couple(y,x-1))){
            coup.second = x-1;
            coup.first = y;
            x = x-1;
            y = y+1;
        }else if(j.case_libre(couple(y-1,x+1))){
            coup.second = x+1;
            coup.first = y-1;
            x = x+1;
            y = y;
        }else{
           // recuperation du dernier coup joué par l'adversaire si aucune case n'est libre autour du dernier coup joué de notre part
            std::string derniercoups = "";
            derniercoups = etatPartie.substr(etatPartie.rfind(".") + 1);
            int y1 = std::stoi(derniercoups.substr(0, 1));
            int x1 = std::stoi(derniercoups.substr(1));
            //je recupere le coup joué par l'adversaire et je le block
            if(j.case_libre(couple(y1,x1+1))){
                coup.second = x1+1;
                coup.first = y1;  
            }else if(x1>0 && j.case_libre(couple(y1,x1-1))){
                coup.second = x1-1;
                coup.first = y1;
            }
        }
    }else{
        // si on est joueur 2 (de gauche à droite)
        if(j.case_libre(couple(y,x))){
            coup.second = x;
            coup.first = y;
            x = x + 1;
            y = y;
        }else if (j.case_libre(couple(y-1,x))){
            coup.second = x;
            coup.first = y-1;
            x = x+1;
            y = y-1;
        }else if(j.case_libre(couple(y+1,x))){
            coup.second = x;
            coup.first = y+1;
            x = x+1;
            y = y+1;
        }else{
            // recuperation du dernier coup joué par l'adversaire si aucune case n'est libre autour du dernier coup joué de notre part
            std::string derniercoups = "";
            derniercoups = etatPartie.substr(etatPartie.rfind(".") + 1);
            int y1 = std::stoi(derniercoups.substr(0, 1));
            int x1 = std::stoi(derniercoups.substr(1));
            //je recupere le coup joué par l'adversaire et je le block
            if(j.case_libre(couple(y1+1,x1))){
                coup.second = x1;
                coup.first = y1+1;  
            }else if(y1>0 && j.case_libre(couple(y1-1,x1))){
                coup.second = x1;
                coup.first = y1-1;
            }
        }
    }
}

/**
 * @brief Fonction qui permet de choisir le meilleur coup à jouer en fonction du jeu et renvoie le coup dans le couple
*/
void Guezmer::recherche_coup(Jeu j, couple& coup)
{
    // on récupère l'état de la partie
    majEtatPartie(j);
    // création de variable pour les futur test
    bool toutLesCoupsSontConnus = true;
    auto coupsPossibles = j.coups_possibles();
    int taille = coupsPossibles.size();
    int inconnu_count = 0;

    // boucle sur tous les coups possibles
    for (int i = 0; i < taille; i++) {
        if (!coupEstConnu(coupsPossibles[i])) {
            toutLesCoupsSontConnus = false;
            if (rand() % ++inconnu_count == 0) {
                // on choisit un coup au hasard parmis les coups non connus par défaut, si la fonction choisirCoupNonConnu ne trouve pas de coup à jouer on joue un coup au hasard
                coup.first = coupsPossibles[i].first;
                coup.second = coupsPossibles[i].second;
            }
        }
    }
    //si tous les coups sont connus on appelle choisirCoupConnus 
    if (!toutLesCoupsSontConnus) {
        choisirCoupNonConnu(j,coup);
        etatPartie == "" ? etatPartie = std::to_string(coup.first) + std::to_string(coup.second) : etatPartie = etatPartie + "." + std::to_string(coup.first) + std::to_string(coup.second);
        return;
    }
    //sinon on recherche le coups à jouer dans le csv en appliquant le calcul du QUBC
    float max = -10;
    for (int i = 0; i < taille; i++) {
        std::string comparateur;
        if (etatPartie.size() <= 1) {
            comparateur = std::to_string(coupsPossibles[i].first) + std::to_string(coupsPossibles[i].second);
        } else {
            comparateur = etatPartie + "." + std::to_string(coupsPossibles[i].first) + std::to_string(coupsPossibles[i].second);
        }
        auto it = movesMap.find(comparateur);
        if (it != movesMap.end()) {
            auto elem = it->second;
            int score = elem.score;
            float q = qubc(score, nbPartiePere(elem.id), elem.nbPartie);
            if (q > max) {
                max = q;
                coup.first = coupsPossibles[i].first;
                coup.second = coupsPossibles[i].second;
                // maj de l'etat de la partie
                etatPartie == "" ? etatPartie = std::to_string(coup.first) + std::to_string(coup.second) : etatPartie = etatPartie + "." + std::to_string(coup.first) + std::to_string(coup.second);
                std::cout << "coup joué connu : " << coup.first << coup.second << std::endl;
            }
        }
    }
}

/**
 * @brief fonction permettant de calculer le QUBC 
*/
float Guezmer::qubc(float score, int nbPartiePere, int nbPartieFils) {
    if(!joueur()){ // si je suis le joueur 2
        score = score * -1; // principe de negamax
    }
    // coeficient de découverte mis à 0 pour le challenge
    return (score / nbPartieFils) + sqrt(0 * log(nbPartiePere)/(nbPartieFils));
}
