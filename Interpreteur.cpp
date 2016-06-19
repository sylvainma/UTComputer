#include "Interpreteur.h"

/*************************
 *  Interpreteur
 ************************/

const map<string, string> Interpreteur::regexs = {
    {"LitteraleEntiere", "-?[[:digit:]]+"},
	{"LitteraleReelle","(-?[[:digit:]]+)(\\.|\\,)([[:digit:]]+)"},
    {"LitteraleRationnelle", "-?[[:digit:]]+/-?[[:digit:]]+"},
    {"LitteraleComplexe","((?:(?:[+-]?\\d+(?:(?:\\.\\d+)|(?:\\/\\d+))?)\\$[+-]?(?:\\d+(?:(?:\\.\\d+)|(?:\\/\\d+))?)))"},
    {"LitteraleAtome","([[:upper:]]{1})([[:upper:]]|[[:digit:]])*"},
    {"LitteraleExpression","'([^']+)'"},
    {"LitteraleProgramme","\\[(.*)\\]"}
};

vector<string>& Interpreteur::split(const string &s, char delim, vector<string>& elems) const {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        if(item.find_first_not_of(' ') != std::string::npos) {
            elems.push_back(item);
        }
    }
    return elems;
}

/*!
 *  \brief Fonctions qui permettent de découper en tokens une string selon un caractère délimitateur.
 *  Source: http://stackoverflow.com/questions/409348/iteration-over-stdvector-unsigned-vs-signed-index-variable
 *  \param La chaîne de caractère à découper, le délimitateur
 */
