#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "../game/ship.hpp"
#include "../game/asteroid.hpp"
#include "../game/bullet.hpp"
#include "scene.hpp"
#include "globalvars.hpp"

#include <QMainWindow>
#include <QGraphicsView>
#include <QList>
#include <QDateTime>

// The maximum amount of asteroids that can be spawned at any given point
const int maxAsteroids = 4;

// Class MainWindow derived from QMainWindow
class MainWindow : public QMainWindow
{
    // Needed for signals and slots for use with menu actions.
    Q_OBJECT

public:

    // Constructor
    MainWindow();

private slots:

    // Functions which execute when Menu actions are clicked
    void gameOpen();
    void gameClose();

    // Updates the game every X seconds, where X is milliseconds
    void updateGame();

private:

    // Scene and QGraphicsView pointer variables
    Scene *m_scene;
    QGraphicsView *m_view;

    // Create pointer variables to game objects
    Ship *player;
    Asteroid *enemy;
    Bullet *bullet;

    // Check if the game has started.
    bool gameStarted;

    // Asteroid Spawn Function an
    void spawnAsteroids();
    void updateEnemyList();
    void updateBulletList();

    QList<Asteroid *> enemyList;
    QList<Bullet *> bulletList;
};

#endif //MAINWINDOW_HPP
