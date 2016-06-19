#ifndef litterale_h
#define litterale_h
/*!
 * \file Litterale.h
 * \brief Les Litterales
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <regex>
#include "Exceptions.h"

using namespace std;

/*! \class Litterale
 * \brief Classe mère de toutes les littérales
 */
class Litterale {
public:
    virtual ~Litterale(){}
    virtual string toString() const=0;
};

/*************************
 *  LitteraleNumerique
 ************************/

/*! \class LitteraleNumerique
 * \brief Classe mère des littérales numériques: réels, entiers, rationnels
 */
class LitteraleNumerique : public Litterale {
    friend class OperateurNumerique;
public:
    virtual void negatif()=0;
    virtual double valeur() const=0;
    virtual string toString() const { return to_string(valeur()); }
};

/*! \class LitteraleEntiere
 * \brief Littérale des valeurs entières
 */
class LitteraleEntiere : public LitteraleNumerique {
    friend class OperateurNumerique;
    long int n;
public:
    LitteraleEntiere(long int v=0):n(v){}
    void negatif() { n=-n; }
    double valeur() const { return static_cast<double>(n); }
    void setValeur(long int v) { n=v; }
    string toString() const { return to_string(n); }
    bool operator==(long int v) const { return n==v; }
    bool operator!=(long int v) const { return n!=v; }
};

/*! \class LitteraleReelle
 * \brief Littérale des valeurs réelles
 */
class LitteraleReelle : public LitteraleNumerique {
    friend class OperateurNumerique;
    double x;
public:
    LitteraleReelle(double v=0):x(v){}
    void negatif() { x=-x; }
    double valeur() const { return (x); }

};

/*! \class LitteraleRationnelle
 * \brief Littérale des valeurs rationnelles
 */
class LitteraleRationnelle : public LitteraleNumerique {
    friend class OperateurNumerique;
    LitteraleEntiere num;
    LitteraleEntiere den;
public:
    LitteraleRationnelle(LitteraleEntiere n=0, LitteraleEntiere d=1):num(n),den(d){
        if(d==0) throw LitteraleException("Fraction au dénominateur égal à 0 impossible");
        simplifier();
    }
    void negatif() { num.negatif(); }
    double valeur() const { return num.valeur()/den.valeur(); }
    string toString() const;
    void simplifier();
    static long int pgcd(long int a, long int b);
};

/*************************
 *  LitteraleComplexe
 ************************/

/*! \class LitteraleComplexe
 * \brief Littérale des valeurs complexes
 */
class LitteraleComplexe : public Litterale {
    friend class OperateurNumerique;
    LitteraleNumerique* reel;
    LitteraleNumerique* imag;
public:
    LitteraleComplexe(LitteraleNumerique* r, LitteraleNumerique* i):reel(r),imag(i){}
    LitteraleComplexe(const LitteraleComplexe& c);
    LitteraleNumerique* getPartReel() { return reel; }
    LitteraleNumerique* getPartImag() { return imag; }
    string toString() const;
};

/*************************
 *  LitteraleAtome
 ************************/

/*! \class LitteraleAtome
 * \brief Littérale atome
 */
class LitteraleAtome : public Litterale {
    string id;
    Litterale* valeur;
public:
    LitteraleAtome(string s):id(s),valeur(nullptr){}
    string getId() const { return id; }
    Litterale* getValeur() const { return valeur; }
    string toString() const { return id; }
    
    // Variables
    static vector<LitteraleAtome*> vars;
    static unsigned int getNbVars();
    static unsigned int getNbPrgs();
    static void addVar(LitteraleAtome* atome, Litterale* l);
    static void delVar(LitteraleAtome* atome);
    static LitteraleAtome* getVar(const string& s);
    static bool isVar(LitteraleAtome* atome);
};

/*************************
 *  LitteraleExpression
 ************************/

/*! \class LitteraleExpression
 * \brief Littérale expression
 */
class LitteraleExpression : public Litterale {
    friend class OperateurExpression;
    string exp;
public:
    LitteraleExpression(string s):exp(s){}
    string getExp() const { return exp; }
    void setExp(string e) { exp=e; }
    string toString() const { return "'"+exp+"'"; }
    LitteraleAtome* getAtome() const;
    Litterale* eval();
};

/*************************
 *  LitteraleProgramme
 ************************/

/*! \class LitteraleProgramme
 * \brief Littérale programme
 */
class LitteraleProgramme : public Litterale {
    friend class OperateurExpression;
    string prg;
public:
    LitteraleProgramme(string s):prg(s){}
    string getPrg() const { return prg; }
    void setPrg(string p) { prg=p; }
    string toString() const { return prg; }
    vector<string> getInstructions() const;
};

/*************************
 *  Factory
 ************************/

/*! \class FactoryLitterale
 * \brief Fabrique de Litterale: permet de construire facilement des objets Litterale
 */
class FactoryLitterale {
    FactoryLitterale(){};
    ~FactoryLitterale();
    FactoryLitterale(const FactoryLitterale&);
    void operator=(const FactoryLitterale&);
    
    struct Handler {
        FactoryLitterale* instance;
        Handler():instance(nullptr){};
        ~Handler(){ delete instance; };
    };
    
    static Handler handler;
    
    /*!
     *  \brief Vecteur contenant tous les objets Litterale et ses objets dérivés, créés pendant l'exécution du programme
     */
    vector<Litterale*> tl;
public:
    static FactoryLitterale& getInstance();
    static void deleteInstance();
    Litterale* copie(Litterale* l);
    LitteraleEntiere* newLitteraleEntiere(long int v=0);
    LitteraleEntiere* newLitteraleEntiere(LitteraleEntiere a);
    LitteraleReelle* newLitteraleReelle(double v=0);
    LitteraleReelle* newLitteraleReelle(LitteraleReelle a);
    LitteraleRationnelle* newLitteraleRationnelle(LitteraleEntiere n=0, LitteraleEntiere d=0);
    LitteraleRationnelle* newLitteraleRationnelle(LitteraleRationnelle a);
	LitteraleComplexe* newLitteraleComplexe(LitteraleNumerique* r, LitteraleNumerique* i);
    LitteraleComplexe* newLitteraleComplexe(LitteraleComplexe& c);
    LitteraleAtome* newLitteraleAtome(string s);
    LitteraleExpression* newLitteraleExpression(string s);
    LitteraleProgramme* newLitteraleProgramme(string s);
    LitteraleProgramme* newLitteraleProgramme(LitteraleProgramme p);
};

#endif
