
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
#ifndef SOLARUS_LUA_CONTEXT_H
#define SOLARUS_LUA_CONTEXT_H

#include "solarus/audio/SoundPtr.h"
#include "solarus/core/Common.h"
#include "solarus/core/Ability.h"
#include "solarus/core/Debug.h"
#include "solarus/core/GameCommands.h"
#include "solarus/core/InputEvent.h"
#include "solarus/core/TimerPtr.h"
#include "solarus/entities/Camera.h"
#include "solarus/entities/EnemyAttack.h"
#include "solarus/entities/EntityPtr.h"
#include "solarus/entities/EntityType.h"
#include "solarus/entities/Ground.h"
#include "solarus/entities/HeroPtr.h"
#include "solarus/graphics/DrawablePtr.h"
#include "solarus/graphics/ShaderPtr.h"
#include "solarus/graphics/SpritePtr.h"
#include "solarus/graphics/SurfacePtr.h"
#include "solarus/lua/ExportableToLuaPtr.h"
#include "solarus/lua/ScopedLuaRef.h"
#include "solarus/lua/LuaTools.h"
#include <lua.hpp>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <queue>
#include <functional>

namespace Solarus {

class Block;
class Camera;
class CarriedObject;
class Chest;
class CircleMovement;
class Color;
class CustomEntity;
class CustomState;
class Destination;
class Destructible;
class Dialog;
class Door;
class Drawable;
class DynamicTile;
class Enemy;
class EnemyReaction;
class Entity;
class EntityData;
class ExportableToLua;
class EquipmentItem;
class Game;
class JumpMovement;
class MainLoop;
class Map;
class Movement;
class Npc;
class PathFindingMovement;
class PathMovement;
class PixelMovement;
class Pickable;
class Point;
class RandomMovement;
class RandomPathMovement;
class Sensor;
class Separator;
class Shader;
class ShopTreasure;
class Sprite;
class StraightMovement;
class Stairs;
class Stream;
class Switch;
class TargetMovement;
class Teletransporter;
class TextSurface;
class Timer;
class Treasure;

class Arguments;

using EntityVector = std::vector<EntityPtr>;

/**
 * \brief This class represents a living Lua context that can execute quest
 * scripts at runtime.
 *
 * Such scripts include the quest main script, map scripts, enemy behaviors,
 * etc. This class implements the Solarus scripting API that scripts can use.
 *
 * All these scripts run in the same Lua context. This means that they share
 * global values and other global mechanisms like the registry and metatables.
 *
 * On the contrary, data files that happen to have a Lua-compatible syntax
 * (like maps) are always parsed in their own, independent Lua states.
 * These files are considered as pure data (not code) and only use the
 * LuaTools class, not this class.
 */
class LuaContext {

  public:

    // Functions and types.
    static const std::string main_module_name;
    static const std::string audio_module_name;
    static const std::string sound_module_name;
    static const std::string video_module_name;
    static const std::string input_module_name;
    static const std::string file_module_name;
    static const std::string timer_module_name;
    static const std::string game_module_name;
    static const std::string map_module_name;
    static const std::string item_module_name;
    static const std::string surface_module_name;
    static const std::string text_surface_module_name;
    static const std::string sprite_module_name;
    static const std::string menu_module_name;
    static const std::string language_module_name;
    static const std::string shader_module_name;
    static const std::string state_module_name;
    static const std::string movement_module_name;
    static const std::string movement_straight_module_name;
    static const std::string movement_random_module_name;
    static const std::string movement_target_module_name;
    static const std::string movement_path_module_name;
    static const std::string movement_random_path_module_name;
    static const std::string movement_path_finding_module_name;
    static const std::string movement_circle_module_name;
    static const std::string movement_jump_module_name;
    static const std::string movement_pixel_module_name;

    explicit LuaContext(MainLoop& main_loop);
    ~LuaContext();

    static LuaContext& get();
    lua_State* get_internal_state();
    lua_State* get_main_state();

    MainLoop& get_main_loop();

    // Main loop from C++.
    void initialize(const Arguments &args);
    void exit();
    void update();
    bool notify_input(const InputEvent& event);
    void notify_map_suspended(Map& map, bool suspended);
    void notify_shop_treasure_interaction(ShopTreasure& shop_treasure);
    void notify_hero_brandish_treasure(
        const Treasure& treasure,
        const ScopedLuaRef& callback_ref
    );
    bool notify_dialog_started(
        Game& game,
        const Dialog& dialog,
        const ScopedLuaRef& info_ref
    );
    void notify_dialog_finished(
        Game& game,
        const Dialog& dialog,
        const ScopedLuaRef& callback_ref,
        const ScopedLuaRef& status_ref
    );
    void run_item(EquipmentItem& item);
    void run_map(Map& map, const std::shared_ptr<Destination>& destination);
    void run_enemy(Enemy& enemy);
    void run_custom_entity(CustomEntity& custom_entity);

    void warning_deprecated(
        const std::pair<int, int>& version_deprecating,
        const std::string& function_name,
        const std::string& message
    );
    static void print_stack(lua_State* current_l);

    // Lua refs.
    ScopedLuaRef create_ref();
    static void push_ref(lua_State* current_l, const ScopedLuaRef& ref);

    // Executing Lua code.
    bool load_file(const std::string& script_name);
    void do_file(const std::string& script_name);
    bool do_file_if_exists(const std::string& script_name);
    bool do_string(const std::string& code, const std::string& chunk_name);
    bool do_string_with_easy_env(const std::string& code, const std::string& chunk_name);

    //TODO put those templates impl in a LuaContext.inl
    //Getting across coroutines state
    template<typename Callable>
    /**
     * @brief run given closure on main lua thread
     * @param current current lua state
     * @param func a void(lua_State* main) closure
     */
    static void run_on_main(Callable&& func) {
      auto& c = LuaContext::get();
      if (c.current_l == c.main_l) {
        func(c.current_l);
      } else {
        c.cross_state_callbacks.push(func);
      }
    }

    template<typename Callable>
    static int state_boundary_handle(lua_State* l, Callable&& func) {
      lua_State* previous = lua_context->get_internal_state();
      lua_context->set_current_state(l);
      int result = LuaTools::exception_boundary_handle(l,func);
      lua_context->set_current_state(previous);
      return result;
    }

    static void set_current_state(lua_State* l);

    // Calling Lua functions.
    bool call_function(
        int nb_arguments,
        int nb_results,
        const char* function_name
    );

    static bool is_solarus_userdata(
        lua_State* current_l,
        int index,
        std::string& module_name
    );
    bool userdata_has_field(
        const ExportableToLua& userdata,
        const char* key
    ) const;
    bool userdata_has_field(
        const ExportableToLua& userdata,
        const std::string& key
    ) const;
    void notify_userdata_destroyed(ExportableToLua& userdata);
    void userdata_close_lua();

    // Timers.
    void add_timer(
        const TimerPtr& timer,
        int context_index,
        const ScopedLuaRef& callback_index
    );
    void remove_timer(const TimerPtr& timer);
    void remove_timers(int context_index);
    void destroy_timers();
    void update_timers();
    void notify_timers_map_suspended(bool suspended);
    void set_entity_timers_suspended_as_map(Entity& entity, bool suspended);
    void do_timer_callback(const TimerPtr& timer);

    // Menus.
    void add_menu(
        const ScopedLuaRef& menu_ref,
        int context_index,
        bool on_top
    );
    void remove_menus(int context_index);
    void remove_menus();
    void destroy_menus();
    void update_menus();

    // Drawable objects.
    bool has_drawable(const DrawablePtr& drawable);
    void add_drawable(const DrawablePtr& drawable);
    void remove_drawable(const DrawablePtr& drawable);
    void destroy_drawables();
    void update_drawables();

