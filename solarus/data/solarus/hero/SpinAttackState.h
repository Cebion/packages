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
#ifndef SOLARUS_HERO_SPIN_ATTACK_STATE_H
#define SOLARUS_HERO_SPIN_ATTACK_STATE_H

#include "solarus/core/Common.h"
#include "solarus/entities/EntityState.h"

namespace Solarus {

/**
 * \brief The state "spin attack" of the hero.
 */
class Hero::SpinAttackState: public HeroState {

  public:

    explicit SpinAttackState(Hero& hero);

    void start(const State* previous_state) override;
    void stop(const State* next_state) override;
    void update() override;
    bool can_sword_hit_crystal() const override;
    bool get_can_be_hurt(Entity* attacker) override;
    bool get_can_pick_treasure(EquipmentItem& item) const override;
    bool is_cutting_with_sword(Entity& entity) override;
    int get_sword_damage_factor() const override;
    bool is_deep_water_obstacle() const override;
    bool is_hole_obstacle() const override;
    bool is_lava_obstacle() const override;
    bool is_prickle_obstacle() const override;
    bool is_teletransporter_obstacle(Teletransporter& teletransporter) override;
    bool is_separator_obstacle(Separator& separator) override;
    void notify_obstacle_reached() override;
    void notify_attacked_enemy(
        EnemyAttack attack,
        Enemy& victim,
        Sprite* victim_sprite,
        const EnemyReaction::Reaction& result,
        bool killed
    ) override;

  private:

    void play_spin_attack_sound();

    bool being_pushed;           /**< indicates that the hero is being pushed after hitting an enemy */

};

}

#endif

