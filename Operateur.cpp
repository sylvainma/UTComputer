#include "Operateur.h"
#include "Interpreteur.h"

/*************************
 *  Opérateur
 ************************/

Litterale* Operateur::lastarg[2]={nullptr,nullptr};

/*!
 *  \brief Recensement des tous les opérateurs connus du programme avec leur multiplicité respective
 */
const map<string, const map<string,unsigned int>> Operateur::liste={
    {"numeriques",
        {{"NEG",1}, {"NUM",1}, {"DEN",1}, {"RE",1}, {"IM",1}, {"+",2}, {"-",2}, {"*",2}, {"/",2}, {"DIV",2}, {"MOD",2}, {"$",2}}
    },
    {"pile",
        {{"UNDO",0}, {"REDO",0}, {"CLEAR",0}, {"DROP",1}, {"SWAP",2}, {"DUP",1}, {"STO",2}, {"FORGET",1},{"LASTOP",0},{"LASTARGS",0}}
    },
    {"expression",
        {{"EVAL",1}}
    },
    {"logiques",
        {{"<",2}, {">",2 },{ "=<",2 },{ "=>",2 }, { "=",2 }, { "!=",2 }, { "AND",2 },{ "OR",2 },{ "NOT",1 }, { "IFT",2 } }
    }
};

Operateur::Operateur(const string o):op(o),fl(FactoryLitterale::getInstance()),mul(0) {

    bool found=false;

    // On cherche la multiplicité de l'opérateur
    map<string, const map<string,unsigned int>>::const_iterator outer_iter;
    for(outer_iter=liste.begin(); outer_iter!=liste.end(); ++outer_iter) {
        for(map<string, unsigned int>::const_iterator inner_iter=outer_iter->second.begin(); inner_iter!=outer_iter->second.end(); ++inner_iter) {
            if(op==inner_iter->first) {
                mul=inner_iter->second;
                found=true;
                break;
            }
        }
    }
    
    // Si on n'a pas trouvé la multiplicité, c'est que o n'est pas un opérateur
    if(!found) throw OperateurException("L'opérateur n'existe pas: "+o);
    
    // Naviguer dans une sous-map:
    // http://stackoverflow.com/questions/4844886/how-to-loop-through-a-c-map
}

/*!
 *  \brief Vérifie si un nombre de type double est en réalité un entier
 */
bool Operateur::isEntier(const double& x) {
    return x==floor(x);
}

/*!
 *  \brief Vérifie si un nombre de type double est en réalité un réel, c'est à dire que ce n'est pas tout simplement un entier
 */
bool Operateur::isReel(const double& x) {
    if(!isEntier(x)) return true;
    else return false;
}

/*************************
 *  Opérateurs numériques
 ************************/

/*
 *  Additions
 */

LitteraleNumerique* OperateurNumerique::addition(LitteraleEntiere* x, LitteraleEntiere* y) {
    return fl.newLitteraleEntiere(x->n+y->n);
}

LitteraleNumerique* OperateurNumerique::addition(LitteraleEntiere* x, LitteraleReelle* y) {
    double cv=x->n+y->x;
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleReelle(cv);
}

LitteraleNumerique* OperateurNumerique::addition(LitteraleEntiere* x, LitteraleRationnelle* y) {
    long int num = y->den.n*x->n + 1*y->num.n;
    long int den = 1*y->den.n;
    double cv=static_cast<double>(num)/static_cast<double>(den);
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleRationnelle(num, den);
}

LitteraleNumerique* OperateurNumerique::addition(LitteraleReelle* x, LitteraleReelle* y) {
    double cv=x->x+y->x;
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleReelle(cv);
}

LitteraleNumerique* OperateurNumerique::addition(LitteraleReelle* x, LitteraleRationnelle* y) {
    double cv=x->x+y->valeur();
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleReelle(cv);
}

LitteraleNumerique* OperateurNumerique::addition(LitteraleRationnelle* x, LitteraleRationnelle* y) {
    long int num = y->den.n*x->num.n + x->den.n*y->num.n;
    long int den = x->den.n*y->den.n;
    double cv=static_cast<double>(num)/static_cast<double>(den);
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleRationnelle(num, den);
}

/*
 *  Soustractions
 */

LitteraleNumerique* OperateurNumerique::soustraction(LitteraleEntiere* x, LitteraleEntiere* y) {
    return fl.newLitteraleEntiere(x->n-y->n);
}

LitteraleNumerique* OperateurNumerique::soustraction(LitteraleEntiere* x, LitteraleReelle* y) {
    double cv=x->n-y->x;
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleReelle(cv);
}

LitteraleNumerique* OperateurNumerique::soustraction(LitteraleEntiere* x, LitteraleRationnelle* y) {
    long int num = y->den.n*x->n - 1*y->num.n;
    long int den = 1*y->den.n;
    double cv=static_cast<double>(num)/static_cast<double>(den);
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleRationnelle(num, den);
}

