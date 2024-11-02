#include "kontocreatedialog.h"
#include "ui_kontocreatedialog.h"
#include "QRegularExpressionValidator"
#include <QStyle>
#include <QPushButton>

KontoCreateDialog::KontoCreateDialog(QWidget *parent)
    : QDialog{parent}, ui(new Ui::KontoCreateDialog)
{
    ui->setupUi(this);

    QFontMetrics fm(ui->leName->font());
    QString W = "W";
    size_t leWidth = fm.tightBoundingRect(" " + W.repeated(32) + " ").width(); // two extra spaces to account for margins etc
    ui->leName->setMinimumWidth(leWidth); // maximal 32 characters für ein Konto
    QRegularExpression regex("[a-zA-Z0-9]{0,32}");
    QValidator* validator = new QRegularExpressionValidator(regex, this);
    ui->leName->setValidator(validator);

    QObject::connect(ui->buttonBox->button(QDialogButtonBox::Save), SIGNAL(clicked()), this, SLOT(emitAddKonto()));
}

void KontoCreateDialog::emitAddKonto(){
    emit addKonto(ui->leName->text());
}
