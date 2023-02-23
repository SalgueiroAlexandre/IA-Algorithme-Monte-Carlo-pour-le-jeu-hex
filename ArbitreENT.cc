#include "ArbitreENT.hh"
#include "./joueurs/Guezmer.hh"

ArbitreENT::ArbitreENT(player player1, player player2, int nombre_parties, std::string fichierENT,std::string fichierV0) 
: Arbitre(player1, player2, nombre_parties),fichierV0(fichierV0),
// initialisation du lecteur de fichier de coups avec le nom du fichier
lecteur(fichierENT)
{
}

void ArbitreENT::initialisation()
{

    switch (_player1)
    {
    case player::M_1:
        _joueur1 = std::make_shared<Guezmer>("GuezmerV0", true, fichierV0);
        break;
    case player::M_2:
        _joueur1 = std::make_shared<Guezmer>("Guezmer", true, "../coups.csv");
        break;
    case player::MANUEL:
        _joueur1 = std::make_shared<Joueur_Manuel>("Caro", true);
        break;
    case player::MANUEL2:
        _joueur1 = std::make_shared<Joueur_Manuel>("Vio", true);
        break;
    case player::RAND:
        _joueur1 = std::make_shared<Joueur_Random>("Random", true);
        break;
    case player::RAND2:
        _joueur1 = std::make_shared<Joueur_Random>("Aleatoire", true);
        break;
    case player::A_1:
        _joueur1 = std::make_shared<Joueur_AlphaBeta>("AlphaBeta", true);
        break;
    case player::A_2:
        _joueur1 = std::make_shared<Joueur_AlphaBeta>("MaxAlphaBeta", true);
        break;
    default:
        break;
    }
    _joueur2 = std::make_shared<Guezmer>("GuezmerJoueur2", false, "../coups.csv");
    _jeu.vider_jeu();
}

int ArbitreENT::challenge()
{
    initialisation(); // Au moins une fois pour que les objets de la ligne qui suit soient définis
    std::cout << "Le challenge de " << _nombre_parties << " parties "
              << "entre " << _joueur1->nom() << " et " << _joueur2->nom()
              << " commence. " << std::endl;
    int victoire_joueur_1 = 0;
    int victoire_joueur_2 = 0;
    while (_numero_partie <= _nombre_parties)
    {
        std::cout << "\n"
                  << "Partie n°" << _numero_partie << " : " << std::endl
                  << _jeu;
        result resultat = partie();
        if (resultat == result::ERREUR)
        {
            std::cerr << "Alerte bug. Sauvez votre terminal et prévenez Me Devred. Merci. " << std::endl;
            return 1;
        }
        else if (resultat != result::NULLE){
            if(resultat == result::P1)
                victoire_joueur_1++;
            else
                victoire_joueur_2++;
        }
                

        std::this_thread::sleep_for(std::chrono::milliseconds(2500)); // temps de latence entre deux parties
        _numero_partie++;
        // ECRITURE DE LA VICTOIRE 
        if (resultat == result::P1)
        {
            lecteur.ecrire(-1);
        }
        else
            lecteur.ecrire(1);
        initialisation();
    }
    std::cout << "FIN DU CHALLENGE\n\t"
              << _joueur1->nom() << " gagne " << (victoire_joueur_1*100/_nombre_parties)<<"% winrate"
              << "\n\t" << _joueur2->nom() << " gagne " << (victoire_joueur_2*100/_nombre_parties)<<"% winrate" << std::endl;
    if (_joueur1->nom() == "Guezmer")
    {
        return victoire_joueur_1;
    }
    else
        return victoire_joueur_2;
}

result ArbitreENT::partie()
{
    int tour = 0;
    bool coup_ok; // si le coup est valide
    while (!_jeu.partie_finie())
    {
        bool try_lock = false;
        _coups[_numero_partie - 1].first = -1;
        _coups[_numero_partie - 1].second = -1;
        coup_ok = true;
        tour++;
        std::cout << "tour : " << tour << std::endl;
        _coups_mutex[_numero_partie - 1].unlock();

        std::thread thread_joueur(&Joueur::jouer,
                                  ((tour % 2) ? (_joueur1) : (_joueur2)),
                                  _jeu,
                                  std::ref(_coups[_numero_partie - 1]),
                                  std::ref(_coups_mutex[_numero_partie - 1]));

        std::this_thread::sleep_for(std::chrono::milliseconds(TEMPS_POUR_UN_COUP));
        //        std::this_thread::sleep_for (std::chrono::seconds(TEMPS_POUR_UN_COUP));

        if (!_coups_mutex[_numero_partie - 1].try_lock())
        {
            Guezmer::resetEtatPartie();
            std::cerr << std::endl
                      << "mutex non rendu " << std::endl;
            try_lock = true;
            // arret du programme
            exit(1);
        }
        else if (_jeu.case_libre(_coups[_numero_partie - 1]) == false)
        {
            std::cerr << "coup invalide abs : " << _coups[_numero_partie - 1].second << " ,ord : " << _coups[_numero_partie - 1].first << std::endl;
            coup_ok = false;
        }

        thread_joueur.detach();

        if (try_lock || !coup_ok)
        {
            Guezmer::resetEtatPartie();
            if (tour % 2)
            {
                std::cout << _joueur2->nom() << " gagne ! Nombre de tours : " << tour << std::endl;
                return result::P2; // joueur jouant en 2eme gagne
            }
            else
            {
                std::cout << _joueur1->nom() << " gagne ! Nombre de tours : " << tour << std::endl;
                return result::P1; // joueur jouant en 1er gagne
            }
        }
        // On joue le coup, on l'affiche et on affiche le plateau
        _jeu.jouer_coup(_coups[_numero_partie - 1], (tour % 2) ? 1 : 2);
        lecteur.addMove(_coups[_numero_partie - 1].first, _coups[_numero_partie - 1].second);
        Guezmer::majEtatPartie(_coups[_numero_partie - 1], tour);

        std::cout << ((tour % 2) ? _joueur1->nom() : _joueur2->nom()) << " abs : " << _coups[_numero_partie - 1].second << " ord : " << _coups[_numero_partie - 1].first
                  << std::endl;
                  //<< _jeu << std::endl; // AFFICHAGE DU JEU
    }

    /*if (_jeu.partie_nulle())
        {
            std::cout << std::endl << "Partie nulle" << std::endl;
            return result::NULLE;
        }
    else */
    if (_jeu.cotes_relies() == 1)
    {
        Guezmer::resetEtatPartie();
        std::cout << std::endl
                  << _joueur1->nom() << " gagne. Nombre de tours : " << tour << std::endl;
        return result::P1;
    }
    else if (_jeu.cotes_relies() == 2)
    {
        Guezmer::resetEtatPartie();
        std::cout << std::endl
                  << _joueur2->nom() << " gagne. Nombre de tours : " << tour << std::endl;
        return result::P2;
    }
    return result::ERREUR;
}
