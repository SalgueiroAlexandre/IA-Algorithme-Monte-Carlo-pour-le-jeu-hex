#include "ArbitreENT.hh"
#include "./joueurs/Guezmer.hh"
#include <iomanip> // Nécessaire pour std::setprecision


ArbitreENT::ArbitreENT(player player1, player player2, int nombre_parties) 
: Arbitre(player1, player2, nombre_parties)
{
}

void ArbitreENT::initialisation()
{
    switch (((_numero_partie%2)? _player1 : _player2)) {
        case player::M_1:
            _joueur1 = std::make_shared<Guezmer> ("Guezmer",true);
            break;
        case player::M_2:
            _joueur1 = std::make_shared<Guezmer> ("Guezmer2",true);
            break;
        case player::MANUEL:
            _joueur1 = std::make_shared<Joueur_Manuel> ("Caro",true);
            break;
        case player::MANUEL2:
            _joueur1 = std::make_shared<Joueur_Manuel> ("Vio",true);
            break;
        case player::RAND:
            _joueur1 = std::make_shared<Joueur_Random> ("Random",true);
            break;
        case player::RAND2:
            _joueur1 = std::make_shared<Joueur_Random> ("Aleatoire",true);
            break;
        case player::A_1:
        _joueur1 = std::make_shared<Joueur_AlphaBeta> ("AlphaBeta",true);
        break;
        case player::A_2:
        _joueur1 = std::make_shared<Joueur_AlphaBeta> ("MaxAlphaBeta",true);
        break;
        default:
            break;
        }

    //si le numero de partie est impair, c'est _joueur2 qui commence
    switch (((!(_numero_partie%2))? _player1 : _player2)) {
        case player::M_1:
            _joueur2 = std::make_shared<Guezmer> ("Guezmer",false);
            break;
        case player::M_2:
            _joueur2 = std::make_shared<Guezmer> ("Guezmer2",false);
            break;
        case player::MANUEL:
            _joueur2 = std::make_shared<Joueur_Manuel> ("Caro",false);
            break;
        case player::MANUEL2:
            _joueur2 = std::make_shared<Joueur_Manuel> ("Vio",false);
            break;
        case player::RAND:
            _joueur2 = std::make_shared<Joueur_Random> ("Random",false);
            break;
        case player::RAND2:
            _joueur2 = std::make_shared<Joueur_Random> ("Aleatoire",false);
            break;
        case player::A_1:
            _joueur1 = std::make_shared<Joueur_AlphaBeta> ("AlphaBeta",false);
            break;
        case player::A_2:
            _joueur1 = std::make_shared<Joueur_AlphaBeta> ("MaxAlphaBeta",false);
            break;
        default:
            break;
        }
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
                  << "Partie n°" << _numero_partie << " : " << std::endl;
                  //<< _jeu;
        // calcul du temps d'une partie
        result resultat = partie();
        if (resultat == result::ERREUR)
        {
            std::cerr << "Alerte bug. Sauvez votre terminal et prévenez Me Devred. Merci. " << std::endl;
            return 1;
        } else if (resultat != result::NULLE)
                (resultat==result::P1?
                        ((_numero_partie%2)?
                             victoire_joueur_1++
                           :
                             victoire_joueur_2++ )
                      :
                        (!(_numero_partie%2)?
                             victoire_joueur_1++
                           :
                             victoire_joueur_2++ ));
                
        //std::this_thread::sleep_for(std::chrono::milliseconds(100)); // temps de latence entre deux parties
        _numero_partie++;
        initialisation();
    }
std::cout << "FIN DU CHALLENGE\n\t"
         << _joueur1->nom() << " gagne " << std::fixed << std::setprecision(2) << (static_cast<double>(victoire_joueur_1)*100/_nombre_parties)<<"% winrate"
          << "\n\t" << _joueur2->nom() << " gagne " << std::fixed << std::setprecision(2) << (static_cast<double>(victoire_joueur_2)*100/_nombre_parties)<<"% winrate" << std::endl;
    
    
    return 0;
}

result ArbitreENT::partie()
{
    int tour = 0;
    bool coup_ok; // si le coup est valide
    while (!_jeu.partie_finie())
    {
        _coups[_numero_partie - 1].first = -1;
        _coups[_numero_partie - 1].second = -1;
        coup_ok = true;
        tour++;
        // std::cout << "tour : " << tour << std::endl;
        (tour % 2 ? _joueur1->jouer(_jeu, _coups[_numero_partie - 1], _coups_mutex[_numero_partie - 1]) : _joueur2->jouer(_jeu, _coups[_numero_partie - 1], _coups_mutex[_numero_partie - 1]));
        
        if (_jeu.case_libre(_coups[_numero_partie - 1]) == false)
        {
            std::cerr << "coup invalide abs : " << _coups[_numero_partie - 1].second << " ,ord : " << _coups[_numero_partie - 1].first << std::endl;
            coup_ok = false;
        }

        if (!coup_ok)
        {

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

        //std::cout << ((tour % 2) ? _joueur1->nom() : _joueur2->nom()) << " abs : " << _coups[_numero_partie - 1].second << " ord : " << _coups[_numero_partie - 1].first
        //         << std::endl
        //       << _jeu << std::endl; // AFFICHAGE DU JEU
        // attendre 1 seconde 
    }

    /*if (_jeu.partie_nulle())
        {
            std::cout << std::endl << "Partie nulle" << std::endl;
            return result::NULLE;
        }
    else */
    if (_jeu.cotes_relies() == 1)
    {
        //appel de la fonction rollback avec 1 car guezmer a gagné
        Guezmer::rollback(1);
        std::cout << std::endl
                  << _joueur1->nom() << " gagne. Nombre de tours : " << tour << std::endl;
        return result::P1;
    }
    else if (_jeu.cotes_relies() == 2)
    {
        //appel de la fonction rollback avec 1 car guezmer 1 à perdu
        Guezmer::rollback(-1);
        std::cout << std::endl
                  << _joueur2->nom() << " gagne. Nombre de tours : " << tour << std::endl;
        return result::P2;
    }
    return result::ERREUR;
}
