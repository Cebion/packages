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
#ifndef SOLARUS_TILE_INFO_H
#define SOLARUS_TILE_INFO_H

#include "solarus/core/Common.h"
#include "solarus/core/Rectangle.h"
#include <memory>
#include <string>

namespace Solarus {

class TilePattern;
class Tileset;

/**
 * \brief Wraps construction parameters of a tile.
 */
struct TileInfo {

  int layer;
  Rectangle box;
  std::string pattern_id;
  std::shared_ptr<TilePattern> pattern;  /**< nullptr if it does not exist in the tileset. */
  const Tileset* tileset = nullptr;      /**< nullptr means the tileset of the map. */
};

}

#endif
