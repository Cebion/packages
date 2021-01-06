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
#ifndef SOLARUS_BLOCK_H
#define SOLARUS_BLOCK_H

#include "solarus/core/Common.h"
#include "solarus/entities/Entity.h"
#include <string>

namespace Solarus {

/**
 * \brief A entity that the hero can push or pull.
 *
 * Some blocks can be moved multiple times or in only one direction.
 */
class Block: public Entity {

  public:

    static constexpr EntityType ThisType = EntityType::BLOCK;

    Block(const std::string& name,
        int layer,
        const Point& xy,
        int direction,
        const std::string& sprite_name,
        bool can_be_pushed,
        bool can_be_pulled,
        int max_moves
    );

    EntityType get_type() const override;
    bool is_ground_observer() const override;

    bool is_obstacle_for(Entity& other) override;
    bool is_hole_obstacle() const override;
    bool is_deep_water_obstacle() const override;
    bool is_lava_obstacle() const override;
    bool is_teletransporter_obstacle(Teletransporter& teletransporter) override;
    bool is_hero_obstacle(Hero& hero) override;
    bool is_enemy_obstacle(Enemy& enemy) override;
    bool is_destructible_obstacle(Destructible& destructible) override;
    bool is_separator_obstacle(Separator& separator, const Rectangle& candidate_position) override;
    void notify_created() override;
    void notify_collision(Entity& entity_overlapping, CollisionMode collision_mode) override;
    void notify_collision_with_switch(Switch& sw, CollisionMode collision_mode) override;
    bool notify_action_command_pressed() override;
    bool start_movement_by_hero() override;
    void stop_movement_by_hero() override;
    void notify_moving_by(Entity& entity) override;
    void notify_moved_by(Entity& entity) override;
    void notify_position_changed() override;
    void notify_obstacle_reached() override;
    void notify_ground_below_changed() override;

    void reset();
    bool is_pushable() const;
    void set_pushable(bool pushable);
    bool is_pullable() const;
    void set_pullable(bool pullable);
    int get_max_moves() const;
    void set_max_moves(int max_moves);

  private:

    void movement_by_hero_finished();

    int max_moves;              /**< indicates whether the block can be pushed
                                 * (-1 means infinite) */
    bool sound_played;          /**< true if the block sound was played while pulling it */
    uint32_t when_can_move;     /**< date when the hero can move the block again */

    Point last_position;        /**< last position of the block before moving */
    Point initial_position;     /**< position of the block when created */
    int initial_max_moves;      /**< value of maximum_moves when the block was created */
    bool can_be_pushed;         /**< indicates that the hero can push this block */
    bool can_be_pulled;         /**< indicates that the hero can pull this block */

    static constexpr uint32_t moving_delay = 500; /**< delay between two successive moves of a block */

};

}

#endif

