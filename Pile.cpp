#include "Pile.h"
#include "Interpreteur.h"

/*!
 *  \brief Retourne dans une queue le contenu de la pile sans la modifier
 */
std::deque<Litterale*> Pile::getListe(unsigned int n) const {
    std::deque<Litterale*> liste;
    std::stack<Litterale*> cpy=items;
    unsigned int i=0;
    while (i<n && !cpy.empty()) {
        liste.push_front(cpy.top());
        cpy.pop();
        i++;
    }
    return liste;
}

/*!
 *  \brief Exécute la suite d'opérande fournie en argument
 */
void Pile::execOperandes(vector<string>& operandes) {

    Interpreteur& inter=Interpreteur::getInstance();
    FactoryLitterale& fl=FactoryLitterale::getInstance();
    
    try {
        
        // On check si un REDO est demandé, si la pile REDO est vide ça ne sert à rien de l'exécuter
        if(operandes[0]=="REDO" && caretaker.sizeRedo()==0) {
            operandes.erase(operandes.begin());
        }
        
        // On regarde s'il y a un REDO qui suit directement un UNDO, dans ce cas un supprime les deux
        for(std::vector<string>::iterator it = operandes.begin(); it != operandes.end(); ) {
            std::vector<string>::iterator it_copie(it);
            if(it>operandes.begin() && *it=="REDO" && *(--it_copie)=="UNDO") {
                it = operandes.erase(it_copie);
                it = operandes.erase(it);
            }
            else ++it;
        }
        
        // On regarde s'il y a un UNDO, si oui on supprime l'opérande qui le précède
        for(std::vector<string>::iterator it = operandes.begin(); it != operandes.end(); ) {
            if(*it=="UNDO" && it>operandes.begin()) {
                std::vector<string>::iterator it_copie(it);
                --it_copie;
                it = operandes.erase(it_copie);
                it = operandes.erase(it);
            }
            else ++it;
        }
        
        // On vérifie qu'il reste bien des opérandes à traiter (évite le save() après la boucle for pour rien)
        if(operandes.size()>0) {
            
            /*
             *  Début des opérandes
             */
        
            // Pour chaque opérande, on l'applique à la pile
            for(std::vector<string>::iterator it = operandes.begin(); it != operandes.end(); ++it)
            {
                if(*it=="UNDO") caretaker.popUndo();
                if(*it!="UNDO" && *it!="REDO") caretaker.emptyRedo();
                
                if(inter.isLitterale(*it)) {
                    
                    Litterale* l=inter.toLitterale(*it);
                    
                    // Si toLitterale renvoi un atome, c'est que c'est une variable
                    // Quand un identifiant de variable est appelé, on push une copie de sa valeur
                    LitteraleAtome* lAtome = dynamic_cast<LitteraleAtome*>(l);
                    if(lAtome) {
                        Litterale* valeur=fl.copie(lAtome->getValeur());
                        push(valeur);
                        LitteraleProgramme* prg=dynamic_cast<LitteraleProgramme*>(valeur);
                        // Si c'est un identifiant de programme, alors on évalue le programme
                        if(prg) {
                            Operateur* o=inter.toOperateur("EVAL");
                            o->apply(*this);
                        }
                    }
                    else {
                        push(l);
                    }
                    
                }
                else if (inter.isOperateur(*it)) {
                    Operateur* o=inter.toOperateur(*it);
                    o->apply(*this);
                    if(*it!="LASTOP") lastOperateur=*it;
                }
                else {
                    throw ManagerCommandeException("Commande incorrecte");
                }
                
            }

            /*
             *  Fin des opérandes
             */
            
            save();
    
        }
        
    } catch(UTComputerException e) {
        undo();
        save();
        throw e;
    }

}

/*************************
 *  Memento
 ************************/

/*!
 *  \brief Sauvegarde de l'état actuel de la pile
 */
void Pile::save(Memento* m) {
    if(m==nullptr) m=createMemento();
    caretaker.addMementoUndo(m);
}

/*!
 *  \brief Retour à l'état antérieur de la pile
 */
void Pile::undo() {
    Memento* m=caretaker.getMementoUndo();
    if(m!=nullptr) {
        // On empile dans REDO
        caretaker.addMementoRedo(createMemento());
        // On applique le UNDO
        setMemento(m);
    }
}

/*!
 *  \brief Retour à l'état postérieur de la pile
 */
void Pile::redo() {
    Memento* m=caretaker.getMementoRedo();
    if(m!=nullptr) {
        // On applique le REDO
        setMemento(m);
    }
}

/*!
 *  \brief Génére un memento de la pile
 */
Memento* Pile::createMemento() const {
    return new Memento(items);
}

/*!
 *  \brief Affecte à la pile un état encapsulé dans un memento
 */
void Pile::setMemento(Memento* mem) {
    items = mem->getSnapshot();
    delete mem;
}

/*!
 *  \brief Sauvegarde du contexte
 */
void Pile::last_save() {        // chercher où c'est appelé
    try {
        saveContext();
    } catch(UTComputerException e) {
        cout<<e.getMessage()<<endl;
    }
}

/*!
 *  \brief Sauvegarde du contexte
 */
void Pile::saveContext() {

    // Ouverture du fichier dans lequel on écrit
    ofstream flux(FILEPATH);

    if(flux) {

        // Sauvegarde des variables
        for(vector<LitteraleAtome*>::const_iterator it=LitteraleAtome::vars.begin(); it!=LitteraleAtome::vars.end(); ++it)
        {
            Litterale* l=(*it)->getValeur();
            flux<<(*it)->getId()<<" "<<l->toString()<<" "<<"STO"<<endl;
        }

        // Balise de sépration variables/pile
        flux<<"<1>"<<endl;

        // Sauvegarde de la pile
        deque<Litterale*> p = getListe(items.size());
        for(deque<Litterale*>::const_iterator it=p.begin(); it!=p.end(); ++it)
        {
            Litterale* l=*it;
            flux<<l->toString()<<endl;
        }

        // Balise de fin
        flux<<"<1>"<<endl;

        // Fermeture du fichier
        flux.close();

        // Confirmation
        cout<<"Sauvegarde du contexte effectuée"<<endl;
    }
    else throw UTComputerException("Impossible de sauvegarder le contexte dans le fichier.");
}

/*!
 *  \brief Chargement du contexte
 */
void Pile::loadContext() {

    // Ouverture du fichier dans lequel on lit
    ifstream flux(FILEPATH);

    if(flux) {

        // Variable qui lit
        string cmd;

        // Chargement des données grâce à execCommande
        while(getline(flux, cmd)){
            if(cmd!="<1>" && !cmd.empty()) {
                try
                {
                    // Interpreteur
                    Interpreteur& inter=Interpreteur::getInstance();

                    // Récupération des opérandes
                    vector<string> operandes=inter.getOperandes(cmd);

                    // Exécution des opérandes
                    execOperandes(operandes);

                } catch(UTComputerException e) {
                    throw UTComputerException("Erreur chargement du contexte: "+e.getMessage());
                }
            }
        }

        // LASTARGS et LASTOP
        Operateur::lastarg[0]=nullptr;
        Operateur::lastarg[1]=nullptr;
        setLastOp("");

        // Fermeture du fichier
        flux.close();

        // Confirmation
        cout<<"Chargement du contexte effectué"<<endl;
    }
    else throw UTComputerException("Impossible de charger un contexte: pas de contexte précédent ou impossible d'ouvrir le fichier.");
}
