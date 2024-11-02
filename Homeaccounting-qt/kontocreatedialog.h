#ifndef KONTOCREATEDIALOG_H
#define KONTOCREATEDIALOG_H

#include <QDialog>
#include <QObject>
#include <QWidget>

namespace Ui {
    class KontoCreateDialog;
}

class KontoCreateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit KontoCreateDialog(QWidget *parent = nullptr);
private:
    Ui::KontoCreateDialog *ui;
signals:
    void addKonto(QString name);
private slots:
    void emitAddKonto();
    void handleSaveButtonActivation(QString text);
};

#endif // KONTOCREATEDIALOG_H
