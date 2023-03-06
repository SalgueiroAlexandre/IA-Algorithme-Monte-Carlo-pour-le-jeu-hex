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
  static void majEtatPartie(couple coup,int tour);
  static void resetEtatPartie(){etatPartie = "";};
  void recherche_coup2(Jeu j, couple & coup);
  bool compareMoyscore(const coupStruct& a, const coupStruct& b);
  void bloquer(Jeu j,couple &coup);
  static void rollback(int result);
  static std::vector<std::string> getMoves();
  static void initMoves(std::vector<std::string> moves);
  void recupererDernierCoup(const Jeu & j);
  int nbPartiePere(std::string id);
  void choisirCoupNonConnu(const Jeu & j, couple& coup);
private:
  std::vector<std::string> _moves;
  static std::unordered_map<std::string, coupStruct> movesMap;
  static std::string etatPartie;
  int x;
  int y;
  
};
