/*
 * Copyright (C) 2003,2004 Daniel Heck
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id: main.hh,v 1.2 2004/05/22 13:04:30 dheck Exp $
 */
#ifndef ENIGMA_MAIN_HH
#define ENIGMA_MAIN_HH

#include <string>
#include <vector>
#include <memory>


namespace enigma
{
/* -------------------- Application services -------------------- */

    class ImageManager;         // in resources.hh
    class FontManager;          // in resources.hh

    class Application {
    public:
        static Application *get_instance();

        Application();

        void init(int argc, char **argv);

        //---------- Variables ----------//

        std::vector<std::string>  args; // List of command line arguments
        std::string               progname; // Name of this program

        bool wizard_mode;
        bool nograb;            // Do not grab mouse cursor during game
        std::string language;   // Language to use
    };

#define APP enigma::Application::get_instance()

    //---------- Logging ----------//
    extern std::ostream Log;

    //---------- Command line options ----------//
    extern bool   WizardMode;
    extern bool   Nograb;
}

#endif
