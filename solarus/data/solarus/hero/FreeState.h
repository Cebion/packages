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
#ifndef SOLARUS_HERO_FREE_STATE_H
#define SOLARUS_HERO_FREE_STATE_H

#include "solarus/core/Common.h"
#include "solarus/hero/PlayerMovementState.h"
#include <cstdint>

namespace Solarus {

/**
 * \brief State of the hero where he can walk normally and interact with entities.
 */
class Hero::FreeState: public Hero::PlayerMovementState {

  public:

    explicit FreeState(Hero& hero);

    void start(const State* previous_state) override;
    void stop(const State* next_state) override;
    void update() override;
    void set_suspended(bool suspended) override;
    void notify_action_command_pressed() override;
    void notify_obstacle_reached() override;

    bool is_free() const override;
    bool get_can_interact_with_npc(Npc& npc) const override;
    bool get_can_start_sword() const override;
    bool get_can_start_item(EquipmentItem& item) const override;
    bool get_can_take_stairs() const override;
    CarriedObject::Behavior get_previous_carried_object_behavior() const override;
    void set_animation_stopped() override;
    void set_animation_walking() override;

  private:

    int pushing_direction4;         /**< direction where the hero is trying to
                                     * push an obstacle (0 to 3) or -1 */
    uint32_t start_pushing_date;    /**< date when the state pushing starts */

};

}

#endif

