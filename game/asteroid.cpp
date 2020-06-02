#include "../game/asteroid.hpp"

#include <QPainter>
#include <qmath.h>
#include<Qdebug>

// Type is the size of the asteroid. 3 is a large asteroid,
// 2 is medium and 1 is small.
Asteroid::Asteroid(qreal x , qreal y, qreal rotation, qreal type)
{
   setPos(x, y);
   setRotation(rotation);
   asteroidHitState = false;
   asteroidType = type;
   this->setScale(asteroidType);

   if(asteroidType == 3){
       maxVel = 5;
       maxSpeed = 0.5;
   }
   else if(asteroidType == 2){
       maxVel = 7;
       maxSpeed = 0.75;
   }
   else{
       maxVel = 10;
       maxSpeed = 1;
   }
}

// Updates the xVel and yVel values
void Asteroid::updateAsteroidCords()
{
    if(xVel <= -maxVel)
    {
        xVel = -maxVel;
    }
    if(xVel >= maxVel)
    {
        xVel = maxVel;
    }
    if(yVel <= -maxVel)
    {
        yVel = -maxVel;
    }
    if(yVel >= maxVel)
    {
        yVel = maxVel;
    }

    xVel += maxSpeed * qSin( 0.0174533*rotation() );
    yVel += maxSpeed * -qCos( 0.0174533*rotation() );

    setPos(x() + (xVel/2.5), y() + (yVel/2.5) );


    //Wrap around code for Asteroids
    if ( (this->pos().x() > m_Width) ){
        this->setPos( (x() - m_Width) ,  y() );
    }
    if( this->pos().x() < 0 ){
        this->setPos( (x() + m_Width),  y() );
    }
    if(  (this->pos().y() > m_Height)  ){
        this->setPos( x(),  (y() - m_Height) );
    }
    if(  (this->pos().y() < 0 )){
        this->setPos( x(),  (y() + m_Height) );
    }
}

bool Asteroid::getAsteroidHitState()
{
    return asteroidHitState;
}

void Asteroid::setAsteroidHitStateT()
{
    asteroidHitState = true;
}

void Asteroid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::yellow, 2));
    painter->drawEllipse(QPointF(-7.5,-7.5), 14, 14);
}
