#ifndef FRAMESETUP_H
#define FRAMESETUP_H

#include <QFrame>

namespace Ui {
class FrameSetup;
}

class FrameSetup : public QFrame
{
    Q_OBJECT

public:
    explicit FrameSetup(QWidget *parent = nullptr);
    ~FrameSetup();

private slots:
    void on_btn1_clicked();
    void on_btn4_clicked();

   // void on_btnControl_clicked();
    void on_btnOther_clicked();

public slots:
    void setForceReboot();

private:
    Ui::FrameSetup *ui;
    void initAllButtonsStyle();

    QString ButtonStytleNormal="";
    QString ButtonStytlePresed="";

    QFrame * currFrame=nullptr;

    bool forceReboot = false;

/***************************************强制重启*******************************/
signals:
    bool isRboot();

//判断是否重启
//public:
//    bool comReBoot = false;
};

#endif // FRAMESETUP_H
