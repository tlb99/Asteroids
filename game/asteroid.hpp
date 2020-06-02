#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "../qt/globalvars.hpp"

#include <QGraphicsItem>
#include <QtCore/qmath.h>

class Asteroid : public QGraphicsItem
{
public:
    Asteroid();

    Asteroid(qreal, qreal, qreal);

   void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

   void updateAsteroidCords();

   QRectF boundingRect() const
   { return QRectF(-25, -25, 35, 35); }

private:

   qreal xCoor;
   qreal yCoor;

};

#endif // ASTEROID_HPP