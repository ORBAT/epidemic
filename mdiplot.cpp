#include "mdiplot.h"
#include "ui_mdiplot.h"
#include "qwt_plot_curve.h"
#include "qwt_painter.h"
#include "qwt_plot_canvas.h"

namespace QtEpidemy {

    MdiPlot::MdiPlot(City *c, QWidget *parent) :
            QWidget(parent),
            m_city(c),
            ui(new Ui::MdiPlot)
    {
        ::QwtPainter::setDeviceClipping(false); // disable polygon clipping
        ui->setupUi(this);
        QwtPlotCurve *inf = new QwtPlotCurve(tr("Infected"));
        QwtPlotCurve *sus = new QwtPlotCurve(tr("Susceptible"));
        sus->attach(ui->qwtPlot);
        inf->attach(ui->qwtPlot);

        sus->setPen(QPen(Qt::blue));
        inf->setPen(QPen(Qt::red));

        // disable caching
        ui->qwtPlot->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
        ui->qwtPlot->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);


    }

    MdiPlot::~MdiPlot()
    {
        delete ui;
    }

    void MdiPlot::rcvInfected(amountType at) {

    }

    void MdiPlot::rcvSusceptible(amountType) {

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
