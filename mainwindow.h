#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "constants.h"

namespace Ui {
    class MainWindow;
}

namespace QtEpidemy {
    class City;
    class CityController;
    class Pathogen;





    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    protected:
        void changeEvent(QEvent *e);
//        City *c;
        Pathogen *p;
        CityController *m_cityController;

        void connectActions();

    private:
        Ui::MainWindow *ui;

private slots:
    void on_actionInfect_city_Derp_triggered();
};
}
#endif // MAINWINDOW_H
