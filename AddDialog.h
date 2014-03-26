#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = 0);
    ~AddDialog();

signals:
    void signalNameAndNumber(QString name, int number);

private slots:
    void on_addButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::AddDialog *ui;
};

#endif // ADDDIALOG_H
