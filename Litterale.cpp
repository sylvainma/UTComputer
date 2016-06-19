#include "Litterale.h"
#include "Interpreteur.h"

/*************************
 *  LitteraleRationnelle
 ************************/

/*!
 *  \brief Simplifie la fraction arithmétiquement
 */
void LitteraleRationnelle::simplifier() {
    // Simplfication de signe
    if((num.valeur()<0 && den.valeur()<0) || (num.valeur()>0 && den.valeur()<0)) {
        num.setValeur(-1*static_cast<long int>(num.valeur()));
        den.setValeur(-1*static_cast<long int>(den.valeur()));
    }
    
    // Simplification fractionelle
    long int temp = pgcd(static_cast<long int>(num.valeur()), static_cast<long int>(den.valeur()));
    num.setValeur(static_cast<long int>(num.valeur()) / abs(temp));
    den.setValeur(static_cast<long int>(den.valeur()) / abs(temp));
}

/*!
 *  \brief PGCD de deux entiers
 */
long int LitteraleRationnelle::pgcd(long int a, long int b) {
    if (b == 0)
        return a;
    else
        return pgcd(b, a%b);
}

string LitteraleRationnelle::toString() const {
    string s;
    s=num.toString();
    if(den!=1) s+="/"+den.toString();
    return s;
}

/*************************
 *  LitteraleComplexe
 ************************/

LitteraleComplexe::LitteraleComplexe(const LitteraleComplexe& c) {

    FactoryLitterale& fl = FactoryLitterale::getInstance();

    if(c.reel!=nullptr)
        reel=dynamic_cast<LitteraleNumerique*>(fl.copie(c.reel));
    if(c.imag!=nullptr)
        imag=dynamic_cast<LitteraleNumerique*>(fl.copie(c.imag));
}

string LitteraleComplexe::toString() const {
    string s;
    s = reel->toString();
	s += "$";
    s += imag->toString();

	return s;
}

/*************************
 *  Variables
 ************************/

/*!
 *  \brief Liste pour stocker les atomes qui sont liés à une littérale, c'est à dire les atomes qui sont des variables
 */
vector<LitteraleAtome*> LitteraleAtome::vars;

/*!
 *  \brief Calcule le nombre de variables de non programmes
 */
unsigned int LitteraleAtome::getNbVars() {
    unsigned int n=0;
    for(vector<LitteraleAtome*>::iterator it=vars.begin(); it!=vars.end(); ++it) {
        LitteraleProgramme* prg = dynamic_cast<LitteraleProgramme*>((*it)->getValeur());
        if(!prg) n++;
    }
    return n;
}

/*!
 *  \brief Calcule le nombre de variables de programmes
 */
unsigned int LitteraleAtome::getNbPrgs() {
    unsigned int n=0;
    for(vector<LitteraleAtome*>::iterator it=vars.begin(); it!=vars.end(); ++it) {
        LitteraleProgramme* prg = dynamic_cast<LitteraleProgramme*>((*it)->getValeur());
        if(prg) n++;
    }
    return n;
}

/*!
 *  \brief Ajoute une littérale atome en tant que variable d'une littérale passée en argument
 */
void LitteraleAtome::addVar(LitteraleAtome* atome, Litterale* l) {
    // On vérifie que l'atome en question n'existe pas déjà dans la liste vars.
    // On vérifie également que le même nom n'existe pas déjà.
    // Si l'une des deux vérifications précédentes n'est pas vérifiée, alors on écrase (on supprime la variable préexistente et on ajoute la nouvelle).
    
    // Parcourt de la liste
    for(vector<LitteraleAtome*>::iterator it=vars.begin(); it!=vars.end(); ++it) {
        // Même pointeur atome OU même nom d'atome -> on delete de la liste
        if(*it==atome || (*it)->getId()==atome->getId()) delVar(*it);
    }
    
    // On ajoute l'atome dans vars en tant que variable pour la littérale
    vars.push_back(atome);
    
    // On lie l'atome à sa littérale
    atome->valeur=l;
}

/*!
 *  \brief Supprimer une variable de la liste dont l'atome est passé en argument
 */
