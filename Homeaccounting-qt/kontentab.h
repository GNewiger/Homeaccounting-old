#ifndef KONTENTAB_H
#define KONTENTAB_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>

class KontenTab : public QWidget
{
    Q_OBJECT
public:
    KontenTab(QWidget *parent = nullptr);

private:
    QTableWidget tableKonten;
    QTableWidgetItem btnAdd;

private slots:
    void restoreStandardLayout();
    void handleItemClicked(QTableWidgetItem*);
    void addKont(QString);
};

#endif // KONTENTAB_H
