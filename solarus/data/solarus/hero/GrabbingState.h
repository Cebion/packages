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
#ifndef SOLARUS_HERO_GRABBING_STATE_H
#define SOLARUS_HERO_GRABBING_STATE_H

#include "solarus/core/Common.h"
#include "solarus/hero/HeroState.h"

namespace Solarus {

/**
 * \brief The state "Grabbing" of the hero.
 */
class Hero::GrabbingState: public HeroState {

  public:

    explicit GrabbingState(Hero& hero);

    void start(const State* previous_state) override;
    void update() override;
    bool is_grabbing_or_pulling() const override;
    bool get_can_be_hurt(Entity* attacker) override;
    bool get_can_pick_treasure(EquipmentItem& item) const override;

};

}

#endif

