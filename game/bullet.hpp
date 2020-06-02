#ifndef BULLET_HPP
#define BULLET_HPP

#include "../qt/globalvars.hpp"
#include "../qt/scene.hpp"

#include <QtCore/qmath.h>
#include <QGraphicsItem>

class Bullet : public QGraphicsItem
{

public:
   Bullet();

   Bullet(qreal, qreal, qreal);

   void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

   QRectF boundingRect() const
   { return QRectF(-25, -25, 35, 35); }

   void updateBulletCords();

   void removeBullet();

private:
   qreal xCoor;
   qreal yCoor;
};

#endif // BULLET_HPP
