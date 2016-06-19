#ifndef Interpreteur_h
#define Interpreteur_h
/*!
 * \file Interpreteur.h
 * \brief L'interpréteur
 */

#include <string>
#include <regex>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include "Litterale.h"
#include "Operateur.h"
#include "Exceptions.h"

using namespace std;

/*! \class Interpreteur
 * \brief Ensembles de méthodes qui permettent d'interpréter les commandes de l'utilisateur
 */
class Interpreteur {
    Interpreteur():fl(FactoryLitterale::getInstance()),fo(FactoryOperateur::getInstance()){};
    ~Interpreteur(){};
    Interpreteur(const Interpreteur&);
    void operator=(const Interpreteur&);
    
    struct Handler {
        Interpreteur* instance;
        Handler():instance(nullptr){};
        ~Handler(){ delete instance; };
    };
    
    static Handler handler;
    
    FactoryLitterale& fl;
    FactoryOperateur& fo;
    
public:
    static const map<string, string> regexs;
    
    static Interpreteur& getInstance();
    static void deleteInstance();
    
    vector<string>& split(const string &s, char delim, vector<string>& elems) const;
    vector<string> split(const string &s, char delim) const;
    vector<string> split2(const string &s, string rgx_str="\\s+") const;
    vector<string> split3(const string &s, string rgx_str) const;
    vector<string> getOperandes(const string& s) const;
    bool isCommandeValide(const string& s) const;
    bool isLitterale(const string& s) const;
    bool isOperateur(const string& s) const;
    Litterale* toLitterale(const string& s) const;
    Operateur* toOperateur(const string& s) const;
};

#endif
