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
#ifndef SOLARUS_HERO_H
#define SOLARUS_HERO_H

#include "solarus/core/Point.h"
#include "solarus/entities/EnemyAttack.h"
#include "solarus/entities/Entity.h"
#include "solarus/entities/Ground.h"
#include "solarus/hero/HeroSprites.h"
#include <memory>
#include <string>

namespace Solarus {

class CustomState;
class CarriedObject;
class Equipment;
class EquipmentItem;
class EquipmentItemUsage;
class HeroSprites;
class HeroState;
class InputEvent;
class Map;
class Treasure;

/**
 * \brief The hero entity.
 *
 * This class represents the hero. It coordinates his position on the map and his state.
 * The hero is animated by several sprites that are handled by the HeroSprites class.
 */
class Hero: public Entity {

  public:

    static constexpr EntityType ThisType = EntityType::HERO;

    /**
     * \name Creation and destruction.
     */
    explicit Hero(Equipment& equipment);

    /**
     * \name Features.
     *
     * These functions, required by Entity, indicate
     * the main properties of this type of entity.
     */
    EntityType get_type() const override;

    /**
     * \name Game loop.
     *
     * Functions called by the game loop.
     */
    void update() override;
    void built_in_draw(Camera& camera) override;
    void set_suspended(bool suspended) override;
    bool notify_input(const InputEvent& event);
    void notify_command_pressed(GameCommand command) override;
    void notify_command_released(GameCommand command) override;

    /**
     * \name Sprites.
     *
     * Functions relative to the sprites.
     * The sprites are managed and drawn by the class HeroSprites.
     */
    const HeroSprites& get_hero_sprites() const;
    HeroSprites& get_hero_sprites();
    int get_animation_direction() const;
    void set_animation_direction(int direction);
    bool is_animation_finished() const;
    void rebuild_equipment();
    bool is_shadow_visible() const;

    /**
     * \name Changing map.
     *
     * Functions called when the player goes to another map.
     */
    void notify_creating() override;
    void notify_map_starting(Map& map, const std::shared_ptr<Destination>& destination) override;
    void notify_map_started(Map& map, const std::shared_ptr<Destination>& destination) override;
    void notify_map_opening_transition_finishing(Map& map, const std::shared_ptr<Destination>& destination) override;
    void notify_map_opening_transition_finished(Map& map, const std::shared_ptr<Destination>& destination) override;
    void notify_map_finished() override;
    void notify_tileset_changed() override;
    void place_on_destination(Map& map, const Rectangle& previous_map_location);

    /**
     * \name Position.
     *
     * These function provide information about the position of
     * the hero relative to other entities, and about
     * what is in front of him (we call this the "facing point").
     */
    Point get_facing_point() const override;
    void notify_facing_entity_changed(Entity* facing_entity) override;
    bool is_facing_obstacle();
    bool is_facing_point_on_obstacle();
    bool is_facing_direction4(int direction4) const;
    bool is_facing_direction8(int direction8) const;
    bool is_on_raised_blocks() const;
    std::shared_ptr<Stairs> get_stairs_overlapping();

    /**
     * \name Movement.
     *
     * Information about the movement of the hero.
     * The hero is controlled by the player most of the time,
     * and sometimes he is controlled automatically or he
     * cannot move.
     */
    bool can_control_movement() const;
    bool can_control_direction() const;
    int get_normal_walking_speed() const;
    void set_normal_walking_speed(int normal_walking_speed);
    int get_walking_speed() const;
    void set_walking_speed(int walking_speed);
    int get_wanted_movement_direction8() const;
    int get_real_movement_direction8();
    bool is_moving_towards(int direction4) const;
    bool is_direction_locked() const;
    void notify_position_changed() override;
    void notify_layer_changed() override;
    void notify_obstacle_reached() override;
    void notify_movement_started() override;
    void notify_movement_changed() override;
    void notify_movement_finished() override;
    void reset_movement();

    /**
     * \name Ground under the hero.
     *
     * Functions to handle the ground of the tiles under the hero.
     * Depending on the kind of ground, a special sprite may be displayed under him (grass, shallow water)
     * or something bad can happen (deep water, holes).
     */
    bool is_ground_visible() const;
    bool is_ground_observer() const override;
    void notify_ground_below_changed() override;
    const Point& get_last_solid_ground_coords() const;
    int get_last_solid_ground_layer() const;
    ScopedLuaRef make_solid_ground_callback(
        const Point& xy, int layer) const;
    const ScopedLuaRef& get_target_solid_ground_callback() const;
    void set_target_solid_ground_callback(const ScopedLuaRef& callback);
    void reset_target_solid_ground_callback();

