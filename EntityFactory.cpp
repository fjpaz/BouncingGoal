// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#include "EntityFactory.h"
#include "Constants.h"
#include "PlayerEntity.h"

#include <Box2D/Box2D.h>
#include <QBrush>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QPen>

// Origin point in Qt graphic items is top-left, X grows right, Y grows down
// Origin point in Box2D shapes is center, X grows right, Y grows up
// This entities adopt Qt standard

EntityFactory::EntityFactory(b2World &world) : _world(world)
{
}

PlayerEntity *EntityFactory::createPlayerEntity(float x, float y, float angle, int team, int number)
{
    // The Player has two components, the main one is a circle and the other one is a rectangle attached to the circle

    // Select textures
    QString texture_circle_path;
    QString texture_rectangle_path;

    switch (team)
    {
        case 1:
            texture_rectangle_path = "images/Team1Tail.png";

            switch (number)
            {
                case 1:
                    texture_circle_path = "images/Team1Player1Highlight.png";
                    break;
                case 2:
                    texture_circle_path = "images/Team1Player2Highlight.png";
                    break;
                case 3:
                    texture_circle_path = "images/Team1Player3Highlight.png";
                    break;
            }
            break;
        case 2:
            texture_rectangle_path = "images/Team2Tail.png";

            switch (number)
            {
                case 1:
                    texture_circle_path = "images/Team2Player1Highlight.png";
                    break;
                case 2:
                    texture_circle_path = "images/Team2Player2Highlight.png";
                    break;
                case 3:
                    texture_circle_path = "images/Team2Player3Highlight.png";
                    break;
            }
            break;
    }

    // ----------------
    // Circle component
    // ----------------

    // Create graphics item
    QAbstractGraphicsShapeItem *item = new QGraphicsEllipseItem(kXPlayerCircleLeft,
                                                                kYPlayerCircleTop,
                                                                kPlayerWidth,
                                                                kPlayerHeight);

    // Set item position
    item->setPos(x, y);

    // Set texture
    QBrush brush;
    brush.setTexture(QPixmap(texture_circle_path));
    QTransform transform;
    transform.translate(item->boundingRect().x(), item->boundingRect().y());
    transform.scale(item->boundingRect().width()/brush.texture().width(), item->boundingRect().height()/brush.texture().height());
    brush.setTransform(transform);
    item->setBrush(brush);
    item->setPen(Qt::NoPen);

    // Create physics body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, -y);
    bodyDef.angle = -(angle * kDegToRad);
    bodyDef.angularDamping = kAngularDamping;
    bodyDef.linearDamping = kLinearDamping;
    b2Body *body = _world.CreateBody(&bodyDef);

    // Attach fixture to body
    b2CircleShape circleShape;
    circleShape.m_radius = kPlayerWidth / 2.0f;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = kDensity;
    fixtureDef.restitution = kRestitution;
    body->CreateFixture(&fixtureDef);

    // -------------------
    // Rectangle component
    // -------------------

    // Create graphics item
    QGraphicsRectItem *box = new QGraphicsRectItem(-kPlayerWidth/16, -kPlayerHeight/8,
                                                   kPlayerWidth/8, kPlayerHeight/4, item);

    // Set item position
    box->setPos(kXPlayerBoxCenter, kYPlayerBoxCenter);

    // Set texture
    brush.setTexture(QPixmap(texture_rectangle_path));
    transform.reset();
    transform.translate(box->boundingRect().x(), box->boundingRect().y());
    transform.scale(box->boundingRect().width()/brush.texture().width(), box->boundingRect().height()/brush.texture().height());
    brush.setTransform(transform);
    box->setBrush(brush);
    box->setPen(Qt::NoPen);

    // Attach fixture to previously created body
    b2PolygonShape polyShape;
    polyShape.SetAsBox(kPlayerWidth/16, kPlayerWidth/8,
                       b2Vec2(kXPlayerCircleLeft - kPlayerWidth/16, 0), 0);
    b2FixtureDef polyFix;
    polyFix.shape = &polyShape;
    polyFix.density = kDensity;
    polyFix.restitution = kRestitution;
    body->CreateFixture(&polyFix);

    return new PlayerEntity(item, body);
}

GenericEntity *EntityFactory::createStadiumEntity(float x, float y, float angle)
{
    // The stadium is the border of a rectangle

    // The polygon is defined by the intersection of outer and inner rectangles
    QPainterPath painter_path;
    // Outer rectangle
    painter_path.addRect(kXStadiumLeft, kYStadiumTop, kStadiumWidth, kStadiumHeight);
    // Inner rectangle
    painter_path.addRect(kXStadiumLeft + kStadiumBorderWidth, kYStadiumTop + kStadiumBorderHeight,
                         kStadiumWidth - kStadiumBorderWidth*2, kStadiumHeight - kStadiumBorderHeight*2);

    // Create graphics item
    QGraphicsPathItem *item = new QGraphicsPathItem(painter_path);

    // Set item position
    item->setPos(x, y);

    // Set texture
    QBrush brush;
    brush.setTexture(QPixmap("images/Stadium.png"));
    QTransform transform;
    transform.translate(item->boundingRect().x(), item->boundingRect().y());
    qreal num_repeat = 10.0;
    transform.scale(item->boundingRect().width() / (brush.texture().width() * num_repeat), item->boundingRect().height() / (brush.texture().height() * num_repeat));
    brush.setTransform(transform);
    item->setBrush(brush);
    item->setPen(Qt::NoPen);

    // Create physics body
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, -y);
    bodyDef.angle = -(angle * kDegToRad);
    b2Body *body = _world.CreateBody(&bodyDef);

    // Attach fixture to body
    b2ChainShape chainShape;
    b2Vec2 vertexs[4];
    vertexs[0].Set(kXStadiumLeft + kStadiumBorderWidth, -(kYStadiumTop + kStadiumBorderHeight));
    vertexs[1].Set(kXStadiumLeft + kStadiumBorderWidth, -(kYStadiumTop + kStadiumHeight - kStadiumBorderHeight));
    vertexs[2].Set(kXStadiumLeft + kStadiumWidth - kStadiumBorderWidth, -(kYStadiumTop + kStadiumHeight - kStadiumBorderHeight));
    vertexs[3].Set(kXStadiumLeft + kStadiumWidth - kStadiumBorderWidth, -(kYStadiumTop + kStadiumBorderHeight));
    chainShape.CreateLoop(vertexs, 4);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &chainShape;
    body->CreateFixture(&fixtureDef);

    return new GenericEntity(item, body);
}

