// ****************************************************************
// Copyright (c) 2013 by Francisco Javier Paz Menendez
//
// This content is licensed under the MIT License. See LICENSE.txt
// in the top directory of this distribution for more information
// ****************************************************************

#include "Team.h"
#include "PlayerEntity.h"

Team::Team() :
    _selected(0),
    _initial(0),
    _score(0)
{
}

Team::~Team()
{
    _next_players.clear();
    _players.clear();
}

void Team::addPlayer(PlayerEntity *player)
{
    _players.append(player);

    if (!_initial)
    {
        _initial = player;
        _selected = player;
        _selected->select();
    }
}

void Team::setNextPlayer(PlayerEntity *player, PlayerEntity *next)
{
    _next_players[player] = next;
}

void Team::setInitialPlayer(PlayerEntity *initial)
{
    _initial = initial;
}

void Team::selectNextPlayer()
{
    changeSelectedTo(_next_players[_selected]);
}

void Team::selectInitialPlayer()
{
    changeSelectedTo(_initial);
}

void Team::incrementScore()
{
    ++_score;
}

uint8_t Team::score()
{
    return _score;
}

PlayerEntity*Team::selected()
{
    return _selected;
}

void Team::changeSelectedTo(PlayerEntity *new_selected)
{
    _selected->unselect();
    _selected = new_selected;
    _selected->select();
}