    /**
     * \name Obstacles.
     *
     * Information about what is considered as an obstacle for the hero.
     */
    bool is_obstacle_for(Entity& other) override;
    bool is_traversable_obstacle() const override;
    bool is_wall_obstacle() const override;
    bool is_low_wall_obstacle() const override;
    bool is_grass_obstacle() const override;
    bool is_shallow_water_obstacle() const override;
    bool is_deep_water_obstacle() const override;
    bool is_hole_obstacle() const override;
    bool is_ice_obstacle() const override;
    bool is_lava_obstacle() const override;
    bool is_prickle_obstacle() const override;
    bool is_ladder_obstacle() const override;
    bool is_block_obstacle(Block& block) override;
    bool is_teletransporter_obstacle(Teletransporter& teletransporter) override;
    bool is_hero_obstacle(Hero& hero) override;
    bool is_stream_obstacle(Stream& stream) override;
    bool is_stairs_obstacle(Stairs& stairs) override;
    bool is_sensor_obstacle(Sensor& sensor) override;
    bool is_switch_obstacle(Switch& sw) override;
    bool is_raised_block_obstacle(CrystalBlock& raised_block) override;
    bool is_crystal_obstacle(Crystal& crystal) override;
    bool is_npc_obstacle(Npc& npc) override;
    bool is_door_obstacle(Door& block) override;
    bool is_enemy_obstacle(Enemy& enemy) override;
    bool is_jumper_obstacle(Jumper& jumper, const Rectangle& candidate_position) override;
    bool is_destructible_obstacle(Destructible& destructible) override;
    bool is_separator_obstacle(Separator& separator, const Rectangle& candidate_position) override;

    /**
     * \name Collisions.
     *
     * Handle collisions between the hero and other entities.
     */
    void check_position() override;
    void notify_collision_with_destructible(Destructible& destructible, CollisionMode collision_mode) override;
    void notify_collision_with_enemy(Enemy& enemy, CollisionMode) override;
    void notify_collision_with_enemy(Enemy& enemy, Sprite& this_sprite, Sprite& enemy_sprite) override;
    void notify_collision_with_teletransporter(Teletransporter& teletransporter, CollisionMode collision_mode) override;
    void notify_collision_with_stream(Stream& stream, int dx, int dy) override;
    void notify_collision_with_stairs(Stairs& stairs, CollisionMode collision_mode) override;
    void notify_collision_with_jumper(Jumper& jumper, CollisionMode collision_mode) override;
    void notify_collision_with_sensor(Sensor& sensor, CollisionMode collision_mode) override;
    void notify_collision_with_switch(Switch& sw, CollisionMode collision_mode) override;
    void notify_collision_with_switch(Switch& sw, Sprite &sprite_overlapping) override;
    void notify_collision_with_crystal(Crystal& crystal, CollisionMode collision_mode) override;
    void notify_collision_with_crystal(Crystal& crystal, Sprite &sprite_overlapping) override;
    void notify_collision_with_chest(Chest& chest) override;
    void notify_collision_with_block(Block& block) override;
    void notify_collision_with_separator(Separator& separator, CollisionMode collision_mode) override;
    void notify_collision_with_explosion(Explosion& explosion, Sprite& sprite_overlapping) override;
    void avoid_collision(Entity& entity, int direction);
    bool is_striking_with_sword(Entity& entity) const;

    /**
     * \name Enemies.
     *
     * Attacking enemies or getting hurt by them.
     */
    void notify_attacked_enemy(
        EnemyAttack attack,
        Enemy& victim,
        Sprite* victim_sprite,
        const EnemyReaction::Reaction& result,
        bool killed
    ) override;
    int get_sword_damage_factor() const;
    bool is_invincible() const;
    void set_invincible(bool invincible, uint32_t duration);
    bool can_be_hurt(Entity* attacker) const;
    void hurt(
        Entity& source,
        Sprite* source_sprite,
        int life_points
    );
    void hurt(const Point& source_xy, int life_points);
    void hurt(int life_points);
    void notify_game_over_finished();

    /**
     * \name State.
     *
     * These functions provide information about the hero's internal state
     * and allow to start actions which may modify this state.
     * Actions can be triggered by equipment items, entities or scripts.
     */
    bool is_free() const;
    bool is_using_item() const;
    EquipmentItemUsage& get_item_being_used();
    std::shared_ptr<CarriedObject> get_carried_object();
    bool is_brandishing_treasure() const;
    bool is_grabbing_or_pulling() const;
    bool is_moving_grabbed_entity() const;
    void notify_grabbed_entity_collision();

    void start_grass();
    void start_shallow_water();
    void start_deep_water();
    void start_hole();
    void start_ice();
    void start_lava();
    void start_prickle(uint32_t delay);

