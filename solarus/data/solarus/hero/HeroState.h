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
#ifndef SOLARUS_HERO_STATE_H
#define SOLARUS_HERO_STATE_H

#include "solarus/core/Common.h"
#include "solarus/entities/EntityState.h"

namespace Solarus {

/**
 * \brief The hero base state.
 */
class HeroState: public Entity::State {

  public:

    virtual Hero& get_entity() override;
    virtual const Hero& get_entity() const override;
    const HeroSprites& get_sprites() const;
    HeroSprites& get_sprites();

    void draw_on_map() override;

    void notify_attack_command_pressed() override;
    void notify_item_command_pressed(int slot) override;

    bool is_block_obstacle(Block& block) override;
    bool is_raised_block_obstacle(CrystalBlock& raised_block) override;
    bool is_jumper_obstacle(Jumper& jumper, const Rectangle& candidate_position) override;

  protected:

    HeroState(Hero& hero, const std::string& state_name);
    explicit HeroState(const std::string& state_name);

};

}

#endif