    // Movements.
    void start_movement_on_point(
        const std::shared_ptr<Movement>& movement,
        int point_index
    );
    void stop_movement_on_point(const std::shared_ptr<Movement>& movement);
    void update_movements();

    // Maps.
    static void check_map_has_game(lua_State* current_l, const Map& map);

    // Entities.
    static const std::string& get_entity_internal_type_name(EntityType entity_type);
    bool create_map_entity_from_data(Map& map, const EntityData& entity_data);

    void do_entity_draw_override_function(
        const ScopedLuaRef& draw_override,
        Entity& entity,
        Camera& camera
    );
    bool do_traversable_test_function(
        const ScopedLuaRef& traversable_test_ref,
        ExportableToLua& userdata,
        Entity& other_entity
    );
    bool do_custom_entity_collision_test_function(
        const ScopedLuaRef& collision_test_ref,
        CustomEntity& custom_entity,
        Entity& other_entity
    );
    void do_custom_entity_collision_callback(
        const ScopedLuaRef& callback_ref,
        CustomEntity& custom_entity,
        Entity& other_entity
    );
    void do_custom_entity_collision_callback(
        const ScopedLuaRef& callback_ref,
        CustomEntity& custom_entity,
        Entity& other_entity,
        Sprite& custom_entity_sprite,
        Sprite& other_entity_sprite
    );
    void do_state_draw_override_function(
        const ScopedLuaRef& draw_override,
        CustomState& state,
        Camera& camera
    );
    bool do_state_can_be_hurt_function(
        const ScopedLuaRef& can_be_hurt,
        CustomState& state,
        Entity* attacker
    );
    bool do_state_can_cut_function(
        const ScopedLuaRef& can_cut,
        CustomState& state,
        Entity* entity
    );

    // Main loop events (sol.main).
    void main_on_started();
    void main_on_finished();
    void main_on_update();
    void main_on_draw(const SurfacePtr& dst_surface);
    bool main_on_input(const InputEvent& event);

    // Video events.
    void video_on_draw(const SurfacePtr& screen);

    // Menu events.
    void menu_on_started(const ScopedLuaRef& menu_ref);
    void menu_on_finished(const ScopedLuaRef& menu_ref);
    void menu_on_update(const ScopedLuaRef& menu_ref);
    void menu_on_draw(const ScopedLuaRef& menu_ref, const SurfacePtr& dst_surface);
    bool menu_on_input(const ScopedLuaRef& menu_ref, const InputEvent& event);
    bool menu_on_command_pressed(
        const ScopedLuaRef& menu_ref,
        GameCommand command
    );
    bool menu_on_command_released(
        const ScopedLuaRef& menu_ref,
        GameCommand command
    );
    void menus_on_update(int context_index);
    void menus_on_draw(int context_index, const SurfacePtr& dst_surface);
    bool menus_on_input(int context_index, const InputEvent& event);
    bool menus_on_command_pressed(int context_index, GameCommand command);
    bool menus_on_command_released(int context_index, GameCommand command);

    // Sprite events.
    void sprite_on_animation_finished(
        Sprite& sprite, const std::string& animation);
    void sprite_on_animation_changed(
        Sprite& sprite, const std::string& animation);
    void sprite_on_direction_changed(
        Sprite& sprite, const std::string& animation, int direction);
    void sprite_on_frame_changed(
        Sprite& sprite, const std::string& animation, int frame);

    // Movement events.
    void movement_on_position_changed(Movement& movement, const Point& xy);
    void movement_on_obstacle_reached(Movement& movement);
    void movement_on_changed(Movement& movement);
    void movement_on_finished(Movement& movement);

    // Equipment item events.
    void item_on_created(EquipmentItem& item);
    void item_on_started(EquipmentItem& item);
    void item_on_finished(EquipmentItem& item);
    void item_on_update(EquipmentItem& item);
    void item_on_suspended(EquipmentItem& item, bool suspended);
    void item_on_map_changed(EquipmentItem& item, Map& map);
    void item_on_pickable_created(EquipmentItem& item, Pickable& pickable);
    void item_on_obtaining(EquipmentItem& item, const Treasure& treasure);
    void item_on_obtained(EquipmentItem& item, const Treasure& treasure);
    void item_on_variant_changed(EquipmentItem& item, int variant);
    void item_on_amount_changed(EquipmentItem& item, int amount);
    void item_on_using(EquipmentItem& item);
    void item_on_ability_used(EquipmentItem& item, Ability ability);
    void item_on_npc_interaction(EquipmentItem& item, Npc& npc);
    bool item_on_npc_interaction_item(EquipmentItem& item, Npc& npc,
        EquipmentItem& item_used);
    void item_on_npc_collision_fire(EquipmentItem& item, Npc& npc);

    // Game events.
    void game_on_started(Game& game);
    void game_on_finished(Game& game);
    void game_on_update(Game& game);
    void game_on_draw(Game& game, const SurfacePtr& dst_surface);
    void game_on_map_changed(Game& game, Map& map);
    void game_on_world_changed(
        Game& game,
        const std::string& previous_world,
        const std::string& new_world
    );
    void game_on_paused(Game& game);
    void game_on_unpaused(Game& game);
    bool game_on_dialog_started(
        Game& game,
        const Dialog& dialog,
        const ScopedLuaRef& info_ref
    );
    void game_on_dialog_finished(Game& game, const Dialog& dialog);
    bool game_on_game_over_started(Game& game);
    void game_on_game_over_finished(Game& game);
    bool game_on_input(Game& game, const InputEvent& event);
    bool game_on_command_pressed(Game& game, GameCommand command);
    bool game_on_command_released(Game& game, GameCommand command);

    // Map events.
    void map_on_started(Map& map, const std::shared_ptr<Destination>& destination);
    void map_on_finished(Map& map);
    void map_on_update(Map& map);
    void map_on_draw(Map& map, const SurfacePtr& dst_surface);
    void map_on_suspended(Map& map, bool suspended);
    void map_on_opening_transition_finished(Map& map,
        const std::shared_ptr<Destination>& destination);
    void map_on_obtaining_treasure(Map& map, const Treasure& treasure);
    void map_on_obtained_treasure(Map& map, const Treasure& treasure);
    bool map_on_input(Map& map, const InputEvent& event);
    bool map_on_command_pressed(Map& map, GameCommand command);
    bool map_on_command_released(Map& map, GameCommand command);

