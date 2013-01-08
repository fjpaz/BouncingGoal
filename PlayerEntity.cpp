// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#include "PlayerEntity.h"
#include "Constants.h"

#include <Box2D/Box2D.h>
#include <QAbstractGraphicsShapeItem>
#include <QBrush>
#include <QPen>
#include <math.h>

PlayerEntity::PlayerEntity(QAbstractGraphicsShapeItem *item, b2Body *body) : DynamicEntity(item, body)
{
}

void PlayerEntity::accelerate()
{
    _body->ApplyForceToCenter(b2Vec2(kPlayerAccelerationForce * cosf(_item->rotation()*-kDegToRad),
                                     kPlayerAccelerationForce * sinf(_item->rotation()*-kDegToRad)));
}

void PlayerEntity::impulse()
{
    _body->ApplyLinearImpulse(b2Vec2(kPlayerLinearImpulse * cosf(_item->rotation()*-kDegToRad),
                                     kPlayerLinearImpulse * sinf(_item->rotation()*-kDegToRad)),
                              b2Vec2(_body->GetPosition().x, _body->GetPosition().y));
}

void PlayerEntity::rotateLeft()
{
    _body->ApplyAngularImpulse(kPlayerRotationImpulse);

}

void PlayerEntity::rotateRight()
{
    _body->ApplyAngularImpulse(-kPlayerRotationImpulse);
}

void PlayerEntity::select()
{
    _item->setPen(QPen(QColor(), 0.4f));
}

void PlayerEntity::unselect()
{
    _item->setPen(Qt::NoPen);
}

void PlayerEntity::setTexture(const QString &texture)
{
    QBrush brush = _item->brush();
    brush.setTexture(QPixmap(texture));
    _item->setBrush(brush);
}
