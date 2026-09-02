#ifndef FORMLOGIN_H
#define FORMLOGIN_H

#include <QWidget>

#include "mainwindow.h"

namespace Ui {
class FormLogin;
}

class FormLogin : public QWidget
{
    Q_OBJECT

public:
    explicit FormLogin(QWidget *parent = nullptr);
    ~FormLogin();

    MainWindow * mainWindow;

private slots:
    void on_btnEnter_clicked();

    void on_btn_1_clicked();

    void on_btn_2_clicked();

    void on_btn_3_clicked();

    void on_btn_4_clicked();

    void on_btn_5_clicked();

    void on_btn_6_clicked();

    void on_btn_7_clicked();

    void on_btn_8_clicked();

    void on_btn_9_clicked();

    void on_btn_0_clicked();

    void on_btnEsc_clicked();

    void on_btnDel_clicked();

    void on_btnStar_clicked();

    void on_btnSharp_clicked();

private:
    Ui::FormLogin *ui;

    void setComponent();
};

#endif // FORMLOGIN_H
