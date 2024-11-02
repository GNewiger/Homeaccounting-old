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

    for(int i = 0; i < 11; ++i){
        tableKonten.insertRow(i);
    }
    btnAdd.setText("Hinzufügen");
    btnAdd.setTextAlignment(Qt::AlignCenter);
    QObject::connect(&tableKonten, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(handleItemClicked(QTableWidgetItem*)));
    tableKonten.setItem(10, 0, &btnAdd);
    tableKonten.setSpan(10,0,1,2);
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
    QTableWidgetItem* someName = new QTableWidgetItem(name);
    QTableWidgetItem* someSaldo = new QTableWidgetItem("0");
    size_t indexLastItem = tableKonten.rowCount() - 2; // the last item is actually the add button, so insert the new item to second last position

    tableKonten.insertRow(indexLastItem);
    tableKonten.setItem(indexLastItem + 1, 0, someName);
    tableKonten.setItem(indexLastItem + 1, 1, someSaldo);
}