void LitteraleAtome::delVar(LitteraleAtome* atome) {
    for(vector<LitteraleAtome*>::iterator it=vars.begin(); it!=vars.end(); ++it) {
        if(*it==atome) {
            // On supprime le lien de variable entre l'atome et la littérale
            (*it)->valeur=nullptr;
            // On supprime l'atome du tableau de variables
            vars.erase(it);
            break;
        }
    }
}

/*!
 *  \brief Retourne la littérale atome dont le nom de variable est passé en argument sous forme de string.
 *  Si aucune variable n'a ce nom/cet identifiant, alors la méthode renvoi un pointeur null
 */
LitteraleAtome* LitteraleAtome::getVar(const string& s) {
    for(vector<LitteraleAtome*>::iterator it=vars.begin(); it!=vars.end(); ++it)
        if((*it)->id==s) return (*it);
    return nullptr;
}

/*!
 *  \brief Vérifie que la littérale atome est une variable
 */
bool LitteraleAtome::isVar(LitteraleAtome* atome) {
    for(vector<LitteraleAtome*>::iterator it=vars.begin(); it!=vars.end(); ++it)
        if((*it)==atome) return true;
    return false;
}

/*************************
 *  LitteraleExpression
 ************************/

/*!
 *  \brief Renvoi un nouvel objet atome si l'expression est en réalité un atome, sinon nullptr
 */
LitteraleAtome* LitteraleExpression::getAtome() const {
    string r = Interpreteur::regexs.at("LitteraleAtome");
    if(regex_match(exp, regex(r))) {
        FactoryLitterale& fl = FactoryLitterale::getInstance();
        return fl.newLitteraleAtome(exp);
    }
    else return nullptr;
}

/*!
 *  \brief Calcule la littérale résultante de l'expression
 */
Litterale* LitteraleExpression::eval() {
    
    Interpreteur& inter=Interpreteur::getInstance();
    FactoryLitterale& fl=FactoryLitterale::getInstance();
    FactoryOperateur& fo=FactoryOperateur::getInstance();
    
    string s(exp);
    regex e;
    smatch m;

    // Traitement des opérateurs unaires s'appliquant à l'expression
    regex opUnaire("([A-Z](?:[A-Z]|[0-9])*)\\(([^,\\(\\)]+)\\)");
    while (std::regex_search (s,m,opUnaire)) {
        cout<<m[0]<<" "<<m[1]<<" "<<m[2]<<endl;
        // m[1] l'opérateur et m[2] l'argument
        if(inter.isOperateur(m[1])) {
            Litterale* l1 = fl.newLitteraleExpression(m[2])->eval();
            Operateur* op= inter.toOperateur(m[1]);
            return op->applyUnaire(l1);
        }
    }
    
    // Traitement des opérateurs binaires s'appliquant à l'expression
    regex opBinaire("([A-Z](?:[A-Z]|[0-9])*)\\(([^,\\(]+),([^,\\(\\)]+)\\)");
    while (std::regex_search (s,m,opBinaire)) {
        //cout<<m[0]<<" "<<m[1]<<" "<<m[2]<<" "<<m[3]<<endl;
        // m[1] l'opérateur et m[2] l'argument 1 et m[2] l'argument 2
        if(inter.isOperateur(m[1])) {
            Litterale* l1 = fl.newLitteraleExpression(m[2])->eval();
            Litterale* l2 = fl.newLitteraleExpression(m[3])->eval();
            Operateur* op= inter.toOperateur(m[1]);
            return op->applyBinaire(l1,l2);
        }
    }

    // Remplacement des variables par leur contenu
    regex atome("((?:[[:upper:]]{1})(?:[[:upper:]]|[[:digit:]])*)");
    while (regex_search(s,m,atome)) {
        LitteraleAtome* var = LitteraleAtome::getVar(m[1]);
        if(var) {
            Litterale* c = var->getValeur();
            s.replace(s.find(m[0]), m[0].length(), c->toString());
        }
    }
    
    // Traitement des groupes parenthésés
    // Le but est de ne se retrouver qu'avec
    // une suite d'opérations arithmétiques sans parenthèses
    regex paren("\\(([^\\(]*?)\\)");
    while (regex_search(s,m,paren)) {
        //cout<<m[0]<<" va être remplcé par "<<m[1]<<endl;
        Litterale* c = fl.newLitteraleExpression(m[1])->eval();
        //cout<<"av. "<<s<<endl;
        s.replace(s.find(m[0]), m[0].length(), c->toString());
        //cout<<"apr. "<<s<<endl;
    }
    
    // Traitement des + et -
    regex somme("([^\\+]*[1-9])([\\+-])(.+)");
    if (regex_search(s,m,somme)) {
        //cout<<m[1]<<" "<<m[2]<<" "<<m[3]<<endl;
            
        // Conversion en littérale des deux cotés
        Litterale* l1=fl.newLitteraleExpression(m[1])->eval();  // On converti en littérale la partie gauche
        Litterale* l2=fl.newLitteraleExpression(m[3])->eval();  // On converti en littérale la partie droite
        
        // Création de l'opérateur
        OperateurNumerique* op=fo.newOperateurNumerique(m[2]);
        
        // Application du résultat
        Litterale* l3=op->applyBinaire(l1, l2);
        return l3;
    }
    
    // Traitement des * et /
    regex produit("([^\\+]*[1-9])([\\*/])(.+)");
    if (regex_search(s,m,produit)) {
        //cout<<m[1]<<" "<<m[2]<<" "<<m[3]<<endl;
        
        // Conversion en littérale des deux cotés
        Litterale* l1=fl.newLitteraleExpression(m[1])->eval();  // On converti en littérale la partie gauche
        Litterale* l2=fl.newLitteraleExpression(m[3])->eval();  // On converti en littérale la partie droite
        
        // Création de l'opérateur
        OperateurNumerique* op=fo.newOperateurNumerique(m[2]);
        
        // Application du résultat
        Litterale* l3=op->applyBinaire(l1, l2);
        return l3;
    }

    // On vérifie que ce n'est pas tout simplement une littérale
    if(inter.isLitterale(s)) {
        return inter.toLitterale(s);
    }
    
    return nullptr;
}