    void start_next_state();
    void start_free();
    void start_free_carrying_loading_or_running();
    void start_treasure(
        const Treasure& treasure,
        const ScopedLuaRef& callback_ref
    );
    void start_forced_walking(const std::string& path, bool loop, bool ignore_obstacles);
    void start_jumping(int direction8, int distance, bool ignore_obstacles,
        bool with_sound);
    void start_frozen();
    void start_victory(const ScopedLuaRef& callback_ref);
    void start_lifting(const std::shared_ptr<CarriedObject>& item_to_lift);
    void start_running();
    void start_pushing();
    void start_grabbing();
    void start_pulling();
    bool can_pick_treasure(EquipmentItem& item);
    bool can_avoid_teletransporter(const Teletransporter& teletransporter) const;
    bool can_run() const;
    bool can_push() const;
    bool can_grab() const;
    bool can_pull() const;
    bool can_interact_with_npc(Npc& npc);
    bool can_use_shield() const;
    bool can_start_sword() const;
    void start_sword();
    void start_sword_loading(int spin_attack_delay);
    bool can_start_item(EquipmentItem& item);
    void start_item(EquipmentItem& item);
    void start_boomerang(int max_distance, int speed,
        const std::string& tunic_preparing_animation,
        const std::string& sprite_name);
    void start_bow();
    void start_hookshot();
    void start_back_to_solid_ground(bool use_specified_position,
        uint32_t end_delay = 0, bool with_sound = true);
    void start_state_from_ground();
    void start_custom_state(const std::shared_ptr<CustomState>& custom_state);

  private:

    // state
    class PlayerMovementState;      /**< base class for states whose movement is controlled by the player */
    class FreeState;                /**< the hero is free to move (stopped or walking) and can interact with entities */
    class CarryingState;            /**< the hero can walk but he is carrying a pot or a bush */
    class SwordLoadingState;        /**< the hero can walk but his sword is loading for a spin attack */
    class SwimmingState;            /**< the hero is swimming in deep water */
    class PushingState;             /**< the hero is trying to push an obstacle */
    class SwordTappingState;        /**< the hero is tapping his sword on a wall */
    class PullingState;             /**< the hero is pulling an object */
    class GrabbingState;            /**< the hero is grabbing an object and can pull it */
    class SwordSwingingState;       /**< the hero is swinging his sword */
    class SpinAttackState;          /**< the hero is releasing a spin attack */
    class LiftingState;             /**< the hero is lifting an destroyable item (a pot, a bush, etc.) */
    class TreasureState;            /**< the hero is brandishing a treasure */
    class RunningState;             /**< the hero is running */
    class ForcedWalkingState;       /**< the hero is walking with a predetermined path */
    class JumpingState;             /**< the hero is jumping */
    class HurtState;                /**< the hero is hurt */
    class PlungingState;            /**< the hero is plunging into water */
    class FallingState;             /**< the hero is falling into a hole */
    class BackToSolidGroundState;   /**< the hero is getting back to solid ground (e.g. after he drowned
                                     * in deep water or falled into a hole) */
    class StairsState;              /**< the hero is being moved by stairs */
    class VictoryState;             /**< the hero is make a victory sequence with his sword */
    class UsingItemState;           /**< the hero is currently using an equipment item */
    class BoomerangState;           /**< the hero is shooting a boomerang */
    class HookshotState;            /**< the hero is throwing his hookshot */
    class BowState;                 /**< the hero is shooting an arrow with a bow */
    class FrozenState;              /**< the hero cannot move for various possible reasons,
                                     * including an instruction from the script */

    // position
    void place_on_map(Map& map);
    void update_direction();
    void update_movement();
    void try_snap_to_facing_entity();
    void apply_additional_ground_movement();
    std::shared_ptr<Teletransporter> get_delayed_teletransporter();

    // ground
    void update_ground_effects();
    void update_ice();
    void stop_ice_movement();

    // life
    void check_gameover();
    void update_invincibility();

    // state
    bool invincible;                       /**< Whether the hero is temporarily invincible. */
    uint32_t end_invincible_date;          /**< When stopping the invincibility (0 means infinite). */

    // sprites
    std::unique_ptr<HeroSprites>
        sprites;                           /**< the hero's sprites (note that we don't use the sprites structure from Entity) */

    // position
    int normal_walking_speed;              /**< speed when normally walking */
    int walking_speed;                     /**< current walking speed (possibly changed by the ground) */

    // state specific
    std::shared_ptr<Teletransporter>
        delayed_teletransporter;           /**< a teletransporter that will be activated when the hero finishes
                                            * a special behavior, such as falling into a hole or walking on stairs */
    bool on_raised_blocks;                 /**< indicates that the hero is currently on
                                            * raised crystal blocks */

    // ground
    Point last_solid_ground_coords;        /**< coordinates of the last hero position on a ground
                                            * where he can walk (e.g. before jumping or falling into a hole) */
    int last_solid_ground_layer;           /**< layer of the last hero position on a solid ground */
    ScopedLuaRef
        target_solid_ground_callback;      /**< Function that gives the position where the hero will go back if he falls
                                            * into a hole (or some other bad ground), or an empty ref to indicate
                                            * that the hero will just return to the last solid ground coordinates. */
    uint32_t next_ground_date;             /**< when something will happen with the ground next time (a sound or a movement) */
    uint32_t next_ice_date;                /**< when recomputing the additional movement on ice */
    int ice_movement_direction8;           /**< wanted movement direction a while ago */
    Point ground_dxy;                      /**< additional movement with special ground (hole or ice) */

};

}

#endif

