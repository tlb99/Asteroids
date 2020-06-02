#include "bullet.hpp"
#include "../qt/scene.hpp"

#include <QPainter>
#include <qmath.h>
#include <QDebug>
#include <QTimer>

Bullet::Bullet()
{

}

Bullet::Bullet(qreal x, qreal y, qreal rotation)
{
    setPos( x, y );
    setRotation( rotation );
    setTransformOriginPoint(-7.5, -7);
    bulletHitState = false;
}

void Bullet::updateBulletCords()
{
    // .00174 is pi radians over 180 degrees, in other words,
    // the rotation is converted from degrees to radians
    xCoor =  30 * qSin( 0.0174533 * rotation() );
    yCoor =  30 * -qCos( 0.0174533 * rotation()) ;
    setPos( x() + xCoor, y() + yCoor);
}

void Bullet::setBulletHitStateT()
{
    bulletHitState = true;
}

bool Bullet::getBulletHitState()
{
    return bulletHitState;
}

void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen((QPen(Qt::red, 5)) );
    painter->drawPoint(-7.5,-7);
}
