#ifndef Pile_h
#define Pile_h
/*!
 * \file Pile.h
 * \brief La pile
 */

#include <stack>
#include <deque>
#include <fstream>
#include "Litterale.h"
#include "Memento.h"

#define FILEPATH "./sauvegarde.txt"
//#define FILEPATH "/Users/Sylvain/Desktop/Programmation/C++/UTComputer/UTComputer/LO21-utcomputer/qt/build-UTComputer-Desktop_Qt_5_6_0_clang_64bit-Debug/sauvegarde.txt"

using namespace std;

/*! \class Pile
 * \brief Gère la pile de littérales
 */
class Pile {
    CareTaker caretaker;
    stack<Litterale*> items;
    string lastOperateur;
public:
    Pile(){};
    
    // Pile
    void push(Litterale* l) { items.push(l); }
    void pop() { items.pop(); }
    Litterale* top() const { return items.top(); }
    unsigned long int size() const { return items.size(); }
    bool empty() const { return items.empty(); }
    deque<Litterale*> getListe(unsigned int n) const;
    
    // Memento
    void save(Memento* m=nullptr);
    void undo();
    void redo();
    Memento* createMemento() const;
    void setMemento(Memento* mem);
    
    // Exécution des opérandes
    void execOperandes(vector<string>& operandes);
    
    // Dernier opérateur et arguments utilisés
    void setLastOp(string op) { lastOperateur=op; }
    string getLastOp() const { return lastOperateur; }

    // LASTOP
    void set_lastop(string& s){ lastOperateur=s; }

    // Sauvegarde du contexte
    void last_save();
    void saveContext();
    void loadContext();
};

#endif