    // Map entity events.
    void entity_on_update(Entity& entity);
    void entity_on_suspended(Entity& entity, bool suspended);
    void entity_on_created(Entity& entity);
    void entity_on_removed(Entity& entity);
    void entity_on_enabled(Entity& entity);
    void entity_on_disabled(Entity& entity);
    void entity_on_pre_draw(Entity& entity, Camera& camera);
    void entity_on_post_draw(Entity& entity, Camera& camera);
    void entity_on_position_changed(Entity& entity, const Point& xy, int layer);
    void entity_on_obstacle_reached(Entity& entity, Movement& movement);
    void entity_on_movement_started(Entity& entity, Movement& movement);
    void entity_on_movement_changed(Entity& entity, Movement& movement);
    void entity_on_movement_finished(Entity& entity);
    bool entity_on_interaction(Entity& entity);
    bool entity_on_interaction_item(Entity& entity, EquipmentItem& item_used);
    void entity_on_state_changing(
        Entity& entity,
        const std::string& state_name,
        const std::string& next_state_name);
    void entity_on_state_changed(Entity& entity, const std::string& new_state_name);
    void entity_on_lifting(
        Entity& entity,
        Entity& carrier,
        CarriedObject& carried_object);
    bool hero_on_taking_damage(Hero& hero, int damage);
    void destination_on_activated(Destination& destination);
    void teletransporter_on_activated(Teletransporter& teletransporter);
    void npc_on_collision_fire(Npc& npc);
    void carried_object_on_lifted(CarriedObject& carried_object);
    void carried_object_on_thrown(CarriedObject& carried_object);
    void carried_object_on_breaking(CarriedObject& carried_object);
    bool chest_on_opened(Chest& chest, const Treasure& treasure);
    void block_on_moving(Block& block);
    void block_on_moved(Block& block);
    void switch_on_activated(Switch& sw);
    void switch_on_inactivated(Switch& sw);
    void switch_on_left(Switch& sw);
    void sensor_on_activated(Sensor& sensor);
    void sensor_on_activated_repeat(Sensor& sensor);
    void sensor_on_left(Sensor& sensor);
    void sensor_on_collision_explosion(Sensor& sensor);
    void separator_on_activating(Separator& separator, int direction4);
    void separator_on_activated(Separator& separator, int direction4);
    void door_on_opened(Door& door);
    void door_on_closed(Door& door);
    bool shop_treasure_on_buying(ShopTreasure& shop_treasure);
    void shop_treasure_on_bought(ShopTreasure& shop_treasure);
    void destructible_on_looked(Destructible& destructible);
    void destructible_on_cut(Destructible& destructible);
    void destructible_on_exploded(Destructible& destructible);
    void destructible_on_regenerating(Destructible& destructible);
    void enemy_on_restarted(Enemy& enemy);
    void enemy_on_collision_enemy(Enemy& enemy,
        Enemy& other_enemy, Sprite& other_sprite, Sprite& this_sprite);
    void enemy_on_custom_attack_received(Enemy& enemy,
        EnemyAttack attack, Sprite* sprite);
    bool enemy_on_hurt_by_sword(Enemy& enemy, Hero& hero, Sprite& enemy_sprite);
    void enemy_on_hurt(Enemy& enemy, EnemyAttack attack);
    void enemy_on_dying(Enemy& enemy);
    void enemy_on_dead(Enemy& enemy);
    void enemy_on_immobilized(Enemy& enemy);
    bool enemy_on_attacking_hero(Enemy& enemy, Hero& hero, Sprite* enemy_sprite);
    void custom_entity_on_ground_below_changed(
        CustomEntity& custom_entity, Ground ground_below);
    void state_on_started(
        CustomState& state,
        const std::string& previous_state_name,
        CustomState* previous_state);
    void state_on_finished(
        CustomState& state,
        const std::string& next_state_name,
        CustomState* next_state);
    void state_on_update(CustomState& state);
    void state_on_suspended(CustomState& state, bool suspended);
    void state_on_pre_draw(CustomState& state, Camera& camera);
    void state_on_post_draw(CustomState& state, Camera& camera);
    void state_on_map_started(CustomState& state, Map& map, const std::shared_ptr<Destination>& destination);
    void state_on_map_opening_transition_finished(CustomState& state, Map& map, const std::shared_ptr<Destination>& destination);
    void state_on_map_finished(CustomState& state);
    void state_on_position_changed(CustomState& state, const Point& xy, int layer);
    void state_on_ground_below_changed(CustomState& state, Ground ground_below);
    void state_on_obstacle_reached(CustomState& state, Movement& movement);
    void state_on_movement_started(CustomState& state, Movement& movement);
    void state_on_movement_changed(CustomState& state, Movement& movement);
    void state_on_movement_finished(CustomState& state);
    void state_on_attacked_enemy(
        CustomState& state,
        Enemy& enemy,
        Sprite* enemy_sprite,
        EnemyAttack attack,
        const EnemyReaction::Reaction& reaction
    );
    bool state_on_input(CustomState& state, const InputEvent& event);
    bool state_on_command_pressed(CustomState& state, GameCommand command);
    bool state_on_command_released(CustomState& state, GameCommand command);

    // Implementation of the API.

    /**
     * \brief Type of the functions that can be called by Lua.
     */
    using FunctionExportedToLua = int(lua_State* current_l);

    // All functions named <type>_api_<name> can be called by Lua.
    static FunctionExportedToLua

      // Main API.
      main_api_get_solarus_version,
      main_api_get_quest_version,
      main_api_get_quest_format,
      main_api_load_file,
      main_api_do_file,
      main_api_reset,
      main_api_exit,
      main_api_get_elapsed_time,
      main_api_get_quest_write_dir,
      main_api_set_quest_write_dir,
      main_api_load_settings,
      main_api_save_settings,
      main_api_get_distance,
      main_api_get_angle,
      main_api_get_resource_ids,
      main_api_resource_exists,
      main_api_get_resource_description,
      main_api_add_resource,
      main_api_remove_resource,
      main_api_get_type,
      main_api_get_metatable,
      main_api_get_os,
      main_api_get_game,
      main_api_rawget,
      main_api_rawset,

      // Audio API.
      audio_api_get_sound_volume,
      audio_api_set_sound_volume,
      audio_api_play_sound,
      audio_api_preload_sounds,
      audio_api_get_music_volume,
      audio_api_set_music_volume,
      audio_api_play_music,
      audio_api_stop_music,
      audio_api_get_music,
      audio_api_get_music_format,
      audio_api_get_music_num_channels,
      audio_api_get_music_channel_volume,
      audio_api_set_music_channel_volume,
      audio_api_get_music_tempo,
      audio_api_set_music_tempo,

      // Sound API.
      sound_api_create,
      sound_api_play,
      sound_api_stop,

      // Video API.
      video_api_get_window_title,
      video_api_set_window_title,
      video_api_get_mode,
      video_api_set_mode,
      video_api_switch_mode,
      video_api_is_mode_supported,
      video_api_get_modes,
      video_api_is_fullscreen,
      video_api_set_fullscreen,
      video_api_is_cursor_visible,
      video_api_set_cursor_visible,
      video_api_get_quest_size,
      video_api_get_window_size,
      video_api_set_window_size,
      video_api_reset_window_size,
      video_api_get_shader,
      video_api_set_shader,

      // Input API.
      input_api_is_joypad_enabled,
      input_api_set_joypad_enabled,
      input_api_is_key_pressed,
      input_api_get_key_modifiers,
      input_api_is_joypad_button_pressed,
      input_api_get_joypad_axis_state,
      input_api_get_joypad_hat_direction,
      input_api_is_mouse_button_pressed,
      input_api_get_mouse_position,
      input_api_is_finger_pressed,
      input_api_get_finger_position,
      input_api_get_finger_pressure,
      input_api_simulate_key_pressed,
      input_api_simulate_key_released,

      // File API.
      file_api_open,
      file_api_exists,
      file_api_remove,
      file_api_mkdir,
      file_api_is_dir,
      file_api_list_dir,

      // Menu API.
      menu_api_start,
      menu_api_stop,
      menu_api_stop_all,
      menu_api_is_started,
      menu_api_bring_to_front,
      menu_api_bring_to_back,

      // Timer API.
      timer_api_start,
      timer_api_stop,
      timer_api_stop_all,
      timer_api_is_with_sound,
      timer_api_set_with_sound,
      timer_api_is_suspended,
      timer_api_set_suspended,
      timer_api_is_suspended_with_map,
      timer_api_set_suspended_with_map,
      timer_api_get_remaining_time,
      timer_api_set_remaining_time,
      // TODO deprecate is_with_sound, set_with_sound (do this in pure Lua, possibly with a second timer)

      // Language API.
      language_api_get_language,
      language_api_set_language,
      language_api_get_language_name,
      language_api_get_languages,
      language_api_get_string,
      language_api_get_dialog,

      // Drawable API (i.e. common to surfaces, text surfaces and sprites).
      drawable_api_draw,
      drawable_api_draw_region,
      drawable_api_get_blend_mode,
      drawable_api_set_blend_mode,
      drawable_api_get_shader,
      drawable_api_set_shader,
      drawable_api_get_color_modulation,
      drawable_api_set_color_modulation,
      drawable_api_get_opacity,
      drawable_api_set_opacity,
      drawable_api_fade_in,
      drawable_api_fade_out,
      drawable_api_get_xy,
      drawable_api_set_xy,
      drawable_api_get_rotation,
      drawable_api_set_rotation,
      drawable_api_get_scale,
      drawable_api_set_scale,
      drawable_api_get_transformation_origin,
      drawable_api_set_transformation_origin,
      drawable_api_get_movement,
      drawable_api_stop_movement,
      drawable_meta_gc,

