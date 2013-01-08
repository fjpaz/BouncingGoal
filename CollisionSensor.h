// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#ifndef COLLISIONSENSOR_H
#define COLLISIONSENSOR_H

#include <Box2D/Box2D.h>
#include <QObject>

class CollisionSensor : public QObject, public b2ContactListener
{
    Q_OBJECT
public:
    CollisionSensor(const b2Body &goal_body_team_1, const b2Body &goal_body_team_2, const b2Body &ball_body);

    void BeginContact(b2Contact *contact);
    void EndContact(b2Contact *contact);

signals:
    void goalDetected(const int goal_team) const;
    void collisionDetected();

private:
    const b2Body &_goal_body_team_1;
    const b2Body &_goal_body_team_2;
    const b2Body &_ball_body;
};

#endif // COLLISIONSENSOR_H
