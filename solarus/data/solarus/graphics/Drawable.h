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
#ifndef SOLARUS_DRAWABLE_H
#define SOLARUS_DRAWABLE_H

#include "solarus/core/Common.h"
#include "solarus/core/Point.h"
#include "solarus/core/Size.h"
#include "solarus/graphics/BlendMode.h"
#include "solarus/graphics/SurfacePtr.h"
#include "solarus/graphics/ShaderPtr.h"
#include "solarus/lua/ExportableToLua.h"
#include "solarus/lua/ScopedLuaRef.h"
#include "solarus/graphics/DrawProxies.h"
#include <memory>

namespace Solarus {

class Movement;
class Transition;
class Rectangle;

/**
 * \brief An object that can be drawn on a surface with additional effects.
 *
 * Possible additional effects are movements and transitions.
 * Lua callbacks may be associated to the end of these effects.
 */
class Drawable: public ExportableToLua {

  public:

    virtual ~Drawable();

    virtual Point get_origin() const;

    /**
     * \brief Returns the size of this drawable.
     * \return The size of this drawable.
     */
    virtual Size get_size() const = 0;

    // dynamic effects
    void start_movement(const std::shared_ptr<Movement>& movement);
    void stop_movement();
    const std::shared_ptr<Movement>& get_movement();
    const Point& get_xy() const;
    void set_xy(const Point& xy);

    void start_transition(
        std::unique_ptr<Transition> transition,
        const ScopedLuaRef& callback_ref
    );
    void stop_transition();
    Transition* get_transition();

    // drawing with effects
    void draw(const SurfacePtr& dst_surface) const;
    void draw(const SurfacePtr& dst_surface, int x, int y) const;
    void draw(const SurfacePtr& dst_surface, const Point& dst_position) const;
    void draw(const SurfacePtr &dst_surface, const Point &dst_position, const DrawProxy& proxy) const;
    void draw_region(const Rectangle& region, const SurfacePtr& dst_surface) const;
    void draw_region(const Rectangle& region,
        const SurfacePtr& dst_surface, const Point& dst_position) const;
    void draw_region(const Rectangle& region,
                     const SurfacePtr& dst_surface, const Point& dst_position, const DrawProxy& proxy) const;

    void draw_with_transition(const Rectangle& region, const SurfacePtr& dst_surface, const Point& dst_position, const Transition &trans) const;

    void set_shader(const ShaderPtr& shader);
    const ShaderPtr& get_shader() const;

    /**
     * \brief Draws this object with effect information passed
     *
     * Redefine this function to draw your object onto the destination
     * surface.
     *
     * This version may ignore the passed infos.region parameter and draw
     * at full size, this allow sprites to optimize drawing when no cropping
     * is needed, as it is often the case
     *
     * \param dst_surface The destination surface.
     * \param infos draw informations bundle
     */
    virtual void raw_draw(
        Surface& dst_surface,
        const DrawInfos& infos
    ) const = 0;

    /**
     * @brief Draw
     *
     * @param dst_surface
     * @param infos draw information bundle
     */
    virtual void raw_draw_region(
        Surface& dst_surface,
        const DrawInfos& infos
    ) const = 0;

    virtual void update();
    bool is_suspended() const;
    virtual void set_suspended(bool suspended);

    BlendMode get_blend_mode() const;
    void set_blend_mode(BlendMode blend_mode);

    const Color& get_color_modulation() const;
    void set_color_modulation(const Color& color);

    uint8_t get_opacity() const;
    void set_opacity(uint8_t opacity);

    double get_rotation() const;
    void set_rotation(double rotation);

    const Scale& get_scale() const;
    void set_scale(const Scale& scale);

    const Point& get_transformation_origin() const;
    void set_transformation_origin(const Point& origin);

    Point get_full_origin() const;

    virtual Rectangle get_region() const = 0;
  protected:
    Drawable();
  private:
    const DrawProxy& terminal() const;

    Point xy;                     /**< Current position of this object
                                   * (result of movements). */
    std::shared_ptr<Movement>
        movement;                 /**< A movement applied or nullptr. */
    std::unique_ptr<Transition>
        transition;               /**< A transition applied or nullptr. */
    ScopedLuaRef transition_callback_ref;
                                  /**< Lua registry ref of a function to call
                                   * when the transition finishes */
    bool suspended;               /**< Whether this object is suspended. */
    BlendMode blend_mode;         /**< How to draw this object on a surface. */
    ShaderPtr shader;             /**< Optional shader used to draw the object */
    uint8_t opacity = 255;        /**< Opacity of this drawable object */
    double rotation = 0;          /**< Rotation of the object around transform_origin*/
    Scale scale;                  /**< Scale of the object around transform_origin*/
    Point transformation_origin;       /**< pivot for the transformations (rot,scale) of the object*/
    Color color_mod = Color::white;
};

}

#endif

