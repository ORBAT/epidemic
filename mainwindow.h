#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "constants.h"

namespace Ui {
    class MainWindow;
}

namespace QtEpidemy {
    class City;
    class Pathogen;





    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    protected:
        void changeEvent(QEvent *e);
        City *c;
        Pathogen *p;

    private:
        Ui::MainWindow *ui;

    private slots:
        void testSlot(amountType);
        void testSlot(PathogenStats, ratioType);
    };
}
#endif // MAINWINDOW_H
