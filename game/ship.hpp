#ifndef SHIP_HPP
#define SHIP_HPP

#include "../qt/scene.hpp"

#include <QKeyEvent>
#include <QSet>
#include <QGraphicsItem> //A combination of QGraphicsItem and QObject

class Ship : public QGraphicsItem
{
public:

    //Default constructor - positions the ship to the middle of the screen.
    Ship();

    //Updates the ship position
    void updateShip();

    //Getters and Setters function for ship fire variable
    //returns a bool value signifying whether the ship has fired or not.
    //Sets the ship fire bool value to false
    bool hasShipFired();
    void setShipFireVarFalse();

    //Used to update the player keys
    void updateShipKeys();

    //Reimplementation of the keyPressEvent
    void keyPressEvent(QKeyEvent *);

    //Updates whenever keys are released
    void keyReleaseEvent(QKeyEvent *);


    //Paint and Hitbox functions for the Ship
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const
        {return QRectF(-20, -25, 25, 36); }

private:

    void reduceVelocity();

    qreal angle;

    //These values take into account the rotation of the ship
    //Which are then used to move the ship to the proper position
    qreal xVel;
    qreal yVel;

    //Determines whether the ship's action key(s) have been pressed.
    bool keyShoot;

    qreal bulletCoolDown;
    bool shipFired;

    //A QSet that will hold all currently held keys
    QSet<Qt::Key> keys;



};

#endif // SHIP_HPP