/*************************
 *  LitteraleProgramme
 ************************/

/*!
 *  \brief Retourne les instructions présentes dans le programme
 */
vector<string> LitteraleProgramme::getInstructions() const {
    
    Interpreteur& inter=Interpreteur::getInstance();
    string s(prg);
    
    // Supprime les []
    s.erase(0,2);
    s.erase(s.length()-1,2);

    return inter.getOperandes(s);
}

/*************************
 *  Factory
 ************************/

/*!
 *  \brief Créé une copie de la littérale, c'est à dire une allocation dynamique puis copie des attributs de la source
 */
Litterale* FactoryLitterale::copie(Litterale* l) {
    
    Litterale* copie=nullptr;
    
    LitteraleEntiere* lEntiere = dynamic_cast<LitteraleEntiere*>(l);
    LitteraleReelle* lReelle = dynamic_cast<LitteraleReelle*>(l);
    LitteraleRationnelle* lRationnelle = dynamic_cast<LitteraleRationnelle*>(l);
    LitteraleComplexe* lComplexe = dynamic_cast<LitteraleComplexe*>(l);
    LitteraleExpression* lExpression = dynamic_cast<LitteraleExpression*>(l);
    LitteraleProgramme* lProgramme = dynamic_cast<LitteraleProgramme*>(l);
    
    if(lEntiere) {
        copie=new LitteraleEntiere(*lEntiere);
    }
    else if (lReelle) {
        copie=new LitteraleReelle(*lReelle);
    }
    else if (lRationnelle) {
        copie=new LitteraleRationnelle(*lRationnelle);
    }
    else if (lComplexe) {
        copie=new LitteraleComplexe(*lComplexe);
    }
    else if (lExpression) {
        copie=new LitteraleExpression(*lExpression);
    }
    else if (lProgramme) {
        copie=new LitteraleProgramme(*lProgramme);
    }
    else {
        throw LitteraleException("Impossible de copier, littérale inconnue");
    }
    
    tl.push_back(copie);
    return copie;
}

/*!
 *  \brief Allocation d'une littérale entière
 */
