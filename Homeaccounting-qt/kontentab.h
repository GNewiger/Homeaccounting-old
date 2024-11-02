#ifndef KONTENTAB_H
#define KONTENTAB_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>

struct Konto {
    short id;
    QString name;
    short saldo = 0;
};

class KontenTab : public QWidget
{
    Q_OBJECT
public:
    KontenTab(QWidget *parent = nullptr);

private:
    QTableWidget tableKonten;
    QTableWidgetItem btnAdd;
    QList<Konto> konten;

private slots:
    void handleItemClicked(QTableWidgetItem*);
    void addKonto(QString);
    void addKonto(Konto);
};

#endif // KONTENTAB_H
