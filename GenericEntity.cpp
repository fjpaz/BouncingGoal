// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#include "GenericEntity.h"
#include "Constants.h"

#include <Box2D/Box2D.h>
#include <QAbstractGraphicsShapeItem>

GenericEntity::GenericEntity(QAbstractGraphicsShapeItem *item, b2Body *body) : _item(item), _body(body)
{
}

GenericEntity::~GenericEntity()
{
    delete _item;
    _body->GetWorld()->DestroyBody(_body);
}

QGraphicsItem* GenericEntity::getItem()
{
    return _item;
}

b2Body *GenericEntity::getBody()
{
    return _body;
}

void GenericEntity::setPos(float x, float y)
{
    _item->setPos(x,y);
    _body->SetTransform(b2Vec2(x, -y), _body->GetAngle());
}

void GenericEntity::setRotation(float angle)
{
    _item->setRotation(angle);
    _body->SetTransform(_body->GetPosition(), -(angle * kDegToRad));
}