LitteraleNumerique* OperateurNumerique::soustraction(LitteraleReelle* x, LitteraleEntiere* y) {
    double cv=x->x-y->n;
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleReelle(cv);
}

LitteraleNumerique* OperateurNumerique::soustraction(LitteraleReelle* x, LitteraleReelle* y) {
    double cv=x->x-y->x;
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleReelle(cv);
}

LitteraleNumerique* OperateurNumerique::soustraction(LitteraleReelle* x, LitteraleRationnelle* y) {
    double cv=x->x-y->valeur();
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleReelle(cv);
}

LitteraleNumerique* OperateurNumerique::soustraction(LitteraleRationnelle* x, LitteraleEntiere* y) {
    long int num = x->num.n - y->n*x->den.n;
    long int den = x->den.n;
    double cv=static_cast<double>(num)/static_cast<double>(den);
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleRationnelle(num, den);
}

LitteraleNumerique* OperateurNumerique::soustraction(LitteraleRationnelle* x, LitteraleReelle* y) {
    double cv=x->valeur() - y->x;
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleReelle(cv);
}

LitteraleNumerique* OperateurNumerique::soustraction(LitteraleRationnelle* x, LitteraleRationnelle* y) {
    long int num = y->den.n*x->num.n - x->den.n*y->num.n;
    long int den = x->den.n*y->den.n;
    double cv=static_cast<double>(num)/static_cast<double>(den);
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleRationnelle(num, den);
}

/*
 *  Multiplications
 */

LitteraleNumerique* OperateurNumerique::multiplication(LitteraleEntiere* x, LitteraleEntiere* y) {
    return fl.newLitteraleEntiere(x->n*y->n);
}

LitteraleNumerique* OperateurNumerique::multiplication(LitteraleEntiere* x, LitteraleReelle* y) {
    double cv=x->n*y->x;
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleReelle(cv);
}

LitteraleNumerique* OperateurNumerique::multiplication(LitteraleEntiere* x, LitteraleRationnelle* y) {
    long int num = x->n*y->num.n;
    long int den = 1*y->den.n;
    double cv=static_cast<double>(num)/static_cast<double>(den);
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleRationnelle(num, den);
}

LitteraleNumerique* OperateurNumerique::multiplication(LitteraleReelle* x, LitteraleReelle* y) {
    double cv=x->x*y->x;
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleReelle(cv);
}

LitteraleNumerique* OperateurNumerique::multiplication(LitteraleReelle* x, LitteraleRationnelle* y) {
    double cv=x->x*y->valeur();
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleReelle(cv);
}

LitteraleNumerique* OperateurNumerique::multiplication(LitteraleRationnelle* x, LitteraleRationnelle* y) {
    long int num = x->num.n*y->num.n;
    long int den = x->den.n*y->den.n;
    double cv=static_cast<double>(num)/static_cast<double>(den);
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleRationnelle(num, den);
}

/*
 *  Divisions
 */

LitteraleNumerique* OperateurNumerique::division(LitteraleEntiere* x, LitteraleEntiere* y) {
    if (y->valeur()==0) throw OperateurException("Division par 0 impossible");
    
    long int num = x->n;
    long int den = y->n;
    double cv=static_cast<double>(num)/static_cast<double>(den);
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleRationnelle(num, den);
}

LitteraleNumerique* OperateurNumerique::division(LitteraleEntiere* x, LitteraleReelle* y) {
    if (y->valeur()==0) throw OperateurException("Division par 0 impossible");
    
    double cv=x->n/y->x;
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleReelle(cv);
}

LitteraleNumerique* OperateurNumerique::division(LitteraleEntiere* x, LitteraleRationnelle* y) {
    if (y->valeur()==0) throw OperateurException("Division par 0 impossible");
    
    long int num = x->n*y->den.n;
    long int den = y->num.n;
    double cv=static_cast<double>(num)/static_cast<double>(den);
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleRationnelle(num, den);
}

LitteraleNumerique* OperateurNumerique::division(LitteraleReelle* x, LitteraleEntiere* y) {
    if (y->valeur()==0) throw OperateurException("Division par 0 impossible");
    
    double cv=x->x/y->n;
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleReelle(cv);
}

LitteraleNumerique* OperateurNumerique::division(LitteraleReelle* x, LitteraleReelle* y) {
    if (y->valeur()==0) throw OperateurException("Division par 0 impossible");
    
    double cv=x->x/y->x;
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleReelle(cv);
}

LitteraleNumerique* OperateurNumerique::division(LitteraleReelle* x, LitteraleRationnelle* y) {
    if (y->valeur()==0) throw OperateurException("Division par 0 impossible");
    
    double cv=x->x/y->valeur();
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleReelle(cv);
}

LitteraleNumerique* OperateurNumerique::division(LitteraleRationnelle* x, LitteraleEntiere* y) {
    if (y->valeur()==0) throw OperateurException("Division par 0 impossible");
    
    long int num = x->num.n;
    long int den = y->n*x->den.n;
    double cv=static_cast<double>(num)/static_cast<double>(den);
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleRationnelle(num, den);
}

