#ifndef BADTESTING_H
#define BADTESTING_H

#include <QObject>
#include "position.h"
namespace QtEpidemy {
class BadTesting : public QObject
{
    Q_OBJECT
public:
    explicit BadTesting(QObject *parent = 0);
    void doPositionTesting();

signals:

public slots:

};

}

#endif // BADTESTING_H
