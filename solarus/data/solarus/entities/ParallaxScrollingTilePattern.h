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
#ifndef SOLARUS_PARALLAX_SCROLLING_TILE_PATTERN_H
#define SOLARUS_PARALLAX_SCROLLING_TILE_PATTERN_H

#include "solarus/core/Common.h"
#include "solarus/entities/SimpleTilePattern.h"

namespace Solarus {

/**
 * \brief Tile pattern with a parallax scrolling effect.
 *
 * The pattern moves from 1 pixel when the camera moves from 2 pixels.
 * This gives an illusion of depth. Only the position of where the pattern
 * is displayed changes: the real position of the tile never changes.
 * Therefore, placing such tiles on your map can be tricky because at runtime,
 * they are displayed elsewhere (except in the particular case where the
 * camera is in (0,0), the top-left corner of the map).
 * Note that collisions properties remain applied to the
 * real position of the tile and not to where they are displayed.
 * Usually, parallax scrolling is used for decoration only, for example to make
 * nice scrolling backgrounds.
 */
class ParallaxScrollingTilePattern: public SimpleTilePattern {

  public:

    ParallaxScrollingTilePattern(Ground ground, const Point& xy, const Size& size);

    virtual void draw(
        const SurfacePtr& dst_surface,
        const Point& dst_position,
        const Tileset& tileset,
        const Point& viewport
    ) const override;

    virtual bool is_animated() const override;
    virtual bool is_drawn_at_its_position() const override;

    static constexpr int ratio = 2;  /**< Distance made by the viewport to move the tile pattern of 1 pixel. */

};

}

#endif

