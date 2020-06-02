#include "mainwindow.hpp"

#include <QMenuBar>
#include <QTimer>
#include <QDebug>
#include <QtGlobal>
#include <typeinfo>

MainWindow::MainWindow() : QMainWindow()
{
    //Create the menu and its actions
    QMenu *start = menuBar()->addMenu(tr("File"));

    QAction *open = new QAction(tr("Start Game"), this );
    open->setShortcut(QKeySequence::Open);
    connect(open, SIGNAL(triggered()), this, SLOT(gameOpen()));

    QAction *closed = new QAction(tr("Exit Game"), this);
    closed->setShortcut(QKeySequence::Close);
    connect(closed, SIGNAL(triggered()), this, SLOT(gameClose()));

    start->addAction(open);
    start->addAction(closed);

    // Scene
    m_scene = new Scene();
    m_view = new QGraphicsView(m_scene);

    // Adding various properties to the view
    m_view->setRenderHint(QPainter::Antialiasing, true);
    m_view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_view->setFrameStyle(0);

    // Fixed window of size 1024 x 768
    m_view->setFixedWidth(m_Width);
    m_view->setFixedHeight(m_Height);
    m_view->setSceneRect(0,0,m_Width,m_Height);

    // Centralize the view area.
    setCentralWidget(m_view);

    gameStarted = false;

    // Set background of the scene to black
    m_view->setBackgroundBrush(Qt::black);

    // Add the asteroids text to scene
    asteroidText = new QGraphicsTextItem("Asteroids");
    asteroidText->setDefaultTextColor(Qt::white);
    asteroidText->setPos((m_Width - 500)/2,(m_Height - 400)/2);
    asteroidText->setScale(10);
    m_view->scene()->addItem(asteroidText);

    // Add the start text to scene
    startText = new QGraphicsTextItem("Press space to start");
    startText->setDefaultTextColor(Qt::white);
    startText->setPos((m_Width - 300)/2,(m_Height + 400)/2);
    startText->setScale(3);
    m_view->scene()->addItem(startText);

    // Add the game over text to scene, but hide it
    gameOver = new QGraphicsTextItem("Game over");
    gameOver->setDefaultTextColor(Qt::white);
    gameOver->setPos((m_Width - 600)/2,(m_Height - 400)/2);
    gameOver->setScale(9);
    m_view->scene()->addItem(gameOver);

    gameOver->hide();

    // Initialize the media players and assign sound to them
    shootSFX = new QMediaPlayer();
    shootSFX->setMedia(QUrl("qrc:/sounds/fire.wav"));
    shootSFX->setVolume(50);

    bigSFX = new QMediaPlayer();
    bigSFX->setMedia(QUrl("qrc:/sounds/large.mp3"));
    bigSFX->setVolume(50);

    medSFX = new QMediaPlayer();
    medSFX->setMedia(QUrl("qrc:/sounds/medium.mp3"));
    medSFX->setVolume(50);

    smallSFX = new QMediaPlayer();
    smallSFX->setMedia(QUrl("qrc:/sounds/small.mp3"));
    smallSFX->setVolume(50);

    extraSFX = new QMediaPlayer();
    extraSFX->setMedia(QUrl("qrc:/sounds/extraShip.mp3"));
    extraSFX->setVolume(100);

}

// Start Game - modifies the view and scene properties
// Adds the player into the middle of the screen aswell
void MainWindow::gameOpen()
{
    if (!gameStarted){
        // Game has started, and can no longer be restarted by pressing 'start' menu action
        gameStarted = true;

        // Create the player and add him to the scene
        player = new Ship();
        m_view->scene()->addItem(player);

        lives = 3;
        level = 1;
        score = 0;
        iframes = 0;

        asteroidText->hide();
        startText->hide();

        // Add the score number to scene
        scoreCounter = new QGraphicsTextItem(QString::number(score));
        scoreCounter->setDefaultTextColor(Qt::white);
        scoreCounter->setPos(105,10);
        scoreCounter->setScale(3);
        m_view->scene()->addItem(scoreCounter);

        // Add the score text to scene
        scoreText = new QGraphicsTextItem("Score:");
        scoreText->setDefaultTextColor(Qt::white);
        scoreText->setPos(10,10);
        scoreText->setScale(3);
        m_view->scene()->addItem(scoreText);

        // Add the lives text to scene
        livesText = new QGraphicsTextItem("Lives:");
        livesText->setDefaultTextColor(Qt::white);
        livesText->setPos(10,50);
        livesText->setScale(3);
        m_view->scene()->addItem(livesText);

        // Add the lives number to scene
        livesCounter = new QGraphicsTextItem(QString::number(lives));
        livesCounter->setDefaultTextColor(Qt::white);
        livesCounter->setPos(100,50);
        livesCounter->setScale(3);
        m_view->scene()->addItem(livesCounter);

        // Add the level text to scene
        levelText = new QGraphicsTextItem("Level");
        levelText->setDefaultTextColor(Qt::white);
        levelText->setPos(10,668);
        levelText->setScale(3);
        m_view->scene()->addItem(levelText);

        // Add the level number to scene
        levelCounter = new QGraphicsTextItem(QString::number(level));
        levelCounter->setDefaultTextColor(Qt::white);
        levelCounter->setPos(100,668);
        levelCounter->setScale(3);
        m_view->scene()->addItem(levelCounter);


        // In order for player object to recieve events such as keyboard events,
        // the object must be focusable, and should be set to be the focus
        player->setFlag(QGraphicsItem::ItemIsFocusable);
        player->setFocus();

        // Use the current time as a seed for true random asteroid generation
        QDateTime cd = QDateTime::currentDateTime();
        qsrand(cd.toTime_t());

        asteroidCheck = false;

        // Create timer and have its parent object be set to the MainWindow object.
        // The Timer object is connected to the MainWindow object.
        // Every X milliseconds, it timeouts and sends a signal to the MainWindow
        // object, which then proceeds to call 'updateGame()'
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateGame()));
        timer->start(33.33);
    }

}

