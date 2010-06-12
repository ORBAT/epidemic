#include <QDebug>
#include "pathogen.h"


namespace QtEpidemy {

    Pathogen::Pathogen(ratioType survival, ratioType infection, ratioType duration,
                       QObject *parent) :
    QObject(parent), m_survivalRate(survival), m_infectionRate(infection),
    m_diseaseDuration(duration)
    {
        qDebug() << tr("Pathogen created with survival rate %1, infection %2, duration %2 days")
                .arg(survival).arg(infection).arg(duration);
    }

    void Pathogen::setStatistic(PathogenStats ps, ratioType rt) {
        clampToZero(rt);
        switch(ps) {
        case SURVIVAL:
            if(m_survivalRate == rt)
                return;
            m_survivalRate = rt;
            break;
        case INFECTION:
            if(m_infectionRate == rt)
                return;
            m_infectionRate = rt;
            break;
        case DURATION:
            if(m_diseaseDuration == rt)
                return;
            m_diseaseDuration = rt;
            break;
        case default:
            qDebug() << "Pathogen::setStatistic() got weird ps" << ps << "value"<<rt;
            return;
        }
        emit changed(this);
    }

}
