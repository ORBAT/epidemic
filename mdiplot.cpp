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
            ui(new Ui::MdiPlot)
    {

        setAttribute(Qt::WA_DeleteOnClose);

        ::QwtPainter::setDeviceClipping(false); // disable polygon clipping
        ui->setupUi(this);

        for(int i = 0; i < plotSize; ++i) {
            m_xarray[i] = i;
        }

        // initialize all pointers to NULL
        for(int i = 0; i < CS_MAX_STATS; ++i) {
            m_yarrayList.append(NULL);
        }

        QwtPlot *plot = ui->qwtPlot;

        // disable caching
        plot->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
        plot->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);

        plot->setAxisScale(QwtPlot::xBottom, 0, m_plotSize);

        plot->plotLayout()->setAlignCanvasToScales(true);

        plot->setAxisScaleDraw(QwtPlot::yLeft, new AmountTypeScaleDraw());


        // whenever a step's finished, replot the curves
        this->connect(m_city, SIGNAL(stepped()), SLOT(replot()));
        this->connect(this, SIGNAL(destroyed()), SLOT(deleteCurveData()));

        this->connect(m_city, SIGNAL(statUpdate(CityStats,amountType)),
                      SLOT(cityStatUpdate(CityStats,amountType)));

    }

    MdiPlot::~MdiPlot()
    {
        delete ui;
        delete[] m_xarray;
    }

    void MdiPlot::deleteCurveData() {
        for(int i = 0; i < CS_MAX_STATS; ++i) {
            delete[] m_yarrayList[(CityStats)i];
        }
    }

    void MdiPlot::cityStatUpdate(CityStats cs, amountType at) {
        // TODO: only change scale if the value of the stat has changed
        if(cs == m_scaleBy) {
            ui->qwtPlot->setAxisScale(QwtPlot::yLeft, 0, at);
        }

        /* do we have an array for the specified stat? If we do, then we're following
           it */
        if(m_yarrayList[cs] == NULL)
            return;
        qDebug() << tr("%1: received update for %2 (%3)").arg("cityStatUpdate()")
                .arg(cs).arg(at);
        double *yarr = m_yarrayList[cs];
        // shift data in array left
        for(int i = 0; i < m_plotSize-1; ++i) {
            yarr[i] = yarr[i+1];
        }
        yarr[m_plotSize-1] = (double)at;

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

        qcur->setRawData(m_xarray, m_yarrayList[cs], m_plotSize);

        // unique colors for the first NUMCOLORS curves
        qcur->setPen(QPen(PENCOLORS[m_numCurves % NUMCOLORS]));
        qcur->attach(ui->qwtPlot);
    }

    void MdiPlot::replot() {
        ui->qwtPlot->replot();
    }

//    void MdiPlot::rcvInfected(amountType at) {
//        // shift data left
//        qDebug() << tr("%1 received %2").arg("rcvInfected();").arg(at);
//        for(int i = 0 ; i < m_plotSize-1; ++i) {
//            m_arry[i] = m_arry[i+1];
//            //qDebug() << tr("%1: at i: %2").arg("rcvInfected();").arg(m_arry[i]);
//        }

//        m_arry[m_plotSize-1] = (double)at;
//        //ui->qwtPlot->replot();
//    }


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
