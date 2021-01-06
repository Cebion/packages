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
#ifndef SOLARUS_DYNAMIC_TILE_H
#define SOLARUS_DYNAMIC_TILE_H

#include "solarus/core/Common.h"
#include "solarus/entities/Entity.h"
#include <string>

namespace Solarus {

class TilePattern;
class Tileset;

/**
 * \brief A special tile that can be enabled or disabled.
 *
 * A dynamic tile is a tile placed on the map
 * that can be enabled or disabled by the script,
 * contrary to the Tile instances that are totally static for performance reasons.
 * An enabled dynamic tile behaves like a normal tile and may be an obstacle or any ground.
 * A disabled dynamic tile is invisible and can be traversed.
 */
class DynamicTile: public Entity {

  public:

    static constexpr EntityType ThisType = EntityType::DYNAMIC_TILE;

    DynamicTile(
        const std::string& name,
        int layer,
        const Point& xy,
        const Size& size,
        const std::string& tile_pattern_id,
        const std::shared_ptr<TilePattern>& tile_pattern,
        const Tileset* tileset
    );

    virtual EntityType get_type() const override;

    const std::string& get_tile_pattern_id() const;
    Ground get_modified_ground() const override;
    const Tileset* get_tileset() const;
    void set_tileset(const Tileset* tileset);
    void set_tileset(const std::string& tileset_id);
    bool is_drawn_at_its_position() const override;
    void built_in_draw(Camera& camera) override;
    void notify_tileset_changed() override;

  private:

    const std::string tile_pattern_id; /**< Id of the tile pattern. */
    std::shared_ptr<TilePattern>
        tile_pattern;                  /**< Pattern of the tile, or nullptr if it does not exist. */
    const Tileset* tileset;            /**< Tileset of the pattern (nullptr means the one of the map). */

};

}

#endif