LitteraleNumerique* OperateurNumerique::division(LitteraleRationnelle* x, LitteraleReelle* y) {
    if (y->valeur()==0) throw OperateurException("Division par 0 impossible");
    
    double cv=x->valeur()/y->x;
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleReelle(cv);
}

LitteraleNumerique* OperateurNumerique::division(LitteraleRationnelle* x, LitteraleRationnelle* y) {
    if (y->valeur()==0) throw OperateurException("Division par 0 impossible");
    
    long int num = x->num.n*y->den.n;
    long int den = x->den.n*y->num.n;
    double cv=static_cast<double>(num)/static_cast<double>(den);
    if(isEntier(cv))
        return fl.newLitteraleEntiere(cv);
    else
        return fl.newLitteraleRationnelle(num, den);
}

/*
 *  Application des opérateurs
 */

/*
 *  OP l1
 */

Litterale* OperateurNumerique::applyUnaire(Litterale* l1) {
    
    Litterale* c=nullptr;
    
    LitteraleNumerique* aNum=dynamic_cast<LitteraleNumerique*>(l1);
    LitteraleComplexe* aCom=dynamic_cast<LitteraleComplexe*>(l1);
    LitteraleExpression* aExp=dynamic_cast<LitteraleExpression*>(l1);
    
    if(aNum) {
        
        if(op=="NEG") {
            aNum->negatif();
            c=aNum;
        }
        else if (op=="NUM") {
            
            LitteraleEntiere* xEntiere=dynamic_cast<LitteraleEntiere*>(aNum);
            LitteraleRationnelle* xRationnelle=dynamic_cast<LitteraleRationnelle*>(aNum);
            
            if(xEntiere) c=xEntiere;
            else if (xRationnelle) c=fl.newLitteraleEntiere(xRationnelle->num);
            else throw OperateurException("Impossible d'appliquer cet opérateur sur des littérales non entières ou non rationnelles");
        }
        else if (op=="DEN") {
            
            LitteraleEntiere* xEntiere=dynamic_cast<LitteraleEntiere*>(aNum);
            LitteraleRationnelle* xRationnelle=dynamic_cast<LitteraleRationnelle*>(aNum);
            
            if(xEntiere) c=fl.newLitteraleEntiere(1);
            else if (xRationnelle) c=fl.newLitteraleEntiere(xRationnelle->den);
            else throw OperateurException("Impossible d'appliquer cet opérateur sur des littérales non entières ou non rationnelles");
        }
        else if (op=="IM") {
            c=fl.newLitteraleEntiere(0);
        }
        else if (op=="RE") {
            c=aNum;
        }
        
    }
    else if (aCom) {
        
        if (op=="RE") {
            c=aCom->getPartReel();
        }
        else if (op=="IM") {
            c=aCom->getPartImag();
        }
        
    }
    else if (aExp) {
        string exp=op;
        exp+="(";
        exp+=aExp->getExp();
        exp+=")";
        
        c=fl.newLitteraleExpression(exp);
    }
    
    return c;
}

/*
 *  l1 OP l2
 */

