// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#ifndef TEAM_H
#define TEAM_H

#include <QHash>
#include <QList>
#include <stdint.h>

class PlayerEntity;

class Team
{
public:
    Team();
    virtual ~Team();

    // The first player added will be initial and selected
    void addPlayer(PlayerEntity *player);
    void setNextPlayer(PlayerEntity* player, PlayerEntity* next);
    void setInitialPlayer(PlayerEntity* initial);
    void selectNextPlayer();
    void selectInitialPlayer();
    void incrementScore();
    uint8_t score();
    PlayerEntity* selected();

private:
    void changeSelectedTo(PlayerEntity* new_selected);

    QList<PlayerEntity*> _players;
    QHash<PlayerEntity*, PlayerEntity*> _next_players;
    PlayerEntity* _selected;
    PlayerEntity* _initial;
    uint8_t _number;
    uint8_t _score;
};

#endif // TEAM_H
