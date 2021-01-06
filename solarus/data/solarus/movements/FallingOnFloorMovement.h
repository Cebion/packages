/*
 * Copyright (C) 2006-2019 Christopho, Solarus - http://www.solarus-games.org
 *
 * Solarus is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Solarus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef SOLARUS_FALLING_ON_FLOOR_MOVEMENT_H
#define SOLARUS_FALLING_ON_FLOOR_MOVEMENT_H

#include "solarus/core/Common.h"
#include "solarus/movements/PixelMovement.h"
#include "solarus/movements/FallingHeight.h"

namespace Solarus {

/**
 * \brief Movement of something that falls on the floor and slightly bounces.
 *
 * This is used for objects that fall on the floor and
 * bounce one or several times before they stop.
 * This movement is typically used by the pickable treasures
 * that appear when the player kills an enemy.
 *
 * TODO kill this class
 */
class FallingOnFloorMovement: public PixelMovement {

  public:

    explicit FallingOnFloorMovement(FallingHeight height);

};

}

#endif

