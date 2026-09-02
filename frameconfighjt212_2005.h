#ifndef FRAMECONFIGHJT212_2005_H
#define FRAMECONFIGHJT212_2005_H

#include <QFrame>

namespace Ui {
class FrameConfigHJT212_2005;
}

class FrameConfigHJT212_2005 : public QFrame
{
    Q_OBJECT

public:
    explicit FrameConfigHJT212_2005(QWidget *parent = nullptr);
    ~FrameConfigHJT212_2005();

private slots:
    void on_btnClose_clicked();

    void on_btnCancel_clicked();

private:
    Ui::FrameConfigHJT212_2005 *ui;
};

#endif // FRAMECONFIGHJT212_2005_H
