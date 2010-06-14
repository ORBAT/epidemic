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
            m_arrx(new double[plotSize]),
            m_arry(new double[plotSize]),
            ui(new Ui::MdiPlot)
    {

        setAttribute(Qt::WA_DeleteOnClose);

        ::QwtPainter::setDeviceClipping(false); // disable polygon clipping
        ui->setupUi(this);
        QwtPlotCurve *inf = new QwtPlotCurve(tr("Infected"));
        QwtPlotCurve *sus = new QwtPlotCurve(tr("Susceptible"));

        //inf->setData(*m_x, *m_y);
        //sus->setData(*m_x, *m_y2);


        sus->setPen(QPen(Qt::blue));
        inf->setPen(QPen(Qt::red));

        for(int i = 0; i < plotSize; ++i) {
            m_arrx[i] = (ratioType)i*DT;
            m_arry[i] = 0.0;
        }

        inf->setRawData(m_arrx, m_arry, m_plotSize);

        sus->attach(ui->qwtPlot);
        inf->attach(ui->qwtPlot);

        // disable caching
        ui->qwtPlot->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
        ui->qwtPlot->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);

        ui->qwtPlot->setAxisScale(QwtPlot::xBottom, 0, m_plotSize);
        ui->qwtPlot->setAxisScale(QwtPlot::yLeft, 0, 1000000);


        this->connect(m_city, SIGNAL(infected(amountType)), SLOT(rcvInfected(amountType)));
        //this->connect(m_city, SIGNAL(susceptible(amountType)), SLOT(rcvSusceptible(amountType)));

    }

    MdiPlot::~MdiPlot()
    {
        delete ui;
        delete[] m_arrx;
        delete[] m_arry;
    }

    void MdiPlot::rcvInfected(amountType at) {
        // shift data left
        qDebug() << tr("%1 received %2").arg("rcvInfected();").arg(at);
        for(int i = 0 ; i < m_plotSize-1; ++i) {
            m_arry[i] = m_arry[i+1];
            //qDebug() << tr("%1: at i: %2").arg("rcvInfected();").arg(m_arry[i]);
        }

        m_arry[m_plotSize-1] = (double)at;
        //ui->qwtPlot->replot();
    }

    void MdiPlot::rcvSusceptible(amountType at) {
        // TODO
        //ui->qwtPlot->replot();
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
