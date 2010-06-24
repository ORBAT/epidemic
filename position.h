#ifndef POSITION_H
#define POSITION_H

#include <algorithm>
#include <math.h>
#include <QPointF>


namespace QtEpidemy {

    class Position
    {
    public:
        // degrees / radian
        const static qreal DEG_TO_RAD = M_PI/180;
        const static qreal EARTH_RADIUS = 6367.5; //in km

        Position(const QPointF &pos);



    protected:
        QPointF m_position;

        /**
          Used to clamp x and y coordinates to the range -90 .. 90
          */
        inline void clampVal(qreal& val) {
            val = std::min(90.0, std::max(-90.0, val));
        }

        /**
          clamps a QPointF's X and Y coordinates to the range -90..90
          */
        inline void clampQPointValues(QPointF &point) {
            clampVal(point.rx());
            clampVal(point.ry());
        }

        inline qreal lon() const {
            return m_position.y();
        }

        inline qreal lat() const {
            return m_position.x();
        }

        inline qreal degToRad(qreal deg) {
            return deg * DEG_TO_RAD;
        }

        /**
          calculates the distance between Position a and Position b using
          Haversine formula
        */
        inline static qreal distanceTo(const Position &a,
                                       const Position &b) {
            /*

             dlon = lon2 - lon1
             dlat = lat2 - lat1
             a = (sin(dlat/2))^2 + cos(lat1) * cos(lat2) * (sin(dlon/2))^2
             c = 2 * atan2(sqrt(a), sqrt(1-a))
             d = R * c

             lon = y axis
             lat = x axis

             */

//            qreal dlon = b.



        }

    };

}

#endif // POSITION_H
