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

int Lecteur::ecrire(int result) {
    // Ouvrir le fichier en mode lecture
    std::ifstream readFile(nomFichier);
    if (!readFile.is_open()) {
        std::cout << "Impossible d'ouvrir le fichier." << std::endl;
        return 1;
    }
    // Lire toutes les lignes dans le fichier et les stocker dans un vecteur
    std::vector<std::string> fichierStock;
    std::string line;
    while (std::getline(readFile, line)) {
        fichierStock.push_back(line);
    }
    readFile.close();

    // Parcourir toutes les entrées dans moves
    for (const auto& move : moves) {
        // Chercher une entrée correspondante dans le vecteur du fichier
        auto found = std::find_if(fichierStock.begin(), fichierStock.end(), [&](const auto& entry) {
            std::string firstPart = entry.substr(0, entry.find(","));
            return firstPart == move.ID;
        });
        if (found != fichierStock.end()) {
            // Si une entrée correspondante est trouvée, la mettre à jour
            std::string delimiter = ",";
            std::string id = found->substr(0, found->find(delimiter));
            std::string score = found->substr(found->find(delimiter) + 1, found->rfind(delimiter) - found->find(delimiter) - 1);
            std::string nb = found->substr(found->rfind(delimiter) + 1);
            int newScore = std::stoi(score) + result;
            int newNb = std::stoi(nb) + 1;
            *found = id + delimiter + std::to_string(newScore) + delimiter + std::to_string(newNb);
            std::cout << "\nROLLBACK sur " << id << " : nouveau score : " << newScore << "  nouveau nb parties : " << newNb << std::endl;
        }
        else {
            // Si aucune entrée correspondante n'a été trouvée, la créer
            fichierStock.push_back(move.ID + "," + std::to_string(result) + ",1");
            std::cout << "\nAJOUT de " << move.ID << " avec un score de " << result << std::endl;
            break;
        }
    }

    // Trier le vecteur mis à jour
    std::sort(fichierStock.begin(), fichierStock.end(), compare);

    // Ouvrir le fichier en mode écriture et écrire les modifications
    std::ofstream outputFile(nomFichier, std::ios::trunc);
    if (!outputFile.is_open()) {
        std::cout << "Impossible d'ouvrir le fichier." << std::endl;
        return 1;
    }
    for (const auto& line : fichierStock) {
        outputFile << line << "\n";
    }
    outputFile.close();

    // Vider les vecteurs de mouvements et de stockage
    moves.clear();
    fichierStock.clear();

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