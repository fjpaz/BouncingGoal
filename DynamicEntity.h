// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#ifndef DYNAMICENTITY_H
#define DYNAMICENTITY_H

#include "GenericEntity.h"

class DynamicEntity : public GenericEntity
{
public:
    DynamicEntity(QAbstractGraphicsShapeItem *item, b2Body *body);

    void update();
    void stop();
};

#endif // DYNAMICENTITY_H