Litterale* OperateurNumerique::applyBinaire(Litterale* l1, Litterale* l2) {
    
    Interpreteur& inter=Interpreteur::getInstance();
    
    Litterale* c=nullptr;
    
    // Numériques
    LitteraleNumerique* aNum=dynamic_cast<LitteraleNumerique*>(l2);
    LitteraleNumerique* bNum=dynamic_cast<LitteraleNumerique*>(l1);
    
    // Expressions
    LitteraleExpression* aExp=dynamic_cast<LitteraleExpression*>(l2);
    LitteraleExpression* bExp=dynamic_cast<LitteraleExpression*>(l1);
    
    // Complexes
    LitteraleComplexe* aCom=dynamic_cast<LitteraleComplexe*>(l2);
    LitteraleComplexe* bCom=dynamic_cast<LitteraleComplexe*>(l1);
    
    if(aNum && bNum) {    // a et b numériques
        
        LitteraleEntiere* xEntiere=dynamic_cast<LitteraleEntiere*>(aNum);
        LitteraleReelle* xReelle=dynamic_cast<LitteraleReelle*>(aNum);
        LitteraleRationnelle* xRationnelle=dynamic_cast<LitteraleRationnelle*>(aNum);
        
        LitteraleEntiere* yEntiere =dynamic_cast<LitteraleEntiere*>(bNum);
        LitteraleReelle* yReelle=dynamic_cast<LitteraleReelle*>(bNum);
        LitteraleRationnelle* yRationnelle=dynamic_cast<LitteraleRationnelle*>(bNum);
        
        if(op=="+") {
            
            // x est entière
            if(xEntiere && yEntiere) c=addition(xEntiere, yEntiere);
            else if (xEntiere && yReelle) c=addition(xEntiere, yReelle);
            else if (xEntiere && yRationnelle) c=addition(xEntiere, yRationnelle);
            // x est réelle
            else if (xReelle && yEntiere) c=addition(yEntiere, xReelle);
            else if (xReelle && yReelle) c=addition(xReelle, yReelle);
            else if (xReelle && yRationnelle) c=addition(xReelle, yRationnelle);
            // x est rationnelle
            else if (xRationnelle && yEntiere) c=addition(yEntiere, xRationnelle);
            else if (xRationnelle && yReelle) c=addition(yReelle, xRationnelle);
            else if (xRationnelle && yRationnelle) c=addition(xRationnelle, yRationnelle);
            else throw OperateurException("Littérales dans la pile inconnues, non numériques, pour l'addition.");
            
        }
        else if (op=="-") {
            
            // x est entière
            if(xEntiere && yEntiere) c=soustraction(yEntiere, xEntiere);
            else if (xEntiere && yReelle) c=soustraction(yReelle, xEntiere);
            else if (xEntiere && yRationnelle) c=soustraction(yRationnelle, xEntiere);
            // x est réelle
            else if (xReelle && yEntiere) c=soustraction(yEntiere, xReelle);
            else if (xReelle && yReelle) c=soustraction(yReelle, xReelle);
            else if (xReelle && yRationnelle) c=soustraction(yRationnelle, xReelle);
            // x est rationnelle
            else if (xRationnelle && yEntiere) c=soustraction(yEntiere, xRationnelle);
            else if (xRationnelle && yReelle) c=soustraction(yReelle, xRationnelle);
            else if (xRationnelle && yRationnelle) c=soustraction(yRationnelle, xRationnelle);
            else throw OperateurException("Littérales dans la pile inconnues, non numériques, pour la soustraction.");
            
        }
        else if (op=="*") {
            
            // x est entière
            if(xEntiere && yEntiere) c=multiplication(xEntiere, yEntiere);
            else if (xEntiere && yReelle) c=multiplication(xEntiere, yReelle);
            else if (xEntiere && yRationnelle) c=multiplication(xEntiere, yRationnelle);
            // x est réelle
            else if (xReelle && yEntiere) c=multiplication(yEntiere, xReelle);
            else if (xReelle && yReelle) c=multiplication(xReelle, yReelle);
            else if (xReelle && yRationnelle) c=multiplication(xReelle, yRationnelle);
            // x est rationnelle
            else if (xRationnelle && yEntiere) c=multiplication(yEntiere, xRationnelle);
            else if (xRationnelle && yReelle) c=multiplication(yReelle, xRationnelle);
            else if (xRationnelle && yRationnelle) c=multiplication(xRationnelle, yRationnelle);
            else throw OperateurException("Littérales dans la pile inconnues, non numériques, pour la multiplication.");
            
        }
        else if (op=="/") {
            
            // x est entière
            if(xEntiere && yEntiere) c=division(yEntiere, xEntiere);
            else if (xEntiere && yReelle) c=division(yReelle, xEntiere);
            else if (xEntiere && yRationnelle) c=division(yRationnelle, xEntiere);
            // x est réelle
            else if (xReelle && yEntiere) c=division(yEntiere, xReelle);
            else if (xReelle && yReelle) c=division(yReelle, xReelle);
            else if (xReelle && yRationnelle) c=division(yRationnelle, xReelle);
            // x est rationnelle
            else if (xRationnelle && yEntiere) c=division(yEntiere, xRationnelle);
            else if (xRationnelle && yReelle) c=division(yReelle, xRationnelle);
            else if (xRationnelle && yRationnelle) c=division(yRationnelle, xRationnelle);
            else throw OperateurException("Littérales dans la pile inconnues, non numériques, pour la soustraction.");
            
        }
        else if (op=="MOD") {
            // x et y doivent être entières
            if(xEntiere && yEntiere) {
                if(xEntiere->n!=0)
                    c=fl.newLitteraleEntiere(yEntiere->n%xEntiere->n);
                else throw OperateurException("Modulo impossible si le second paramètre est nul");
            }
            else throw OperateurException("Impossible d'appliquer cet opérateur sur des littérales non entières");
        }
        else if (op=="DIV") {
            // x et y doivent être entières
            if(xEntiere && yEntiere) {
                if(xEntiere->n!=0)
                    c=fl.newLitteraleEntiere(yEntiere->n/xEntiere->n);
                else throw OperateurException("Division entière impossible par 0");
            }
            else throw OperateurException("Impossible d'appliquer cet opérateur sur des littérales non entières");
        }
        else if (op=="$") {
            
            LitteraleNumerique* r=dynamic_cast<LitteraleNumerique*>(bNum);
            LitteraleNumerique* i=dynamic_cast<LitteraleNumerique*>(aNum);
            c=fl.newLitteraleComplexe(r, i);
        }
        
    }
    else if(aExp && bExp) {     // a et b expressions
        
        if(op=="+" || op=="-" || op=="*" || op=="/") {
            string exp="(";
            exp+=bExp->getExp();
            exp+=")";
            exp+=op;
            exp+="(";
            exp+=aExp->getExp();
            exp+=")";
            
            c=fl.newLitteraleExpression(exp);
        }
        else {
            string exp=op;
            exp+="(";
            exp+=bExp->getExp();
            exp+=",";
            exp+=aExp->getExp();
            exp+=")";
            
            c=fl.newLitteraleExpression(exp);
        }
    }
    else if (aCom && bCom) {    // a et b complexes
        
        if(op=="+") {
            
            // (a + i.b) + (a' + i.b') = (a + a') + i (b + b')
            
            OperateurNumerique* o=dynamic_cast<OperateurNumerique*>(inter.toOperateur("+"));
            LitteraleNumerique* r=dynamic_cast<LitteraleNumerique*>(o->applyBinaire(aCom->getPartReel(), bCom->getPartReel()));
            LitteraleNumerique* i=dynamic_cast<LitteraleNumerique*>(o->applyBinaire(aCom->getPartImag(), bCom->getPartImag()));
            
            c=fl.newLitteraleComplexe(r, i);
        }
        else if (op=="-") {
            
            // (a + i.b) – (a' + i.b') = (a – a') + i (b – b')
            
            OperateurNumerique* o=dynamic_cast<OperateurNumerique*>(inter.toOperateur("-"));
            LitteraleNumerique* r=dynamic_cast<LitteraleNumerique*>(o->applyBinaire(bCom->getPartReel(), aCom->getPartReel()));
            LitteraleNumerique* i=dynamic_cast<LitteraleNumerique*>(o->applyBinaire(bCom->getPartImag(), aCom->getPartImag()));
            
            c=fl.newLitteraleComplexe(r, i);
        }
        else if (op=="*") {
            
            // (a+ib)*(a'+ib') = (aa'–bb')+i(ab'+ba')
            
            OperateurNumerique* o1=dynamic_cast<OperateurNumerique*>(inter.toOperateur("+"));
            OperateurNumerique* o2=dynamic_cast<OperateurNumerique*>(inter.toOperateur("-"));
            OperateurNumerique* o3=dynamic_cast<OperateurNumerique*>(inter.toOperateur("*"));
            
            LitteraleNumerique* r1=dynamic_cast<LitteraleNumerique*>(o3->applyBinaire(bCom->getPartReel(), aCom->getPartReel()));
            LitteraleNumerique* r2=dynamic_cast<LitteraleNumerique*>(o3->applyBinaire(bCom->getPartImag(), aCom->getPartImag()));
            LitteraleNumerique* r=dynamic_cast<LitteraleNumerique*>(o2->applyBinaire(r1, r2));
            
            LitteraleNumerique* i1=dynamic_cast<LitteraleNumerique*>(o3->applyBinaire(bCom->getPartReel(), aCom->getPartImag()));
            LitteraleNumerique* i2=dynamic_cast<LitteraleNumerique*>(o3->applyBinaire(bCom->getPartImag(), aCom->getPartReel()));
            LitteraleNumerique* i=dynamic_cast<LitteraleNumerique*>(o1->applyBinaire(i1, i2));
            
            c=fl.newLitteraleComplexe(r, i);
        }
        else if (op=="/") {
            
            // (a+ib)/(a'+ib') = [(aa'+bb')/(a'^2+b'^2)] + i [(ba'-ab')/(a'^2+b'^2)]
            
            OperateurNumerique* o1=dynamic_cast<OperateurNumerique*>(inter.toOperateur("+"));
            OperateurNumerique* o2=dynamic_cast<OperateurNumerique*>(inter.toOperateur("-"));
            OperateurNumerique* o3=dynamic_cast<OperateurNumerique*>(inter.toOperateur("*"));
            OperateurNumerique* o4=dynamic_cast<OperateurNumerique*>(inter.toOperateur("/"));
            
            LitteraleNumerique* d1=dynamic_cast<LitteraleNumerique*>(o3->applyBinaire(bCom->getPartReel(), bCom->getPartReel()));   // a'^2
            LitteraleNumerique* d2=dynamic_cast<LitteraleNumerique*>(o3->applyBinaire(bCom->getPartImag(), bCom->getPartImag()));   // b'^2
            LitteraleNumerique* d=dynamic_cast<LitteraleNumerique*>(o1->applyBinaire(d1, d2));                                      // (a'^2+b'^2)
            
            LitteraleNumerique* r11=dynamic_cast<LitteraleNumerique*>(o3->applyBinaire(bCom->getPartReel(), aCom->getPartReel()));   // aa'
            LitteraleNumerique* r12=dynamic_cast<LitteraleNumerique*>(o3->applyBinaire(bCom->getPartImag(), aCom->getPartImag()));   // bb'
            LitteraleNumerique* r1=dynamic_cast<LitteraleNumerique*>(o1->applyBinaire(r11, r12));                                    // (aa'+bb')
            LitteraleNumerique* r=dynamic_cast<LitteraleNumerique*>(o4->applyBinaire(r1, d));                                        // (aa'+bb')/(a'^2+b'^2)
            
            LitteraleNumerique* i11=dynamic_cast<LitteraleNumerique*>(o3->applyBinaire(bCom->getPartImag(), aCom->getPartReel()));   // ba'
            LitteraleNumerique* i12=dynamic_cast<LitteraleNumerique*>(o3->applyBinaire(bCom->getPartReel(), aCom->getPartReel()));   // ab'
            LitteraleNumerique* i1=dynamic_cast<LitteraleNumerique*>(o2->applyBinaire(i11, i12));                                    // (ba'-ab')
            LitteraleNumerique* i=dynamic_cast<LitteraleNumerique*>(o4->applyBinaire(i1, d));                                        // (ba'-ab')/(a'^2+b'^2)
            
            c=fl.newLitteraleComplexe(r, i);
        }
        
    }
    else if ((aCom && bNum) || (aNum && bCom)) {
        
        // On convertit celui qui n'est pas complexe en complexe, avec sa partie imaginaire un entier nul
        // On applique ensuite les règles précédentes entre complexes
        
        if(aCom && bNum) {
        
            // On convertit bNum en complexe dont la partie imaginaire est nulle
            LitteraleNumerique* i = fl.newLitteraleEntiere(0);
            LitteraleComplexe* bNumToCom = fl.newLitteraleComplexe(bNum, i);
                
            OperateurNumerique* o=dynamic_cast<OperateurNumerique*>(inter.toOperateur(op));
            c=o->applyBinaire(bNumToCom, aCom);
        }
        else {
            
            // On convertit bNum en complexe dont la partie imaginaire est nulle
            LitteraleNumerique* i = fl.newLitteraleEntiere(0);
            LitteraleComplexe* aNumToCom = fl.newLitteraleComplexe(aNum, i);
            
            OperateurNumerique* o=dynamic_cast<OperateurNumerique*>(inter.toOperateur(op));
            c=o->applyBinaire(aNumToCom, bCom);
        }
        
    }
    
    return c;
}

