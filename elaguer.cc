#include <iostream>
#include <sstream>
#include "ArbitreENT.hh"
#include "jeu.hh"
#include "lecteur.hh"
#include <cmath> // Ajout nécessaire
#include <unistd.h>



struct coupStructPre{
  std::string id;
  int score;
  int nbPartie;
  std::string pre;
};

std::string extractPrefix(const std::string& id) {
    std::size_t lastDotPos = id.find_last_of(".");
    if (lastDotPos != std::string::npos && lastDotPos > 0) {
        return id.substr(0, lastDotPos);
    }
    return "";
}


int elaguer()
{
std::vector<std::string> Bestmoves;
std::vector<std::string> moves;
Lecteur l1("../coups.csv");
l1.lire(moves);
std::unordered_map<std::string, coupStructPre> movesMap = {};
for (const auto& elem : moves) {
    std::istringstream ss(elem);
    std::string id;
    int score;
    int nbPartie;
    std::getline(ss, id, ',');  
    // Extraire le préfixe de l'id
    std::string id_prefix = extractPrefix(id);
    //std::cout << "ID : "<<id<< " Prefix : "<<id_prefix << std::endl;
    // Extraire le score et le nombre de parties
    ss >> score;
    ss.ignore();
    ss >> nbPartie;
    if(nbPartie>5)
        movesMap[id] = {id, score, nbPartie, id_prefix};
    else
        Bestmoves.push_back(id + "," + std::to_string(score) + "," + std::to_string(nbPartie));

}
std::cout<<"le fichier est chargé"<<std::endl;

std::vector<std::unordered_map<std::string, coupStructPre>> VectPre;
for (const auto& [id, elem] : movesMap) {
    // Extraire le préfixe de l'id
    std::string id_prefix = elem.pre;
    bool prefix_found = false;
    

    // Chercher si un élément avec ce préfixe existe déjà dans VectPre
    for (auto& pre_map : VectPre) {
    auto it = pre_map.begin();
    if (it != pre_map.end() && it->second.pre == id_prefix) {
        it->second.score = std::max(it->second.score, elem.score);
        it->second.nbPartie += elem.nbPartie;
        prefix_found = true;
        break;
    }
    }

    // Si aucun élément avec ce préfixe n'a été trouvé, créer un nouvel unordered_map
    if (!prefix_found) {
        std::unordered_map<std::string, coupStructPre> new_map;
        new_map[id] = elem;
        VectPre.push_back(new_map);
    }
}

// Itérer sur les éléments de VectPre
for (const auto& pre_map : VectPre) {
    // Initialiser les variables pour le score maximum et l'élément correspondant
    int max_score = -10000;
    coupStructPre max_elem = {"", -1, -1, ""};
    
    // Parcourir chaque élément de l'unordered_map et trouver l'élément avec le score maximum
    for (const auto& [id, elem] : pre_map) {
        if (elem.score*100/elem.nbPartie > max_score) {
            max_score = elem.score*100/elem.nbPartie;
            max_elem = elem;
        }
    }
    
    // Ajouter l'élément avec le score maximum à la liste des meilleurs coups
    Bestmoves.push_back(max_elem.id + "," + std::to_string(max_elem.score) + "," + std::to_string(max_elem.nbPartie));
}


    Lecteur l2("../coupsElaguer.csv");
    l2.write(Bestmoves);

}
