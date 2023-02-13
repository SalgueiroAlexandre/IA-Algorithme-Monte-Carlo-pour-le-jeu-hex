#pragma once
#include <vector>


#include "joueur.hh"


class Guezmer : public Joueur
{
public:
  Guezmer(std::string nom, bool joueur, std::string nomFichier);
  void recherche_coup(Jeu j, couple & coup) override;
  float qubc(float score,int nbPartiePere,int nbPartieFils);
private:
  std::vector<std::string> moves;
};
