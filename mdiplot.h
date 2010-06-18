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

    class MdiSettingsController;

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
           /*
             Time is measured in ticks in-game, and DT days elapse every tick (see
             constants.h for value). The value that gets passed to label(double) is
             the amount of ticks elapsed after starting the game.

             v * DT gives the number of DAYS elapsed since starting the game

             Since DT is (currently, anyhow) < 24h, it's best to convert v * DT to
             seconds. */
            /**
              FIXME:
              This will roll over when v reaches 596523
              */
            int secondsElapsed = (v * DT) * 86400; // 24h * 60min * 60sec
            QDateTime derp = m_start.addSecs(secondsElapsed);
            return derp.toString("M/dd hh");
        }


    private:
        QDateTime m_start;
    };


    class City;



    class MdiPlot : public QWidget
    {
        Q_OBJECT

    public:
        explicit MdiPlot(City *c, int plotSize, const QDateTime &start, QWidget *parent = 0);
        ~MdiPlot();

    signals:
        void statVisibilityToggled(CityStats, bool);

    protected:
        void changeEvent(QEvent *e);
        City *m_city;

        /* populates the Settings tab with the necessary controllers and sends signals
           when they change */
        MdiSettingsController *m_settingsController;
        int m_plotSize;

        struct CurveData {
            QwtPlotCurve *curve;
            double *curvePoints;  // array of points on the curve
        };

        /* this list has CS_MAX_STATS elements. It is used to store all City stats
           (a maximum of m_plotSize values are stored) and the associated curve of the
           stat (if any) */
        QList<CurveData> m_curveData;

        double *m_xAxisData; // the data for the x-axis

        CityStats m_scaleBy; // scale y-axis by this City stat. CS_POPULATION by default

        /* how many curves there are on the current plot. Basically the same as the number
           of non-null elements in m_yarrayList or m_pcList */
        qint8 m_numCurves;

        qint32 m_numDataPoints; // how many data points have been collected so far


    public slots:
        /* used to add data to a City stat. Note that this WON'T cause the data to be shown.
           showStatistic() will actually add the curve, and replot() will, well, replot
           everything */
        void cityStatUpdate(CityStats, AmountType);
        void changeYScale(CityStats);

        // shows the curve for the specified statistic
        void showStatistic(CityStats);
        // hides the curve for a statistic
        void hideStatistic(CityStats cs);

        // calls either showStatistic() or hideStatistic() depending on the bool
        void setStatVisibility(CityStats cs, bool);

        void replot();


    private:
        Ui::MdiPlot *ui;
        QwtPlot * m_qwtPlot;
    };

}

#endif // MDIPLOT_H
