#ifndef DIALOGSETUPENVIRONMENT_H
#define DIALOGSETUPENVIRONMENT_H

#include <QDialog>

namespace Ui {
class DialogSetupEnvironment;
}

class DialogSetupEnvironment : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupEnvironment(QWidget *parent = nullptr);
    ~DialogSetupEnvironment();

private:
    Ui::DialogSetupEnvironment *ui;
};

#endif // DIALOGSETUPENVIRONMENT_H
