#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "city.h"
#include "pathogen.h"
#include "mdiplot.h"
#include "citycontroller.h"


namespace QtEpidemy {

    MainWindow::MainWindow(QWidget *parent) :
            QMainWindow(parent),
            m_cityController(new CityController(this)),
            ui(new Ui::MainWindow)
    {
        ui->setupUi(this);

        additionalUiSetup();

        m_cityController->createCity("Derp", 100000, QPointF(42.0,42.0));
        City *c = m_cityController->getCity("Derp");
        MdiPlot *mp = NULL;
        if(c) {
            mp = new MdiPlot(c, 100, QDateTime::currentDateTime());
            ui->centralWidget->addSubWindow(mp, Qt::SubWindow);
        }
        connectActions();
    }


    void MainWindow::connectActions() {
        connect(ui->actionStart, SIGNAL(triggered()), m_cityController, SLOT(start()));
        connect(ui->actionPause, SIGNAL(triggered()), m_cityController, SLOT(pause()));
    }

    void MainWindow::additionalUiSetup() {
        this->setWindowState(Qt::WindowMaximized);
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }

    void MainWindow::changeEvent(QEvent *e)
    {
        QMainWindow::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
        }
    }


}

void QtEpidemy::MainWindow::on_actionInfect_city_Derp_triggered()
{
    QtEpidemy::City *c = m_cityController->getCity("Derp");
    QtEpidemy::Pathogen *p = new QtEpidemy::Pathogen(0.99, 0.00002, 10, this);
    if(c) {
        c->setPathogen(p);
        c->addInfected(1);
    }


}


