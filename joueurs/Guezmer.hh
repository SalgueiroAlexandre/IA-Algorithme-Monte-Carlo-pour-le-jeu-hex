#pragma once
#include <vector>


#include "joueur.hh"

struct coupStruct{
  std::string id;
  float score;
  int nbPartie;
};

class Guezmer : public Joueur {
public:
  Guezmer(std::string nom, bool joueur, std::string nomFichier);
  void recherche_coup(Jeu j, couple & coup) override;
  float qubc(float score,int nbPartiePere,int nbPartieFils);
  bool coupEstConnu(couple coup) const;
  void majEtatPartie(couple coup);
private:
  std::vector<std::string> moves;
  std::vector<coupStruct> movesStruct;
  std::string etatPartie;
};
