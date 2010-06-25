#include "position.h"

namespace QtEpidemy {


    Position::Position(const QPointF &pos) : m_position(pos)
    {
        clampQPointValues(m_position);
    }

    Position::Position(qreal latitude, qreal longtitude) :
            m_position(QPointF(latitude,longtitude)) {
        clampQPointValues(m_position);
    }

    qreal Position::distanceBetween(const Position &a,
                            const Position &b) {
        /*

         dlon = lon2 - lon1
         dlat = lat2 - lat1
         a = (sin(dlat/2))^2 + cos(lat1) * cos(lat2) * (sin(dlon/2))^2
         c = 2 * atan2(sqrt(a), sqrt(1-a))
         d = R * c

         lon = y axis
         lat = x axis

var R = 6371; // km
var dLat = (lat2-lat1).toRad();
var dLon = (lon2-lon1).toRad();
var a = Math.sin(dLat/2) * Math.sin(dLat/2) +
        Math.cos(lat1.toRad()) * Math.cos(lat2.toRad()) *
        Math.sin(dLon/2) * Math.sin(dLon/2);
var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));
var d = R * c;
var value2 = d * 3280.8399;


         */

        qreal lat1 = a.latRad();
        qreal lat2 = b.latRad();
        qreal lon1 = a.lonRad();
        qreal lon2 = b.lonRad();
        qreal dLat = lat2 - lat1;
        qreal dLon = lon2 - lon1;

        qreal ah = pow(sin(dLat / 2.0), 2.0) + cos(lat1) *
                  cos(lat2) * pow(sin(dLon / 2.0), 2.0);
        return EARTH_RADIUS * 2.0 * atan2(sqrt(ah), sqrt(1.0-ah));
    }

}