void OperateurNumerique::apply(Pile& p) {
    if(mul>p.size()) throw OperateurException("Pas assez d'élements pour l'opérateur: "+op);
    
    Litterale* c=nullptr;
    
    // Opérateurs numériques de multiplicité 1
    if(mul==1) {
        
        Litterale* l1=p.top(); p.pop();
        lastarg[0]=fl.copie(l1);
        lastarg[1]=nullptr;
        c=applyUnaire(l1);

    }
    // Opérateurs numériques de multiplicité 2
    else if(mul==2) {
        
        Litterale* l2=p.top(); p.pop();
        Litterale* l1=p.top(); p.pop();
        lastarg[0]=fl.copie(l1);
        lastarg[1]=fl.copie(l2);
        c=applyBinaire(l1, l2);
    }
    
    if(c!=nullptr)
        p.push(c);
    else
        throw OperateurException("Impossible de trouver l'opérateur à appliquer.");
}

/*************************
 *  Opérateurs de pile
 ************************/

Litterale* OperateurPile::applyUnaire(Litterale* l1) {

    return nullptr;
}

Litterale* OperateurPile::applyBinaire(Litterale* l1, Litterale* l2) {

    return nullptr;
}

void OperateurPile::apply(Pile& p) {
    if(mul>p.size()) throw OperateurException("Pas assez d'élements pour l'opérateur: "+op);
    
    if(op=="STO") {
        Litterale* a = p.top(); p.pop();
        Litterale* b = p.top(); p.pop();

        lastarg[0]=fl.copie(a);
        lastarg[1]=fl.copie(b);

        // 'b a STO' avec b littérale expression contenant uniquement un atome et a non littérale atome
        
        // Vérifie que l'on affecte bien à une littérale expression
        LitteraleExpression* bExp=dynamic_cast<LitteraleExpression*>(b);
        if(!bExp) throw OperateurException("STO doit s'appliquer à une littérale expression");
        
        // Vérifie que l'on affecte bien à une littérale expression contenant uniquement un atome
        LitteraleAtome* bAtome=dynamic_cast<LitteraleAtome*>(bExp->getAtome());
        if(!bAtome) throw OperateurException("STO doit s'appliquer à une littérale expression contenant un atome uniquement !");
        
        // Vérifie que la littérale à stocker n'est pas une littérale atome
        // Seule les littérales numériques/complexes/expressions/programmes peuvent être stocker dans une variable
        LitteraleAtome* aAtome=dynamic_cast<LitteraleAtome*>(a);
        if(aAtome) throw OperateurException("Impossible de stocker une littérale atome dans une variable");
        
        // Vérifie que l'atome n'est pas déjà variable
        if(LitteraleAtome::getVar(bAtome->toString())) throw OperateurException("Ce nom de variable existe déjà, veuillez utiliser FORGET avant.");

        // On stocke la variable, addVar écrase la potentielle variable reliée à l'atome b
        LitteraleAtome::addVar(bAtome, a);
    }
    else if (op=="FORGET") {
        
    }
    else if (op=="DUP") {
        
        Litterale* a = p.top();
        Litterale* b = fl.copie(a);
        p.push(b);
    }
    else if (op=="DROP") {
        p.pop();
    }
    else if (op=="SWAP") {
        Litterale* a= p.top(); p.pop();
        Litterale* b= p.top(); p.pop();
        
        p.push(a);
        p.push(b);
    }
        else if (op=="LASTOP") {
        Interpreteur& inter=Interpreteur::getInstance();
        Operateur* op=inter.toOperateur(p.getLastOp());
        if(op!=nullptr) op->apply(p);
        else throw OperateurException("Impossible d'appliquer LASTOP");
    }
    else if(op=="UNDO") {
        p.undo();
    }
    else if (op=="REDO") {
        p.redo();
    }
    else if (op=="CLEAR") {
        while (!p.empty()) p.pop();
    }
    else if (op=="LASTARGS") {
        if(lastarg[0]!=nullptr || lastarg[1]!=nullptr){
            if(lastarg[0]!=nullptr){
                 p.push(lastarg[0]);
            }
            if(lastarg[1]!=nullptr)
                 p.push(lastarg[1]);
        }else throw OperateurException("Impossible d'appliquer LASTARGS");


       }
    
}

