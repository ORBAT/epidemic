#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "city.h"
#include "pathogen.h"
#include "mdiplot.h"


namespace QtEpidemy {

    MainWindow::MainWindow(QWidget *parent) :
            QMainWindow(parent),
            c(new City(tr("Derperton"), 10000, this)),
            p(new Pathogen(1,0,1,this)),
            ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
        c->setPathogen(p);
        p->setStatistic(PS_DURATION, 5.0);
        p->setStatistic(PS_INFECTION, 0.002);
        p->setStatistic(PS_SURVIVAL, 0.9);

        MdiPlot *mp = new MdiPlot(c, 100, QDateTime::currentDateTime());
        mp->showStatistic(CS_INFECTED);
        mp->showStatistic(CS_SUSCEPTIBLE);
        mp->showStatistic(CS_DEAD);
        ui->centralWidget->addSubWindow(mp);
        QTimer *timer = new QTimer(this);
        QTimer *remover = new QTimer(this);
        remover->setSingleShot(true);

        connect(timer, SIGNAL(timeout()), c, SLOT(step()));
        connect(remover, SIGNAL(timeout()), mp, SLOT(hideStatistic()));

        c->addInfected(1);
        timer->start(450);
        remover->start(2000);

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
