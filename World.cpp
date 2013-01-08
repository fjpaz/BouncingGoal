// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************


#include "World.h"
#include "CollisionSensor.h"
#include "Constants.h"
#include "EntityFactory.h"
#include "MainWindow.h"
#include "PlayerEntity.h"
#include "SoundPlayer.h"
#include "Team.h"

#include <Box2D/Box2D.h>
#include <QApplication>
#include <QGLWidget>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QTimerEvent>
#include <math.h>

World::World(MainWindow &window, QObject *parent) : QObject(parent),
    _scene(new QGraphicsScene()),
    _view(window.getView()),
    _glwidget(new QGLWidget(QGLFormat(QGL::SampleBuffers))),
    _timer_id(0),
    _physworld(new b2World(b2Vec2(0.0f, 0.0f))),
    _reset_positions(false),
    _scale_factor(1),
    _impulse_team_1(kImpulseRecharge),
    _impulse_team_2(kImpulseRecharge),
    _impulse_team_1_enabled(true),
    _impulse_team_2_enabled(true),
    _playing(false),
    _sound_player(new SoundPlayer),
    _team_1(new Team()),
    _team_2(new Team())
{
    // Play music
    _sound_player->playMusic();

    // Configure scene
    _scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    // Configure view
    _view.setScene(_scene);
    _view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    _view.setViewport(_glwidget);
    _view.setRenderHint(QPainter::HighQualityAntialiasing);
    _scale_factor = fminf(_view.width() / kStadiumWidth,
                           _view.height() / kStadiumHeight);
    _view.scale(_scale_factor, _scale_factor);
    _view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Set background
    QBrush brush;
    brush.setTexture(QPixmap("images/Floor.png"));
    QTransform transform;
    transform.translate(_view.x(), _view.y());
    qreal num_repeat = 1.0;
    qreal scale_factor = kStadiumWidth / (_view.width() * num_repeat);
    transform.scale(scale_factor, scale_factor);
    brush.setTransform(transform);
    _view.setBackgroundBrush(brush);

    // Set view and scene size
    _view.setFrameRect(QRect(0, 0, kStadiumWidth * _scale_factor, kStadiumHeight * _scale_factor));
    _scene->setSceneRect(0, 0, kStadiumWidth, kStadiumHeight);

    // Display keys and wait until ENTER is pressed
    _text_item = new QGraphicsTextItem();
    _text_item->setTextWidth(kStadiumWidth);
    _text_item->setFont(QFont("Courier", 2, QFont::Bold));
    _text_item->setHtml(QString("<div align='center'>") +
                       QString("Keys:<br/>") +
                       QString("-----<br/>") +
                       QString("Team 1 Accelerate: ........ W<br/>") +
                       QString("Team 1 Rotate Left: ....... A<br/>") +
                       QString("Team 1 Rotate Right: ...... D<br/>") +
                       QString("Team 1 Impulse: ........... S<br/>") +
                       QString("Team 1 Change Player: ..... E<br/>") +
                       QString("Team 2 Accelerate: ....... UP<br/>") +
                       QString("Team 2 Rotate Left: .... LEFT<br/>") +
                       QString("Team 2 Rotate Right: .. RIGHT<br/>") +
                       QString("Team 2 Rotate Impulse: . DOWN<br/>") +
                       QString("Team 2 Change Player: .. CTRL<br/>") +
                       QString("Fullscreen On/Off: ........ F<br/>") +
                       QString("Exit: ................... ESC<br/><br/>") +
                       QString("Press ENTER to Start<br/></div>"));
    _scene->addItem(_text_item);

    // Connect signals
    connect(&window, SIGNAL(keyPressed(int)), this, SLOT(keyPressed(int)));
    connect(&window, SIGNAL(keyReleased(int)), this, SLOT(keyReleased(int)));
    connect(&window, SIGNAL(windowResized()), this, SLOT(resize()));
}

World::~World()
{
    delete _physworld;
}

void World::addEntity(GenericEntity *entity)
{
    _scene->addItem(entity->getItem());
}

void World::addDynamicEntity(QString name, DynamicEntity *entity)
{
    _scene->addItem(entity->getItem());

    _entities[name] = entity;
}

