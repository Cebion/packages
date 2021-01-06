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
#ifndef SOLARUS_EXPLOSION_H
#define SOLARUS_EXPLOSION_H

#include "solarus/core/Common.h"
#include "solarus/entities/Entity.h"
#include <list>
#include <string>

namespace Solarus {

/**
 * \brief Represents an explosion on the map.
 *
 * An explosion can hurt the hero, the enemies and open weak walls.
 */
class Explosion: public Entity {

  public:

    static constexpr EntityType ThisType = EntityType::EXPLOSION;

    Explosion(
        const std::string& name,
        int layer,
        const Point& xy,
        bool with_damage
    );

    virtual EntityType get_type() const override;

    // state
    virtual void update() override;
    virtual void notify_sprite_frame_changed(Sprite& sprite, const std::string& animation, int frame) override;

    // collisions
    virtual void notify_collision(Entity& other_entity, Sprite& this_sprite, Sprite& other_sprite) override;
    virtual void notify_collision_with_switch(Switch& sw, Sprite& sprite_overlapping) override;
    virtual void notify_collision_with_crystal(Crystal& crystal, Sprite& sprite_overlapping) override;
    virtual void notify_collision_with_sensor(Sensor& sensor, CollisionMode collision_mode) override;
    virtual void notify_collision_with_enemy(Enemy& enemy, Sprite& this_sprite, Sprite& enemy_sprite) override;
    void try_attack_enemy(Enemy& enemy, Sprite &enemy_sprite);
    virtual void notify_attacked_enemy(
        EnemyAttack attack,
        Enemy& victim,
        Sprite* victim_sprite,
        const EnemyReaction::Reaction& result,
        bool killed
    ) override;

  private:

    std::list<Enemy*> victims; /**< list of enemies successfully hurt by this explosion */

};

}

#endif

