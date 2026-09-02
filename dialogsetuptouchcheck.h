#ifndef DIALOGSETUPTOUCHCHECK_H
#define DIALOGSETUPTOUCHCHECK_H

#include <QDialog>

namespace Ui {
class DialogSetupTouchCheck;
}

class DialogSetupTouchCheck : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupTouchCheck(QWidget *parent = nullptr);
    ~DialogSetupTouchCheck();

private slots:
    void on_pushButton_3_clicked();

private:
    Ui::DialogSetupTouchCheck *ui;
};

#endif // DIALOGSETUPTOUCHCHECK_H
