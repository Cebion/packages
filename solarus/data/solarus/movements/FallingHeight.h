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
#ifndef SOLARUS_FALLING_HEIGHT_H
#define SOLARUS_FALLING_HEIGHT_H

namespace Solarus {

/**
 * \brief Indicates from which height an entity falls.
 *
 * This enumeration is used by the FallingOnFloorMovement class,
 * typically to make pickable items fall on the ground
 * when an enemy is killed or when a pot is lifted.
 */
enum FallingHeight {
  FALLING_NONE          = 0,
  FALLING_LOW           = 1,
  FALLING_MEDIUM        = 2,
  FALLING_HIGH          = 3
};

}

#endif

