// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#ifndef GENERICENTITY_H
#define GENERICENTITY_H

class QAbstractGraphicsShapeItem;
class QGraphicsItem;
class b2Body;

class GenericEntity
{
public:
    GenericEntity(QAbstractGraphicsShapeItem *item, b2Body *body);
    virtual ~GenericEntity();

    QGraphicsItem* getItem();
    b2Body* getBody();
    void setPos(float x, float y);
    void setRotation(float angle);

protected:
    QAbstractGraphicsShapeItem *_item;
    b2Body *_body;
};

#endif // GENERICENTITY_H
