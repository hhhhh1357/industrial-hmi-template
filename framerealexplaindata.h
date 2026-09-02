#ifndef FRAMEREALEXPLAINDATA_H
#define FRAMEREALEXPLAINDATA_H

#include <QFrame>

namespace Ui {
class FrameRealExplainData;
}

class FrameRealExplainData : public QFrame
{
    Q_OBJECT

public:
    explicit FrameRealExplainData(QWidget *parent = nullptr);
    ~FrameRealExplainData();

private:
    Ui::FrameRealExplainData *ui;
};

#endif // FRAMEREALEXPLAINDATA_H
