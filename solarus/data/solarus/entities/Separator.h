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
#ifndef SOLARUS_SEPARATOR_H
#define SOLARUS_SEPARATOR_H

#include "solarus/core/Common.h"
#include "solarus/entities/Entity.h"
#include <string>

namespace Solarus {

/**
 * \brief An horizontal or vertical separation between two parts of the map.
 *
 * The camera cannot cross this separation except when the hero touches it:
 * in this case, scrolling is performed.
 * This entity allows to separate parts of the map like if there was several
 * maps.
 */
class Separator: public Entity {

  public:

    static constexpr EntityType ThisType = EntityType::SEPARATOR;

    Separator(
        const std::string& name,
        int layer,
        const Point& xy,
        const Size& size
    );

    virtual EntityType get_type() const override;

    bool is_horizontal() const;
    bool is_vertical() const;
    bool is_crossed_by(const Rectangle& candidate_position) const;

    bool is_obstacle_for(Entity& other, const Rectangle& candidate_position) override;
    bool test_collision_custom(Entity& entity) override;
    void notify_collision(
        Entity& entity_overlapping,
        CollisionMode collision_mode
    ) override;
    void notify_activating(int direction4);
    void notify_activated(int direction4);

};

}

#endif

