#ifndef DIALOGDIAGRAM_H
#define DIALOGDIAGRAM_H

#include "framediagram.h"

#include <QDialog>

namespace Ui {
class DialogDiagram;
}

class DialogDiagram : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDiagram(QWidget *parent = nullptr);
    DialogDiagram(MFactor *factor, QWidget *parent = nullptr);
    ~DialogDiagram();

private slots:
    void on_btnClose_clicked();

private:
    Ui::DialogDiagram *ui;

    FrameDiagram * frameDiagram;
    MFactor * factor;
};

#endif // DIALOGDIAGRAM_H
