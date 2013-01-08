// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#ifndef PLAYERENTITY_H
#define PLAYERENTITY_H

#include "DynamicEntity.h"

class QString;

class PlayerEntity : public DynamicEntity
{
public:
    PlayerEntity(QAbstractGraphicsShapeItem *item, b2Body *body);

    void accelerate();
    void impulse();
    void rotateLeft();
    void rotateRight();
    void select();
    void unselect();
    void setTexture(const QString &texture);
};

#endif // PLAYERENTITY_H