/*************************
 *  Opérateurs d'expressions
 ************************/

Litterale* OperateurExpression::applyUnaire(Litterale* l1) {
    
    Litterale* c=nullptr;
    
    if (op=="EVAL") {
        
        // On vérifie que l'on applique bien sur une littérale expression ou programme
        LitteraleExpression* a=dynamic_cast<LitteraleExpression*>(l1);
        if(!a) throw OperateurExpression("EVAL ne peut s'appliquer que sur une littérale expression ou programme");
        
        Litterale* res=a->eval();
        
        if(res) c=res;
        else throw OperateurExpression("Impossible d'évaluer la littérale expression");
        
    }
    
    return c;
}

Litterale* OperateurExpression::applyBinaire(Litterale* l1, Litterale* l2) {
    
    Litterale* c=nullptr;
    
    return c;
}

void OperateurExpression::apply(Pile& p) {
    if(mul>p.size()) throw OperateurException("Pas assez d'élements pour l'opérateur: "+op);
    
    Litterale* c=nullptr;
    
    // Opérateurs numériques de multiplicité 1
    if(mul==1) {
        
        if(op=="EVAL") {
            
            Litterale* l1=p.top(); p.pop();
            lastarg[0]=fl.copie(l1);
            lastarg[1]=nullptr;
            LitteraleProgramme* prg=dynamic_cast<LitteraleProgramme*>(l1);
            
            if(prg) {
                // Traitement du programme
                vector<string> operandes = prg->getInstructions();
                p.execOperandes(operandes);
                return;
            }
            // Taitement de l'expression
            else c=applyUnaire(l1);
            
        }
    }
    
    if(c!=nullptr) {
        p.push(c);
    }
    else throw OperateurException("Impossible de trouver l'opérateur à appliquer.");
    
}

