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
            Q_ASSERT(c->getPosition().bearingToInDegrees(s->getPosition()) == 260);

        DPR(tr("Traveling 42km from Helsinki towards Stockholm brings us to %1").
            arg(c->getPosition().moveTowards(s->getPosition(), 42).toString())
        );

        DPR(tr("Using moveFrom() the result is %1").
            arg(Position::moveFrom(s->getPosition(), 259, 42).toString()));

        DPR(tr("Point 42km from Helsinki at a bearing of 259 degrees: %1 (should be ~60.09583N, 24.19639E)").
            arg(Position::moveFrom(c->getPosition(), Position::degToRad(259), 42).toString())
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
    DPR(tr("Starting out at %1, should end up at %2 after traveling %3 km").
        arg(p.toString()).
        arg(nextp.toString()).
        arg(TRANSPORT_SPEEDS[TT_BOAT]*DT*24)
        );
    DPR(tr("Distance between those points is %1").arg(p.distanceTo(nextp)));
    Q_ASSERT((p.distanceTo(nextp) - 30.0) < 0.5); // 0.5 is small enough

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
