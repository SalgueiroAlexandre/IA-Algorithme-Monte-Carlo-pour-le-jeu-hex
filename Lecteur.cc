#include "Lecteur.hh"

Lecteur::Lecteur(std::string f): nomFichier(f)
{

}

bool compare(const std::string& a, const std::string& b) {
    // comparer les longueurs
    if (a.find(',') != std::string::npos && b.find(',') != std::string::npos) {
        int lenA = a.find(',') + 1; // longueur de la première partie de a (avant la virgule)
        int lenB = b.find(',') + 1; // longueur de la première partie de b (avant la virgule)
        if (lenA < lenB) {
            return true;
        } else if (lenA > lenB) {
            return false;
        }
    }
    // si les longueurs sont égales ou que les éléments ne contiennent pas de virgule, comparer alphabétiquement
    return a < b;
}


int Lecteur::lire(std::vector<std::string> & moves)
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
    readFile.close();
    return 0;
}

void Lecteur::write(std::vector<std::string> moves){
    // efface le fichier nomFichier et écrit les coups du vecteur moves dans le fichier
    std::ofstream outputFile(nomFichier, std::ios::trunc);
    if (!outputFile.is_open()) {
        std::cout << "Impossible d'ouvrir le fichier." << std::endl;
    }
    // trier le vecteur moves avec la fonction compare
    std::sort(moves.begin(), moves.end(), compare);
    // écrire les coups dans le fichier
    for (const auto& line : moves) {
        outputFile << line << "\n";
    }
    outputFile.close();
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