#include "Controleur.h"

/*!
 *  \brief Affiche sur le flux f le contenu des n premiers éléments de la pile
 */
void Controleur::printPile(ostream& f, unsigned int n) {
    std::deque<Litterale*> liste = pile.getListe(n);
    for (deque<Litterale*>::const_iterator it = liste.begin(); it != liste.end(); ++it) {
        Litterale* l = *it;
        f << l->toString() <<endl;
    }
}

/*!
 *  \brief Appelle le manager pour exécuter la suite d'opérandes
 */
void Controleur::commande(const string& cmd) {
    try {
        manager.execCommande(cmd);
        message="Commande exécutée";
    } catch (UTComputerException e) {
        message=e.getMessage();
    }
}

/*!
 *  \brief Boucle du programme
 */
void Controleur::exec() {
    string cmd;
    do {
		system("cls");  // Windows
        cout<<"--- Message -----------------------"<<endl;
        cout<<message<<endl;
        cout<<"--- Pile --------------------------"<<endl;
        printPile(cout);
        cout<<"-----------------------------------"<<endl;
        cout<<"-? ";
        getline(cin, cmd);
        if(cmd!="Q") commande(cmd);
    } while(cmd!="Q");
}