/*************************
 *  Opérateurs logiques
 ************************/

Litterale* OperateurLogique::applyUnaire(Litterale* l1) {
    
    Litterale* c=nullptr;
	LitteraleEntiere* aEnt = dynamic_cast<LitteraleEntiere*>(l1);
	if (op == "NOT") {
		if (aEnt) {
			if (aEnt->valeur() == 0 ) c = fl.newLitteraleEntiere(1);
			else c = fl.newLitteraleEntiere(0);

		}
		else throw OperateurException("Des valeurs entières etaient attendus");

	}
    
    return c;
}

Litterale* OperateurLogique::applyBinaire(Litterale* l1, Litterale* l2) {

	Litterale* c = nullptr;
	// Numériques
	LitteraleNumerique* aNum = dynamic_cast<LitteraleNumerique*>(l1);
	LitteraleNumerique* bNum = dynamic_cast<LitteraleNumerique*>(l2);
	LitteraleEntiere* aEnt = dynamic_cast<LitteraleEntiere*>(l1);
	LitteraleEntiere* bEnt = dynamic_cast<LitteraleEntiere*>(l2);

    if (op == "<") {
		if (aNum && bNum) {
			if (aNum->valeur() < bNum->valeur()) c = fl.newLitteraleEntiere(1);
			else c = fl.newLitteraleEntiere(0);

		}
		else throw OperateurException("Impossible d'appliquer cet opérateur");

	}

	if (op =="=<") {
		if (aNum && bNum) {
			if (aNum->valeur() <= bNum->valeur()) c = fl.newLitteraleEntiere(1);
			else c = fl.newLitteraleEntiere(0);

		}
		else throw OperateurException("Impossible d'appliquer cet opérateur");

	}  
	if (op ==">") {
		if (aNum && bNum) {
			if (aNum->valeur() > bNum->valeur()) c = fl.newLitteraleEntiere(1);
			else c = fl.newLitteraleEntiere(0);

		}
		else throw OperateurException("Impossible d'appliquer cet opérateur");

	}
	if (op == "=>") {
		if (aNum && bNum) {
			if (aNum->valeur() >= bNum->valeur()) c = fl.newLitteraleEntiere(1);
			else c = fl.newLitteraleEntiere(0);

		}
		else throw OperateurException("Impossible d'appliquer cet opérateur");

	}
	if (op == "=") {
		if (aNum && bNum) {
			if (aNum->valeur() == bNum->valeur()) c = fl.newLitteraleEntiere(1);
			else c = fl.newLitteraleEntiere(0);

		}
		else throw OperateurException("Impossible d'appliquer cet opérateur");

	}
	if (op == "!=") {
		if (aNum && bNum) {
			if (aNum->valeur() != bNum->valeur()) c = fl.newLitteraleEntiere(1);
			else c = fl.newLitteraleEntiere(0);

		}
		else throw OperateurException("Impossible d'appliquer cet opérateur");

	}
	if (op == "AND") {
		if (aEnt && bEnt) {
			if (aEnt->valeur()!=0 && bEnt->valeur()!=0) c = fl.newLitteraleEntiere(1);
			else c = fl.newLitteraleEntiere(0);

		}
		else throw OperateurException("Des valeurs entières etaient attendus");

	}
	if (op == "OR") {
		if (aEnt && bEnt) {
			if (aEnt->valeur() !=0 || bEnt->valeur() !=0) c = fl.newLitteraleEntiere(1);
			else c = fl.newLitteraleEntiere(0);

		}
		else throw OperateurException("Des valeurs entières etaient attendus");

	}
    
    return c;
}

