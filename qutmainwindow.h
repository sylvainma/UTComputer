#ifndef UTMAINWINDOW_H
#define UTMAINWINDOW_H
/*!
 * \file QUTMainWindow.h
 * \brief Fenêtre principale
 */

#include <QtWidgets>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QLineEdit>
#include <deque>
#include "qutsettingswindow.h"
#include "Litterale.h"
#include "Manager.h"
#include "Pile.h"
#define NB_MAX_PILE 5

/*! \class QPushDigit
 * \brief Les bouttons cliquables du clavier qui représentent un digit
 */
class QPushDigit : public QPushButton {
    Q_OBJECT
    int digit;
public:
    QPushDigit(int n):QPushButton(QString::number(n)),digit(n){}
    int getDigit() const { return digit; }
    void mousePressEvent(QMouseEvent *event);
signals:
    void clicked(int n);
};

/*! \class QPushOp
 * \brief Les bouttons cliquables du clavier qui représentent un opérateur
 */
class QPushOp : public QPushButton {
    Q_OBJECT
    QString op;
public:
    QPushOp(QString o):QPushButton(o),op(o){}
    QString getOp() const { return op; }
    void mousePressEvent(QMouseEvent *event);
signals:
    void clicked(QString o);
};

/*! \class QUTMainWindow
 * \brief Fenêtre principale du programme UTComputer
 */
class QUTMainWindow : public QWidget
{
    Q_OBJECT

    // Frontend
    QLineEdit* QMessage;
    QTableWidget* QPile;
    QLineEdit* QCommande;
    QPushButton* QExec;
    QWidget* QKeyboard;
    QPushButton* QSettings;
    QUTSettingsWindow* QSettingsWindow;

    // Backend
    Pile& pile;
    Manager& manager;
    QString message;

    // Settings
    unsigned int nbPile;
    bool keyboard;

public:
    QUTMainWindow(Pile& p, Manager& m);
    ~QUTMainWindow(){}
    void initWindow();
    void refreshWindow();

    QWidget* getKeyboard() { return QKeyboard; }
    QTableWidget* getPile() { return QPile; }

    unsigned int getNbPile() const { return nbPile; }
    void setNbPile(unsigned int n) { nbPile=n; }
    bool isKeyboard() const { return keyboard; }
    void setKeyboard(bool k) { keyboard=k; }
public slots:
    void commande();
    void settings();
    void addDigit(int d);
    void addOp(QString op);
    void closeEvent(QCloseEvent *event);

};

#endif // UTMAINWINDOW_H
