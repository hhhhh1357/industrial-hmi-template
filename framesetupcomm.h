#ifndef FRAMESETUPCOMM_H
#define FRAMESETUPCOMM_H

#include <QFrame>

namespace Ui {
class FrameSetupComm;
}

class FrameSetupComm : public QFrame
{
    Q_OBJECT

public:
    explicit FrameSetupComm(QWidget *parent = nullptr);
    ~FrameSetupComm();

private slots:


    void on_btnRS232_clicked();

    void on_btnRS485_clicked();

    void on_btnAI_clicked();

    void on_btnSwitch_clicked();

    void on_btnRelay_clicked();

    void on_btnWater_clicked();

    void on_btnServer_clicked();

    void on_btnNetwork_clicked();

    void on_btnSite_clicked();

    void on_btnDataUp_clicked();

private:
    Ui::FrameSetupComm *ui;

public slots:
    void setForceReboot();

signals:
    void toReboot();

//判断是否重启
public:
    bool comReBoot = false;

};

#endif // FRAMESETUPCOMM_H
