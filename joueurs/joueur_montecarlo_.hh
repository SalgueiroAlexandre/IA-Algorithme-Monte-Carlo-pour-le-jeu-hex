#pragma once


#include "joueur.hh"


class Joueur_MonteCarlo_ : public Joueur
{
public:
  Joueur_MonteCarlo_(std::string nom, bool joueur);


  void recherche_coup(Jeu j, couple & coup) override;
};
