// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#include "CollisionSensor.h"

CollisionSensor::CollisionSensor(const b2Body &goal_body_team_1, const b2Body &goal_body_team_2, const b2Body &ball_body) :
    QObject(),
    _goal_body_team_1(goal_body_team_1), _goal_body_team_2(goal_body_team_2),
    _ball_body(ball_body)
{
}

void CollisionSensor::BeginContact(b2Contact *contact)
{
    emit collisionDetected();
}

void CollisionSensor::EndContact(b2Contact *contact)
{
    bool ball_is_in_contact = false;
    bool goal_is_in_contact = false;
    int team_who_score = 0;

    b2Fixture *fixtureA = contact->GetFixtureA();
    b2Fixture *fixtureB = contact->GetFixtureB();
    b2Fixture *ball_candidate = NULL;

    // Determine if goal is in contact
    if ( fixtureA->IsSensor() )
    {
        goal_is_in_contact = true;
        ball_candidate = fixtureB;

        if ( fixtureA->GetBody()->GetPosition() == _goal_body_team_1.GetPosition() )
        {
            team_who_score = 2;
        }
        else if ( fixtureA->GetBody()->GetPosition() == _goal_body_team_2.GetPosition() )
        {
            team_who_score = 1;
        }
    }
    else if ( fixtureB->IsSensor() )
    {
        goal_is_in_contact = true;
        ball_candidate = fixtureA;

        if ( fixtureB->GetBody()->GetPosition() == _goal_body_team_1.GetPosition() )
        {
            team_who_score = 2;
        }
        else if ( fixtureB->GetBody()->GetPosition() == _goal_body_team_2.GetPosition() )
        {
            team_who_score = 1;
        }
    }

    // Determine if ball is also in contact
    if ( goal_is_in_contact &&
         ball_candidate->GetBody()->GetPosition() == _ball_body.GetPosition() )
    {
        ball_is_in_contact = true;
    }

    if ( ball_is_in_contact && goal_is_in_contact )
    {
        if ( 2 == team_who_score &&
             _ball_body.GetLinearVelocity().x < 0.0f )
        {
            // If linear velocity x is negative the ball goes to the left
            emit goalDetected(2);
        }
        else if ( 1 == team_who_score &&
             _ball_body.GetLinearVelocity().x > 0.0f )
        {
            // If linear velocity x is positive the ball goes to the right
            emit goalDetected(1);
        }
    }
}