DynamicEntity *EntityFactory::createBallEntity(float x, float y)
{
    // The ball is a circle

    // Create graphics item
    QAbstractGraphicsShapeItem *item = new QGraphicsEllipseItem(kXBallLeft,
                                                                kYBallTop,
                                                                kBallWidth,
                                                                kBallHeight);

    // Set item position
    item->setPos(x, y);

    // Set texture
    QBrush brush;
    brush.setTexture(QPixmap("images/Ball.png"));
    QTransform transform;
    transform.translate(item->boundingRect().x(), item->boundingRect().y());
    transform.scale(item->boundingRect().width()/brush.texture().width(), item->boundingRect().height()/brush.texture().height());
    brush.setTransform(transform);
    item->setBrush(brush);
    item->setPen(Qt::NoPen);

    // Create physics body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, -y);
    bodyDef.angularDamping = kAngularDamping;
    bodyDef.linearDamping = kLinearDamping;
    b2Body *body = _world.CreateBody(&bodyDef);

    // Attach fixture to body
    b2CircleShape circleShape;
    circleShape.m_radius = kBallWidth / 2.0f;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = kDensity;
    fixtureDef.restitution = kRestitution;
    body->CreateFixture(&fixtureDef);

    return new DynamicEntity(item, body);
}

GenericEntity *EntityFactory::createGoalEntity(float x, float y, float angle)
{
    // The goal is the border of a [ shape

    // The polygon is defined by the intersection of two rectangles
    QPainterPath painter_path;
    painter_path.addRect(kXGoalLeft, kYGoalTop, kGoalWidth, kGoalHeight);
    painter_path.addRect(kXGoalLeft + kGoalBorderWidth,
                         kYGoalTop + kGoalBorderHeight,
                         kGoalWidth - kGoalBorderWidth,
                         kGoalHeight - kGoalBorderHeight*2);

    // Create graphics item
    QGraphicsPathItem *item = new QGraphicsPathItem(painter_path);

    // Set position an rotation
    item->setPos(x, y);
    item->setRotation(angle);

    // Set texture
    QBrush brush;
    brush.setTexture(QPixmap("images/Stadium.png"));
    QTransform transform;
    transform.translate(item->boundingRect().x(), item->boundingRect().y());
    transform.scale(item->boundingRect().width()/brush.texture().width(), item->boundingRect().height()/brush.texture().height());
    brush.setTransform(transform);
    item->setBrush(brush);

    // Create physics body
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, -y);
    bodyDef.angle = -(angle * kDegToRad);
    b2Body *body = _world.CreateBody(&bodyDef);

    // Attach fixture to body
    b2ChainShape chainShape;
    b2Vec2 vertexs[8];
    vertexs[0].Set(kXGoalLeft, -kYGoalTop);
    vertexs[1].Set(kXGoalLeft, -(kYGoalTop + kGoalHeight));
    vertexs[2].Set(kXGoalLeft + kGoalWidth, -(kYGoalTop + kGoalHeight));
    vertexs[3].Set(kXGoalLeft + kGoalWidth, -(kYGoalTop + kGoalHeight - kGoalBorderHeight));
    vertexs[4].Set(kXGoalLeft + kGoalBorderWidth, -(kYGoalTop + kGoalHeight - kGoalBorderHeight));
    vertexs[5].Set(kXGoalLeft + kGoalBorderWidth, -(kYGoalTop + kGoalBorderHeight));
    vertexs[6].Set(kXGoalLeft + kGoalWidth, -(kYGoalTop + kGoalBorderHeight));
    vertexs[7].Set(kXGoalLeft + kGoalWidth, -kYGoalTop);
    chainShape.CreateLoop(vertexs, 8);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &chainShape;
    body->CreateFixture(&fixtureDef);

    // Attach sensor fixture to body (for detecting goals scored when the ball cross the line)
    b2ChainShape lineShape;
    b2Vec2 line[2];
    line[0].Set(kXGoalLeft + kGoalWidth, -(kYGoalTop + kGoalBorderHeight));
    line[1].Set(kXGoalLeft + kGoalWidth, -(kYGoalTop + kGoalHeight - kGoalBorderHeight));
    lineShape.CreateChain(line, 2);
    fixtureDef.shape = &lineShape;
    fixtureDef.isSensor = true;
    body->CreateFixture(&fixtureDef);

    return new GenericEntity(item, body);
}
