#include "Memento.h"

/*!
 *  \brief CareTaker: initialisation de la pile de memento
 */
stack<Memento*> CareTaker::undoStack;
stack<Memento*> CareTaker::redoStack;

/*!
 *  \brief Dépile un memento undo
 */
Memento* CareTaker::getMementoUndo() {
    Memento* m=nullptr;
    if(undoStack.size()>0) {
        m=undoStack.top();
        undoStack.pop();
    }
    return m;
}

/*!
 *  \brief Dépile un memento redo
 */
Memento* CareTaker::getMementoRedo() {
    Memento* m=nullptr;
    if(redoStack.size()>0) {
        m=redoStack.top();
        redoStack.pop();
    }
    return m;
}

/*!
 *  \brief Vide la pile de redo
 */
void CareTaker::emptyRedo() {
    while (!redoStack.empty()) {
        delete redoStack.top();
        redoStack.pop();
    }
}

/*!
 *  \brief Destruction de tous les mementos
 */
CareTaker::~CareTaker() {
    while (!undoStack.empty()) {
        delete undoStack.top();
        undoStack.pop();
    }
    while (!redoStack.empty()) {
        delete redoStack.top();
        redoStack.pop();
    }
}
