// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#ifndef WORLD_H
#define WORLD_H

#include <QHash>
#include <QObject>
#include <QSet>
#include <QString>

class QGraphicsScene;
class QGraphicsView;
class QGLWidget;
class QGraphicsTextItem;
class GenericEntity;
class DynamicEntity;
class PlayerEntity;
class CollisionSensor;
class Team;
class CenteredTextItem;
class b2World;
class QKeyEvent;
class MainWindow;
class SoundPlayer;

class World : public QObject
{
    Q_OBJECT
public:
    World(MainWindow &window, QObject *parent = 0);
    virtual ~World();

    void addEntity(GenericEntity *entity);
    void addDynamicEntity(QString name, DynamicEntity *entity);
    void start();
    void handleInput();
    void populate();
    void setInitialPosition();
    void changeSelectedPlayer(int team);
    void updateImpulseRecharge();
    void enableTeamImpulse(int team);
    void disableTeamImpulse(int team);

public slots:
    void keyPressed(int key);
    void keyReleased(int key);
    void handleGoalScored(int goal_team);
    void resize();

private:
    void timerEvent(QTimerEvent *event);

    QGraphicsScene *_scene;
    QGraphicsView &_view;
    QGLWidget *_glwidget;
    QGraphicsTextItem *_text_item;
    int _timer_id;
    QHash <QString, DynamicEntity*> _entities;
    b2World *_physworld;
    QSet<int> _keys;
    bool _reset_positions;
    QScopedPointer<CollisionSensor> _collision_sensor;
    QScopedPointer<QGraphicsTextItem> _score_item_team1;
    QScopedPointer<QGraphicsTextItem> _score_item_team2;
    float _scale_factor;
    float _impulse_team_1;
    float _impulse_team_2;
    bool _impulse_team_1_enabled;
    bool _impulse_team_2_enabled;
    bool _playing;
    QScopedPointer<SoundPlayer> _sound_player;
    QScopedPointer<Team> _team_1;
    QScopedPointer<Team> _team_2;
};

#endif // WORLD_H
