#ifndef MDIPLOT_H
#define MDIPLOT_H

#include <QWidget>
#include "constants.h"
#include "qwt_array.h"

namespace Ui {
    class MdiPlot;
}

class QwtPlotCurve;



namespace QtEpidemy {

    class City;

    class MdiPlot : public QWidget
    {
        Q_OBJECT

    public:
        explicit MdiPlot(City *c, int plotSize, QWidget *parent = 0);
        ~MdiPlot();

    protected:
        void changeEvent(QEvent *e);
        City *m_city;
        int m_plotSize;
        double *m_arrx;
        double *m_arry;


    protected slots:
        void rcvSusceptible(amountType);
        void rcvInfected(amountType);


    private:
        Ui::MdiPlot *ui;
    };

}

#endif // MDIPLOT_H
