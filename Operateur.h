#ifndef Operateur_h
#define Operateur_h
/*!
 * \file Operateur.h
 * \brief Les opérateurs
 */

#include <iostream>
#include <map>
#include <cmath>
#include <regex>
#include "Pile.h"
#include "Exceptions.h"

using namespace std;

/*! \class Operateur
 * \brief Classe mère de tous les opérateurs
 */
class Operateur {
protected:
    string op;
    unsigned int mul;
    FactoryLitterale& fl;

public:
    static Litterale* lastarg[2];
       //comme son nom l'indique il sert a stocker le ou les derniere littéral utilisé
    Operateur(const string o);
    virtual ~Operateur(){}
    /*!
     *  \brief Applique sur la pile l'opérateur
     */
    virtual void apply(Pile& p)=0;
    /*!
     *  \brief Applique sur une littérale un opérateur unaire
     */
    virtual Litterale* applyUnaire(Litterale* l1)=0;
    /*!
     *  \brief Applique sur une paire de littérales un opérateur binaire
     */
    virtual Litterale* applyBinaire(Litterale* l1, Litterale* l2)=0;
    static const map<string, const map<string,unsigned int>> liste;
    static bool isEntier(const double& x);
    static bool isReel(const double& x);
};

/*************************
 *  Opérateurs numériques
 ************************/

/*! \class OperateurNumerique
 * \brief Les opérateurs numériques
 */
class OperateurNumerique : public Operateur {
public:
    OperateurNumerique(const string o):Operateur(o){}
    void apply(Pile& p);
    Litterale* applyUnaire(Litterale* l1);
    Litterale* applyBinaire(Litterale* l1, Litterale* l2);
    
    LitteraleNumerique* addition(LitteraleEntiere* x, LitteraleEntiere* y);
    LitteraleNumerique* addition(LitteraleEntiere* x, LitteraleReelle* y);
    LitteraleNumerique* addition(LitteraleEntiere* x, LitteraleRationnelle* y);
    LitteraleNumerique* addition(LitteraleReelle* x, LitteraleReelle* y);
    LitteraleNumerique* addition(LitteraleReelle* x, LitteraleRationnelle* y);
    LitteraleNumerique* addition(LitteraleRationnelle* x, LitteraleRationnelle* y);
    
    LitteraleNumerique* soustraction(LitteraleEntiere* x, LitteraleEntiere* y);
    LitteraleNumerique* soustraction(LitteraleEntiere* x, LitteraleReelle* y);
    LitteraleNumerique* soustraction(LitteraleEntiere* x, LitteraleRationnelle* y);
    LitteraleNumerique* soustraction(LitteraleReelle* x, LitteraleEntiere* y);
    LitteraleNumerique* soustraction(LitteraleReelle* x, LitteraleReelle* y);
    LitteraleNumerique* soustraction(LitteraleReelle* x, LitteraleRationnelle* y);
    LitteraleNumerique* soustraction(LitteraleRationnelle* x, LitteraleEntiere* y);
    LitteraleNumerique* soustraction(LitteraleRationnelle* x, LitteraleReelle* y);
    LitteraleNumerique* soustraction(LitteraleRationnelle* x, LitteraleRationnelle* y);
    
    LitteraleNumerique* multiplication(LitteraleEntiere* x, LitteraleEntiere* y);
    LitteraleNumerique* multiplication(LitteraleEntiere* x, LitteraleReelle* y);
    LitteraleNumerique* multiplication(LitteraleEntiere* x, LitteraleRationnelle* y);
    LitteraleNumerique* multiplication(LitteraleReelle* x, LitteraleReelle* y);
    LitteraleNumerique* multiplication(LitteraleReelle* x, LitteraleRationnelle* y);
    LitteraleNumerique* multiplication(LitteraleRationnelle* x, LitteraleRationnelle* y);
    
    LitteraleNumerique* division(LitteraleEntiere* x, LitteraleEntiere* y);
    LitteraleNumerique* division(LitteraleEntiere* x, LitteraleReelle* y);
    LitteraleNumerique* division(LitteraleEntiere* x, LitteraleRationnelle* y);
    LitteraleNumerique* division(LitteraleReelle* x, LitteraleEntiere* y);
    LitteraleNumerique* division(LitteraleReelle* x, LitteraleReelle* y);
    LitteraleNumerique* division(LitteraleReelle* x, LitteraleRationnelle* y);
    LitteraleNumerique* division(LitteraleRationnelle* x, LitteraleEntiere* y);
    LitteraleNumerique* division(LitteraleRationnelle* x, LitteraleReelle* y);
    LitteraleNumerique* division(LitteraleRationnelle* x, LitteraleRationnelle* y);
    
};

/*************************
 *  Opérateurs de pile
 ************************/

/*! \class OperateurPile
 * \brief Les opérateurs de pile
 */
class OperateurPile : public Operateur {
public:
    OperateurPile(const string o):Operateur(o){}
    void apply(Pile& p);
    Litterale* applyUnaire(Litterale* l1);
    Litterale* applyBinaire(Litterale* l1, Litterale* l2);
};

/*************************
 *  Opérateurs d'expressions
 ************************/

/*! \class OperateurExpression
 * \brief Les opérateurs d'expressions
 */
class OperateurExpression : public Operateur {
public:
    OperateurExpression(const string o):Operateur(o){}
    void apply(Pile& p);
    Litterale* applyUnaire(Litterale* l1);
    Litterale* applyBinaire(Litterale* l1, Litterale* l2);
};

/*************************
 *  Opérateurs logiques
 ************************/

/*! \class OperateurLogique
 * \brief Les opérateurs logiques
 */
class OperateurLogique : public Operateur {
public:
    OperateurLogique(const string o):Operateur(o){}
    void apply(Pile& p);
    Litterale* applyUnaire(Litterale* l1);
    Litterale* applyBinaire(Litterale* l1, Litterale* l2);
};

/*************************
 *  Factory
 ************************/

/*! \class FactoryOperateur
 * \brief Fabrique d'opérateurs: génére facilement les objets Operateur
 */
class FactoryOperateur {
    FactoryOperateur(){};
    ~FactoryOperateur();
    FactoryOperateur(const FactoryOperateur&);
    void operator=(const FactoryOperateur&);
    
    struct Handler {
        FactoryOperateur* instance;
        Handler():instance(nullptr){};
        ~Handler(){ delete instance; };
    };
    
    static Handler handler;
    
    /*!
     *  \brief Vecteur contenant tous les objets Operateur et ses objets dérivés, créés pendant l'exécution du programme
     */
    vector<Operateur*> to;
    
public:
    static FactoryOperateur& getInstance();
    static void deleteInstance();
    OperateurNumerique* newOperateurNumerique(const string& s);
    OperateurPile* newOperateurPile(const string& s);
    OperateurExpression* newOperateurExpression(const string& s);
	OperateurLogique* newOperateurLogique(const string& s);
};

#endif
