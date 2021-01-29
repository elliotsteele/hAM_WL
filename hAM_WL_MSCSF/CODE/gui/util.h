/*
 * hAMster_WheeL : GUI for the hAM_WL cellular simulation program
 * Copyright (C) 2021 Elliot Steele
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CELLCAMVIEW_UTIL_H
#define CELLCAMVIEW_UTIL_H

#include <memory>
#include <type_traits>

#include <gtkmm.h>

namespace util_internal {
    template<typename T, typename Func>
    inline auto get_widget(const Glib::ustring& name, Func fn) requires std::is_invocable_v<Func, T**>
    {
        T* widget;
        fn(&widget);

        if (widget == nullptr)
            throw std::runtime_error(("Failed to create widget with name : " + name).c_str());

        if constexpr (std::is_base_of_v<Gtk::Window, T>) {
            return std::unique_ptr<T>(widget);
        } else {
            return widget;
        }
    }
}

template<typename T>
auto get_widget(const Glib::RefPtr<Gtk::Builder>& builder, const Glib::ustring& name)
    requires std::is_base_of_v<Gtk::Widget, T>
{
   return util_internal::get_widget<T>(name, [&builder, &name](T** widget) { builder->get_widget(name, *widget); });
}

template<typename T, typename ...Args>
auto get_widget_derived(const Glib::RefPtr<Gtk::Builder>& builder, const Glib::ustring& name, Args&& ...args)
    requires std::is_base_of_v<Gtk::Widget, T>
{
    return util_internal::get_widget<T>(name, [&builder, &name, &args...](T** widget) {
        builder->get_widget_derived(name, *widget, std::forward<Args>(args)...);
    });
}

#endif //CELLCAMVIEW_UTIL_H
