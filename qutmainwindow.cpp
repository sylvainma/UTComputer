#include "qutmainwindow.h"

void QPushDigit::mousePressEvent(QMouseEvent *event) {
    QPushButton::mousePressEvent(event);
    emit clicked(digit);
}

void QPushOp::mousePressEvent(QMouseEvent *event) {
    QPushButton::mousePressEvent(event);
    emit clicked(op);
}

QUTMainWindow::QUTMainWindow(Pile& p, Manager& m):pile(p),manager(m),message("Bienvenue"),nbPile(NB_MAX_PILE),keyboard(true)
{
    // Fenêtre principale
    adjustSize();
    setMinimumSize(400, 200);
    setWindowTitle("UTComputer");

    // Message
    QMessage = new QLineEdit(this);
    QMessage->setReadOnly(true);

    // Pile
    QPile = new QTableWidget(nbPile,1,this);
    QPile->horizontalHeader()->setVisible(true);
    QPile->horizontalHeader()->setStretchLastSection(true);
    QPile->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(int i=0; i<QPile->rowCount(); i++) QPile->setItem(i,0,new QTableWidgetItem(""));

    // Commande
    QCommande = new QLineEdit(this);
    QExec = new QPushButton("ENTER", this);
    QHBoxLayout* CommandeLayout = new QHBoxLayout();
    CommandeLayout->addWidget(QCommande);
    CommandeLayout->addWidget(QExec);

    // Clavier
    QKeyboard = new QWidget;
    QGridLayout* QKeyboardLayout = new QGridLayout(QKeyboard);

    QPushDigit* zero = new QPushDigit(0);
    QPushDigit* un = new QPushDigit(1);
    QPushDigit* deux = new QPushDigit(2);
    QPushDigit* trois = new QPushDigit(3);
    QPushDigit* quatre = new QPushDigit(4);
    QPushDigit* cinq = new QPushDigit(5);
    QPushDigit* six = new QPushDigit(6);
    QPushDigit* sept = new QPushDigit(7);
    QPushDigit* huit = new QPushDigit(8);
    QPushDigit* neuf = new QPushDigit(9);
    QPushOp* plus = new QPushOp("+");
    QPushOp* moins = new QPushOp("-");
    QPushOp* fois = new QPushOp("*");
    QPushOp* divi = new QPushOp("/");
    QPushOp* comp = new QPushOp("$");
    QPushOp* sto = new QPushOp("STO");
    QPushOp* undo = new QPushOp("UNDO");
    QPushOp* redo = new QPushOp("REDO");
    QPushOp* drop = new QPushOp("DROP");
    QPushOp* clear = new QPushOp("CLEAR");

    QKeyboardLayout->addWidget(un, 0, 0);
    QKeyboardLayout->addWidget(deux, 0, 1);
    QKeyboardLayout->addWidget(trois, 0, 2);
    QKeyboardLayout->addWidget(plus, 0, 3);
    QKeyboardLayout->addWidget(quatre, 1, 0);
    QKeyboardLayout->addWidget(cinq, 1, 1);
    QKeyboardLayout->addWidget(six, 1, 2);
    QKeyboardLayout->addWidget(moins, 1, 3);
    QKeyboardLayout->addWidget(sept, 2, 0);
    QKeyboardLayout->addWidget(huit, 2, 1);
    QKeyboardLayout->addWidget(neuf, 2, 2);
    QKeyboardLayout->addWidget(fois, 2, 3);
    QKeyboardLayout->addWidget(zero, 3, 0);
    QKeyboardLayout->addWidget(comp, 3, 2);
    QKeyboardLayout->addWidget(divi, 3, 3);
    QKeyboardLayout->addWidget(undo, 0, 4);
    QKeyboardLayout->addWidget(redo, 1, 4);
    QKeyboardLayout->addWidget(drop, 2, 4);
    QKeyboardLayout->addWidget(clear, 3, 4);
    QKeyboardLayout->addWidget(sto, 3, 1);

    // Settings
    QSettings = new QPushButton("Options");
    QSettingsWindow = new QUTSettingsWindow(this);

    // Layout vertical
    QVBoxLayout* VLayout = new QVBoxLayout(this);
    VLayout->addWidget(QMessage);
    VLayout->addWidget(QPile);
    VLayout->addLayout(CommandeLayout);
    VLayout->addWidget(QKeyboard);
    VLayout->addWidget(QSettings);

    // Initialisation de la fenêtre
    initWindow();

    // Clavier cliquable : digits
    QObject::connect(un, SIGNAL(clicked(int)), this, SLOT(addDigit(int)));
    QObject::connect(deux, SIGNAL(clicked(int)), this, SLOT(addDigit(int)));
    QObject::connect(trois, SIGNAL(clicked(int)), this, SLOT(addDigit(int)));
    QObject::connect(quatre, SIGNAL(clicked(int)), this, SLOT(addDigit(int)));
    QObject::connect(cinq, SIGNAL(clicked(int)), this, SLOT(addDigit(int)));
    QObject::connect(six, SIGNAL(clicked(int)), this, SLOT(addDigit(int)));
    QObject::connect(sept, SIGNAL(clicked(int)), this, SLOT(addDigit(int)));
    QObject::connect(huit, SIGNAL(clicked(int)), this, SLOT(addDigit(int)));
    QObject::connect(neuf, SIGNAL(clicked(int)), this, SLOT(addDigit(int)));
    QObject::connect(zero, SIGNAL(clicked(int)), this, SLOT(addDigit(int)));

    // Clavier cliquable : opérateurs
    QObject::connect(plus, SIGNAL(clicked(QString)), this, SLOT(addOp(QString)));
    QObject::connect(moins, SIGNAL(clicked(QString)), this, SLOT(addOp(QString)));
    QObject::connect(fois, SIGNAL(clicked(QString)), this, SLOT(addOp(QString)));
    QObject::connect(divi, SIGNAL(clicked(QString)), this, SLOT(addOp(QString)));
    QObject::connect(comp, SIGNAL(clicked(QString)), this, SLOT(addOp(QString)));
    QObject::connect(sto, SIGNAL(clicked(QString)), this, SLOT(addOp(QString)));
    QObject::connect(undo, SIGNAL(clicked(QString)), this, SLOT(addOp(QString)));
    QObject::connect(redo, SIGNAL(clicked(QString)), this, SLOT(addOp(QString)));
    QObject::connect(clear, SIGNAL(clicked(QString)), this, SLOT(addOp(QString)));
    QObject::connect(drop, SIGNAL(clicked(QString)), this, SLOT(addOp(QString)));

    // Déclaration des évènements divers
    QObject::connect(QExec, SIGNAL(clicked()), this, SLOT(commande()));
    QObject::connect(QExec, SIGNAL(clicked()), QCommande, SLOT(clear()));
    QObject::connect(QCommande, SIGNAL(returnPressed()), this, SLOT(commande()));
    QObject::connect(QCommande, SIGNAL(returnPressed()), QCommande, SLOT(clear()));
    QObject::connect(QSettings, SIGNAL(clicked()), this, SLOT(settings()));
}

