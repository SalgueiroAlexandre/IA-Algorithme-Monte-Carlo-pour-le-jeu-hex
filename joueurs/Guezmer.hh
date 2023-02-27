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
  static void majEtatPartie(couple coup,int tour);
  static void resetEtatPartie(){etatPartie = "";};
  void recherche_coup2(Jeu j);
  void bloquer(Jeu j,couple &coup);
private:
  std::vector<std::string> moves;
  std::vector<coupStruct> movesStruct;
  static std::string etatPartie;
};
