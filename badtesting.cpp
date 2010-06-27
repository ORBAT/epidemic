#include "badtesting.h"
#include "position.h"
#include "city.h"
#include "transport.h"
namespace QtEpidemy {


    BadTesting::BadTesting(QObject *parent) :
            QObject(parent)
    {
    }


    void BadTesting::doPositionTesting() {

        City *c = new City("Helsinki", 500000, Position(60.17,24.94));
        City *s = new City("Stockholm",1700000, Position(59.33, 18.07));

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
            arg(Position::moveTowards(c->getPosition(), Position::degToRad(42), 42).toString())
            );

        Transport t(TT_ONFOOT, c, s, this);
    }

    void BadTesting::doTests() {
        doPositionTesting();
    }

}
