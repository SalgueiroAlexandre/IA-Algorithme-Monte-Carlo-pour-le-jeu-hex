#include "Lecteur.hh"
#include <vector>
#include <unordered_map>
#include <iostream>
#include "ArbitreENT.hh"
#include "jeu.hh"

struct coupStruct{
  std::string id;
  int score;
  int nbPartie;
};

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
    movesMap[id] = {id, score, nbPartie, id_prefix};
}


std::cout<<"test 1 "<<std::endl;
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
        new_map[id_prefix] = elem;
        VectPre.push_back(new_map);
    }
}

//print vectpre
for (const auto& p : VectPre) {
    for (const auto& [id, elem] : p) {
        std::cout << id << " " << elem.score << " " << elem.nbPartie << " " << elem.pre << std::endl;
    }
}


}