void World::timerEvent(QTimerEvent *event)
{
    // Update world periodically

    updateImpulseRecharge();

    handleInput();

    _physworld->Step(kB2TimeStep,kB2VelIterations, kB2PosIterations);

    QHash <QString, DynamicEntity*>::const_iterator entity;

    for ( entity = _entities.begin(); entity != _entities.end(); ++entity )
    {
        entity.value()->update();
    }

    // This call must be after the Step because we can't change the b2World while is in a Step
    // and the contact callback is called while in Step
    if (_reset_positions)
    {
        setInitialPosition();
        _reset_positions = false;
    }

    QObject::timerEvent(event);
}


void World::start()
{
    if (!_timer_id)
        _timer_id = startTimer(kTimeStep);
}

void World::handleInput()
{
    QSet<int>::const_iterator key;

    for (key = _keys.begin(); key != _keys.end(); ++key)
    {
        switch(*key)
        {
        // Player 1 Keys
        case Qt::Key_Left:
            _team_2->selected()->rotateLeft();
            break;
        case Qt::Key_Right:
            _team_2->selected()->rotateRight();
            break;
        case Qt::Key_Down:
            if (_impulse_team_2_enabled)
            {
                _team_2->selected()->impulse();
                disableTeamImpulse(2);
            }
            break;
        case Qt::Key_Up:
            _team_2->selected()->accelerate();
            break;
        // Player 2 Keys
        case Qt::Key_W:
            _team_1->selected()->accelerate();
            break;
        case Qt::Key_A:
            _team_1->selected()->rotateLeft();
            break;
        case Qt::Key_S:
            if (_impulse_team_1_enabled)
            {
                _team_1->selected()->impulse();
                disableTeamImpulse(1);
            }
            break;
        case Qt::Key_D:
            _team_1->selected()->rotateRight();
            break;
        default:
            break;
        }
    }
}

void World::populate()
{
    EntityFactory entityFactory(*_physworld);

    // Stadium
    GenericEntity *stadium = entityFactory.createStadiumEntity(kStadiumWidth/2.0f, kStadiumHeight/2.0f, 0.0f);
    addEntity(stadium);

    // Players Team 1
    PlayerEntity *team1_player1 = entityFactory.createPlayerEntity(kXTeam1Player1, kYTeam1Player1, kAngleTeam1, 1, 1);
    addDynamicEntity("Team1Player1", team1_player1);

    PlayerEntity *team1_player2 = entityFactory.createPlayerEntity(kXTeam1Player2, kYTeam1Player2, kAngleTeam1, 1, 2);
    addDynamicEntity("Team1Player2", team1_player2);

    PlayerEntity *team1_player3 = entityFactory.createPlayerEntity(kXTeam1Player3, kYTeam1Player3, kAngleTeam1, 1, 3);
    addDynamicEntity("Team1Player3", team1_player3);

    // Players Team 2
    PlayerEntity *team2_player1 = entityFactory.createPlayerEntity(kXTeam2Player1, kYTeam2Player1, kAngleTeam2, 2, 1);
    addDynamicEntity("Team2Player1", team2_player1);

    PlayerEntity *team2_player2 = entityFactory.createPlayerEntity(kXTeam2Player2, kYTeam2Player2, kAngleTeam2, 2, 2);
    addDynamicEntity("Team2Player2", team2_player2);

    PlayerEntity *team2_player3 = entityFactory.createPlayerEntity(kXTeam2Player3, kYTeam2Player3, kAngleTeam2, 2, 3);
    addDynamicEntity("Team2Player3", team2_player3);

    // Ball
    DynamicEntity *ball = entityFactory.createBallEntity(kXBall, kYBall);
    addDynamicEntity("Ball", ball);

    // Goals
    GenericEntity *goal_team_1 = entityFactory.createGoalEntity(kXGoalTeam1, kYGoalTeam1, kAngleGoalTeam1);
    addEntity(goal_team_1);

    GenericEntity *goal_team_2 = entityFactory.createGoalEntity(kXGoalTeam2, kYGoalTeam2, kAngleGoalTeam2);
    addEntity(goal_team_2);

    // Set teams
    QHash<PlayerEntity*, PlayerEntity*> next_players_team_1;
    next_players_team_1[team1_player1] = team1_player2;
    next_players_team_1[team1_player2] = team1_player3;
    next_players_team_1[team1_player3] = team1_player1;

    foreach (PlayerEntity* player, next_players_team_1)
    {
        _team_1->addPlayer(player);
        _team_1->setNextPlayer(player, next_players_team_1[player]);
    }

    _team_1->setInitialPlayer(team1_player1);
    _team_1->selectInitialPlayer();

    QHash<PlayerEntity*, PlayerEntity*> next_players_team_2;
    next_players_team_2[team2_player1] = team2_player2;
    next_players_team_2[team2_player2] = team2_player3;
    next_players_team_2[team2_player3] = team2_player1;

    foreach (PlayerEntity* player, next_players_team_2)
    {
        _team_2->addPlayer(player);
        _team_2->setNextPlayer(player, next_players_team_2[player]);
    }

    _team_2->setInitialPlayer(team2_player1);
    _team_2->selectInitialPlayer();

    // Collision sensor
    _collision_sensor.reset(new CollisionSensor(*goal_team_1->getBody(), *goal_team_2->getBody(), *ball->getBody()));
    _physworld->SetContactListener(_collision_sensor.data());
    connect(_collision_sensor.data(), SIGNAL(goalDetected(int)), this, SLOT(handleGoalScored(int)));
    connect(_collision_sensor.data(), SIGNAL(collisionDetected()), _sound_player.data(), SLOT(playCollision()));

    // Score widgets
    _score_item_team1.reset(new QGraphicsTextItem(goal_team_1->getItem()));
    _score_item_team1->setPos(-kGoalWidth/2.0, -kGoalHeight/3.0);
    _score_item_team1->setFont(QFont("Helvetica", 4));
    _score_item_team1->setOpacity(0.5);
    _score_item_team1->setHtml(QString("<div align='center'>") +
                               QString::number(_team_1->score()) +
                               QString("</div>"));

    _score_item_team2.reset(new QGraphicsTextItem(goal_team_2->getItem()));
    _score_item_team2->setRotation(180.0);
    _score_item_team2->setPos(kGoalWidth/2.0, kGoalHeight/3.0);
    _score_item_team2->setFont(QFont("Helvetica", 4));
    _score_item_team2->setOpacity(0.5);
    _score_item_team2->setHtml(QString("<div align='center'>") +
                               QString::number(_team_2->score()) +
                               QString("</div>"));
}

