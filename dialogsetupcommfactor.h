#ifndef DIALOGSETUPCOMMFACTOR_H
#define DIALOGSETUPCOMMFACTOR_H

#include <QDialog>

#include "Headers/Model/MComArgs.h"
#include "Headers/Tools/keyBoard.h"

namespace Ui {
class DialogSetupCommFactor;
}

class DialogSetupCommFactor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupCommFactor(QWidget *parent = nullptr);
    DialogSetupCommFactor(MComArgs * comArgs,QWidget *parent = nullptr);
    ~DialogSetupCommFactor();

signals:
    void updateFactors(QString factors);

private slots:
    void on_btnClose_clicked();

    void on_factorC_textEdited(const QString &arg1);

    void on_btnAdd_clicked();

    void on_btnDelete_clicked();

    void on_tableDetail_itemSelectionChanged();

    void on_btnUpdate_clicked();

    void on_btnSave_clicked();

private:
    Ui::DialogSetupCommFactor *ui;
    MComArgs * comArgs ;
    KeyBoard *keyBoard;

    int getItemRowByCode(QString code);
    void initControlsRS232_485();
    void initControlsOther();

    void setControlsValueRS232_485(const QString &factorCode);
    void setControlsValueOther(const QString &factorCode);

    bool eventFilter(QObject *obj, QEvent*event);

};

#endif // DIALOGSETUPCOMMFACTOR_H
