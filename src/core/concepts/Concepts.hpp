/// @file    Concepts.hpp
/// @brief   C++20 Concepts for type constraints.
/// @author  saintson (pan.aleksandr.off@gmail.com)
/// @date    2025-12-16
/// @copyright Copyright (c) 2025 saintson. All rights reserved.
///            Licensed under the GNU General Public License v3.0 (GPLv3).

#pragma once

#include <concepts>

namespace cad {

struct GeometryData;
struct Transform;
struct Material;

/// @brief Concept for types that provide geometry data.
template <typename T>
concept HasGeometryData = requires(const T& obj) {
    { obj.geometry() } -> std::convertible_to<const GeometryData&>;
};

/// @brief Concept for transformable types.
template <typename T>
concept IsTransformable = requires(T& obj, const Transform& t) {
    { obj.transform() } -> std::convertible_to<const Transform&>;
    { obj.setTransform(t) };
};

/// @brief Concept for types with material properties.
template <typename T>
concept HasMaterial = requires(T& obj, const Material& m) {
    { obj.material() } -> std::convertible_to<const Material&>;
    { obj.setMaterial(m) };
};

/// @brief Concept for command pattern types.
template <typename T>
concept IsCommand = requires(T cmd) {
    { cmd.execute() };
    { cmd.undo() };
    { cmd.isValid() } -> std::convertible_to<bool>;
};

/// @brief Concept for types with visibility state.
template <typename T>
concept HasVisibility = requires(T& obj, bool v) {
    { obj.isVisible() } -> std::convertible_to<bool>;
    { obj.setVisible(v) };
};

/// @brief Concept for identifiable types.
template <typename T>
concept IsIdentifiable = requires(const T& obj) {
    { obj.id() } -> std::convertible_to<std::uint64_t>;
};

/// @brief Concept for selectable types.
template <typename T>
concept IsSelectable = requires(T& obj, bool s) {
    { obj.isSelected() } -> std::convertible_to<bool>;
    { obj.setSelected(s) };
};

/// @brief Complete scene object concept.
template <typename T>
concept IsSceneObject = IsIdentifiable<T> && 
                        IsTransformable<T> && 
                        HasMaterial<T> && 
                        HasVisibility<T> &&
                        HasGeometryData<T>;

} // namespace cad
