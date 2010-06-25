#ifndef POSITION_H
#define POSITION_H

#include <algorithm>
#include <math.h>
#include <QPointF>
#include <QLocale>


namespace QtEpidemy {


    /**
      convenience class for map coordinates, backed by a QPointF. X and Y
      coordinate values are clamped to proper ranges (-90..90 for latitude
      and -180..180 for longitude)

      longitude is the X-axis (with Southern longitudes represented as
      negative numbers), and latitude is the Y-axis (West being negative)

      Can calculate distances between two points.
      */
    class Position
    {
    public:
        // degrees / radian
        const static qreal DEG_TO_RAD = M_PI/180;
        const static qreal EARTH_RADIUS = 6371.0; //in km

        Position(const QPointF &pos);
        Position(qreal latitude, qreal longitude);

        /**
          allows casting a Position to a QPointF
          */
        operator QPointF() const {
            return m_position;
        }

        /**
          returns the longitude in degrees
          */
        inline qreal lon() const {
            return m_position.x();
        }

        /**
          longitude but in radians
          */
        inline qreal lonRad() const {
            return lon() * DEG_TO_RAD;
        }

        /**
          latitude
          */
        inline qreal lat() const {
            return m_position.y();
        }

        /**
          latitude in radians
          */
        inline qreal latRad() const {
            return lat() * DEG_TO_RAD;
        }

        /**
          converts degrees to radians
          */
        inline qreal degToRad(qreal deg) {
            return deg * DEG_TO_RAD;
        }

        /**
          calculates the distance between Position a and Position b using
          Haversine formula
        */
        static qreal distanceBetween(const Position &a, const Position &b);

        /**
          calculates the distance between this Position and Position b
          */
        qreal distanceTo(const Position &b) const;

        QString toString() const {
            QLocale sys = QLocale::system();
            QString Lat = sys.toString(lat(), 'f', 2);
            QString Lon = sys.toString(lon(), 'f', 2);
            return Lat % "N " % Lon % "E";
        }

        qreal bearingTo(const Position&);



    protected:
        QPointF m_position;

        /**
          Used to clamp x and y coordinates to the range -end .. end
          */
        inline void clampVal(qreal& val, const qreal &end) {
            val = std::min(end, std::max(-end, val));
        }

        /**
          clamps a QPointF's X and Y coordinates to proper ranges.
          -90..90 for latitude
          -180..180 for longitude
          */
        inline void clampQPointValues(QPointF &point) {
            clampVal(point.ry(), 90);
            clampVal(point.rx(), 180);
        }
    };

}

#endif // POSITION_H
