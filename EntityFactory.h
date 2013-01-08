// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

class PlayerEntity;
class GenericEntity;
class DynamicEntity;
class b2World;

class EntityFactory
{
public:
    EntityFactory(b2World &world);

    PlayerEntity* createPlayerEntity(float x, float y, float angle, int team, int number);
    GenericEntity* createStadiumEntity(float x, float y, float angle);
    DynamicEntity* createBallEntity(float x, float y);
    GenericEntity* createGoalEntity(float x, float y, float angle);

private:
    b2World &_world;
};

#endif // ENTITYFACTORY_H
