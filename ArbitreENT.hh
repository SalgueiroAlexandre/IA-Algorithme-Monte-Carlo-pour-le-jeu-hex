#pragma once
#include <memory>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <utility>
#include <functional>
#include <vector>
#include "jeu.hh"
#include "joueurs/joueur.hh"
#include "joueurs/joueur_random.hh"
#include "joueurs/joueur_manuel.hh"
#include "joueurs/joueur_montecarlo_.hh"
#include "joueurs/joueur_alphabeta.hh"
#include "joueurs/Guezmer.hh"
#include "Lecteur.hh"
//const int TEMPS_POUR_UN_COUP(15000); // millisecondes
//millisecondes -- temps lors de la compétition
/**********************************************************************************
 *             classe lançant la partie et qui se sert des                        *
 *             mutex pour s'assurer que les joueurs joue                          *
 *             dans les temps                                                     *
 **********************************************************************************/

/*Pour appeler les joueurs
 * M_1 et M_2 instancieront la classe Joueur_monte_carlo
 * M_1 s'appelle MonteCarlo (M) ; M_2 s'appelle StoMonteCarlo (S)
 * A_1 et A_2 instancieront la classe Joueur_AlphaBeta
 * MANUEL et MANUEL2 instancieront la classe Joueur_manuel
 * RAND et RAND2 instancieront la classe Joueur_Random
 * RAND s'appelle Random (R) et RAND2 s'appelle Aleatoire (A)
 * MANUEL s'appelle Caro (C) et MANUEL2 s'appelle Vio (V)
 * */
/**
 * @brief The Arbitre class
 * classe gerant la partie et les tours des joueurs,
 * et s'ils commencent ou s'ils jouent en deuxième.
 * Le joueur qui commence la partie joue les '1'
 * Le joueur qui joue en deuxième joue les '2'.
 * Si un joueur ne joue pas dans les temps, il perd,
 * si un joueur ne joue pas un coup correct, il perd.
 */
class ArbitreENT : public Arbitre
{
private:

public:
    /**
     * @brief Arbitre
     * @param player1
     * @param player2
     * @param nombre_parties
     */
    ArbitreENT(player player1, player player2, int nombre_parties);


    void initialisation() override;


    /**
     * @brief challenge
     * @param nombre_parties
     * methode lançant un challenge avec un nombre de parties
     * affiche le nombre de parties gagnées pour chaque participant
     * @return 0 si tout c'est bien passé, 1 sinon
     */
    int challenge() override;

    /**
     * @brief partie
     * @return le gagnant de la partie (result::1 pour le joueur 1, result::2 pour joueur 2, result::NULLE en cas de partie nulle, result::ERREUR en cas de problème)
     * lance une partie,
     *
     */
    result partie() override;
};
