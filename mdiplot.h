#ifndef MDIPLOT_H
#define MDIPLOT_H

#include <QWidget>
#include <qwt_scale_draw.h>
#include <qwt_plot_layout.h>
#include <QDateTime>
#include <QDebug>
#include "constants.h"


namespace Ui {
    class MdiPlot;
}

class QwtPlotCurve;



namespace QtEpidemy {


    class AmountTypeScaleDraw : public QwtScaleDraw {
    public:
        virtual QwtText label(double v) const {
            // return a decimal number without the goddamn E notation
            return QString::number(v,'f',0);
        }
    };

    class DateScaleDraw : public QwtScaleDraw {
    public:
        DateScaleDraw(const QDateTime &start) : m_start(start) {}

        virtual QwtText label(double v) const {
            qDebug() << Q_FUNC_INFO << "called for" << v << "ticks";
            /*
             Time is measured in ticks in-game, and DT days elapse every tick (see
             constants.h for value). The value that gets passed to label(double) is
             the amount of ticks elapsed after starting the game.

             v * DT gives the number of DAYS elapsed since starting the game

             Since DT is (currently, anyhow) < 24h, it's best to convert v * DT to
             seconds. */
            int secondsElapsed = (v * DT) * 86400; // 24h * 60min * 60sec
            QDateTime derp = m_start.addSecs(secondsElapsed);
            return derp.toString("yy-M-dd hh");
        }


    private:
        QDateTime m_start;
    };


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
        double *m_xarray; // the data for the x-axis
        QList<double*> m_yarrayList;
        CityStats m_scaleBy; // scale y-axis by this City stat. CS_POPULATION by default
        quint8 m_numCurves;


    public slots:
        void cityStatUpdate(CityStats, amountType);
        void changeYScale(CityStats);

        // adds a new curve for the specified statistic
        void followStatistic(CityStats);
        void replot();

    protected:
        void deleteCurveData();  // deletes all curve data from m_yarrayList


    private:
        Ui::MdiPlot *ui;
    };

}

#endif // MDIPLOT_H
