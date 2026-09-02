#ifndef DIALOGSETUPSERVER_H
#define DIALOGSETUPSERVER_H

#include <QDialog>
#include <QRegExpValidator>

#include <Headers/Model/MConfig.h>

#include "Headers/BLL/BComArgs.h"
#include "Headers/BLL/BFactor.h"
#include "dialogsetupcommfactor.h"
#include "mainwindow.h"

namespace Ui {
class DialogSetupServer;
}

class DialogSetupServer : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupServer(QWidget *parent = nullptr);

    ~DialogSetupServer();

private slots:
      void on_btnClose_clicked();

      void on_btnSave_clicked();

      void on_rbServer1_clicked();

      void on_rbServer2_clicked();

      void on_rbServer3_clicked();

      void on_rbServer4_clicked();

      void on_chkEnabled_stateChanged(int arg1);

private:
    Ui::DialogSetupServer *ui;

    KeyBoard *keyBoard=nullptr;
    QRegExpValidator * pReg;

    QList <Server*> ServerList;

    void initComponents( );

    void setParamentComponentsEnable(bool enable);

    bool getArgs(QString argsStr, QString *IP, QString *PORT, QString *PASSWORD, int *INTERVAL);
    bool saveServerArgs(MComArgs * mca, QString IP, QString PORT, QString PASSWORD, int INTERVAL);
signals:
    void toReboot();
};

#endif // DIALOGSETUPSERVER_H
