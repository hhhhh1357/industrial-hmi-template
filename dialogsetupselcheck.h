#ifndef DIALOGSETUPSELCHECK_H
#define DIALOGSETUPSELCHECK_H

#include <QDialog>

namespace Ui {
class DialogSetupSelCheck;
}

class DialogSetupSelCheck : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupSelCheck(QWidget *parent = nullptr);
    ~DialogSetupSelCheck();

private slots:
    void on_btnAiCheck_clicked();

    void on_btnDiCheck_clicked();

    void on_btnDocheck_clicked();



//    void on_btnDogCheck_clicked();



    void on_btnDogCheck_3_clicked();

    void on_btnrs232Detection_clicked();

    void on_btnrs485Detection_clicked();

private:
    Ui::DialogSetupSelCheck *ui;

    void initAllButtonsStyle();

    QString ButtonStytleNormal="";
    QString ButtonStytlePresed="";
};

#endif // DIALOGSETUPSELCHECK_H
