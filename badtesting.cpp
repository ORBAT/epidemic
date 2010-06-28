#include "badtesting.h"
#include "position.h"
#include "city.h"
#include "transport.h"
namespace QtEpidemy {


    BadTesting::BadTesting(QObject *parent) :
            QObject(parent), c(new City("Helsinki", 500000, Position(60.17,24.94))),
            s(new City("Stockholm",1700000, Position(59.33, 18.07)))
    {
    }


    void BadTesting::doPositionTesting() {

        DPR("Starting Position tests...\n\n");

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
    }

    void BadTesting::doTransportTesting() {
    DPR("Starting Transport tests...\n\n");
//        Transport t(TT_ONFOOT, c, s, );
    QList<quint16> arr;
    arr << 1 << 2;
    DPR("Time to Stockholm should be about 13h 11min");
    Transport boat(TT_BOAT, c, s, arr ,0);

    Position p = boat.position();
    Position nextp = p.moveTowards(s->getPosition(),
                                   TRANSPORT_SPEEDS[TT_BOAT]*DT*24);

    DPR(tr("DT is %1 HOUR(S). Boat speed is %2 km/h. Boat reports %3 travel time"
            " left before step()").
          arg(DT*24.0).arg(TRANSPORT_SPEEDS[TT_BOAT]).
          arg(boat.travelTimeLeft().toString()));



    }

    void BadTesting::doTests() {
        doPositionTesting();
        doTransportTesting();
    }

}
