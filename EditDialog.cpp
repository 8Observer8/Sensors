#include "EditDialog.h"
#include "ui_EditDialog.h"

EditDialog::EditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDialog)
{
    ui->setupUi(this);
}

EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::on_saveButton_clicked()
{
    QString name = ui->sensorNameLineEdit->text();
    int number = ui->portNumberSpinBox->value();
    emit signalNameAndNumber(name, number);
    close();
}

void EditDialog::on_cancelButton_clicked()
{
    close();
}
