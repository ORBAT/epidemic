#ifndef MDIPLOT_H
#define MDIPLOT_H

#include <QWidget>

namespace Ui {
    class MdiPlot;
}

class MdiPlot : public QWidget
{
    Q_OBJECT

public:
    explicit MdiPlot(QWidget *parent = 0);
    ~MdiPlot();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MdiPlot *ui;
};

#endif // MDIPLOT_H
