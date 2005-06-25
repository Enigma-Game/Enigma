/*
 * Copyright (C) 2000,2003 Daniel Heck
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $Id: testargp.cc,v 1.3 2003/01/30 20:29:59 dheck Exp $
 */
#include "ecl_argp.hh"

#include <iostream>
#include <iterator>             // for ostream_iterator

using namespace std;
using namespace ecl::argp;

namespace
{
    class AP : public ArgParser {
    public:
        enum {
            OPT_HELP, OPT_VERBOSE, OPT_VERSION, OPT_OUTPUT
        };

        AP() {
            show_help = verbose_flag = show_version = false;
            def (OPT_HELP,      'h', "help");
            def (OPT_VERBOSE,   'v', "verbose");
            def (OPT_VERSION,   0,   "version");
            def (OPT_OUTPUT,    'o', "output", true);
        }

        // ArgParser interface
        virtual void on_argument (const std::string &arg) {
            ifiles.push_back(arg);
        }

        virtual void on_option (int id, const std::string &param) {
            switch (id) {
            case OPT_HELP: show_help = true; break;
            case OPT_VERBOSE: verbose_flag = true; break;
            case OPT_VERSION: show_version = true; break;
            case OPT_OUTPUT:
                ofiles.push_back(param);
                break;
            }
        }

        // Variables
        bool show_help;
        bool verbose_flag;
        bool show_version;

        list<string> ofiles;
        list<string> ifiles;
    };
}

int
main(int argc, char *argv[])
{
    AP ap;
    ap.feed (argv+1, argv+argc);
    ap.parse ();

    cout << "show_help : " << ap.show_help << endl;
    cout << "show_version: " << ap.show_version << endl;
    cout << "verbose_flag: " << ap.verbose_flag << endl;
    cout << "output_file: ";
    copy(ap.ofiles.begin(), ap.ofiles.end(), std::ostream_iterator<string>(cout, " "));
    cout << endl;
    cout << "input files: ";
    copy(ap.ifiles.begin(), ap.ifiles.end(), ostream_iterator<string>(cout, " "));
    cout << endl;
}
