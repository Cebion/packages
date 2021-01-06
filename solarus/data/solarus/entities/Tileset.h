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
#ifndef SOLARUS_TILESET_H
#define SOLARUS_TILESET_H

#include "solarus/core/Common.h"
#include "solarus/graphics/Color.h"
#include "solarus/graphics/Surface.h"
#include "solarus/graphics/SurfacePtr.h"
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

struct lua_State;

namespace Solarus {

class TilePattern;
class TilePatternData;

/**
 * \brief A set of tile patterns that are used to compose a map.
 *
 * A tileset represents the skin of a map.
 */
class Tileset {

  public:

    explicit Tileset(const std::string& id);

    bool is_loaded() const;
    void load();
    void unload();

    const std::string& get_id() const;
    const Color& get_background_color() const;
    const SurfacePtr& get_tiles_image() const;
    const SurfacePtr& get_entities_image() const;
    std::shared_ptr<TilePattern> get_tile_pattern(const std::string& id) const;
    void set_images(const std::string& other_id);
    void update();

  private:

    void add_tile_pattern(
        const std::string& id,
        const TilePatternData& pattern_data
    );

    const std::string id;          /**< Id of the tileset. */
    std::unordered_map<std::string, std::shared_ptr<TilePattern>>
        tile_patterns;             /**< Tile patterns in this tileset. */
    std::vector<TilePattern*>
        animated_tile_patterns;    /**< Subset of animated patterns. */
    Color background_color;        /**< Background color of the tileset. */
    mutable SDL_Surface_UniquePtr
        tiles_image_soft;          /**< Image from which the tile patterns are extracted. */
    mutable SurfacePtr
        tiles_image;               /**< Image from which the tile patterns are extracted. */
    mutable SDL_Surface_UniquePtr
         entities_image_soft;      /**< Image from which the tileset-dependent sprites are. */
    mutable SurfacePtr
         entities_image;           /**< Image from which the tileset-dependent sprites are. */
    bool loaded;                   /**< Whether the tileset is loaded. */
    std::mutex load_mutex;         /**< Lock to protect concurrent tileset loading. */

};

}

#endif