      // Surface API.
      surface_api_create,
      surface_api_get_size,
      surface_api_clear,
      surface_api_fill_color,
      surface_api_get_opacity,
      surface_api_set_opacity,
      surface_api_get_pixels,
      surface_api_set_pixels,
      surface_api_gl_bind_as_texture,
      surface_api_gl_bind_as_target,

      // Text surface API.
      text_surface_api_create,
      text_surface_api_get_predicted_size,
      text_surface_api_get_horizontal_alignment,
      text_surface_api_set_horizontal_alignment,
      text_surface_api_get_vertical_alignment,
      text_surface_api_set_vertical_alignment,
      text_surface_api_get_font,
      text_surface_api_set_font,
      text_surface_api_get_rendering_mode,
      text_surface_api_set_rendering_mode,
      text_surface_api_get_color,
      text_surface_api_set_color,
      text_surface_api_get_font_size,
      text_surface_api_set_font_size,
      text_surface_api_get_text,
      text_surface_api_set_text,
      text_surface_api_set_text_key,
      text_surface_api_get_size,

      // Sprite API.
      sprite_api_create,
      sprite_api_get_animation_set,
      sprite_api_has_animation,
      sprite_api_get_animation,
      sprite_api_set_animation,
      sprite_api_stop_animation,
      sprite_api_is_animation_started,
      sprite_api_get_direction,
      sprite_api_set_direction,
      sprite_api_get_num_directions,
      sprite_api_get_frame,
      sprite_api_set_frame,
      sprite_api_get_num_frames,
      sprite_api_get_frame_delay,
      sprite_api_set_frame_delay,
      sprite_api_get_size,
      sprite_api_get_origin,
      sprite_api_get_frame_src_xy,
      sprite_api_is_paused,
      sprite_api_set_paused,
      sprite_api_get_ignore_suspend,
      sprite_api_set_ignore_suspend,
      sprite_api_synchronize,

      // Shader API.
      shader_api_create,
      shader_api_get_opengl_version,
      shader_api_get_shading_language_version,
      shader_api_get_id,
      shader_api_get_vertex_file,
      shader_api_get_vertex_source,
      shader_api_get_fragment_file,
      shader_api_get_fragment_source,
      shader_api_get_scaling_factor,
      shader_api_set_scaling_factor,
      shader_api_set_uniform,

      // Movement API.
      movement_api_create,
      movement_api_get_xy,
      movement_api_set_xy,
      movement_api_is_suspended,
      movement_api_get_ignore_suspend,
      movement_api_set_ignore_suspend,
      movement_api_get_ignore_obstacles,
      movement_api_set_ignore_obstacles,
      movement_api_start,
      movement_api_stop,
      movement_api_get_direction4,
      straight_movement_api_get_speed,
      straight_movement_api_set_speed,
      straight_movement_api_get_angle,
      straight_movement_api_set_angle,
      straight_movement_api_get_max_distance,
      straight_movement_api_set_max_distance,
      straight_movement_api_is_smooth,
      straight_movement_api_set_smooth,
      random_movement_api_get_speed,
      random_movement_api_set_speed,
      random_movement_api_get_angle,
      random_movement_api_get_max_distance,
      random_movement_api_set_max_distance,
      random_movement_api_is_smooth,
      random_movement_api_set_smooth,
      target_movement_api_set_target,
      target_movement_api_get_speed,
      target_movement_api_set_speed,
      target_movement_api_get_angle,
      target_movement_api_is_smooth,
      target_movement_api_set_smooth,
      path_movement_api_get_path,
      path_movement_api_set_path,
      path_movement_api_get_speed,
      path_movement_api_set_speed,
      path_movement_api_get_angle,
      path_movement_api_get_loop,
      path_movement_api_set_loop,
      path_movement_api_get_snap_to_grid,
      path_movement_api_set_snap_to_grid,
      random_path_movement_api_get_speed,
      random_path_movement_api_set_speed,
      random_path_movement_api_get_angle,
      path_finding_movement_api_set_target,
      path_finding_movement_api_get_speed,
      path_finding_movement_api_set_speed,
      path_finding_movement_api_get_angle,
      circle_movement_api_get_center,
      circle_movement_api_set_center,
      circle_movement_api_get_radius,
      circle_movement_api_set_radius,
      circle_movement_api_get_radius_speed,
      circle_movement_api_set_radius_speed,
      circle_movement_api_is_clockwise,
      circle_movement_api_set_clockwise,
      circle_movement_api_get_angle_from_center,
      circle_movement_api_set_angle_from_center,
      circle_movement_api_get_initial_angle,
      circle_movement_api_set_initial_angle,
      circle_movement_api_get_angular_speed,
      circle_movement_api_set_angular_speed,
      circle_movement_api_get_angle_speed,
      circle_movement_api_set_angle_speed,
      circle_movement_api_get_max_rotations,
      circle_movement_api_set_max_rotations,
      circle_movement_api_get_duration,
      circle_movement_api_set_duration,
      circle_movement_api_get_loop_delay,
      circle_movement_api_set_loop_delay,
      jump_movement_api_get_direction8,
      jump_movement_api_set_direction8,
      jump_movement_api_get_distance,
      jump_movement_api_set_distance,
      jump_movement_api_get_speed,
      jump_movement_api_set_speed,
      pixel_movement_api_get_trajectory,
      pixel_movement_api_set_trajectory,
      pixel_movement_api_get_loop,
      pixel_movement_api_set_loop,
      pixel_movement_api_get_delay,
      pixel_movement_api_set_delay,

      // Game API.
      game_api_exists,
      game_api_delete,
      game_api_load,
      game_api_save,  // TODO allow to change the file name (e.g. to copy)
      game_api_start,
      game_api_is_started,
      game_api_is_suspended,
      game_api_set_suspended,
      game_api_is_paused,
      game_api_set_paused,
      game_api_is_pause_allowed,
      game_api_set_pause_allowed,
      game_api_is_dialog_enabled,
      game_api_start_dialog,
      game_api_stop_dialog,
      game_api_is_game_over_enabled,
      game_api_start_game_over,
      game_api_stop_game_over,
      game_api_get_map,
      game_api_get_hero,
      game_api_get_value,
      game_api_set_value,
      game_api_get_starting_location,
      game_api_set_starting_location,
      game_api_get_transition_style,
      game_api_set_transition_style,
      game_api_get_life,
      game_api_set_life,
      game_api_add_life,
      game_api_remove_life,
      game_api_get_max_life,
      game_api_set_max_life,
      game_api_add_max_life,
      game_api_get_money,
      game_api_set_money,
      game_api_add_money,
      game_api_remove_money,
      game_api_get_max_money,
      game_api_set_max_money,
      game_api_get_magic,
      game_api_set_magic,
      game_api_add_magic,
      game_api_remove_magic,
      game_api_get_max_magic,
      game_api_set_max_magic,
      game_api_has_ability,
      game_api_get_ability,
      game_api_set_ability,
      game_api_get_item,
      game_api_has_item,
      game_api_get_item_assigned,
      game_api_set_item_assigned,
      game_api_is_command_pressed,
      game_api_get_commands_direction,
      game_api_get_command_effect,  // TODO also return "run" for action command
      game_api_get_command_keyboard_binding,
      game_api_set_command_keyboard_binding,
      game_api_get_command_joypad_binding,
      game_api_set_command_joypad_binding,
      game_api_capture_command_binding,
      game_api_simulate_command_pressed,
      game_api_simulate_command_released,

