#include "SettingDialog.h"
#include "ui_SettingDialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::on_connectButton_clicked()
{
    QString hostName = ui->hostNameLineEdit->text();
    int portNumber = ui->portNumberSpinBox->value();
    emit signalHostNameAndPortNumber(hostName, portNumber);
    close();
}

void SettingDialog::on_cancelButton_clicked()
{
    close();
}
