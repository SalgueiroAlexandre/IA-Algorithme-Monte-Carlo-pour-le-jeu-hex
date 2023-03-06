#include "Guezmer.hh"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath> 
#include <sstream>
#include <unordered_map>


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
    
}

// Initialisation de la variable statique
std::string Guezmer::etatPartie = "";

// Initialisation de la variable statique
std::unordered_map<std::string, coupStruct> Guezmer::movesMap = {};

void Guezmer::initMoves(std::vector<std::string> moves){
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

void Guezmer::majEtatPartie(couple coup,int tour) {
    tour == 1 ? etatPartie += std::to_string(coup.first) + std::to_string(coup.second) : etatPartie += "." + std::to_string(coup.first) + std::to_string(coup.second);
}

std::vector<std::string> Guezmer::getMoves() {
    std::vector<std::string> moves;
    for (const auto& [id, elem] : movesMap) {
        moves.push_back(id + "," + std::to_string(elem.score) + "," + std::to_string(elem.nbPartie));
    }
    return moves;
}

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

bool Guezmer::coupEstConnu(couple coup) const {
    std::string coup_id;
    if(etatPartie.size() <= 1) {
        coup_id = std::to_string(coup.first) + std::to_string(coup.second);
    } else {
        coup_id = etatPartie + "." + std::to_string(coup.first) + std::to_string(coup.second);
    }
    return movesMap.find(coup_id) != movesMap.end();
}

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

void Guezmer::recupererDernierCoup(const Jeu & j){
    auto grille = j.grille();
    int taille = grille.size();
    // affichage de la grille
    for (int i = 0; i < taille; i++) {
        for (int j = 0; j < taille; j++) {
            std::cout << grille[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Guezmer::choisirCoupNonConnu(const Jeu & j, couple& coup){
    // si on est joueur 1 (de haut en bas)
    if(joueur()){
        if(j.case_libre(couple(y,x))){
            coup.second = x;
            coup.first = y;
            x = x;
            y = y + 1;
        }else if(j.case_libre(couple(y-1,x+1))){
            coup.second = x+1;
            coup.first = y-1;
            x = x+1;
            y = y;
        }else if (j.case_libre(couple(y-1,x-1)))
        {
            coup.second = x-1;
            coup.first = y-1;
            x = x-1;
            y = y;
        }
    }
    // affichage du coup
    std::cout << "Coup non connu joué : " << coup.first << " " << coup.second << std::endl;
}


void Guezmer::recherche_coup(Jeu j, couple& coup)
{
    bool toutLesCoupsSontConnus = true;
    auto coupsPossibles = j.coups_possibles();
    int taille = coupsPossibles.size();
    int inconnu_count = 0;

    // boucle sur tous les coups possibles
    for (int i = 0; i < taille; i++) {
        if (!coupEstConnu(coupsPossibles[i])) {
            toutLesCoupsSontConnus = false;
            if (rand() % ++inconnu_count == 0) {
                coup.first = coupsPossibles[i].first;
                coup.second = coupsPossibles[i].second;
            }
        }
    }
    if (!toutLesCoupsSontConnus) {
        choisirCoupNonConnu(j,coup);
        return;
    }
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
            }
        }
    }
}


float Guezmer::qubc(float score, int nbPartiePere, int nbPartieFils) {
    if(!joueur()){ // si je suis le joueur 2
        score = score * -1;
    }
    return (score / nbPartieFils) + sqrt(0 * log(nbPartiePere)/(nbPartieFils));
}
/*
bool Guezmer::compareMoyscore(const coupStruct& a, const coupStruct& b) {
    return a.score/static_cast<float>(a.nbPartie) > b.score/static_cast<float>(b.nbPartie);
}


void Guezmer::recherche_coup2(Jeu j, couple &coup){
    std::string etat_parti_pre = etatPartie.substr(0, etatPartie.size() - 3);
    std::cout<<"etat_parti_pre : "<<etat_parti_pre<<std::endl;
    std::vector<coupStruct> coupsInteressant;
    for (const auto& elem : movesStruct){
        std::string coup_prec = elem.id.substr(0, elem.id.size() - 3);
        if (coup_prec == etat_parti_pre && elem.id.size()>2){
            coupsInteressant.push_back(elem);
        }
    }
    for(const auto& elem : coupsInteressant){
        std::cout<<"coup : "<<elem.id<<" score : "<<elem.score<<std::endl;
    }
    std::sort(coupsInteressant.begin(), coupsInteressant.end(), [](const coupStruct& a, const coupStruct& b) {
    return a.score/static_cast<float>(a.nbPartie) > b.score/static_cast<float>(b.nbPartie);});
    // trier le vector en fonction du score
    std::cout<<"coup le plus interessant : "<<coupsInteressant[0].id<<", "<<coupsInteressant[0].score<<", "<<coupsInteressant[0].nbPartie<<std::endl;
}


void Guezmer::bloquer(Jeu j,couple &coup){

    //récuperer les derneir caractère de l'etatPartie après le dernier point
    std::string derniercoups = "";
    derniercoups = etatPartie.substr(etatPartie.rfind(".") + 1);
    std::cout<<"derniercoups : "<<derniercoups<<std::endl;
    int x = 0;
    int y = 0;
    if (!etatPartie.empty())
    {
        if (derniercoups.size() == 2){
            y = std::stoi(derniercoups.substr(0, 1));
            x = std::stoi(derniercoups.substr(1));
        }
        else{
            if (derniercoups.size() == 3)
            {
                if(derniercoups.substr(1)=="10")
                {
                    x = std::stoi(derniercoups.substr(1));
                    y = std::stoi(derniercoups.substr(0,1));
                }
                else
                {
                    x = std::stoi(derniercoups.substr(2));
                    y = std::stoi(derniercoups.substr(0,2));
                }
            }
            else
            {
                y = std::stoi(derniercoups.substr(0, 2));
                x = std::stoi(derniercoups.substr(2));
            }
        
        }
    }
    std::cout<<"x : "<<x<<", y : "<<y<<std::endl;

   if(etatPartie!=""){ //1er = bloqué en le 2eme donc en ligne en haut ou au dessus
        std::vector<couple> coups_adjacent;
        for(int i =-1; i<=1;i++){
            for (int k = -1; k <=1; k++)
            {
                if((i != 0) || (k != 0)){
                    if(x+i >= 0 && y+k >= 0 && x+i <= 10 && y+k <= 10)
                    {
                        couple c(y+k,x+i);
                        std::vector<std::vector<int>> g = j.grille();
                        std::cout<<"g["<<x+i<<","<<y+k<<"] : "<<g[y+k][x+i]<<std::endl;
                        if(g[y+k][x+i]==0)
                        {
                            coups_adjacent.push_back(c);
                        }
                    
                    }
                }
                
            }
            
        }
        //cout vector coup adjacent	
        for (const auto& elem : coups_adjacent){

            std::cout<<"coup adjacent : "<<elem.first<<", "<<elem.second<<std::endl;
        }
        if(coups_adjacent.size()!=0){
            int random = rand() % (coups_adjacent.size());
            coup.first = coups_adjacent[random].first;
            coup.second = coups_adjacent[random].second;
        }
        else
        {
            int taille = j.coups_possibles().size();
            int num = rand()%(taille);
            coup.first=j.coups_possibles()[num].first;
            coup.second=j.coups_possibles()[num].second;
        }
        std::cout<<"coup : "<<coup.first<<", "<<coup.second<<std::endl;

    }



}
*/