      // Equipment item API.
      item_api_get_name,
      item_api_get_game,
      item_api_get_map,
      item_api_get_savegame_variable,
      item_api_set_savegame_variable,
      item_api_get_amount_savegame_variable,
      item_api_set_amount_savegame_variable,
      item_api_is_being_used,
      item_api_is_obtainable,
      item_api_set_obtainable,
      item_api_is_assignable,   // TODO remove
      item_api_set_assignable,  // TODO remove
      item_api_get_can_disappear,
      item_api_set_can_disappear,
      item_api_get_brandish_when_picked,
      item_api_set_brandish_when_picked,
      item_api_get_shadow,
      item_api_set_shadow,
      item_api_get_sound_when_picked,
      item_api_set_sound_when_picked,
      item_api_get_sound_when_brandished,
      item_api_set_sound_when_brandished,
      item_api_has_variant,
      item_api_get_variant,
      item_api_set_variant,
      item_api_has_amount,
      item_api_get_amount,
      item_api_set_amount,
      item_api_add_amount,
      item_api_remove_amount,
      item_api_get_max_amount,
      item_api_set_max_amount,
      item_api_set_finished,

      // Map API.
      map_api_get_id,
      map_api_get_game,
      map_api_get_world,
      map_api_set_world,
      map_api_get_floor,
      map_api_set_floor,
      map_api_get_min_layer,
      map_api_get_max_layer,
      map_api_get_size,
      map_api_get_location,
      map_api_get_tileset,
      map_api_set_tileset,
      map_api_get_music,
      map_api_get_camera,
      map_api_get_camera_position,
      map_api_move_camera,
      map_api_get_ground,
      map_api_draw_visual,
      map_api_draw_sprite,
      map_api_get_crystal_state,
      map_api_set_crystal_state,
      map_api_change_crystal_state,
      map_api_open_doors,
      map_api_close_doors,
      map_api_set_doors_open,
      map_api_get_entity,
      map_api_has_entity,
      map_api_get_entities,
      map_api_get_entities_count,
      map_api_has_entities,
      map_api_get_entities_by_type,
      map_api_get_entities_in_rectangle,
      map_api_get_entities_in_region,
      map_api_get_hero,
      map_api_set_entities_enabled,
      map_api_remove_entities,
      map_api_create_entity,  // Same function used for all entity types.

      // Map entity API.
      entity_api_get_type,
      entity_api_get_map,
      entity_api_get_game,
      entity_api_get_name,
      entity_api_exists,
      entity_api_remove,
      entity_api_is_enabled,
      entity_api_set_enabled,
      entity_api_get_size,
      entity_api_set_size,
      entity_api_get_origin,
      entity_api_set_origin,
      entity_api_get_position,
      entity_api_set_position,
      entity_api_get_center_position,
      entity_api_get_facing_position,
      entity_api_get_facing_entity,
      entity_api_get_ground_position,
      entity_api_get_ground_below,
      entity_api_get_bounding_box,
      entity_api_get_max_bounding_box,
      entity_api_get_layer,
      entity_api_set_layer,
      entity_api_overlaps,
      entity_api_get_distance,
      entity_api_get_angle,
      entity_api_get_direction4_to,
      entity_api_get_direction8_to,
      entity_api_bring_to_front,
      entity_api_bring_to_back,
      entity_api_is_drawn_in_y_order,
      entity_api_set_drawn_in_y_order,
      entity_api_snap_to_grid,
      entity_api_get_sprite,
      entity_api_get_sprites,
      entity_api_create_sprite,
      entity_api_remove_sprite,
      entity_api_bring_sprite_to_front,
      entity_api_bring_sprite_to_back,
      entity_api_is_visible,
      entity_api_set_visible,
      entity_api_get_draw_override,
      entity_api_set_draw_override,
      entity_api_get_weight,
      entity_api_set_weight,
      entity_api_get_controlling_stream,
      entity_api_get_movement,
      entity_api_stop_movement,
      entity_api_has_layer_independent_collisions,
      entity_api_set_layer_independent_collisions,
      entity_api_test_obstacles,
      entity_api_get_optimization_distance,
      entity_api_set_optimization_distance,
      entity_api_is_in_same_region,
      entity_api_get_state,
      entity_api_get_property,
      entity_api_set_property,
      entity_api_get_properties,
      entity_api_set_properties,
      hero_api_teleport,
      hero_api_get_direction,
      hero_api_set_direction,
      hero_api_get_walking_speed,
      hero_api_set_walking_speed,
      hero_api_save_solid_ground,
      hero_api_reset_solid_ground,
      hero_api_get_solid_ground_position,
      hero_api_get_animation,
      hero_api_set_animation,
      hero_api_get_tunic_sprite_id,
      hero_api_set_tunic_sprite_id,
      hero_api_get_sword_sprite_id,
      hero_api_set_sword_sprite_id,
      hero_api_get_sword_sound_id,
      hero_api_set_sword_sound_id,
      hero_api_get_shield_sprite_id,
      hero_api_set_shield_sprite_id,
      hero_api_is_blinking,
      hero_api_set_blinking,
      hero_api_is_invincible,
      hero_api_set_invincible,
      hero_api_get_carried_object,
      hero_api_freeze,
      hero_api_unfreeze,
      hero_api_walk,
      hero_api_start_attack,
      hero_api_start_attack_loading,
      hero_api_start_item,
      hero_api_start_grabbing,
      hero_api_start_jumping,
      hero_api_start_treasure,
      hero_api_start_victory,
      hero_api_start_boomerang,
      hero_api_start_bow,
      hero_api_start_hookshot,
      hero_api_start_running,
      hero_api_start_hurt,
      hero_api_start_state,
      hero_api_get_state_object,
      camera_api_get_position_on_screen,
      camera_api_set_position_on_screen,
      camera_api_get_state,
      camera_api_start_tracking,
      camera_api_start_manual,
      camera_api_get_position_to_track,
      camera_api_get_tracked_entity,
      camera_api_get_surface,
      destination_api_get_starting_location_mode,
      destination_api_set_starting_location_mode,
      teletransporter_api_get_sound,
      teletransporter_api_set_sound,
      teletransporter_api_get_transition,
      teletransporter_api_set_transition,
      teletransporter_api_get_destination_map,
      teletransporter_api_set_destination_map,
      teletransporter_api_get_destination_name,
      teletransporter_api_set_destination_name,
      npc_api_is_traversable,
      npc_api_set_traversable,
      chest_api_is_open,
      chest_api_set_open,
      chest_api_get_treasure,
      chest_api_set_treasure,
      block_api_reset,
      block_api_is_pushable,
      block_api_set_pushable,
      block_api_is_pullable,
      block_api_set_pullable,
      block_api_get_max_moves,
      block_api_set_max_moves,
      block_api_get_maximum_moves,
      block_api_set_maximum_moves,
      switch_api_is_activated,
      switch_api_set_activated,
      switch_api_is_locked,
      switch_api_set_locked,
      switch_api_is_walkable,
      stream_api_get_direction,
      stream_api_set_direction,
      stream_api_get_speed,
      stream_api_set_speed,
      stream_api_get_allow_movement,
      stream_api_set_allow_movement,
      stream_api_get_allow_attack,
      stream_api_set_allow_attack,
      stream_api_get_allow_item,
      stream_api_set_allow_item,
      door_api_is_open,
      door_api_is_opening,
      door_api_is_closed,
      door_api_is_closing,
      door_api_open,
      door_api_close,
      door_api_set_open,
      stairs_api_get_direction,
      stairs_api_is_inner,
      pickable_api_get_followed_entity,
      pickable_api_get_falling_height,
      pickable_api_get_treasure,
      destructible_api_get_treasure,
      destructible_api_set_treasure,
      destructible_api_get_destruction_sound,
      destructible_api_set_destruction_sound,
      destructible_api_get_can_be_cut,
      destructible_api_set_can_be_cut,
      destructible_api_get_can_explode,
      destructible_api_set_can_explode,
      destructible_api_get_can_regenerate,
      destructible_api_set_can_regenerate,
      destructible_api_get_damage_on_enemies,
      destructible_api_set_damage_on_enemies,
      destructible_api_get_modified_ground,
      dynamic_tile_api_get_pattern_id,
      dynamic_tile_api_get_modified_ground,
      dynamic_tile_api_get_tileset,
      dynamic_tile_api_set_tileset,
      carried_object_api_get_carrier,
      carried_object_api_get_destruction_sound,
      carried_object_api_set_destruction_sound,
      carried_object_api_get_damage_on_enemies,
      carried_object_api_set_damage_on_enemies,
      enemy_api_get_breed,
      enemy_api_get_life,
      enemy_api_set_life,
      enemy_api_add_life,
      enemy_api_remove_life,
      enemy_api_get_damage,
      enemy_api_set_damage,
      enemy_api_is_pushed_back_when_hurt,
      enemy_api_set_pushed_back_when_hurt,
      enemy_api_get_push_hero_on_sword,
      enemy_api_set_push_hero_on_sword,
      enemy_api_get_can_hurt_hero_running,
      enemy_api_set_can_hurt_hero_running,
      enemy_api_get_hurt_style,
      enemy_api_set_hurt_style,
      enemy_api_get_dying_sprite_id,
      enemy_api_set_dying_sprite_id,
      enemy_api_get_can_attack,
      enemy_api_set_can_attack,
      enemy_api_get_minimum_shield_needed,
      enemy_api_set_minimum_shield_needed,
      enemy_api_get_attack_consequence,
      enemy_api_set_attack_consequence,
      enemy_api_get_attack_consequence_sprite,
      enemy_api_set_attack_consequence_sprite,
      enemy_api_set_default_attack_consequences,
      enemy_api_set_default_attack_consequences_sprite,
      enemy_api_set_invincible,
      enemy_api_set_invincible_sprite,
      enemy_api_get_treasure,
      enemy_api_set_treasure,
      enemy_api_is_traversable,
      enemy_api_set_traversable,
      enemy_api_get_attacking_collision_mode,
      enemy_api_set_attacking_collision_mode,
      enemy_api_get_obstacle_behavior,
      enemy_api_set_obstacle_behavior,
      enemy_api_restart,
      enemy_api_hurt,
      enemy_api_is_immobilized,
      enemy_api_immobilize,
      enemy_api_create_enemy,
      custom_entity_api_get_model,
      custom_entity_api_get_direction,
      custom_entity_api_set_direction,
      custom_entity_api_is_tiled,
      custom_entity_api_set_tiled,
      custom_entity_api_set_traversable_by,
      custom_entity_api_set_can_traverse,
      custom_entity_api_can_traverse_ground,
      custom_entity_api_set_can_traverse_ground,
      custom_entity_api_add_collision_test,
      custom_entity_api_clear_collision_tests,
      custom_entity_api_get_modified_ground,
      custom_entity_api_set_modified_ground,
      custom_entity_api_get_follow_streams,
      custom_entity_api_set_follow_streams,

