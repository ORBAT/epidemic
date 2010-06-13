#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "city.h"
#include "pathogen.h"

namespace QtEpidemy {

    MainWindow::MainWindow(QWidget *parent) :
            QMainWindow(parent),
            c(new City(tr("Derperton"), 1000000, this)),
            p(new Pathogen(1,0,1,this)),
            ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
        this->connect(p, SIGNAL(statChanged(PathogenStats,ratioType)),SLOT(testSlot(PathogenStats,ratioType)));
        c->setPathogen(p);
        p->setStatistic(PS_DURATION, 5.0);
        p->setStatistic(PS_INFECTION, 0.002);
        p->setStatistic(PS_SURVIVAL, 0.9);


    }


    void MainWindow::testSlot(PathogenStats ps, ratioType rt) {
        qDebug() << "testSlot() ps" << ps << "rt" << rt;
    }

    void MainWindow::testSlot(amountType at) {
        qDebug() << "testSlot() at"<<at;
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
