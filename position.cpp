#include "position.h"

namespace QtEpidemy {


    Position::Position(const QPointF &pos) : m_position(pos)
    {
        clampQPointValues(m_position);
    }

    Position::Position(qreal latitude, qreal longitude) :
            m_position(QPointF(longitude,latitude)) {
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
         */

        qreal lat1 = a.latRad();
        qreal lat2 = b.latRad();
        qreal lon1 = a.lonRad();
        qreal lon2 = b.lonRad();
        qreal dLat = lat2 - lat1;
        qreal dLon = lon2 - lon1;

        qreal ah = pow(sin(dLat / 2.0), 2.0) + cos(lat1) *
                  cos(lat2) * pow(sin(dLon / 2.0), 2.0);
        qreal ch = 2.0 * atan2(sqrt(ah), sqrt(1.0-ah));
        return EARTH_RADIUS * ch;
    }

    qreal Position::distanceTo(const Position &b) const {
        return Position::distanceBetween(*this, b);
    }

    /**
      returns bearing to Position b in degrees.
      */
    qreal Position::bearingTo(const Position &b) {
        /*
        var y = Math.sin(dLon) * Math.cos(lat2);
        var x = Math.cos(lat1)*Math.sin(lat2) -
                Math.sin(lat1)*Math.cos(lat2)*Math.cos(dLon);
        var brng = Math.atan2(y, x).toDeg();
        */

        qreal lat1 = this->latRad();
        qreal lat2 = b.latRad();
        qreal lon1 = this->lonRad();
        qreal lon2 = b.lonRad();
        qreal dlat = lat2 - lat1;
        qreal dlon = lon2 - lon1;
        qreal y = sin(dlon) * cos(lat2);
        qreal x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dlon);
        qreal br = atan2(y, x) / DEG_TO_RAD;
        return (br+360) % 360;
    }

}