      // State API.
      state_api_create,
      state_api_get_description,
      state_api_set_description,
      state_api_get_entity,
      state_api_get_map,
      state_api_get_game,
      state_api_is_started,
      state_api_is_visible,
      state_api_set_visible,
      state_api_get_draw_override,
      state_api_set_draw_override,
      state_api_get_can_control_direction,
      state_api_set_can_control_direction,
      state_api_get_can_control_movement,
      state_api_set_can_control_movement,
      state_api_set_can_traverse,
      state_api_get_can_traverse_ground,
      state_api_set_can_traverse_ground,
      state_api_is_gravity_enabled,
      state_api_set_gravity_enabled,
      state_api_is_affected_by_ground,
      state_api_set_affected_by_ground,
      state_api_get_can_come_from_bad_ground,
      state_api_set_can_come_from_bad_ground,
      state_api_get_can_be_hurt,
      state_api_set_can_be_hurt,
      state_api_get_can_use_sword,
      state_api_set_can_use_sword,
      state_api_get_can_cut,
      state_api_set_can_cut,
      state_api_get_can_use_shield,
      state_api_set_can_use_shield,
      state_api_get_can_use_item,
      state_api_set_can_use_item,
      state_api_get_can_interact,
      state_api_set_can_interact,
      state_api_get_can_grab,
      state_api_set_can_grab,
      state_api_get_can_push,
      state_api_set_can_push,
      state_api_get_pushing_delay,
      state_api_set_pushing_delay,
      state_api_get_can_pick_treasure,
      state_api_set_can_pick_treasure,
      state_api_get_can_use_teletransporter,
      state_api_set_can_use_teletransporter,
      state_api_get_can_use_switch,
      state_api_set_can_use_switch,
      state_api_get_can_use_stream,
      state_api_set_can_use_stream,
      state_api_get_can_use_stairs,
      state_api_set_can_use_stairs,
      state_api_get_can_use_jumper,
      state_api_set_can_use_jumper,
      state_api_get_jumper_delay,
      state_api_set_jumper_delay,
      state_api_get_carried_object_action,
      state_api_set_carried_object_action,

      // available to all userdata types
      userdata_rawget_as_table,
      userdata_meta_gc,
      userdata_meta_newindex_as_table,
      userdata_meta_index_as_table,

      // Lua backtrace error function
      l_backtrace;

  private:

    /**
     * \brief Data associated to any Lua menu.
     */
    struct LuaMenuData {
      ScopedLuaRef ref;      /**< Lua ref of the table of the menu.
                              * LUA_REFNIL means that the menu will be removed. */
      ScopedLuaRef context;   /**< Lua table or userdata the menu is attached to. */
      bool recently_added;   /**< Used to avoid elements added during an iteration. */

      LuaMenuData(const ScopedLuaRef& ref, const ScopedLuaRef& context):
        ref(ref),
        context(context),
        recently_added(true) {
      }
    };

    /**
     * \brief Data associated to any Lua timer.
     */
    struct LuaTimerData {
      ScopedLuaRef callback_ref;  /**< Lua ref of the function to call after the timer. */
      ScopedLuaRef context;       /**< Lua table or userdata the timer is attached to. */
    };

    // Executing Lua code.
    bool userdata_has_metafield(
        const ExportableToLua& userdata, const char* key) const;
    bool find_method(int index, const char* function_name);
    bool find_method(const char* function_name);
    void print_lua_version();

    // Initialization of modules.
    void register_functions(
        const std::string& module_name,
        std::vector<luaL_Reg> functions
    );
    void register_type(
        const std::string& module_name,
        std::vector<luaL_Reg> functions,
        std::vector<luaL_Reg> methods,
        std::vector<luaL_Reg> metamethods
    );
    void register_modules();
    void register_main_module();
    void register_audio_module();
    void register_sound_module();
    void register_video_module();
    void register_input_module();
    void register_file_module();
    void register_timer_module();
    void register_item_module();
    void register_surface_module();
    void register_text_surface_module();
    void register_sprite_module();
    void register_shader_module();
    void register_movement_module();
    void register_menu_module();
    void register_language_module();
    void register_game_module();
    void register_map_module();
    void register_entity_module();
    void register_state_module();