// This function is called every 33.33ms (30FPS).
// It is used to update objects properties, such as
// rotation, velocity, hitpoints, etc.
// Every object that is updated in this function is rendered at 30FPS.
void MainWindow::updateGame()
{
    // Update score every frame
    updateScore();

    // Adds an extra life whenever extraLife reaches 10,000 points
    if(extraLife >= 10000){
        extraSFX->play();

        m_view->scene()->removeItem(livesCounter);
        lives++;
        livesCounter->setPlainText(QString::number(lives));
        m_view->scene()->addItem(livesCounter);

        extraLife = extraLife - 10000;
    }

    // Go Through the lists updating each object.
    if( !enemyList.empty() ) updateEnemyList();
    if ( !bulletList.empty() ) updateBulletList();

    // Update the player and currently held player keys if the game isn't over
    if(!gameIsOver && iframes < 60){
        player->updateShipKeys();
        player->updateShip();
    }

    // Only executes if updateShip registered a ship fire event and the bullet list size is less than 5
    if( player->hasShipFired() )
    {
        shootSFX->stop();
        shootSFX->play();

        bulletList.append( new Bullet( player->x(), player->y(), player->rotation() ) );
        m_view->scene()->addItem( bulletList.back() );

        // The ship Fire variable is reset back to false for the next frame.
        player->setShipFireVarFalse();

    }

    // After updating bullets,enemies and player, check for collision using a while loop nested in a for loop.
    // Start from the back of the asteroid/enemy list - so when there is a collision and the enemy
    // is removed from the list, there will be no problems with 'i' increasing while enemyList decreases.
    for (int i = (enemyList.size() - 1); i >= 0 ; i--)
    {
        enemy = enemyList.at(i);

        // Traverse through bullet list and check if there is a collision,
        // then remove from proper list and scene
        int k = bulletList.size()-1;
        bool collideWithBullet = false;
        // If there is a collision with a bullet, remove both items from list and scene
        while (!collideWithBullet && k >= 0)
        { 
            bullet = bulletList.at(k);
            if( enemy->collidesWithItem(bullet) )
            {
                // Spawn two new asteroids if asteroid was of size medium or large
               if(enemy->asteroidType > 1){
                   qreal rotation = qrand() % 360;
                   spawnAsteroids(enemy->x(), enemy->y(), rotation, enemy->asteroidType - 1);
                   enemyList.back()->xVel += enemyList.at(i)->xVel;
                   enemyList.back()->yVel += enemyList.at(i)->yVel;

                   spawnAsteroids(enemy->x(), enemy->y(), (rotation + 180), enemy->asteroidType - 1);
                   enemyList.back()->xVel += enemyList.at(i)->xVel;
                   enemyList.back()->yVel += enemyList.at(i)->yVel;
                }

               // Play a different sound effect and add to score depending on the size of the asteroid
               if(enemy->asteroidType == 3){
                   bigSFX->stop();
                   bigSFX->play();
                   score += 20;
                   extraLife += 20;
               }
               else if(enemy->asteroidType == 2){
                   medSFX->stop();
                   medSFX->play();
                   score += 50;
                   extraLife += 50;
               }
               else{
                   smallSFX->stop();
                   smallSFX->play();
                   score += 100;
                   extraLife += 100;
               }

                // Remove the colliding asteroid and bullets from scene/list
                enemyList.removeAt(i);
                bulletList.removeAt(k);
                m_scene->removeItem(enemy);
                m_scene->removeItem(bullet);
                delete enemy;
                delete bullet;
                enemy = NULL;
                bullet = NULL;
                collideWithBullet = true;
            }
            k--;
        }

        // If it went through the bullet list but found no collision on the asteroid, could be a player collision
        if (enemy != NULL)
        {
            if( enemy->collidesWithItem(player) && iframes == 0)
            {
             bigSFX->stop();
             bigSFX->play();

             if(enemy->asteroidType > 1){
                     qreal rotation = qrand() % 360;
                     spawnAsteroids(enemy->x(), enemy->y(), rotation, enemy->asteroidType - 1);
                     enemyList.back()->xVel += enemyList.at(i)->xVel;
                     enemyList.back()->yVel += enemyList.at(i)->yVel;

                     spawnAsteroids(enemy->x(), enemy->y(), (rotation + 180), enemy->asteroidType - 1);
                     enemyList.back()->xVel += enemyList.at(i)->xVel;
                     enemyList.back()->yVel += enemyList.at(i)->yVel;
              }

             enemyList.removeAt(i);
             m_scene->removeItem(enemy);
             delete enemy;
             enemy = NULL;

             m_view->scene()->removeItem(livesCounter);
             lives--;
             livesCounter->setPlainText(QString::number(lives));
             m_view->scene()->addItem(livesCounter);

             if(lives > 0){
                iframes = 120;
                player->setOpacity(0);
             }
             else{
                 // If the player loses all lives, hide all currently displayed text, hide player,
                 // and display game over text
                 gameIsOver = true;

                 iframes = 120;

                 player->setOpacity(0);

                 livesCounter->hide();
                 livesText->hide();
                 scoreCounter->hide();
                 scoreText->hide();
                 levelCounter->hide();
                 levelText->hide();

                 gameOver->show();
             }
            }
        }

    }

    // Spawns asteroids if the enemyList is smaller than the maxAsteroids int value
    // and if asteroidCheck is false. The two while loops ensure that the asteroids can't spawn
    // with the same x and y coordinates as the ship

    if(maxAsteroids == enemyList.size()) asteroidCheck = true;

    if(maxAsteroids > enemyList.size() && !asteroidCheck){

        // Generate random x value between 0 and 1024
        qreal x = qrand() % 1024;
        while(x < (((m_Width + 17)/2) + 300) && x > (((m_Width + 17)/2) - 300))
             x = qrand() % 1024;

        // Generate random y value between 0 and 768
        qreal y = qrand() % 768;
        while(y < (((m_Height + 15)/2) + 150) && y > (((m_Height + 15)/2) - 150))
             y = qrand() % 768;

        // Generate random rotation degree value between 0 and 360
        qreal rotation = qrand() % 360;

        spawnAsteroids(x, y, rotation, 3);
    }

    // If every single asteroid has been destroyed, increase the level counter, reset player position,
    // delete all on-screen bullets and spawn an additional asteroid
    if(enemyList.empty()){

        m_view->scene()->removeItem(levelCounter);
        level++;
        levelCounter->setPlainText(QString::number(level));
        m_view->scene()->addItem(levelCounter);

        for (int i = (bulletList.size() - 1); i >= 0 ; i--)
        {
            bullet = bulletList.at(i);
            bulletList.removeAt(i);
            m_scene->removeItem(bullet);
            delete bullet;
            bullet = NULL;
        }

        player->resetPos();

        asteroidCheck = false;
        maxAsteroids++;
    }

    if(iframes <= 60 && iframes != 0){
        if(iframes == 60){
            player->resetPos();
        }
        if(flicker > 0){
            player->setOpacity(0);
        }
        else{
            flicker = 2;
            player->setOpacity(1);
        }
        flicker--;
    }

    if(iframes > 0 && !gameIsOver)
        iframes--;
}


