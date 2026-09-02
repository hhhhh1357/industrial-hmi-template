#ifndef DIALOGSETUPDICHECK_H
#define DIALOGSETUPDICHECK_H

#include <QDialog>

namespace Ui {
class DialogSetupDiCheck;
}

class DialogSetupDiCheck : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupDiCheck(QWidget *parent = nullptr);
    ~DialogSetupDiCheck();

private slots:
    void on_btnClose_clicked();

private:
    Ui::DialogSetupDiCheck *ui;
};

#endif // DIALOGSETUPDICHECK_H
