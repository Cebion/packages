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
#ifndef SOLARUS_HERO_HURT_STATE_H
#define SOLARUS_HERO_HURT_STATE_H

#include "solarus/core/Common.h"
#include "solarus/hero/HeroState.h"
#include <cstdint>

namespace Solarus {

/**
 * \brief The state "Hurt" of the hero.
 */
class Hero::HurtState: public HeroState {

  public:

    HurtState(Hero& hero,
        const Point* source_xy,
        int damage);

    virtual void start(const State* previous_state) override;
    virtual void stop(const State* next_state) override;
    virtual void update() override;
    virtual void set_suspended(bool suspended) override;
    virtual bool can_start_gameover_sequence() const override;
    virtual bool is_touching_ground() const override;
    virtual bool is_teletransporter_obstacle(Teletransporter& teletransporter) override;
    virtual bool is_stream_obstacle(Stream& stream) override;
    virtual bool is_sensor_obstacle(Sensor& sensor) override;
    virtual bool is_separator_obstacle(Separator& separator) override;
    virtual bool get_can_be_hurt(Entity* attacker) override;
    virtual bool can_avoid_switch() const override;
    virtual bool can_avoid_ice() const override;

  private:

    bool has_source;              /**< Whether there is a source_xy rectangle.
                                   * If no, the hero does not move. */
    const Point source_xy;        /**< Coordinates of whatever is hurting the
                                   * hero (usually an enemy). */
    int damage;                   /**< Number of life points to remove
                                   * (this number may be reduced by the tunic
                                   * or by hero:on_taking_damage()). */
    uint32_t end_hurt_date;       /**< Date when the state ends. */

};

}

#endif