    // Pushing objects to Lua.
    static void push_main(lua_State* current_l);
    static void push_video(lua_State* current_l);
    static void push_string(lua_State* current_l, const std::string& text);
    static void push_color(lua_State* current_l, const Color& color);
public:
    static void push_userdata(lua_State* current_l, ExportableToLua& userdata);
private:
    static void push_dialog(lua_State* current_l, const Dialog& dialog);
    static void push_timer(lua_State* current_l, const TimerPtr& timer);
    static void push_sound(lua_State* current_l, Sound& sound);
    static void push_surface(lua_State* current_l, Surface& surface);
    static void push_text_surface(lua_State* current_l, TextSurface& text_surface);
    static void push_sprite(lua_State* current_l, Sprite& sprite);
    static void push_shader(lua_State* current_l, Shader& shader);
    static void push_item(lua_State* current_l, EquipmentItem& item);
    static void push_movement(lua_State* current_l, Movement& movement);
    static void push_game(lua_State* current_l, Savegame& game);
    static void push_map(lua_State* current_l, Map& map);
    static void push_state(lua_State* current_l, CustomState& state);
    static void push_entity(lua_State* current_l, Entity& entity);
    static void push_entity_iterator(lua_State* current_l, const EntityVector& entities);
    static void push_named_sprite_iterator(
        lua_State* current_l,
        const std::vector<Entity::NamedSprite>& sprites
    );
    static void push_hero(lua_State* current_l, Hero& hero);
    static void push_camera(lua_State* current_l, Camera& camera);
    static void push_npc(lua_State* current_l, Npc& npc);
    static void push_destination(lua_State* current_l, Destination& destination);
    static void push_teletransporter(lua_State* current_l, Teletransporter& teletransporter);
    static void push_chest(lua_State* current_l, Chest& chest);
    static void push_block(lua_State* current_l, Block& block);
    static void push_switch(lua_State* current_l, Switch& sw);
    static void push_stream(lua_State* current_l, Stream& stream);
    static void push_door(lua_State* current_l, Door& door);
    static void push_stairs(lua_State* current_l, Stairs& stairs);
    static void push_shop_treasure(lua_State* current_l, ShopTreasure& shop_treasure);
    static void push_pickable(lua_State* current_l, Pickable& pickable);
    static void push_destructible(lua_State* current_l, Destructible& destructible);
    static void push_carried_object(lua_State* current_l, CarriedObject& carried_object);
    static void push_dynamic_tile(lua_State* current_l, DynamicTile& dynamic_tile);
    static void push_enemy(lua_State* current_l, Enemy& enemy);
    static void push_custom_entity(lua_State* current_l, CustomEntity& entity);

    // Getting objects from Lua.
    static bool is_main(lua_State* current_l, int index);
    static bool is_menu(lua_State* current_l, int index);
    static void* test_userdata(lua_State* current_l, int index,
        const char* module_name);
    static bool is_userdata(lua_State* current_l, int index,
        const std::string& module_name);
    static const ExportableToLuaPtr& check_userdata(
        lua_State* current_l,
        int index,
        const std::string& module_name
    );
    static bool is_timer(lua_State* current_l, int index);
    static TimerPtr check_timer(lua_State* current_l, int index);
    static bool is_sound(lua_State* current_l, int index);
    static SoundPtr check_sound(lua_State* current_l, int index);
    static bool is_drawable(lua_State* current_l, int index);
    static DrawablePtr check_drawable(lua_State* current_l, int index);
    static bool is_surface(lua_State* current_l, int index);
    static SurfacePtr check_surface(lua_State* current_l, int index);
    static bool is_text_surface(lua_State* current_l, int index);
    static std::shared_ptr<TextSurface> check_text_surface(lua_State* current_l, int index);
    static bool is_sprite(lua_State* current_l, int index);
    static SpritePtr check_sprite(lua_State* current_l, int index);
    static bool is_shader(lua_State* current_l, int index);
    static ShaderPtr check_shader(lua_State* current_l, int index);
    static bool is_item(lua_State* current_l, int index);
    static std::shared_ptr<EquipmentItem> check_item(lua_State* current_l, int index);
    static bool is_movement(lua_State* current_l, int index);
    static std::shared_ptr<Movement> check_movement(lua_State* current_l, int index);
    static bool is_straight_movement(lua_State* current_l, int index);
    static std::shared_ptr<StraightMovement> check_straight_movement(lua_State* current_l, int index);
    static bool is_random_movement(lua_State* current_l, int index);
    static std::shared_ptr<RandomMovement> check_random_movement(lua_State* current_l, int index);
    static bool is_target_movement(lua_State* current_l, int index);
    static std::shared_ptr<TargetMovement> check_target_movement(lua_State* current_l, int index);
    static bool is_path_movement(lua_State* current_l, int index);
    static std::shared_ptr<PathMovement> check_path_movement(lua_State* current_l, int index);
    static bool is_random_path_movement(lua_State* current_l, int index);
    static std::shared_ptr<RandomPathMovement> check_random_path_movement(lua_State* current_l, int index);
    static bool is_path_finding_movement(lua_State* current_l, int index);
    static std::shared_ptr<PathFindingMovement> check_path_finding_movement(lua_State* current_l, int index);
    static bool is_circle_movement(lua_State* current_l, int index);
    static std::shared_ptr<CircleMovement> check_circle_movement(lua_State* current_l, int index);
    static bool is_jump_movement(lua_State* current_l, int index);
    static std::shared_ptr<JumpMovement> check_jump_movement(lua_State* current_l, int index);
    static bool is_pixel_movement(lua_State* current_l, int index);
    static std::shared_ptr<PixelMovement> check_pixel_movement(lua_State* current_l, int index);
    static bool is_game(lua_State* current_l, int index);
    static std::shared_ptr<Savegame> check_game(lua_State* current_l, int index);
    static bool is_map(lua_State* current_l, int index);
    static std::shared_ptr<Map> check_map(lua_State* current_l, int index);
    static bool is_state(lua_State* current_l, int index);
    static std::shared_ptr<CustomState> check_state(lua_State* current_l, int index);
    static bool is_entity(lua_State* current_l, int index);
    static EntityPtr check_entity(lua_State* current_l, int index);
    static bool is_hero(lua_State* current_l, int index);
    static HeroPtr check_hero(lua_State* current_l, int index);
    static bool is_camera(lua_State* current_l, int index);
    static std::shared_ptr<Camera> check_camera(lua_State* current_l, int index);
    static bool is_destination(lua_State* current_l, int index);
    static std::shared_ptr<Destination> check_destination(lua_State* current_l, int index);
    static bool is_teletransporter(lua_State* current_l, int index);
    static std::shared_ptr<Teletransporter> check_teletransporter(lua_State* current_l, int index);
    static bool is_npc(lua_State* current_l, int index);
    static std::shared_ptr<Npc> check_npc(lua_State* current_l, int index);
    static bool is_chest(lua_State* current_l, int index);
    static std::shared_ptr<Chest> check_chest(lua_State* current_l, int index);
    static bool is_block(lua_State* current_l, int index);
    static std::shared_ptr<Block> check_block(lua_State* current_l, int index);
    static bool is_switch(lua_State* current_l, int index);
    static std::shared_ptr<Switch> check_switch(lua_State* current_l, int index);
    static bool is_stream(lua_State* current_l, int index);
    static std::shared_ptr<Stream> check_stream(lua_State* current_l, int index);
    static bool is_door(lua_State* current_l, int index);
    static std::shared_ptr<Door> check_door(lua_State* current_l, int index);
    static bool is_stairs(lua_State* current_l, int index);
    static std::shared_ptr<Stairs> check_stairs(lua_State* current_l, int index);
    static bool is_shop_treasure(lua_State* current_l, int index);
    static std::shared_ptr<ShopTreasure> check_shop_treasure(lua_State* current_l, int index);
    static bool is_pickable(lua_State* current_l, int index);
    static std::shared_ptr<Pickable> check_pickable(lua_State* current_l, int index);
    static bool is_destructible(lua_State* current_l, int index);
    static std::shared_ptr<Destructible> check_destructible(lua_State* current_l, int index);
    static bool is_carried_object(lua_State* current_l, int index);
    static std::shared_ptr<CarriedObject> check_carried_object(lua_State* current_l, int index);
    static bool is_dynamic_tile(lua_State* current_l, int index);
    static std::shared_ptr<DynamicTile> check_dynamic_tile(lua_State* current_l, int index);
    static bool is_enemy(lua_State* current_l, int index);
    static std::shared_ptr<Enemy> check_enemy(lua_State* current_l, int index);
    static bool is_custom_entity(lua_State* current_l, int index);
    static std::shared_ptr<CustomEntity> check_custom_entity(lua_State* current_l, int index);

