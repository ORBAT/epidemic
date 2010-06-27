#include <ctime>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "city.h"
#include "pathogen.h"
#include "mdiplot.h"
#include "world.h"
#include "citycontroller.h"

// debugging. remove when done
#include "transport.h"
#include "position.h"



namespace QtEpidemy {

    MainWindow::MainWindow(QWidget *parent) :
            QMainWindow(parent),
            m_world(new World(this)),
            ui(new Ui::MainWindow)
    {
        ui->setupUi(this);

        additionalUiSetup();

        CityController* m_cityController = m_world->getCityController();
        m_cityController->createCity("Helsinki", 500000, Position(60.17,24.94));
        m_cityController->createCity("Stockholm",1700000, Position(59.33, 18.07));
        City *c = m_cityController->getCity("Helsinki");
        City *s = m_cityController->getCity("Stockholm");
        MdiPlot *mp = NULL;
        if(c) {
            mp = new MdiPlot(c, 100, QDateTime::currentDateTime());
            mp->showStatistic(CS_INFECTED);
            mp->showStatistic(CS_RECOVERED);
            mp->showStatistic(CS_D_INFECTIONS);
            mp->showStatistic(CS_D_INF_DEATHS);
            mp->showStatistic(CS_D_INF_RECOVER);

            ui->centralWidget->addSubWindow(mp, Qt::SubWindow);
        }
        connectActions();

        Position sanfran(37.76, -122.44);
        DPR(tr("Distance between Helsinki and San Fransisco according to Position: %1 (should be ~8724.73)").
            arg(Position::distanceBetween(c->getPosition(),
                                          sanfran))
            );

        DPR(tr("Distance between Helsinki and Stockholm according to Position: %1 (should be ~395.81)").
            arg(Position::distanceBetween(c->getPosition(),
                                          s->getPosition()))
            );

        DPR(tr("Initial bearing from Helsinki to Stockholm is %1 (should be ~259)").
            arg(c->getPosition().bearingToInDegrees(s->getPosition())));

        DPR(tr("Point 42km from Helsinki at a bearing of 42 degrees: %1 (should be ~60.449N 25.452E)").
            arg(Position::moveTowards(c->getPosition(), 42*Position::DEG_TO_RAD, 42).toString())
            );

        Transport t(TT_ONFOOT, c, s, this);

    }


    void MainWindow::connectActions() {
        connect(ui->actionStart, SIGNAL(triggered()), m_world, SLOT(start()));
        connect(ui->actionPause, SIGNAL(triggered()), m_world, SLOT(pause()));
    }

    void MainWindow::additionalUiSetup() {
        this->setWindowState(Qt::WindowMaximized);
        ui->tblCityData->setModel(m_world->getCityController()->getModel());
//        ui->tblCityData->horizontalHeader()->
//                setResizeMode(QHeaderView::ResizeToContents);
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
    CityController *m_cityController = m_world->getCityController();
    QtEpidemy::City *c = m_cityController->
                         getCities()
                         .at(qrand()%m_cityController->getCities().size());
    QtEpidemy::Pathogen *p = new QtEpidemy::Pathogen(0.99, 0.0000001, 10, 0.05,
                                                     this);
    if(c) {
        c->setPathogen(p);
        c->addInfected(1);
    }
}



void QtEpidemy::MainWindow::on_actionAdd_random_city_triggered()
{
    qsrand(time(NULL));
    QString cityName = QString("%1CityName").arg(time(NULL), 0, 16);
    int pop = qrand()%100000000;
    qreal x = qrand()%100;
    qreal y = qrand()%100;
    m_world->getCityController()->createCity(cityName, pop, Position(x,y));
}
