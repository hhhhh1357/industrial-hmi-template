#ifndef DIALOGSETUPDOCHECK_H
#define DIALOGSETUPDOCHECK_H

#include <QDialog>

namespace Ui {
class DialogSetupDoCheck;
}

class DialogSetupDoCheck : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupDoCheck(QWidget *parent = nullptr);
    ~DialogSetupDoCheck();

private slots:
    void on_btnClose_clicked();

private:
    Ui::DialogSetupDoCheck *ui;
};

#endif // DIALOGSETUPDOCHECK_H
