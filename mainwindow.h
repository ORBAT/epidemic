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
        CityController *m_cityController;

        void additionalUiSetup();
        void connectActions();

    private:
        Ui::MainWindow *ui;

private slots:
    void on_actionAdd_random_city_triggered();
    void on_actionInfect_city_Derp_triggered();
};
}
#endif // MAINWINDOW_H
