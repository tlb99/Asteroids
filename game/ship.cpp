#include "ship.hpp"
#include "../qt/globalvars.hpp"
#include "bullet.hpp"

#include <QPainter>
#include <QDebug>
#include <QtCore/qmath.h>
#include <QTimer>

Ship::Ship()
{
    shipFired = false;
    shipHitState = false;

    bulletCoolDown = 0;
    angle = 0;
    xVel = 0;
    yVel = 0;

    qreal x = (m_Width + 17)/2;
    qreal y = (m_Height + 15)/2;
    setPos(x,y);

    // Set the origin point to center of the ship
    setTransformOriginPoint(-7.5, -7);

    shipSFX = new QMediaPlayer();
    shipSFX->setMedia(QUrl("qrc:/sounds/thrust.wav"));
    shipSFX->setVolume(25);
}

// Updates various properties of the ship such as:
// rotaton, new position, bullet creation
void Ship::updateShip()
{

        // Always update angle regardless if the ship has speed or not
        if (angle > 360) angle = angle - 360;
        if (angle < 0) angle = angle + 360;
        setRotation( angle );

        if(xVel <= -17.5)
        {
            xVel = -17.5;
        }
        if(xVel >= 17.5)
        {
            xVel = 17.5;
        }
        if(yVel <= -17.5)
        {
            yVel = -17.5;
        }
        if(yVel >= 17.5)
        {
            yVel = 17.5;
        }

        setPos(x() + (xVel/2.5), y() + (yVel/2.5) );

        // Shoot a bullet if player isn't moving.
        if(shipFired)
        {
            if (bulletCoolDown == 0)
                bulletCoolDown = 5;
            else shipFired = false;
        }

        if (bulletCoolDown > 0)
            bulletCoolDown -= 1;

        // Ship wrap around
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

bool Ship::hasShipFired()
{
    return shipFired;
}

void Ship::setShipFireVarFalse()
{
    shipFired = false;
}

// Ship movement keys
// Up key moves the ship forwards, left and right keys turn the ship by -/+ 10 degrees respectively
void Ship::updateShipKeys()
{

    foreach(Qt::Key key, keys){
        switch ( key ){
        case Qt::Key_Up:
            xVel += 1.25 * qSin( 0.0174533*rotation() );
            yVel += 1.25 * -qCos( 0.0174533*rotation() );
            shipSFX->play();
            break;
        case Qt::Key_Left:
            angle -= 10;
            break;
        case Qt::Key_Right:
            angle += 10;
            break;
        case Qt::Key_Down:
            reduceVelocity();
            break;
        case Qt::Key_Space:
            shipFired = true;
            break;
        default:
            break;
        }
    }
}

// Add currently held key to set
void Ship::keyPressEvent(QKeyEvent *event)
{
    keys += (Qt::Key)event->key();
}

// Remove released key from set
void Ship::keyReleaseEvent(QKeyEvent *event)
{
    keys -= (Qt::Key)event->key();
}

bool Ship::getShipHitState()
{
    return shipHitState;
}

void Ship::setShipHitStateT()
{
    shipHitState = true;
}

void Ship::resetPos()
{
    xVel = 0;
    yVel = 0;
    angle = 0;
    setPos((m_Width + 17)/2,(m_Height + 17)/2);
    setRotation(angle);
}

