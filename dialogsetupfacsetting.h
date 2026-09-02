#ifndef DIALOGSETUPFACSETTING_H
#define DIALOGSETUPFACSETTING_H

#include <QDialog>

namespace Ui {
class DialogSetupFacSetting;
}

class DialogSetupFacSetting : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupFacSetting(QWidget *parent = nullptr);
    ~DialogSetupFacSetting();

private slots:
    void on_btnClose_clicked();

    void on_btnSave_clicked();

private:
    Ui::DialogSetupFacSetting *ui;
    bool isRecovered=false;
};

#endif // DIALOGSETUPFACSETTING_H
