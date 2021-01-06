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
#ifndef SOLARUS_SIMPLE_TILE_PATTERN_H
#define SOLARUS_SIMPLE_TILE_PATTERN_H

#include "solarus/core/Common.h"
#include "solarus/core/Rectangle.h"
#include "solarus/entities/TilePattern.h"

namespace Solarus {

/**
 * \brief Non-animated tile pattern.
 *
 * This tile pattern does not show any special effects and displays
 * a single image.
 */
class SimpleTilePattern: public TilePattern {

  public:

    SimpleTilePattern(Ground ground, const Point& xy, const Size& size);

    virtual void draw(
        const SurfacePtr& dst_surface,
        const Point& dst_position,
        const Tileset& tileset,
        const Point& viewport
    ) const override;

    virtual bool is_animated() const override;

  protected:

    Rectangle position_in_tileset; /**< position of the tile pattern in the tileset image */

};

}

#endif