    // Events.
    void check_callback_thread() const;

    void on_started();
    void on_started(const std::string& previous_state_name, CustomState* previous_state);
    void on_finished();
    void on_finished(const std::string& next_state_name, CustomState* next_state);
    void on_update();
    void on_draw(const SurfacePtr& dst_surface);
    void on_suspended(bool suspended);
    void on_paused();
    void on_unpaused();
    bool on_dialog_started(const Dialog& dialog, const ScopedLuaRef& info_ref);
    void on_dialog_finished(const Dialog& dialog);
    bool on_game_over_started();
    void on_game_over_finished();
    bool on_input(const InputEvent& event);
    bool on_key_pressed(const InputEvent& event);
    bool on_key_released(const InputEvent& event);
    bool on_character_pressed(const InputEvent& event);
    bool on_joypad_button_pressed(const InputEvent& event);
    bool on_joypad_button_released(const InputEvent& event);
    bool on_joypad_axis_moved(const InputEvent& event);
    bool on_joypad_hat_moved(const InputEvent& event);
    bool on_mouse_button_pressed(const InputEvent& event);
    bool on_mouse_button_released(const InputEvent& event);
    bool on_finger_pressed(const InputEvent& event);
    bool on_finger_released(const InputEvent& event);
    bool on_finger_moved(const InputEvent& event);
    bool on_command_pressed(GameCommand command);
    bool on_command_released(GameCommand command);
    void on_animation_finished(const std::string& animation);
    void on_animation_changed(const std::string& animation);
    void on_direction_changed(const std::string& animation, int direction);
    void on_frame_changed(const std::string& animation, int frame);
    void on_position_changed(const Point& xy);
    void on_obstacle_reached();
    void on_changed();
    void on_started(const std::shared_ptr<Destination>& destination);
    void on_opening_transition_finished(const std::shared_ptr<Destination>& destination);
    void on_obtaining_treasure(const Treasure& treasure);
    void on_obtained_treasure(const Treasure& treasure);
    void on_state_changing(const std::string& state_name, const std::string& next_state_name);
    void on_state_changed(const std::string& new_state_name);
    bool on_taking_damage(int damage);
    void on_activating();
    void on_activating(int direction4);
    void on_activated();
    void on_activated(int direction4);
    void on_activated_repeat();
    void on_inactivated();
    void on_left();
    bool on_interaction();
    bool on_interaction_item(EquipmentItem& item_used);
    void on_npc_interaction(Npc& npc);
    bool on_npc_interaction_item(Npc& npc, EquipmentItem& item_used);
    void on_npc_collision_fire(Npc& npc);
    void on_collision_fire();
    void on_collision_explosion();
    void on_collision_enemy(Enemy& enemy, Sprite& other_sprite, Sprite& this_sprite);
    void on_lifted();
    void on_thrown();
    void on_breaking();
    bool on_buying();
    void on_bought();
    void on_opened();
    bool on_opened(const Treasure& treasure);
    void on_closed();
    void on_moving();
    void on_moved();
    void on_map_changed(Map& map);
    void on_world_changed(const std::string& previous_world, const std::string& new_world);
    void on_pickable_created(Pickable& pickable);
    void on_variant_changed(int variant);
    void on_amount_changed(int amount);
    void on_obtaining(const Treasure& treasure);
    void on_obtained(const Treasure& treasure);
    void on_using();
    void on_ability_used(Ability ability);
    void on_created();
    void on_removed();
    void on_enabled();
    void on_disabled();
    void on_restarted();
    void on_pre_draw(Camera& camera);
    void on_post_draw(Camera& camera);
    void on_position_changed(const Point& xy, int layer);
    void on_obstacle_reached(Movement& movement);
    void on_movement_started(Movement& movement);
    void on_movement_changed(Movement& movement);
    void on_movement_finished();
    void on_looked();
    void on_cut();
    void on_lifting(
        Entity& carrier,
        CarriedObject& carried_object);
    void on_exploded();
    void on_regenerating();
    void on_custom_attack_received(EnemyAttack attack, Sprite* sprite);
    bool on_hurt_by_sword(Hero& hero, Sprite& enemy_sprite);
    void on_hurt(EnemyAttack attack);
    void on_dying();
    void on_dead();
    void on_immobilized();
    bool on_attacking_hero(Hero& hero, Sprite* attacker_sprite);
    void on_attacked_enemy(
        Enemy& enemy,
        Sprite* enemy_sprite,
        EnemyAttack attack,
        const EnemyReaction::Reaction& reaction
    );
    void on_ground_below_changed(Ground ground_below);
    void on_map_started(
        Map& map, const std::shared_ptr<Destination>& destination);
    void on_map_opening_transition_finished(
        Map& map, const std::shared_ptr<Destination>& destination);
    void on_map_finished();

    // Functions exported to Lua for internal needs.
    static FunctionExportedToLua
      l_panic,
      l_loader,
      l_get_map_entity_or_global,
      l_easy_index,
      l_hero_teleport,
      l_entity_iterator_next,
      l_named_sprite_iterator_next,
      l_treasure_brandish_finished,
      l_shop_treasure_description_dialog_finished,
      l_shop_treasure_question_dialog_finished,
      l_create_tile,
      l_create_destination,
      l_create_teletransporter,
      l_create_pickable,
      l_create_destructible,
      l_create_chest,
      l_create_jumper,
      l_create_enemy,
      l_create_npc,
      l_create_block,
      l_create_dynamic_tile,
      l_create_switch,
      l_create_wall,
      l_create_sensor,
      l_create_crystal,
      l_create_crystal_block,
      l_create_shop_treasure,
      l_create_stream,
      l_create_door,
      l_create_stairs,
      l_create_separator,
      l_create_custom_entity,
      l_create_bomb,
      l_create_explosion,
      l_create_fire;

    // Script data.
    lua_State* main_l;                 /**< The MAIN Lua state encapsulated. */
    lua_State* current_l;              /**< The  presumed current Lua state running */
    MainLoop& main_loop;               /**< The Solarus main loop. */

    std::list<LuaMenuData> menus;      /**< The menus currently running in their context.
                                        * Invalid ones are to be removed at the next cycle. */
    std::map<TimerPtr, LuaTimerData>
        timers;                        /**< The timers currently running, with
                                        * their context and callback. */
    std::list<TimerPtr>
        timers_to_remove;              /**< Timers to be removed at the next cycle. */

    std::set<DrawablePtr>
        drawables;                     /**< All drawable objects created by
                                        * this script. */
    std::set<DrawablePtr>
        drawables_to_remove;           /**< Drawable objects to be removed at the
                                        * next cycle. */
    std::map<const ExportableToLua*, std::set<std::string>>
        userdata_fields;               /**< Existing string keys created on each
                                        * userdata with our __newindex. This is
                                        * only for performance, to avoid Lua
                                        * lookups for callbacks like on_update. */
    std::set<std::string>
        warning_deprecated_functions;  /**< Names of deprecated functions of
                                        * the API for which a warning was emitted. */

    std::queue<std::function<void(lua_State*)>>
        cross_state_callbacks;         /**< Callbacks that must be executed on main from other coroutines */

    static const std::map<EntityType, lua_CFunction>
        entity_creation_functions;     /**< Creation function of each entity type. */
    static LuaContext*
        lua_context;                  /**< Singleton context */
};

}

#endif

