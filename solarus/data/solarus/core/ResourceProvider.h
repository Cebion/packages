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
#ifndef SOLARUS_RESOURCE_PROVIDER_H
#define SOLARUS_RESOURCE_PROVIDER_H

#include "solarus/audio/SoundBuffer.h"
#include "solarus/core/Common.h"
#include "solarus/core/ResourceType.h"
#include "solarus/entities/Tileset.h"
#include "solarus/entities/TilePattern.h"
#include <map>
#include <memory>
#include <thread>
#include <string>

namespace Solarus {

/**
 * \brief Provides fast access to quest resources.
 *
 * Maintains a cache of already loaded quest resources
 * so that next accesses are faster.
 */
class SOLARUS_API ResourceProvider {

  public:

    ResourceProvider();
    void clear();

    Tileset& get_tileset(const std::string& tileset_id);
    const std::map<std::string, std::shared_ptr<Tileset>>& get_loaded_tilesets();
    SoundBuffer& get_sound(const std::string& sound_id, bool language_specific = false);

    // TODO other types of resources
    // TODO clear/update when the resource list changes dynamically

    void invalidate_resource_element(ResourceType resource_type, const std::string& element_id);

    void start_preloading_resources();

  private:

    std::thread preloader_thread;  /**< Thread that loads resources in background. */
    std::map<std::string, std::shared_ptr<Tileset>>
        tileset_cache;             /**< Cache of loaded tilesets. */
    std::map<std::string, std::shared_ptr<SoundBuffer>>
        sound_cache;               /**< Cache of loaded sounds. */
};

}

#endif

