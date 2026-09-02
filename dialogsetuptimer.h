#ifndef DIALOGSETUPTIMER_H
#define DIALOGSETUPTIMER_H

#include <QDialog>

namespace Ui {
class DialogSetupTimer;
}

class DialogSetupTimer : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupTimer(QWidget *parent = nullptr);
    ~DialogSetupTimer();

private slots:


    void on_btnClose_4_clicked();

    void on_btnCheck_4_clicked();

private:
    Ui::DialogSetupTimer *ui;
};

#endif // DIALOGSETUPTIMER_H
