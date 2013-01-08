// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#include "DynamicEntity.h"
#include "Constants.h"

#include <Box2D/Box2D.h>

DynamicEntity::DynamicEntity(QAbstractGraphicsShapeItem *item, b2Body *body) : GenericEntity(item, body)
{
}

void DynamicEntity::update()
{
    b2Vec2 position = _body->GetPosition();
    float32 angle = _body->GetAngle();
    setPos(position.x, -position.y);
    setRotation(-angle * kRadToDeg);
}

void DynamicEntity::stop()
{
    _body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    _body->SetAngularVelocity(0.0f);
}
