#include "kontentab.h"
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include "kontocreatedialog.h"
#include <QHeaderView>

KontenTab::KontenTab(QWidget *parent) : QWidget(parent), tableKonten(this), btnAdd(){
    tableKonten.setMinimumWidth(800);
    tableKonten.setMinimumHeight(400);
    tableKonten.insertColumn(0);
    tableKonten.insertColumn(1);
    tableKonten.setEditTriggers(QAbstractItemView::NoEditTriggers);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(&tableKonten);
    QStringList headers;
    headers.append("Name");
    headers.append("Saldo");
    tableKonten.setHorizontalHeaderLabels(headers);

    btnAdd.setText("Hinzufügen");
    btnAdd.setTextAlignment(Qt::AlignCenter);
    QObject::connect(&tableKonten, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(handleItemClicked(QTableWidgetItem*)));
    tableKonten.insertRow(0);
    tableKonten.setItem(0, 0, &btnAdd);
    tableKonten.setSpan(0,0,1,2);
    tableKonten.horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void KontenTab::handleItemClicked(QTableWidgetItem* item){
    if (item == &btnAdd){
        KontoCreateDialog createDialog(this);
        QObject::connect(&createDialog, SIGNAL(addKonto(QString)), this, SLOT(addKonto(QString)));
        createDialog.exec();
    }
}

void KontenTab::addKonto(QString name){
    struct Konto konto = {0, name};
    addKonto(konto);
}

void KontenTab::addKonto(Konto konto){
    konten.append(konto);
    QTableWidgetItem* name = new QTableWidgetItem(konto.name);
    QTableWidgetItem* saldo = new QTableWidgetItem(konto.saldo);
    short indexLastItem = tableKonten.rowCount() - 1;

    //remove add button
    tableKonten.takeItem(indexLastItem, 0);
    tableKonten.removeRow(indexLastItem);

    // add new item
    tableKonten.insertRow(indexLastItem);
    tableKonten.setItem(indexLastItem, 0, name);
    tableKonten.setItem(indexLastItem, 1, saldo);

    // re-insert add button (so it's the last bztton)
    tableKonten.insertRow(indexLastItem + 1);
    tableKonten.setItem(indexLastItem + 1, 0, &btnAdd);
    tableKonten.setSpan(indexLastItem + 1,0,1,2);
}