vector<string> Interpreteur::split(const string &s, char delim) const {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

/*!
 *  \brief Split plus flexible: split de la string avec un délimitateur regex
 *  Source: http://stackoverflow.com/questions/16749069/c-split-string-by-regex 2ème et 3ème réponse
 *  \param La chaîne de caractère à découper, le délimitateur sous forme de regex
 */
vector<string> Interpreteur::split2(const string &s, string rgx_str) const {
    vector<string> tokens;
    regex rgx(rgx_str);
    
    sregex_token_iterator iter(s.begin(), s.end(), rgx, -1);
    sregex_token_iterator end;
    
    while (iter != end)  {
        tokens.push_back(*iter);
        ++iter;
    }
    
    return tokens;
}

/*!
 *  \brief Split encore plus flexible: extrait les morceaux de string qui match la regex
 *  Source: http://stackoverflow.com/questions/16749069/c-split-string-by-regex 2ème et 3ème réponse
 *  \param La chaîne de caractère à découper, le pattern sous forme de regex
 */
vector<string> Interpreteur::split3(const string &s, string rgx_str) const {
    vector<string> tokens;
    
    string tmp(s);
    smatch m;
    regex e(rgx_str);
    
    while (std::regex_search (tmp,m,e)) {
        if(m[1]!=' ' && m[1]!='\r' && m[1]!='\n' && m[1]!='\t' && m[1]!='\f') {
            cout<<endl<<"Enregistré: "<<m[1]<<endl;
            tokens.push_back(m[1]);
        }
        tmp = m.suffix().str();
    }
    
    return tokens;
}

/*!
 *  \brief Vérifie que toutes les opérandes entrées sont correctes
 *  \param La chaîne de caractère contenant la suite d'opérandes
 */
bool Interpreteur::isCommandeValide(const string& s) const {
    
    // Mettre de la forme ^((?: ... )*)\s*$
    regex r("^((?:\\s*(?:(?:(?:[+-]?\\d+(?:(?:\\.\\d+)|(?:\\/\\d+))?)?\\$[+-]?(?:\\d+(?:(?:\\.\\d+)|(?:\\/\\d+))?)?)|(?:(?:\\d+(?:\\.\\d+)?)\\/(?:\\d+(?:\\.\\d+)?))|(?:[+-]?\\d+(?:\\.\\d+)?)|(?:[A-Z0-9]+)|(?:'[^']+')|(?:\\[.*\\])|(?:!=|=<|=>|[\\+\\-\\*\\/\\$=<>])))+)\\s*$");
    
    if (regex_match(s, r)) return true;
    else return false;
}

/*!
 *  \brief Retourne un vecteur contenant chaque opérande de la string.
 *  Lance une exception s'il n'y a pas pas que des opérandes dans la string.
 *  https://regex101.com/r/mV9jO4/8
 *  \param La chaîne de caractère contenant la suite d'opérandes
 */
vector<string> Interpreteur::getOperandes(const string& s) const {
    if(!isCommandeValide(s)) throw InterpreteurException("Suite d'opérandes invalide: "+s);

    string rgx="\\s*((?:(?:[+-]?\\d+(?:(?:\\.\\d+)|(?:\\/\\d+))?)?\\$[+-]?(?:\\d+(?:(?:\\.\\d+)|(?:\\/\\d+))?)?)|(?:(?:\\d+(?:\\.\\d+)?)\\/(?:\\d+(?:\\.\\d+)?))|(?:[+-]?\\d+(?:\\.\\d+)?)|(?:[A-Z0-9]+)|(?:'[^']+')|(?:\\[.*\\])|(?:!=|=<|=>|[\\+\\-\\*\\/\\$=<>]))";

    vector<string> tokens = split3(s, rgx);
    
    for(std::vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        if(!isLitterale(*it) && !isOperateur(*it))
            throw InterpreteurException("Opérande inconnue: "+*it);
    }
    
    return tokens;
}

/*!
 *  \brief Renvoie vrai si la string correspond à une Litterale
 *  \param La chaîne de caractère contenant la littérale
 */
bool Interpreteur::isLitterale(const string& s) const {
    
    regex r;
    
    // Littérales entières
    r=regexs.at("LitteraleEntiere");
    if(regex_match(s, r)) return true;
    
    // Littérales réelles
    r=regexs.at("LitteraleReelle");
    if(regex_match(s, r)) return true;
    
    // Littérales rationnelle
    r=regexs.at("LitteraleRationnelle");
    if(regex_match(s, r)) return true;
    
    // Littérales Complexe
    r = regexs.at("LitteraleComplexe");
    if (regex_match(s, r)) return true;
    
    // Littérales atomes
    r=regexs.at("LitteraleAtome");
    if(regex_match(s, r) && !isOperateur(s)) return true;
    
    // Littérales expressions
    r=regexs.at("LitteraleExpression");
    if(regex_match(s, r)) return true;
    
    // Littérales programmes
    r=regexs.at("LitteraleProgramme");
    if(regex_match(s, r)) return true;
    
    return false;
}

/*!
 *  \brief Transforme une string en Litterale, si c'est une litterale
 *  \param La chaîne de caractère contenant la littérale
 */
Litterale* Interpreteur::toLitterale(const string& s) const {
    
    Litterale* l=nullptr;
    if(!isLitterale(s)) return nullptr;
    
    // Littérales entières
    if(regex_match(s, regex(regexs.at("LitteraleEntiere")))) {
        l=fl.newLitteraleEntiere(stoi(s));
    }
    // Littérales réelles
    else if(regex_match(s, regex(regexs.at("LitteraleReelle")))) {
		vector<string> tokens = split(s, ',');
		if (tokens.size() == 1) {
			l = fl.newLitteraleReelle(stod(s));
		}
		else {
			string ns;
			ns = tokens[0] + "." + tokens[1]; // ptite boucle pour autoriser la virgule je me faisais chier dans le train
			l = fl.newLitteraleReelle(stod(ns));
		}

    }
    // Littérales rationnelles
    else if(regex_match(s, regex(regexs.at("LitteraleRationnelle")))) {
        // Extraction numérateur et dénominateur
        LitteraleEntiere* entiers[2];
        vector<string> tokens=split(s, '/');
        
        // Vérifie que ce n'est pas seulement un entier (1 au dén)
        if(stoi(tokens[1])==1) {
            l=fl.newLitteraleEntiere(stoi(tokens[0]));
        }
        else {
            entiers[0]=fl.newLitteraleEntiere(stoi(tokens[0]));
            entiers[1]=fl.newLitteraleEntiere(stoi(tokens[1]));
            l=fl.newLitteraleRationnelle(*entiers[0], *entiers[1]);
            cout<<l->toString()<<endl;;
        }
	
    }
    //Littérales Complexes
    if (regex_match(s, regex(regexs.at("LitteraleComplexe")))) { //branche 'a+bi'
        LitteraleNumerique* r;
        LitteraleNumerique* i;
        
        vector<string> tokens=split(s, '$');
        //for(auto x:tokens) cout<<x<<endl;
        
        // Check que la partie réelle n'est pas vide, sinon on met 0
        if(isLitterale(tokens[0]))
            r=dynamic_cast<LitteraleNumerique*>(toLitterale(tokens[0]));
        else
            r=dynamic_cast<LitteraleNumerique*>(fl.newLitteraleEntiere(0));
        
        // Check que la partie imaginaire n'est pas vide, sinon on met 0
        if(isLitterale(tokens[1]))
            i=dynamic_cast<LitteraleNumerique*>(toLitterale(tokens[1]));
        else
            i=dynamic_cast<LitteraleNumerique*>(fl.newLitteraleEntiere(0));
        
        l = fl.newLitteraleComplexe(r, i);
    }
    // Littérale atome
    else if(regex_match(s, regex(regexs.at("LitteraleAtome"))) && !isOperateur(s)) {
        LitteraleAtome* var=LitteraleAtome::getVar(s);
        if (var) {
            // Si c'est une variable, on retourne l'atome
            l=var;
        }
        else {
            // Sinon c'est une littérale atome non variable, on retour une littérale expression
            l=fl.newLitteraleExpression(s);
        }
    }
    // Littérale expression
    else if(regex_match(s, regex(regexs.at("LitteraleExpression")))) {
        
        string exp(s);
        
        // Supression des quotes
        exp.erase(remove(exp.begin(), exp.end(), '\''), exp.end());
        
        // Supression des espaces
        exp.erase(remove(exp.begin(), exp.end(), ' '), exp.end());
        
        l=fl.newLitteraleExpression(exp);
    }
    // Littérale programme
    else if(regex_match(s, regex(regexs.at("LitteraleProgramme")))) {
        l=fl.newLitteraleProgramme(s);
    }

    return l;
}

/*!
 *  \brief Renvoie vrai si la string correspond à un Operateur
 *  \param La chaîne de caractère contenant l'opérateur
 */
bool Interpreteur::isOperateur(const string& s) const {
    map<string, const map<string,unsigned int>>::const_iterator outer_iter;
    for(outer_iter=Operateur::liste.begin(); outer_iter!=Operateur::liste.end(); ++outer_iter) {
        map<string, unsigned int>::const_iterator inner_iter;
        inner_iter=outer_iter->second.find(s);
        if(inner_iter!=outer_iter->second.end()) return true;
    }
    return false;
}

/*!
 *  \brief Transforme une string en Operateur, si c'est un Operateur
 *  \param La chaîne de caractère contenant l'opérateur
 */
Operateur* Interpreteur::toOperateur(const string& s) const {
    
    Operateur* o=nullptr;
    if(!isOperateur(s)) return nullptr;
    
    // Itérateur pour parcourir la map d'opérateurs
    map<string, unsigned int>::const_iterator it;
    
    // Opérateurs numériques
    it=Operateur::liste.at("numeriques").find(s);
    if (it!=Operateur::liste.at("numeriques").end()) o=fo.newOperateurNumerique(s);
    
    // Opérateurs de pile
    it=Operateur::liste.at("pile").find(s);
    if (it!=Operateur::liste.at("pile").end()) o=fo.newOperateurPile(s);
    
    // Opérateurs d'expression
    it=Operateur::liste.at("expression").find(s);
    if (it!=Operateur::liste.at("expression").end()) o=fo.newOperateurExpression(s);
	
	//Opérateur Logiques
	it = Operateur::liste.at("logiques").find(s);
	if (it != Operateur::liste.at("logiques").end()) o = fo.newOperateurLogique(s);
    return o;
}

/*************************
 *  Singleton
 ************************/

/*
 *  Initialisation du handler (singleton)
 */
Interpreteur::Handler Interpreteur::handler;

/*
 *  Retourne l'instance singleton
 */
Interpreteur& Interpreteur::getInstance() {
    if(handler.instance==nullptr) handler.instance = new Interpreteur;
    return *handler.instance;
}

/*
 *  Supprime le singleton dans le cas où c'est souhaité avant la fin du programme
 */
void Interpreteur::deleteInstance() {
    delete handler.instance;
    handler.instance=nullptr;
}
