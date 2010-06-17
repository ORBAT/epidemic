#include <QDebug>
#include <QFontMetrics>
#include <qwt_plot_curve.h>
#include <qwt_plot_layout.h>
#include <qwt_painter.h>
#include <qwt_plot_canvas.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include "mdiplot.h"
#include "ui_mdiplot.h"
#include "city.h"
#include "mdisettingscontroller.h"

namespace QtEpidemy {

    MdiPlot::MdiPlot(City *c, int plotSize, const QDateTime &start, QWidget *parent) :
            QWidget(parent),
            m_city(c),
            m_plotSize(plotSize),
            m_xAxisData(new double[plotSize]),
            m_scaleBy(CS_POPULATION),
            m_numCurves(0),
            m_numDataPoints(0),
            ui(new Ui::MdiPlot)
    {

        setAttribute(Qt::WA_DeleteOnClose);


        ui->setupUi(this);
        m_qwtPlot = ui->qwtPlot;
        m_settingsController = new MdiSettingsController(ui->settingsGridLayout, this);


        for(int i = 0; i < m_plotSize; ++i) {
//            m_xAxisData[m_plotSize - 1 - i] = i;
            m_xAxisData[i] = m_plotSize - i - 1;
        }

        /* there are at most CS_MAX_STATS stats a City can send */
        for(int i = 0; i < CS_MAX_STATS; ++i) {
            m_curveData.append(CurveData());
            m_curveData[i].curve = NULL;
            m_curveData[i].curvePoints = new double[m_plotSize];
            for(int k = 0; k < m_plotSize; ++k) {
                m_curveData[i].curvePoints[k] = 0;
            }
        }

        // disable caching
        m_qwtPlot->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
        m_qwtPlot->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
        // disable polygon clipping
        ::QwtPainter::setDeviceClipping(false);

        QwtLegend *legend = new QwtLegend();
        legend->setItemMode(QwtLegend::ReadOnlyItem);
        m_qwtPlot->insertLegend(legend);

//        m_qwtPlot->plotLayout()->setAlignCanvasToScales(true);

        m_qwtPlot->setAxisScaleDraw(QwtPlot::yLeft, new AmountTypeScaleDraw());
        m_qwtPlot->setAxisScaleDraw(QwtPlot::xBottom, new DateScaleDraw(start));

        m_qwtPlot->setAxisScale(QwtPlot::xBottom, 0, m_plotSize);
        m_qwtPlot->setAxisLabelRotation(QwtPlot::xBottom, -75.0);
        m_qwtPlot->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignTop);





        /*
         To avoid having the canvas jump every time there's a label at the rightmost
         part of the X axis, a permanent margin needs to be added.

         This trick is from Qwt's cpuplot example
         */
        QwtScaleWidget *scaleWidget = m_qwtPlot->axisWidget(QwtPlot::xBottom);
        const int fontHgh = QFontMetrics(scaleWidget->font()).height();
        scaleWidget->setMinBorderDist(0, fontHgh/2);


        // whenever a step's finished, replot the curves
        this->connect(m_city, SIGNAL(stepped()), SLOT(replot()));
//        this->connect(this, SIGNAL(destroyed()), SLOT(deleteCurveData()));

        this->connect(m_city, SIGNAL(statUpdate(CityStats,amountType)),
                      SLOT(cityStatUpdate(CityStats,amountType)));

    }

    MdiPlot::~MdiPlot()
    {
        delete[] m_xAxisData;
        for(int i = 0; i < CS_MAX_STATS; ++i)
            delete[] m_curveData[i].curvePoints;
        delete ui;

    }

    void MdiPlot::cityStatUpdate(CityStats cs, amountType at) {

        if(cs == m_scaleBy) {
        // only change scale if the value of the stat has changed
            if(m_curveData[cs].curvePoints[0] != at)
                m_qwtPlot->setAxisScale(QwtPlot::yLeft, 0, at);
        }

        DPR(tr("Received update for %2 (%3)").arg(CS_NAMES[cs]).arg(at));

        double * yarr = m_curveData[cs].curvePoints;

        // shift data in array left
        for(int i = m_numDataPoints - 1; i > 0; --i) {
            yarr[i] = yarr[i-1];
        }
        yarr[0] = (double)at;

    }

    void MdiPlot::changeYScale(CityStats cs) {
        m_scaleBy = cs;
    }

    void MdiPlot::showStatistic(CityStats cs) {

        QwtPlotCurve *qcur = new QwtPlotCurve(CS_NAMES[cs]);
        QPen qp;

        // unique colors for the first CURVE_NUMCOLORS curves
        qp.setColor(CURVE_PENCOLORS[m_numCurves % CURVE_NUMCOLORS]);
        // and when we run out of colors, change the style
        /*
         Example with 3 colors and 3 styles:
         curve # 0 1 2 3 4 5 6 7 8 9
         color # 0 1 2 0 1 2 0 1 2 0
         style # 0 0 0 1 1 1 2 2 2 0

         This means that you can have at most CURVE_NUMCOLORS * CURVE_NUMSTYLES curves if
         you want the color/style combination to be unique
         */
        qp.setStyle(CURVE_PENSTYLES[(m_numCurves / (CURVE_NUMCOLORS)) % CURVE_NUMSTYLES]);
        qp.setWidth(2);


        qcur->setPen(qp);

        qcur->attach(m_qwtPlot);

        m_curveData[cs].curve = qcur;

        ++m_numCurves;
    }

    void MdiPlot::hideStatistic(CityStats cs) {
        DPR(tr("Hiding %1").arg(CS_NAMES[cs]));
        QwtPlotCurve *&item = m_curveData[cs].curve;
        // is the stat actually being shown?
        if(item != NULL) {
            item->hide();
            item->detach();
            delete item;
            item = NULL;
        }
        --m_numCurves;
    }


    void MdiPlot::replot() {

        if(m_numDataPoints < m_plotSize)
            ++m_numDataPoints;


        // "shift" the X axis to the left
        for(int i = 0; i < m_plotSize; ++i) {
            m_xAxisData[i]++;
        }

        // rescale the X axis to fit the new X axis data.
        m_qwtPlot->setAxisScale(QwtPlot::xBottom,
                                  m_xAxisData[m_plotSize-1], m_xAxisData[0]);


        for(int i = 0; i < CS_MAX_STATS; i++) {
            if(m_curveData.at(i).curve != NULL)
                m_curveData[i].curve->setRawData(m_xAxisData, m_curveData.at(i).curvePoints,
                                                 m_numDataPoints);
        }

        m_qwtPlot->replot();
    }

    void MdiPlot::setStatVisibility(CityStats cs, bool visible) {
        if(visible)
            showStatistic(cs);
        else
            hideStatistic(cs);
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
