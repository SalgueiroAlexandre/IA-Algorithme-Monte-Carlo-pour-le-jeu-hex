#include "Lecteur.hh"

Lecteur::Lecteur(std::string f): nomFichier(f)
{

}


int Lecteur::ecrire(int result){
    std::ifstream readFile(nomFichier);
    if (!readFile.is_open()) {
        std::cout << "Impossible d'ouvrir le fichier." << std::endl;
        return 1;
    }
    // Lire toutes les lignes dans le fichier et les stocker dans fichierStock
    std::vector<std::string> fichierStock;
    std::string line;
    while (std::getline(readFile, line)) {
        fichierStock.push_back(line);
    }
    readFile.close();
    // Ouvrir le fichier en mode écriture
    std::ofstream file(nomFichier, std::ios::trunc);
    if (!file.is_open()) {
        std::cout << "Impossible d'ouvrir le fichier." << std::endl;
        return 1;
    }
    // Parcourir toutes les entrées dans moves
    for (std::vector<Move>::size_type i = 0; i < moves.size();i++) {
        bool found = false;
        // Parcourir fichierStock pour trouver une entrée correspondante
        for (std::vector<std::string>::size_type j = 0; j < fichierStock.size(); j++) {
            std::string firstPart = fichierStock[j].substr(0, fichierStock[j].find(","));
            if (firstPart == moves[i].ID) {
                found = true;
                // Si une entrée correspondante est trouvée, la mettre à jour
                std::string delimiter = ",";
                std::string id = fichierStock[j].substr(0, fichierStock[j].find(delimiter));
                std::string score = fichierStock[j].substr(fichierStock[j].find(delimiter) + 1, fichierStock[j].find(delimiter));
                std::string nb = fichierStock[j].substr(fichierStock[j].rfind(delimiter) + 1);
                int newScore = std::stoi(score) + result;
                int newNb = std::stoi(nb) + 1;
                fichierStock[j] = id + "," + std::to_string(newScore) + "," + std::to_string(newNb);
            }
        }
        // Si aucune entrée correspondante n'a été trouvée, la créer
        if (!found) {
            fichierStock.push_back(moves[i].ID + "," + std::to_string(result) + "," + "1");
            // clear the vector
            moves.clear();
            break;
        }

            
    }
    // écrire le fichier mis à jour
    std::ofstream outputFile(nomFichier);
    for (const auto &line : fichierStock) {
        outputFile << line << std::endl;
    }
    outputFile.close();
    // clear the vector
    fichierStock.clear();
    moves.clear();
    return 0;
}

int Lecteur::lire()
{
    std::ifstream readFile(nomFichier);
    if (!readFile.is_open()) {
        std::cout << "Impossible d'ouvrir le fichier." << std::endl;
        return 1;
    }

    // Lire les coups du fichier
    int x, y;
    while (readFile >> x >> y) {
        std::cout << "Coup: " << x << " " << y << std::endl;
    }
    readFile.close();
    return 0;
}

void Lecteur::addMove(int x, int y) {
    std::string ID;
    ID = std::to_string(x) + std::to_string(y);
    if(moves.size() > 0)
        ID = moves.back().ID + "." + ID;
    moves.push_back(Move{ID});
}

std::vector<Move> Lecteur::getMoves() const{
    return moves;
}