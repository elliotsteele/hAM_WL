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

#include <gtkmm.h>

#include "util.h"
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv);

    const auto builder = Gtk::Builder::create_from_file("gui/forms/main_window.glade");

    auto window = get_widget_derived<MainWindow>(builder, "main_window");

    return app->run(*window);
}