void World::setInitialPosition()
{
    // Set initial position and orientation
    _entities["Team1Player1"]->setPos(kXTeam1Player1, kYTeam1Player1);
    _entities["Team1Player1"]->setRotation(kAngleTeam1);
    _entities["Team1Player2"]->setPos(kXTeam1Player2, kYTeam1Player2);
    _entities["Team1Player2"]->setRotation(kAngleTeam1);
    _entities["Team1Player3"]->setPos(kXTeam1Player3, kYTeam1Player3);
    _entities["Team1Player3"]->setRotation(kAngleTeam1);
    _entities["Team2Player1"]->setPos(kXTeam2Player1, kYTeam2Player1);
    _entities["Team2Player1"]->setRotation(kAngleTeam2);
    _entities["Team2Player2"]->setPos(kXTeam2Player2, kYTeam2Player2);
    _entities["Team2Player2"]->setRotation(kAngleTeam2);
    _entities["Team2Player3"]->setPos(kXTeam2Player3, kYTeam2Player3);
    _entities["Team2Player3"]->setRotation(kAngleTeam2);
    _entities["Ball"]->setPos(kXBall, kYBall);

    // Stop all dynamic entities
    QHash <QString, DynamicEntity*>::const_iterator entity;

    for ( entity = _entities.begin(); entity != _entities.end(); ++entity )
    {
        entity.value()->stop();
    }

    // Set default selected players
    _team_1->selectInitialPlayer();
    _team_2->selectInitialPlayer();

    // Enable impulses
    enableTeamImpulse(1);
    enableTeamImpulse(2);
}

void World::changeSelectedPlayer(int team)
{
    if ( 1 == team )
    {
        _team_1->selectNextPlayer();
    }

    if ( 2 == team )
    {
        _team_2->selectNextPlayer();
    }
}

