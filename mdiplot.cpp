#include "mdiplot.h"
#include "ui_mdiplot.h"

MdiPlot::MdiPlot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MdiPlot)
{
    ui->setupUi(this);
}

MdiPlot::~MdiPlot()
{
    delete ui;
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
