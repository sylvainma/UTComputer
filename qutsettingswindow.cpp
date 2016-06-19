#include "qutsettingswindow.h"
#include "qutmainwindow.h"
#include "Litterale.h"

QUTEditWindow::QUTEditWindow(QUTSettingsWindow* p) {

    // Fenêtre d'édition
    setWindowTitle("UTComputer - édition");
    setFixedSize(300, 200);

    // Fenêtre des options
    parent=p;

    // Content
    l=nullptr;

    // Éditeur de texte
    editField = new QTextEdit;

    // Bouton de validation
    validation = new QPushButton("Enregistrer");
    // Bouton d'oublie



    QVBoxLayout* vEditView = new QVBoxLayout;
    vEditView->addWidget(editField);
    vEditView->addWidget(validation);
    this->setLayout(vEditView);

    QObject::connect(validation, SIGNAL(clicked()), this, SLOT(save()));
}

/*!
 *  \brief Ouvre la fenêtre d'édition avec la littérale à éditier
 */
void QUTEditWindow::showWith(QString s, Litterale* lit) {
    id=s;
    l=lit;
    if(l!=nullptr) {
        editField->setText(QString::QString::fromStdString(l->toString()));
        this->show();
    }
    else qDebug() << QString("échec lors de l'ouverture de l'éditeur");
}

/*!
 *  \brief Enregistre les modifications, met à jour la fenêtre d'options et ferme l'édition
 */
void QUTEditWindow::save() {
    QString p = editField->toPlainText();
    std::string s = p.toStdString();
    LitteraleProgramme* prg = dynamic_cast<LitteraleProgramme*>(l);

    // Traitement
    if(prg) {
        prg->setPrg(s);
    }
    else {
        Interpreteur& inter = Interpreteur::getInstance();
        Litterale* res = inter.toLitterale(s);
        if(res) {
            LitteraleAtome* a=LitteraleAtome::getVar(id.toStdString());
            LitteraleAtome::delVar(a);
            LitteraleAtome::addVar(a,res);
        }
        else qDebug() << QString("Impossible de trouver la littérale associée");
    }

    // Reset de la fenêtre + fermeture
    editField->setText("");
    l=nullptr;
    parent->refreshWindow();
    this->hide();
}

QUTSettingsWindow::QUTSettingsWindow(QUTMainWindow* m):main(m)
{
    // Fenêtre
    setFixedSize(400, 400);
    setWindowTitle("UTComputer - Options");

    // Édition
    editView = new QUTEditWindow(this);

    // Onglets
    tabs = new QTabWidget(this);
    tabs->setGeometry(10, 10, 380, 380);
    QWidget *tab1 = new QWidget;
    QWidget *tab2 = new QWidget;
    QWidget *tab3 = new QWidget;

    // Onglet n°1 : options
        // Clavier
    QGroupBox* KeyboardGroup = new QGroupBox("Afficher le clavier ?");
    KeyboardYes = new QRadioButton("Oui");
    KeyboardNo = new QRadioButton("Non");
    QHBoxLayout* hKeyboardGroup = new QHBoxLayout;
    hKeyboardGroup->addWidget(KeyboardYes);
    hKeyboardGroup->addWidget(KeyboardNo);
    KeyboardGroup->setLayout(hKeyboardGroup);
    KeyboardYes->setChecked(main->isKeyboard());

        // Nombre d'éléments de la pile affichés
    QGroupBox* PileGroup = new QGroupBox("Nombre d'objets dans la pile");
    QSpinBox* numberPile = new QSpinBox(PileGroup);
    numberPile->setMinimum(1);
    numberPile->setValue(main->getPile()->rowCount());
    QVBoxLayout* vNumberPile = new QVBoxLayout;
    vNumberPile->addWidget(numberPile);
    PileGroup->setLayout(vNumberPile);

    QVBoxLayout* vTab1 = new QVBoxLayout(tab1);
    vTab1->addWidget(KeyboardGroup);
    vTab1->addWidget(PileGroup);

    // Onglet n°2
    forget1 = new QPushButton("Forget");
    vars = new QTableWidget(1,2);
    vars->horizontalHeader()->setVisible(false);
    vars->horizontalHeader()->setStretchLastSection(true);
    vars->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QVBoxLayout* vVars = new QVBoxLayout(tab2);
    vVars->addWidget(vars);
    vVars->addWidget(forget1);
    forget1->setDisabled(true);

    // Onglet n°3
    forget2 = new QPushButton("Forget");
    prgs = new QTableWidget(1,2);
    prgs->horizontalHeader()->setVisible(false);
    prgs->horizontalHeader()->setStretchLastSection(true);
    prgs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QVBoxLayout* vPrgs = new QVBoxLayout(tab3);
    vPrgs->addWidget(prgs);
    vPrgs->addWidget(forget2);
    forget2->setDisabled(true);

    // Ajout des onglets
    tabs->addTab(tab1, "Options");
    tabs->addTab(tab2, "Variables");
    tabs->addTab(tab3, "Programmes");

    refreshWindow();

    // Déclaration des évènements
    QObject::connect(KeyboardYes, SIGNAL(clicked()), this, SLOT(switchKeyboard()));
    QObject::connect(KeyboardNo, SIGNAL(clicked()), this, SLOT(switchKeyboard()));
    QObject::connect(numberPile, SIGNAL(valueChanged(int)), this, SLOT(switchPile(int)));
    QObject::connect(vars, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editVars(int,int)));
    QObject::connect(prgs, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editPrg(int,int)));
    QObject::connect(vars, SIGNAL(cellPressed(int,int)), this, SLOT(forgetv(int,int)));
    QObject::connect(forget1, SIGNAL(clicked(bool)), this, SLOT(forget1_action(bool)));
    QObject::connect(prgs, SIGNAL(cellPressed(int,int)), this, SLOT(forgetp(int,int)));
    QObject::connect(forget2, SIGNAL(clicked(bool)), this, SLOT(forget2_action(bool)));

}

