#include <QDebug>
#include "mdiplot.h"
#include "ui_mdiplot.h"
#include "qwt_plot_curve.h"
#include "qwt_painter.h"
#include "qwt_plot_canvas.h"
#include "city.h"

namespace QtEpidemy {

    MdiPlot::MdiPlot(City *c, int plotSize, QWidget *parent) :
            QWidget(parent),
            m_city(c),
            m_plotSize(plotSize),
            m_xarray(new double[plotSize]),
            m_scaleBy(CS_POPULATION),
            m_numCurves(0),
            m_numDataPoints(0),
            ui(new Ui::MdiPlot)
    {

        setAttribute(Qt::WA_DeleteOnClose);

        ::QwtPainter::setDeviceClipping(false); // disable polygon clipping
        ui->setupUi(this);

        for(int i = 0; i < m_plotSize; ++i) {
            m_xarray[m_plotSize - 1 - i] = i;
        }

        /* there are at most CS_MAX_STATS stats a City can send */
        for(int i = 0; i < CS_MAX_STATS; ++i) {
            m_yarrayList.append(NULL);
            m_pcList.append(NULL);
        }

        QwtPlot *plot = ui->qwtPlot;

        // disable caching
        plot->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
        plot->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);

        plot->setAxisScale(QwtPlot::xBottom, 0, m_plotSize);
        plot->setAxisLabelRotation(QwtPlot::xBottom, -50.0);
        plot->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);

        plot->plotLayout()->setAlignCanvasToScales(true);

        plot->setAxisScaleDraw(QwtPlot::yLeft, new AmountTypeScaleDraw());


        // whenever a step's finished, replot the curves
        this->connect(m_city, SIGNAL(stepped()), SLOT(replot()));
//        this->connect(this, SIGNAL(destroyed()), SLOT(deleteCurveData()));

        this->connect(m_city, SIGNAL(statUpdate(CityStats,amountType)),
                      SLOT(cityStatUpdate(CityStats,amountType)));

    }

    MdiPlot::~MdiPlot()
    {
//        delete[] m_xarray;
    }

    void MdiPlot::deleteCurveData() {
        for(int i = 0; i < CS_MAX_STATS; ++i) {
            delete[] m_yarrayList[i];
        }
    }

    void MdiPlot::cityStatUpdate(CityStats cs, amountType at) {
        // TODO: only change scale if the value of the stat has changed
        if(cs == m_scaleBy) {
            ui->qwtPlot->setAxisScale(QwtPlot::yLeft, 0, at);
        }

        if(m_numDataPoints < m_plotSize)
            m_numDataPoints++;

        /* do we have an array for the specified stat? If we do, then we're following
           it */
        if(m_yarrayList[cs] == NULL)
            return;


        qDebug() << tr("%1: received update for %2 (%3)").arg("cityStatUpdate()")
                .arg(cs).arg(at);
        double *yarr = m_yarrayList[cs];
        // shift data in array left
        for(int i = m_numDataPoints; i > 0; --i) {
            yarr[i] = yarr[i-1];
        }
        yarr[0] = (double)at;

    }

    void MdiPlot::changeYScale(CityStats cs) {
        m_scaleBy = cs;
    }

    void MdiPlot::followStatistic(CityStats cs) {
        if(m_yarrayList[cs] != NULL) {
            qDebug() << tr("%1: already following cs %2").arg("followStatistic()").arg(cs);
            return;
        }
        qDebug() << tr("%1: following cs %2").arg("followStatistic()").arg(cs);
        // create a new double[] for the data we want to follow
        m_yarrayList[cs] = new double[m_plotSize];

        // zero the data
        for(int i = 0; i < m_plotSize; ++i) {
            m_yarrayList[cs][i] = 0;
        }
        QwtPlotCurve *qcur = new QwtPlotCurve(CS_NAMES[cs]);

        // unique colors for the first NUMCOLORS curves
        qcur->setPen(QPen(PENCOLORS[m_numCurves % NUMCOLORS]));
        qcur->attach(ui->qwtPlot);
        m_pcList[cs] = qcur;
        m_numCurves++;
    }

    void MdiPlot::replot() {

        // "shift" the X axis to the left
        for(int i = 0; i < m_plotSize; ++i) {
            m_xarray[i]++;
        }

        for(int i = 0; i < CS_MAX_STATS; ++i) {
            if(m_pcList.at(i) != NULL)
                m_pcList[i]->setRawData(m_xarray, m_yarrayList.at(i), m_numDataPoints);
        }

        ui->qwtPlot->replot();

        // rescale the X axis to fit the new
        ui->qwtPlot->setAxisScale(QwtPlot::xBottom,
                                  m_xarray[m_plotSize-1], m_xarray[0]);


    }


    void MdiPlot::changeEvent(QEvent *e)
    {
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
        }
    }

}
