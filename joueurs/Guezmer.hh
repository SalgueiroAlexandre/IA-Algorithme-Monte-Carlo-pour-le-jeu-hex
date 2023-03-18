#pragma once
#include <vector>
#include <unordered_map>
#include "joueur.hh"

struct coupStruct{
  std::string id;
  int score;
  int nbPartie;
};

class Guezmer : public Joueur {
public:
  Guezmer(std::string nom, bool joueur);
  void recherche_coup(Jeu j, couple & coup) override;
  float qubc(float score,int nbPartiePere,int nbPartieFils);
  bool coupEstConnu(couple coup) const;
  static void majEtatPartie(Jeu j);
  void resetEtatPartie(){etatPartie = "";};
  bool compareMoyscore(const coupStruct& a, const coupStruct& b);
  void bloquer(Jeu j,couple &coup);
  static void rollback(int result);
  std::vector<std::string> getMoves();
  static void initMoves();
  int nbPartiePere(std::string id);
  void choisirCoupNonConnu(const Jeu & j, couple& coup);
private:
  std::vector<std::string> _moves;
  static std::unordered_map<std::string, coupStruct> movesMap;
  static std::string etatPartie;
  int x;
  int y;
};