/*!
 *  \brief Pour mettre à jour les informations sur les variables et les programmes
 */
void QUTSettingsWindow::refreshWindow() {

    // Parcourt de la liste des variables
    vars->setRowCount(LitteraleAtome::getNbVars());
    int i=0;
    for(vector<LitteraleAtome*>::iterator it1=LitteraleAtome::vars.begin(); it1!=LitteraleAtome::vars.end(); ++it1) {
        LitteraleProgramme* prg = dynamic_cast<LitteraleProgramme*>((*it1)->getValeur());
        if(!prg) {
            QUTTableWidgetItem* item1 = new QUTTableWidgetItem(QString::fromStdString((*it1)->toString()), QString::fromStdString((*it1)->toString()), (*it1)->getValeur());
            QUTTableWidgetItem* item2 = new QUTTableWidgetItem(QString::fromStdString((*it1)->getValeur()->toString()), QString::fromStdString((*it1)->toString()), (*it1)->getValeur());
            vars->setItem(i,0,item1);
            vars->setItem(i,1,item2);
            i++;
        }
    }

    // Parcourt de la liste des programmes
    prgs->setRowCount(LitteraleAtome::getNbPrgs());
    int j=0;
    for(vector<LitteraleAtome*>::iterator it2=LitteraleAtome::vars.begin(); it2!=LitteraleAtome::vars.end(); ++it2) {
        LitteraleProgramme* prg = dynamic_cast<LitteraleProgramme*>((*it2)->getValeur());
        if(prg) {
            QUTTableWidgetItem* item1 = new QUTTableWidgetItem(QString::fromStdString((*it2)->toString()), QString::fromStdString((*it2)->toString()), prg);
            QUTTableWidgetItem* item2 = new QUTTableWidgetItem(QString::fromStdString(prg->toString()), QString::fromStdString((*it2)->toString()), prg);
            prgs->setItem(j,0,item1);
            prgs->setItem(j,1,item2);
            j++;
        }
    }

}

/*!
 *  \brief Fonction appelée lorsque la fenêtre est show()
 */
void QUTSettingsWindow::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    refreshWindow();
}

/*!
 *  \brief Fonction pour afficher ou non le clavier sur la vue principale
 */
void QUTSettingsWindow::switchKeyboard() {
    if(KeyboardYes->isChecked()) {
        if(!main->isKeyboard()) {
            main->getKeyboard()->show();
            main->setKeyboard(true);
        }
    }
    else {
        if(main->isKeyboard()) {
            main->getKeyboard()->hide();
            main->setKeyboard(false);
        }
    }
}

/*!
 *  \brief Fonction pour modifier le nombre d'éléments de la pile sur la vue principale
 */
void QUTSettingsWindow::switchPile(int n) {

    // Purge
    for(int i=0; i<main->getPile()->rowCount(); i++) {
        QTableWidgetItem* item = main->getPile()->item(i,0);
        delete item;
    }

    // Nouvelle valeur
    main->getPile()->setRowCount(n);
    main->setNbPile(n);

    // Allocation des nouveaux items
    for(int i=0; i<main->getPile()->rowCount(); i++)
        main->getPile()->setItem(i,0,new QTableWidgetItem(""));

    // Refresh
    main->refreshWindow();
}

/*!
 *  \brief Appelle la fenêtre d'édition -> variables
 */
void QUTSettingsWindow::editVars(int i,int j) {
    QUTTableWidgetItem* item = dynamic_cast<QUTTableWidgetItem*>(vars->item(i,j));
    editView->showWith(item->getId(), item->getContent());
}

/*!
 *  \brief Appelle la fenêtre d'édition -> programmes
 */
void QUTSettingsWindow::editPrg(int i,int j) {
    QUTTableWidgetItem* item = dynamic_cast<QUTTableWidgetItem*>(prgs->item(i,j));
    editView->showWith(item->getId(), item->getContent());
}

// forget

/*bool QUTSettingsWindow::forget1_bool(){
    if (verif) return TRUE;
    else return FALSE;

}*/

void QUTSettingsWindow::forget1_action(bool verif) {
    int i=vars->currentRow(); // je me suis sans doute compliqué la vie mais avant j'avais fais plus simple ça marché pas a cause d'un autre problème simple mais du coup je n'i pas changer
    int j=vars->currentColumn();
    std::cout<<i<<j;
    QUTTableWidgetItem* item = dynamic_cast<QUTTableWidgetItem*>(vars->item(i,j));
    QString str= item->getId();
    string S=str.toStdString();
    LitteraleAtome* atome=LitteraleAtome::getVar(S);
    LitteraleAtome::delVar(atome);
    refreshWindow();
}

void QUTSettingsWindow::forgetv(int i, int j) {
    forget1->setDisabled(false);

}

void QUTSettingsWindow::forgetp(int i, int j) {

    forget2->setDisabled(false);
}

void QUTSettingsWindow::forget2_action(bool verif) {
    int i=prgs->currentRow();
    int j=prgs->currentColumn();
    std::cout<<i<<j;
    QUTTableWidgetItem* item = dynamic_cast<QUTTableWidgetItem*>(prgs->item(i,j));
    QString str= item->getId();
    string S=str.toStdString();
    LitteraleAtome* atome=LitteraleAtome::getVar(S);
    LitteraleAtome::delVar(atome);
    refreshWindow();
}