void QUTMainWindow::initWindow() {
    // Chargement du contexte
    // TODO

    // Refresh
    refreshWindow();
}

void QUTMainWindow::refreshWindow() {

    int i=0;

    // Message
    QMessage->setText(message);

    // Purge de la pile
    for(i=0; i<QPile->rowCount(); i++){
        QTableWidgetItem* x=QPile->item(i,0);
        x->setText(QString(""));
    }

    // Remplissement de la pile
    std::deque<Litterale*> liste = pile.getListe(nbPile);
    i=0;
    QTableWidgetItem* item;
    for (deque<Litterale*>::const_iterator it = liste.begin(); it != liste.end(); ++it) {
        Litterale* l = *it;
        item=new QTableWidgetItem(QString::fromStdString(l->toString()));
        QPile->setItem(i, 0, item);
        i++;
    }

}

void QUTMainWindow::commande() {
    try {
        QString cmd=QCommande->text();
        manager.execCommande(cmd.toStdString());
        message="Commande exécutée";
        refreshWindow();
    } catch (UTComputerException e) {
        message=QString::fromStdString(e.getMessage());
        refreshWindow();
    }
}

void QUTMainWindow::settings() {
    QSettingsWindow->show();
}

void QUTMainWindow::addDigit(int d) {
    QString cmd=QCommande->text();
    QCommande->setText(cmd+QString::number(d));
}

void QUTMainWindow::addOp(QString op) {
    // Exécution de la commande en cours
    if(QCommande->text()!="") commande();
    // Exécution de l'opérateur
    QCommande->setText(op);
    commande();
    QCommande->setText("");
}
void QUTMainWindow::closeEvent(QCloseEvent* event){
    // Sauvegarde du contexte
    QUTMainWindow::pile.saveContext();
}
