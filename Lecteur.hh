#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Structure pour stocker les coups 
struct Move {
    std::string ID;
};

class Lecteur
{
private:
    /* data */
    std::string nomFichier;
    std::vector<Move> moves;
public:
    Lecteur(std::string f);
    int lire(std::vector<std::string> & moves);
    void addMove(int x, int y);
    std::vector<Move> getMoves() const;
    void write(std::vector<std::string> moves);
};



