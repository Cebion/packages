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
#ifndef SOLARUS_HOOKSHOT_H
#define SOLARUS_HOOKSHOT_H

#include "solarus/core/Common.h"
#include "solarus/entities/Entity.h"
#include "solarus/graphics/Sprite.h"

namespace Solarus {

/**
 * \brief The hookshot thrown by the hero.
 */
class Hookshot: public Entity {

  public:

    static constexpr EntityType ThisType = EntityType::HOOKSHOT;

    explicit Hookshot(const Hero& hero);

    EntityType get_type() const override;

    bool is_teletransporter_obstacle(Teletransporter& teletransporter) override;
    bool is_stream_obstacle(Stream& stream) override;
    bool is_stairs_obstacle(Stairs& stairs) override;
    bool is_deep_water_obstacle() const override;
    bool is_hole_obstacle() const override;
    bool is_lava_obstacle() const override;
    bool is_prickle_obstacle() const override;
    bool is_ladder_obstacle() const override;
    bool is_switch_obstacle(Switch& sw) override;
    bool is_crystal_obstacle(Crystal& crystal) override;
    bool is_jumper_obstacle(Jumper& jumper, const Rectangle& candidate_position) override;

    // state
    void update() override;
    void built_in_draw(Camera& camera) override;
    bool is_flying() const;
    bool is_going_back() const;
    void go_back();
    void attach_to(Entity& entity_reached);

    // collisions
    void notify_obstacle_reached() override;
    void notify_collision_with_enemy(Enemy& enemy, Sprite& this_sprite, Sprite& enemy_sprite) override;
    void notify_attacked_enemy(
        EnemyAttack attack,
        Enemy& victim,
        Sprite* victim_sprite,
        const EnemyReaction::Reaction& result,
        bool killed
    ) override;
    void notify_collision_with_chest(Chest& chest) override;
    void notify_collision_with_destructible(Destructible& destructible, CollisionMode collision_mode) override;
    void notify_collision_with_block(Block& block) override;
    void notify_collision_with_switch(Switch& sw, CollisionMode collision_mode) override;
    void notify_collision_with_crystal(Crystal& crystal, CollisionMode collision_mode) override;

  private:

    uint32_t next_sound_date;    /**< date when the hookshot sound is be played next time */

    bool has_to_go_back;         /**< true if the hookshot is about to go back */
    bool going_back;             /**< indicates that the hookshot is going back towards the hero */
    Entity* entity_reached;   /**< the entity the hookshot is attached to (or nullptr) */

    SpritePtr
        link_sprite;             /**< sprite of the links */

};

}

#endif

