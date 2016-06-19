#ifndef QUTSETTINGSWINDOW_H
#define QUTSETTINGSWINDOW_H
/*!
 * \file QUTSettingsWindow.h
 * \brief Fenêtre secondaire, pour les options
 */

#include <QtWidgets>
#include <QObject>
#include <QWidget>
#include <QPushButton>

class QUTMainWindow;
class Litterale;

/*! \class QUTTableWidgetItem
 * \brief Représente un QTableWidgetItem dérivé encapsulant une littérale
 */
class QUTTableWidgetItem : public QTableWidgetItem {
    QString identifiant;
    Litterale* content;
public:
    QUTTableWidgetItem(const QString& text, QString id, Litterale* l):QTableWidgetItem(text),identifiant(id),content(l){}
    Litterale* getContent() { return content; }
    QString getId() { return identifiant; }
};

class QUTSettingsWindow;

/*! \class QUTEditWindow
 * \brief Fenêtre d'édition textuelle des Litterales
 */
class QUTEditWindow : public QWidget {
    Q_OBJECT
    QString id;
    Litterale* l;
    QUTSettingsWindow* parent;
    QTextEdit* editField;
    QPushButton* validation;
public:
    QUTEditWindow(QUTSettingsWindow* p);
    void showWith(QString s, Litterale* lit);
public slots:
    void save();
};

/*! \class QUTSettingsWindow
 * \brief Fenêtre secondaire, pour les options du programme
 */
class QUTSettingsWindow : public QWidget
{
    Q_OBJECT
    QUTMainWindow* main;
    QTabWidget* tabs;
    QTableWidget* vars;
    QTableWidget* prgs;
    QPushButton* forget1;
    QPushButton* forget2;
    QRadioButton* KeyboardYes;
    QRadioButton* KeyboardNo;
    QUTEditWindow* editView;
public:
    QUTSettingsWindow(QUTMainWindow* m);
    void refreshWindow();
    void showEvent(QShowEvent* event);
public slots:
    void switchKeyboard();
    void switchPile(int n);
    void editVars(int i, int j);
    void editPrg(int i, int j);
    void forgetv(int i, int j); //activation
    void forgetp(int i, int j);
    void forget1_action(bool verif);
    void forget2_action(bool);
};

#endif // QUTSETTINGSWINDOW_H