// Spawns asteroid, adds it to the Asteroid list and then adds it to the scene.
void MainWindow::spawnAsteroids(qreal x, qreal y, qreal rotation, qreal type)
{
    enemyList.append( new Asteroid(x, y, rotation, type) );
    m_view->scene()->addItem( enemyList.back() );

}

void MainWindow::updateEnemyList()
{
    for(int i = 0; i< enemyList.size(); i++)
    {
        enemy = enemyList.at(i);
        enemy->updateAsteroidCords();
    }

}

void MainWindow::updateBulletList()
{
    for(int i = 0; i< bulletList.size(); i++)
    {
        bullet = bulletList.at(i);
        bullet->updateBulletCords();

        // Delete bullet if it has gone out of the view area
        if ( (bullet->pos().x() > m_Width) | (bullet->pos().x() < -10) |
             (bullet->pos().y() > m_Height) | (bullet->pos().y() < -10) )
        {
            // Remove bullet from the list
            bulletList.removeAt(i);
            m_scene->removeItem(bullet);
            delete bullet;
            bullet = NULL;

        }
    }
}

void MainWindow::updateScore()
{
    m_view->scene()->removeItem(scoreCounter);
    scoreCounter->setPlainText(QString::number(score));
    m_view->scene()->addItem(scoreCounter);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
        gameOpen();
}

//Exit Functon - closes program.
void MainWindow::gameClose()
{
    this->close();
}