void OperateurLogique::apply(Pile& p) {
    if(mul>p.size()) throw OperateurException("Pas assez d'élements pour l'opérateur: "+op);
    
    Litterale* c=nullptr;
    
    // Opérateurs numériques de multiplicité 1
    if(mul==1) {
		Litterale* l1 = p.top(); p.pop();
        if(op!="LASTARGS"){
               lastarg[0]=fl.copie(l1);
               lastarg[1]=nullptr;
               }
		c = applyUnaire(l1);

    }
    // Opérateurs numériques de multiplicité 2
    else if (mul==2) {
        
        Litterale* l2=p.top(); p.pop();
        Litterale* l1=p.top(); p.pop();
        lastarg[0]=fl.copie(l1);
        lastarg[1]=fl.copie(l2);

        if(op == "IFT") {

            // Littérales
            LitteraleEntiere* aEnt = dynamic_cast<LitteraleEntiere*>(l1);
            LitteraleExpression* bExp= dynamic_cast<LitteraleExpression*>(l2);
            LitteraleProgramme* bProg= dynamic_cast<LitteraleProgramme*>(l2);

            if(aEnt && (bExp || bProg)) {
                // Valeur booléenne à TRUE ?
                if(aEnt->valeur()!=0) {
                    if(bExp) c=bExp->eval();
                    else {
                        // Traitement du programme
                        vector<string> operandes = bProg->getInstructions();
                        p.execOperandes(operandes);
                        return;
                    }
                } else return;
            }

        }
        else c=applyBinaire(l1, l2);

    }
    
    if(c!=nullptr) {
        p.push(c);
    }
    else throw OperateurException("Impossible de trouver l'opérateur à appliquer.");
    
}

/*************************
 *  Factory
 ************************/

/*!
 *  \brief Génère un nouvel objet OperateurNumerique
 */
OperateurNumerique* FactoryOperateur::newOperateurNumerique(const string& s) {
    OperateurNumerique* o=new OperateurNumerique(s);
    to.push_back(o);
    return o;
}

/*!
 *  \brief Génère un nouvel objet OperateurPile
 */
OperateurPile* FactoryOperateur::newOperateurPile(const string& s) {
    OperateurPile* o=new OperateurPile(s);
    to.push_back(o);
    return o;
}

/*!
 *  \brief Génère un nouvel objet OperateurExpression
 */
OperateurExpression* FactoryOperateur::newOperateurExpression(const string& s) {
    OperateurExpression* o=new OperateurExpression(s);
    to.push_back(o);
    return o;
}

/*!
 *  \brief Génère un nouvel objet OperateurLogique
 */
OperateurLogique* FactoryOperateur::newOperateurLogique(const string& s) {
	OperateurLogique* o = new OperateurLogique(s);
	to.push_back(o);
	return o;
}

/*************************
 *  Factory : Singleton
 ************************/

/*
 *  Initialisation du handler (singleton)
 */
FactoryOperateur::Handler FactoryOperateur::handler;

/*
 *  Retourne l'instance singleton
 */
FactoryOperateur& FactoryOperateur::getInstance() {
    if(handler.instance==nullptr) handler.instance = new FactoryOperateur;
    return *handler.instance;
}

/*
 *  Supprime le singleton dans le cas où c'est souhaité avant la fin du programme
 */
void FactoryOperateur::deleteInstance() {
    delete handler.instance;
    handler.instance=nullptr;
}

/*
 *  Suppression de tous les Operateurs créées depuis le début
 */
FactoryOperateur::~FactoryOperateur() {
    for(vector<Operateur*>::const_iterator it=to.begin(); it!=to.end(); ++it) {
        delete *it;
    }
}
