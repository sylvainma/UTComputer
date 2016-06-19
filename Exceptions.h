#ifndef Exceptions_h
#define Exceptions_h
/*!
 * \file Exceptions.h
 * \brief Les Exceptions
 */

#include <string>

using namespace std;

/*! \class UTComputerException
 * \brief Base pour toutes les exceptions du projet
 */
class UTComputerException {
    string msg;
public:
    UTComputerException(const string& m):msg(m){}
    string getMessage() const { return msg; }
};

ostream& operator<<(ostream& f, UTComputerException& e);

/*! \class ManagerCommandeException
 * \brief Exceptions liées aux commandes entrées par l'utilisateur
 */
class ManagerCommandeException : public UTComputerException {
public:
    ManagerCommandeException(const string& m):UTComputerException(m){}
};

/*! \class InterpreteurException
 * \brief Exceptions liées aux interprétations des suites d'opérandes
 */
class InterpreteurException : public UTComputerException {
public:
    InterpreteurException(const string& m):UTComputerException(m){}
};

/*! \class LitteraleException
 * \brief Exceptions liées aux littérales
 */
class LitteraleException : public UTComputerException {
public:
    LitteraleException(const string& m):UTComputerException(m){}
};

/*! \class OperateurException
 * \brief Exceptions liées aux opérateurs
 */
class OperateurException : public UTComputerException {
public:
    OperateurException(const string& m):UTComputerException(m){}
};

#endif