void World::updateImpulseRecharge()
{
    if (_impulse_team_1 < kImpulseRecharge)
    {
        _impulse_team_1 += kTimeStep / 1000.0f;

        if (_impulse_team_1 >= kImpulseRecharge)
        {
            enableTeamImpulse(1);
        }
    }

    if (_impulse_team_2 < kImpulseRecharge)
    {
        _impulse_team_2 += kTimeStep / 1000.0f;

        if (_impulse_team_2 >= kImpulseRecharge)
        {
            enableTeamImpulse(2);
        }
    }
}

void World::enableTeamImpulse(int team)
{
    if (1 == team)
    {
        _impulse_team_1_enabled = true;
        _impulse_team_1 = kImpulseRecharge;
        PlayerEntity *player = static_cast<PlayerEntity*>(_entities["Team1Player1"]);
        player->setTexture("images/Team1Player1Highlight.png");
        player = static_cast<PlayerEntity*>(_entities["Team1Player2"]);
        player->setTexture("images/Team1Player2Highlight.png");
        player = static_cast<PlayerEntity*>(_entities["Team1Player3"]);
        player->setTexture("images/Team1Player3Highlight.png");
    }
    else if (2 == team)
    {
        _impulse_team_2_enabled = true;
        _impulse_team_2 = kImpulseRecharge;
        PlayerEntity *player = static_cast<PlayerEntity*>(_entities["Team2Player1"]);
        player->setTexture("images/Team2Player1Highlight.png");
        player = static_cast<PlayerEntity*>(_entities["Team2Player2"]);
        player->setTexture("images/Team2Player2Highlight.png");
        player = static_cast<PlayerEntity*>(_entities["Team2Player3"]);
        player->setTexture("images/Team2Player3Highlight.png");
    }
}

void World::disableTeamImpulse(int team)
{
    if (1 == team)
    {
        _impulse_team_1_enabled = false;
        _impulse_team_1 = 0.0f;
        PlayerEntity *player = static_cast<PlayerEntity*>(_entities["Team1Player1"]);
        player->setTexture("images/Team1Player1.png");
        player = static_cast<PlayerEntity*>(_entities["Team1Player2"]);
        player->setTexture("images/Team1Player2.png");
        player = static_cast<PlayerEntity*>(_entities["Team1Player3"]);
        player->setTexture("images/Team1Player3.png");
    }
    else if (2 == team)
    {
        _impulse_team_2_enabled = false;
        _impulse_team_2 = 0.0f;
        PlayerEntity *player = static_cast<PlayerEntity*>(_entities["Team2Player1"]);
        player->setTexture("images/Team2Player1.png");
        player = static_cast<PlayerEntity*>(_entities["Team2Player2"]);
        player->setTexture("images/Team2Player2.png");
        player = static_cast<PlayerEntity*>(_entities["Team2Player3"]);
        player->setTexture("images/Team2Player3.png");
    }
}

void World::keyPressed(int key)
{
    _keys += key;
}

void World::keyReleased(int key)
{
    _keys -= key;

    switch ( key )
    {
    case Qt::Key_E:
        changeSelectedPlayer(1);
        break;
    case Qt::Key_Control:
        changeSelectedPlayer(2);
        break;
    case Qt::Key_Escape:
        QApplication::quit();
        break;
    case Qt::Key_Return:
        if (!_playing)
        {
            _scene->removeItem(_text_item);
            delete _text_item;

            populate();
            start();
            _playing = true;
        }
        break;
    default:
        break;
    }
}

void World::handleGoalScored(int goal_team)
{
    _sound_player->playGoal();

    if (1 == goal_team)
    {
        _team_1->incrementScore();
        _score_item_team1->setHtml(QString("<div align='center'>") +
                                   QString::number(_team_1->score()) +
                                   QString("</div>"));
    }
    else if (2 == goal_team)
    {
        _team_2->incrementScore();
        _score_item_team2->setHtml(QString("<div align='center'>") +
                                   QString::number(_team_2->score()) +
                                   QString("</div>"));
    }

    // Indicate that must reset positions in the update loop (after Step)
    _reset_positions = true;
}

void World::resize()
{
    // Zoom factor is the number to use with the scale transform
    float zoom_factor = fminf(_view.width() / (kStadiumWidth * _scale_factor),
                        _view.height() / (kStadiumHeight * _scale_factor));

    // Scale factor is the ratio between view and scene
    _scale_factor *= zoom_factor;

    _view.scale(zoom_factor, zoom_factor);
}
