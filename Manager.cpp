#include "Manager.h"

Manager::Manager(Pile& p):pile(p),fl(FactoryLitterale::getInstance()),inter(Interpreteur::getInstance()){
    // Initialisation de quelques données
    init();
}

/*!
 *  \brief Initialise la pile, avec le contexte précédent s'il existe
 */
void Manager::init() {
    try {
        // Chargement du contexte
        pile.loadContext();
    } catch (UTComputerException e) {
        cout<<e<<endl;
    }
}

/*!
 *  \brief Traite la commande de l'utilisateur, c'est à dire la suite d'opérandes
 */
void Manager::execCommande(const string& cmd) {
    try
    {
        // Récupération des opérandes
        vector<string> operandes=inter.getOperandes(cmd);
        
        // Exécution des opérandes
        pile.execOperandes(operandes);
        
    } catch(UTComputerException e) {
        // Renvoi de l'exception au controleur
        throw e;
    }
}
