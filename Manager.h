#ifndef Manager_h
#define Manager_h
/*!
 * \file Manager.h
 * \brief Le manager du programme
 */

#include "Pile.h"
#include "Interpreteur.h"
#include "Exceptions.h"
#include "Operateur.h"
#include <vector>

/*! \class Manager
 * \brief Manager est la classe responsable de la création et la destruction des objets
 *  Litterales et Operateurs. Elle traite également les commandes, cad les suites
 *  d'opérandes entrées par l'utilisateur. Elle récupère cette string par le controleur.
 */
class Manager {
    Pile& pile;
    FactoryLitterale& fl;
    Interpreteur& inter;
    void init();
public:
    Manager(Pile& p);
    void execCommande(const string& cmd);
};

#endif
