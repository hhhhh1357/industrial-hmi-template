#ifndef DIALOGSETUPPWD_H
#define DIALOGSETUPPWD_H

#include <QDialog>

#include <Headers/Tools/keyBoard.h>

namespace Ui {
class DialogSetupPwd;
}

class DialogSetupPwd : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupPwd(QWidget *parent = nullptr);
    ~DialogSetupPwd();

private slots:
    void on_btnClose_clicked();

    void on_btnSave_clicked();

private:
    Ui::DialogSetupPwd *ui;

    KeyBoard *keyBoard;
signals:
    void toReboot();

};

#endif // DIALOGSETUPPWD_H
