#include "Guezmer.hh"
#include <algorithm>
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
    auto coupsPossibles = j.coups_possibles();
    int taille = coupsPossibles.size();
    std::cout << std::endl;
    std::vector<couple> coupsInconnus;
    
    // boucle sur tous les coups possibles
    for (int i = 0; i < taille; i++) {
        // si le coup n'est pas connu par montecarlo on le joue
        if (!coupEstConnu(coupsPossibles[i])) {
            toutLesCoupsSontConnus = false;
            coupsInconnus.push_back(coupsPossibles[i]);
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
        std::cout<< "tous les coups sont connus" << std::endl;
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
                    comparateur = std::to_string(coupsPossibles[i].first) + std::to_string(coupsPossibles[i].second);
                }else{
                    // on verifie que le coup est dans le vecteur movesStruct
                    comparateur = etatPartie + "." + std::to_string(coupsPossibles[i].first) + std::to_string(coupsPossibles[i].second);
                }
                if (elem.id == comparateur) {
                    int score = elem.score;
                    if (qubc(score,nbPartiePere,elem.nbPartie) > max) {
                        max = qubc(elem.score,nbPartiePere,elem.nbPartie);
                        // maj a jour du meilleur coup
                        coup.first=coupsPossibles[i].first;
                        coup.second=coupsPossibles[i].second;
                    }
                }
            }
        }
    }
}

float Guezmer::qubc(float score, int nbPartiePere, int nbPartieFils) {
    if(nbPartiePere == 0){
        nbPartiePere = moves.size();
    }
    if(!joueur()){ // si je suis le joueur 2
        score = score * -1;
    }
    return (score / nbPartieFils) + sqrt((1 * log(nbPartiePere))/(nbPartieFils));
}

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
    std::string derniercoups = etatPartie.substr(etatPartie.rfind(".") + 1);
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
                if(i!=0 && k!=0){
                    if(x+i>=0 && y+k >=0)
                    {
                        couple c(y+k,x+i);
                        if(j.case_libre(c)){
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
        int random = rand() % (coups_adjacent.size());
        coup.first = coups_adjacent[random].first;
        coup.second = coups_adjacent[random].second;
        //cout coup
        std::cout<<"coup : "<<coup.first<<", "<<coup.second<<std::endl;


    }



}

