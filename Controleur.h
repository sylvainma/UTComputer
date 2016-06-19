#ifndef controleur_h
#define controleur_h
/*!
 * \file Controleur.h
 * \brief Controleur pour exécuter le programme en console
 */

#define NB_MAX_PILE 5
#include <iostream>
#include <stack>
#include <deque>
#include "Litterale.h"
#include "Manager.h"
#include "Pile.h"

using namespace std;

/*! \class Controleur
 * \brief Classe permettant de faire tourner le programme en console
 */
class Controleur {
    Manager& manager;
    Pile& pile;
    string message;
public:
    Controleur(Manager& m, Pile& p):pile(p),manager(m),message("Bienvenue"){}
    void printPile(ostream& f, unsigned int i=NB_MAX_PILE);
    void commande(const string& cmd);
    void exec();
};

#endif
