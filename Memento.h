#ifndef Memento_h
#define Memento_h
/*!
 * \file Memento.h
 * \brief Design pattern memento
 */

#include <stack>
#include "Litterale.h"

/*! \class Memento
 * \brief Classe qui enregistre l'état de la pile,
 *  un objet Memento contient donc un état de la pile
 *  et un seul, celui au moment où le memento a été instancié.
 */
class Memento {
    stack<Litterale*> object;
public:
    Memento(const stack<Litterale*>& obj):object(obj){}
    stack<Litterale*> getSnapshot() const { return object; }
};

/*! \class CareTaker
 * \brief Classe gardienne qui enregistre les memento,
 *  c'est par cette classe que l'on rétabli
 *  les memento: undo/redo
 */
class CareTaker {
    static stack<Memento*> undoStack;
    static stack<Memento*> redoStack;
public:
    CareTaker(){}
    ~CareTaker();
    void popUndo() { undoStack.pop(); }
    size_t sizeRedo() const { return redoStack.size(); }
    void emptyRedo();
    void addMementoUndo(Memento* m) { undoStack.push(m); }
    void addMementoRedo(Memento* m) { redoStack.push(m); }
    Memento* getMementoUndo();
    Memento* getMementoRedo();
};

#endif
