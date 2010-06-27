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
            return degToRad(lon());
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
            return degToRad(lat());
        }

        /**
          converts degrees to radians
          */
        inline static qreal degToRad(qreal deg) {
            return deg * DEG_TO_RAD;
        }

        /**
          converst radians to degrees
          */
        inline static qreal radToDeg(qreal rad) {
            return rad / DEG_TO_RAD;
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

        /* returns bearing in degrees instead of radians. This has its own
           dedicated function since the conversion is a bit more involved than
           calling Position::radToDeg() */
        inline int bearingToInDegrees(const Position& b) const {
            return ((int)radToDeg(bearingTo(b))+360) % 360;
        }

        /**
          returns bearing to Position b in radians, between -pi and +pi.
          */
        qreal bearingTo(const Position& b) const;

        /**
          returns the Position that is d kilometers towards Position b from
          this Position. Uses the static moveTowards(Position,qreal,qreal)
          function.
          */
        Position moveTowards(const Position& b, qreal d) const;

        /**
          returns the destination point when moving d kilometers from
          this Position towards bearing b (in RADIANS). Uses the static
          moveTowards(Position,qreal,qreal) function.
        */
        Position moveTowards(qreal b, qreal d) const;

        /**
         returns the Position that is d kilometers on bearing b (in RADIANS)
         from Position a.
        */
        static Position moveTowards(const Position &a, qreal b, qreal d);



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