LitteraleEntiere* FactoryLitterale::newLitteraleEntiere(long int v) {
    LitteraleEntiere* l=new LitteraleEntiere(v);
    tl.push_back(l);
    return l;
}

/*!
 *  \brief Allocation d'une littérale entière par copie
 */
LitteraleEntiere* FactoryLitterale::newLitteraleEntiere(LitteraleEntiere a) {
    LitteraleEntiere* l=new LitteraleEntiere(a);
    tl.push_back(l);
    return l;
}

/*!
 *  \brief Allocation d'une littérale réelle
 */
LitteraleReelle* FactoryLitterale::newLitteraleReelle(double v) {
    LitteraleReelle* l=new LitteraleReelle(v);
    tl.push_back(l);
    return l;
}

/*!
 *  \brief Allocation d'une littérale réelle par copie
 */
LitteraleReelle* FactoryLitterale::newLitteraleReelle(LitteraleReelle a) {
    LitteraleReelle* l=new LitteraleReelle(a);
    tl.push_back(l);
    return l;
}

/*!
 *  \brief Allocation d'une littérale rationnelle
 */
LitteraleRationnelle* FactoryLitterale::newLitteraleRationnelle(LitteraleEntiere n, LitteraleEntiere d) {
    LitteraleRationnelle* l=new LitteraleRationnelle(n,d);
    tl.push_back(l);
    return l;
}

/*!
 *  \brief Allocation d'une littérale rationnelle par copie
 */
LitteraleRationnelle* FactoryLitterale::newLitteraleRationnelle(LitteraleRationnelle a) {
    LitteraleRationnelle* l=new LitteraleRationnelle(a);
    tl.push_back(l);
    return l;
}

/*!
 *  \brief Allocation d'une littérale complexe
 */
LitteraleComplexe* FactoryLitterale::newLitteraleComplexe(LitteraleNumerique* r, LitteraleNumerique* i) {
	LitteraleComplexe* l= new LitteraleComplexe(r, i);
	tl.push_back(l);
	return l;
}

/*!
 *  \brief Allocation d'une littérale complexe par copie
 */
LitteraleComplexe* FactoryLitterale::newLitteraleComplexe(LitteraleComplexe& c) {
    LitteraleComplexe* l= new LitteraleComplexe(c);
    tl.push_back(l);
    return l;
}

/*!
 *  \brief Allocation d'une littérale atome
 */
LitteraleAtome* FactoryLitterale::newLitteraleAtome(string s) {
    LitteraleAtome* l= new LitteraleAtome(s);
    tl.push_back(l);
    return l;
}

/*!
 *  \brief Allocation d'une littérale expression
 */
LitteraleExpression* FactoryLitterale::newLitteraleExpression(string s) {
    LitteraleExpression* l= new LitteraleExpression(s);
    tl.push_back(l);
    return l;
}

/*!
 *  \brief Allocation d'une littérale programme
 */
LitteraleProgramme* FactoryLitterale::newLitteraleProgramme(string s) {
    LitteraleProgramme* l= new LitteraleProgramme(s);
    tl.push_back(l);
    return l;
}

/*!
 *  \brief Allocation d'une littérale programme par copie
 */
LitteraleProgramme* FactoryLitterale::newLitteraleProgramme(LitteraleProgramme p) {
    LitteraleProgramme* l= new LitteraleProgramme(p);
    tl.push_back(l);
    return l;
}

/*************************
 *  Factory : Singleton
 ************************/

/*
 *  Initialisation du handler (singleton)
 */
FactoryLitterale::Handler FactoryLitterale::handler;

/*
 *  Retourne l'instance singleton
 */
FactoryLitterale& FactoryLitterale::getInstance() {
    if(handler.instance==nullptr) handler.instance = new FactoryLitterale;
    return *handler.instance;
}

/*
 *  Supprime le singleton dans le cas où c'est souhaité avant la fin du programme
 */
void FactoryLitterale::deleteInstance() {
    delete handler.instance;
    handler.instance=nullptr;
}

/*
 *  Suppression de toutes les Litterales créées depuis le début
 */
FactoryLitterale::~FactoryLitterale() {
    for(vector<Litterale*>::const_iterator it=tl.begin(); it!=tl.end(); ++it) {
        delete *it;
    }
}

