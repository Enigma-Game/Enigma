/*
 * Copyright (C) 2002,2003,2004,2005,2006 Daniel Heck
 * Copyright (C) 2006,2007,2008,2009 Ronald Lamprecht
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
 */

#include "gui/MainMenu.hh"
#include "gui/LevelMenu.hh"
#include "gui/LevelWidget.hh"
#include "gui/SearchMenu.hh"
#include "gui/OptionsMenu.hh"
#include "gui/InfoMenu.hh"
#include "gui/LanguageMenu.hh"
#include "gui/LevelPackMenu.hh"
#include "gui/LevelPreviewCache.hh"
#include "display.hh"
#include "ecl_font.hh"
#include "ecl_system.hh"
#include "ecl_util.hh"
#include "lev/Index.hh"
#include "lev/Proxy.hh"
#include "main.hh"
#include "nls.hh"
#include "options.hh"
#include "resource_cache.hh"
#include "MusicManager.hh"
#include "world.hh"

#include "netgame.hh"

using namespace ecl;
using namespace std;

namespace enigma { namespace gui {

    /* -------------------- NetworkMenu -------------------- */

    NetworkMenu::NetworkMenu ()
    {
        const VMInfo *vminfo = video_engine->GetInfo();

        BuildVList b(this, Rect((vminfo->width - 200)/2, 200, 200, 40), 10);
        m_hostgame = b.add(new StaticTextButton(N_("Host Game"), this));
        m_joingame = b.add(new StaticTextButton(N_("Join Game"), this));
        m_back     = b.add(new StaticTextButton(N_("Back"), this));
    }

    NetworkMenu::~NetworkMenu ()
    {
    }

    bool NetworkMenu::on_event (const SDL_Event &e)
    {
        return false;
    }

    void NetworkMenu::on_action(gui::Widget *w)
    {
        if (w == m_hostgame) {
            HostLobbyMenu m;
            m.manage();
            invalidate_all();
        } else if (w == m_joingame) {
            JoinLobbyMenu m;
            m.manage();
            invalidate_all();
        } else if (w == m_back) {
            Menu::quit();
        }
    }

    void NetworkMenu::draw_background(ecl::GC &gc)
    {
        set_caption(_("Enigma - Network Menu"));
        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
    }

    void NetworkMenu::tick(double dtime)
    {
    }

    /* -------------------- Lobby helpers -------------------- */

    namespace {
        bool proxy_is_network_mode(lev::Proxy *p) {
            if (!p) return false;
            try {
                p->loadMetadata(true);
            } catch (...) {
                return false;
            }
            return p->hasNetworkMode();
        }
    }

    /* -------------------- HostLobbyMenu -------------------- */

    HostLobbyMenu::HostLobbyMenu()
    : lbl_code(new Label("", HALIGN_LEFT)),
      lbl_port(new Label("", HALIGN_RIGHT)),
      lbl_pack(new Label("", HALIGN_LEFT)),
      lbl_level(new Label("", HALIGN_LEFT)),
      lbl_status(new Label("", HALIGN_LEFT)),
      lbl_failed(new Label("", HALIGN_LEFT)),
      levelwidget(new LevelWidget(/*withScoreIcons=*/true,
                                  /*withEditBorder=*/false)),
      game_started(false)
    {
        const VMInfo *vminfo = video_engine->GetInfo();
        int w = vminfo->width;
        int h = vminfo->height;
        int margin = 20;
        int label_h = 26;

        // Top row: code on the left, port on the right.
        int top_y = 50;
        this->add(lbl_code, Rect(margin,             top_y, (w-2*margin)/2, label_h));
        this->add(lbl_port, Rect(w/2,                top_y, (w-2*margin)/2, label_h));

        // Pack row with prev/next buttons.
        int pack_y = top_y + label_h + 8;
        int btn_w = 80;
        int btn_h = label_h;
        but_prev_pack = new StaticTextButton(N_("< Pack"), this);
        but_next_pack = new StaticTextButton(N_("Pack >"), this);
        int pack_label_x = margin;
        int pack_label_w = w - 2*margin - 2*(btn_w + 6);
        this->add(lbl_pack,      Rect(pack_label_x, pack_y, pack_label_w, btn_h));
        this->add(but_prev_pack, Rect(pack_label_x + pack_label_w + 6,
                                      pack_y, btn_w, btn_h));
        this->add(but_next_pack, Rect(pack_label_x + pack_label_w + 6 + btn_w + 6,
                                      pack_y, btn_w, btn_h));

        // Level grid: takes the bulk of the remaining vertical space.
        int grid_y = pack_y + btn_h + 8;
        int bottom_block_h = 3 * (label_h + 6) + 40 + margin;
        int grid_h = std::max(120, h - grid_y - bottom_block_h);
        Rect grid_area(margin, grid_y, w - 2*margin, grid_h);
        levelwidget->set_listener(this);
        levelwidget->realize(grid_area);
        levelwidget->set_area(grid_area);
        this->add(levelwidget);

        // Below the grid: selected-level label, then status + failed,
        // then Start/Cancel buttons.
        int info_y = grid_y + grid_h + 6;
        this->add(lbl_level,  Rect(margin, info_y,                       w - 2*margin, label_h));
        this->add(lbl_status, Rect(margin, info_y + label_h + 4,         w - 2*margin, label_h));
        this->add(lbl_failed, Rect(margin, info_y + 2*(label_h + 4),     w - 2*margin, label_h));

        int sb_w = 140;
        int sb_h = 36;
        int sb_gap = 20;
        int sb_y = h - sb_h - margin;
        int sb_total = sb_w * 2 + sb_gap;
        int sb_x = (w - sb_total) / 2;
        but_start  = new StaticTextButton(N_("Start Game"), this);
        but_cancel = new StaticTextButton(N_("Cancel"), this);
        this->add(but_start,  Rect(sb_x,                 sb_y, sb_w, sb_h));
        this->add(but_cancel, Rect(sb_x + sb_w + sb_gap, sb_y, sb_w, sb_h));

        // Open the listener. If it fails (port in use), put the error
        // into the status label; user can hit Cancel.
        int port = 12345;
        if (!netgame::OpenHostLobby(port)) {
            lbl_status->set_text(_("Could not open listening port."));
        }

        levelwidget->syncFromIndexMgr();
        update_level_label();
        update_status();
    }

    HostLobbyMenu::~HostLobbyMenu() {
        if (!game_started)
            netgame::CloseHostLobby();
    }

    bool HostLobbyMenu::current_level_is_network() {
        lev::Index *ind = lev::Index::getCurrentIndex();
        if (!ind) return false;
        return proxy_is_network_mode(ind->getProxy(ind->getCurrentPosition()));
    }

    void HostLobbyMenu::update_level_label() {
        lev::Index *ind = lev::Index::getCurrentIndex();
        if (!ind || ind->size() == 0) {
            lbl_pack->set_text(_("Level pack: (none)"));
            lbl_level->set_text(_("Level: (none)"));
        } else {
            int pos = ind->getCurrentPosition();
            lev::Proxy *p = ind->getProxy(pos);
            std::string title = p ? p->getTitle() : "?";
            lbl_pack->set_text(ecl::strf(_("Level pack: %s"),
                                         ind->getName().c_str()));
            std::string suffix = current_level_is_network()
                                     ? _(" [network]")
                                     : _(" [single-player — may not work]");
            lbl_level->set_text(ecl::strf(_("Level: #%d - %s"),
                                          pos + 1, title.c_str()) + suffix);
        }
    }

    void HostLobbyMenu::update_status() {
        lbl_code->set_text(ecl::strf(_("Access code: %s"),
                                      netgame::LobbyCode().c_str()));
        lbl_port->set_text(ecl::strf(_("Listening on port %d"),
                                      netgame::LobbyPort()));
        if (netgame::LobbyHasReadyClient()) {
            lbl_status->set_text(_("Client connected — press Start Game."));
        } else if (netgame::LobbyHasPendingClient()) {
            lbl_status->set_text(_("Client connecting — waiting for code..."));
        } else if (netgame::LobbyPort() != 0) {
            lbl_status->set_text(_("Waiting for client..."));
        }
        int n = netgame::LobbyFailedAttempts();
        if (n == 0) {
            lbl_failed->set_text(_("Failed attempts: 0"));
        } else {
            lbl_failed->set_text(
                ecl::strf(_("Failed attempts: %d (last: %s)"),
                          n, netgame::LobbyLastFailReason().c_str()));
        }
    }

    bool HostLobbyMenu::on_event(const SDL_Event &e) {
        return false;
    }

    void HostLobbyMenu::on_action(gui::Widget *w) {
        if (w == but_cancel) {
            Menu::quit();
            return;
        }
        if (w == but_start) {
            if (!netgame::LobbyHasReadyClient()) {
                lbl_status->set_text(_("No client connected yet."));
                invalidate_all();
                return;
            }
            lev::Index *ind = lev::Index::getCurrentIndex();
            if (!ind || ind->size() == 0) {
                lbl_status->set_text(_("No level selected."));
                invalidate_all();
                return;
            }
            game_started = true;
            std::string pack = ind->getName();
            int pos = ind->getCurrentPosition();
            // StartHostedGame takes over the listener and runs the
            // whole game synchronously. When it returns, the lobby is
            // already closed.
            netgame::StartHostedGame(pack, pos);
            Menu::quit();
            return;
        }
        if (w == but_prev_pack || w == but_next_pack) {
            lev::Index *cur = lev::Index::getCurrentIndex();
            if (!cur) return;
            lev::Index *target = (w == but_next_pack)
                ? lev::Index::nextGroupIndex()
                : lev::Index::previousGroupIndex();
            if (target && target != cur)
                lev::Index::setCurrentIndex(target->getName());
            levelwidget->syncFromIndexMgr();
            update_level_label();
            invalidate_all();
            return;
        }
        if (w == levelwidget) {
            // LevelWidget fired a "selected" action (click or Enter).
            // In the regular menu this launches the game; here we just
            // adopt the chosen level and update the label. The actual
            // game start waits for the Start Game button.
            update_level_label();
            invalidate_all();
            return;
        }
    }

    void HostLobbyMenu::draw_background(ecl::GC &gc) {
        set_caption(_("Enigma - Host Lobby"));
        const VMInfo *vminfo = video_engine->GetInfo();
        blit(gc, vminfo->mbg_offsetx, vminfo->mbg_offsety,
             enigma::GetImage("menu_bg", ".jpg"));
        Font *f = enigma::GetFont("menufontsel");
        std::string title = _("Host Lobby");
        int tw = f->get_width(title.c_str());
        f->render(gc, (vminfo->width - tw) / 2, 40, title.c_str());
    }

    void HostLobbyMenu::tick(double dtime) {
        if (game_started) return;
        levelwidget->tick(dtime);
        netgame::ServiceHostLobby();
        static double accu = 0;
        accu += 0.01;
        if (accu >= 0.2) {
            accu = 0;
            // Cursor on the LevelWidget changes via mouse motion or
            // arrow keys, neither of which routes through on_action.
            // Refresh the label so the selection display stays
            // in sync.
            update_level_label();
            update_status();
            invalidate_all();
        }
    }

    /* -------------------- JoinLobbyMenu -------------------- */

    JoinLobbyMenu::JoinLobbyMenu()
    : tf_host(new TextField("localhost")),
      tf_port(new TextField("12345")),
      tf_code(new TextField("")),
      lbl_status(new Label("", HALIGN_LEFT))
    {
        const VMInfo *vminfo = video_engine->GetInfo();
        int w = vminfo->width;

        int label_w = 90;
        int field_w = 260;
        int row_w = label_w + 10 + field_w;
        int row_x = (w - row_w) / 2;
        int y = 130;
        int row_h = 36;
        int row_gap = 14;

        auto add_field = [&](const char *labeltext, TextField *tf) {
            Label *l = new Label(labeltext, HALIGN_RIGHT);
            this->add(l,  Rect(row_x, y, label_w, row_h));
            this->add(tf, Rect(row_x + label_w + 10, y, field_w, row_h));
            y += row_h + row_gap;
        };
        add_field(N_("Host:"), tf_host);
        add_field(N_("Port:"), tf_port);
        add_field(N_("Code:"), tf_code);

        tf_port->setInvalidChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!?");
        tf_port->setMaxChars(5);
        tf_code->setMaxChars(6);
        tf_code->setInvalidChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!?@./:");

        y += 6;
        this->add(lbl_status, Rect(row_x, y, row_w, row_h));
        y += row_h + 16;

        int sb_w = 140;
        int sb_h = 36;
        int sb_gap = 20;
        int sb_total = sb_w * 2 + sb_gap;
        int sb_x = (w - sb_total) / 2;
        but_connect = new StaticTextButton(N_("Connect"), this);
        but_back    = new StaticTextButton(N_("Back"), this);
        this->add(but_connect, Rect(sb_x,                 y, sb_w, sb_h));
        this->add(but_back,    Rect(sb_x + sb_w + sb_gap, y, sb_w, sb_h));

        std::string last_err = netgame::LastJoinError();
        if (!last_err.empty())
            lbl_status->set_text(_("Last error: ") + last_err);
    }

    JoinLobbyMenu::~JoinLobbyMenu() {
    }

    bool JoinLobbyMenu::on_event(const SDL_Event &e) {
        return false;
    }

    void JoinLobbyMenu::do_connect() {
        std::string host = tf_host->getText();
        std::string portstr = tf_port->getText();
        std::string code = tf_code->getText();
        int port = std::atoi(portstr.c_str());
        if (host.empty()) {
            lbl_status->set_text(_("Please enter a host name."));
            invalidate_all();
            return;
        }
        if (code.size() != 6) {
            lbl_status->set_text(_("Code must be 6 digits."));
            invalidate_all();
            return;
        }
        lbl_status->set_text(_("Connecting..."));
        draw_all();
        refresh();
        netgame::Join(host, port, code);
        std::string err = netgame::LastJoinError();
        if (err.empty()) {
            // Game ran to completion; close this menu.
            Menu::quit();
        } else {
            lbl_status->set_text(err);
            invalidate_all();
        }
    }

    void JoinLobbyMenu::on_action(gui::Widget *w) {
        if (w == but_connect) {
            do_connect();
        } else if (w == but_back) {
            Menu::quit();
        }
    }

    void JoinLobbyMenu::draw_background(ecl::GC &gc) {
        set_caption(_("Enigma - Join Game"));
        const VMInfo *vminfo = video_engine->GetInfo();
        blit(gc, vminfo->mbg_offsetx, vminfo->mbg_offsety,
             enigma::GetImage("menu_bg", ".jpg"));
        Font *f = enigma::GetFont("menufontsel");
        std::string title = _("Join Game");
        int tw = f->get_width(title.c_str());
        f->render(gc, (vminfo->width - tw) / 2, 40, title.c_str());
    }

    void JoinLobbyMenu::tick(double /*dtime*/) {
    }


    /* -------------------- Help menu -------------------- */
    static const char *credit_text[] = {
        N_("Main developers of the 1.30 release:"),
        " ",
        "  THOMAS BERNHARDT",
        "  RAOUL BOURQUIN",
        N_("  DANIEL HECK (project founder)"),
        "  GEORG HONOLD",
        N_("  ANDREAS LOCHMANN (lead)"),
        "  NOBBY",
        " ",
        N_("Special Thanks To:"),
        " ",
        N_("  RONALD LAMPRECHT (lead up to 1.1)"),
        N_("  MEINOLF SCHNEIDER (game idea, level design)"),
        " ",
        N_("Please refer to the manual or the next pages for full credits."),
        " ",
        N_("Home Page: https://www.nongnu.org/enigma   Contact: enigma-devel@nongnu.org"),
        " ",
        N_("Enigma is free software and may be distributed under the terms of the GNU General Public License, version 2."),
        N_("Copyright (C) 2002-2025 Daniel Heck and contributors."),
        "\n",
        N_("Main developers of all releases:"),
        " ",
        "  Thomas Bernhardt  (Level design and testing, movies)",
        "  Raoul Bourquin  (Level admin. & design, all over invaluable additions)",
        "  Siegfried Fennig  (Level design, graphics)",
        "  Martin Hawlisch  (Level design, graphics, programming)",
        "  Daniel Heck  (Main developer up to 0.92, graphics, documentation)",
        "  Georg Honold  (Level administration and design)",
        "  Ronald Lamprecht  (Main developer up to 1.1, XML, new API, engine, portability, GUI, documentation)",
        "  Andreas Lochmann  (Programming, level admin. & design, documentation)",
        "  Petr Machata  (Level design, programming)",
        "  Nobby  (Level design and testing, score evaluation)",
        "  Nat Pryce  (Level design)",
        "  Jacob Scott  (Level design)",
        "  Sven Siggelkow  (Level design and special Oxyd expertise)",
        "  Ralf Westram  (Programming, level design)",
        "\n",
        N_("Special Thanks:"),
        " ",
        "  Johannes Fortmann  (Mac OS X port, some programming, graphics)",
        "  illmind  (forum mag-heut.net administration, level design)",
        // waiting for licence to add the sounds
        // "  \'Cellar of Rats\'  (Sound effects)",
        "  Sidney Markowitz  (Mac OS X port and Homebrew Formula)",
        "  Jennifer Robertson  (graphics second generation)",
        "  Jeremy Sawicki  (Oxydlib)",
        "  Erich Schubert  (Debian/Ubuntu packages, level design)",
        "  Andrew \'Necros\' Sega  (menu music \'Pentagonal Dreams\')",
        "  David W. Skinner  (many Sokoban Levels)",
        "  Clifford J. Tasner  (music second generation, proof reading)",
        "  Michał Trzebiatowski  (translation administration, German and Polish translations)",
        "  Юрий Жиромский - Yuriy Zhyromskiy  (Russian and Ukrainian translations, Russian manual and homepage translation, Slackware package)",
        "\n",
        N_("Contributors"),
        " ",
        "  Andreas Abraham  (German translation, code clean-up, bug fixes)",
        "  Samuel Elías Martínez Álvarez  (Spanish translation for 0.92)",
        "  Another  (Level design)",
        "  Γιάννης Ανθυμίδης - Yannis Anthymidis  (Greek translation)",
        "  Sébastien Aperghis-Tramoni  (French translation)",
        "  Ash  (Level design)",
        "  Claudio B.  (Italian translation)",
        "  Андрій Бандура  (Ukrainian translation)",
        "  Roberto Bardin  (Level design)",
        "  C.W. Betts  (Updates to .gitignore and .DS_Store)",
        "  George Birbilis  (Greek translation)",
        "  Helge Blohmer  (Level design)",
        "  Nathan Bronecke  (Level design)",
        "  Harry Bruder  (Level design, LotM articles)",  // daydreamer
        "  Alain Busser  (Level design, French translation, manual)",
        "  Guy Busser  (Level design)",
        "  Richi Bützer  (Level design)",
        "  Espen Bye  (Norwegian translation)",
        "  Guglielmo Cafaggi  (Italian translation)",
        "  Calinou  (French translation)",
        "  capkoh  (Level design)",
        "  Manfredi Carta  (Level design)",
        "  Yuri Chornoivan  (Ukrainian translation)",
        "  Christoph & Anita  (Level design)",
        "  Dan Church (Fix locale directory)",
        "  David D  (French translation)",
        "  Dawid Job  (Polish translation)",
        "  Дремук Сергей - Serge Dremuk  (Russian and Ukrainian translations)",
        "  Joseph Dunne  (Level design)",
        "  Xerxes M. Dynatos  (Level design)",
        "  Edward  (Level design)",
        "  Els van Eijk  (Dutch translation)",
        "  Manuel Eisentraut  (Level design)",
        "  Stephanie Fabian  (Invaluable bug reports)",
        "  Irina Fedulova  (Russian translation)",
        "  Johann Freymuth  (Level design, invaluable bug reports)",
        "  fri  (Czech translation)",
        "  firespin  (Greek translation)",
        "  Stefan Gagner  (Swedish translation)",
        "  Roberto García  (Spanish translation)",
        "  João Gaspar  (Portuguese translation)",
        "  Andy Geldmacher  (Level design)",
        "  Helmut Grohne  (Patch to ease cross compilation)",
        "  Edwin Groothuis  (FreeBSD port)",
        "  GunChleoc  (Scottish Gaelic translation, fixes to localization)",
        "  Daniel Hadas  (Level design)",
        "  Jonatan Hadas  (Level design)",
        "  Hairball  (Level design)",
        "  Joe Hansen  (Danish translation)",
        "  Haris  (Bosnian translation)",
        "  hasufell  (Improved enet support)",
        "  heiko123abc  (Higher resolution modes)",
        "  Immanuel Herrmann  (Level design)",
        "  Zoltan Hoppár  (Hungarian translation)",
        "  Tea Horvatic  (Croatian translation)",
        "  Michal Hriň  (Slovak translation)",
        "  Brian Huffman  (Level design)",
        "  Johannes Hüsing  (Level design)",
        "  Hypocrite  (Finnish translation)",
        "  Icewing  (Level design)",
        "  ItTheIt  (Level design)",
        "  jokker87  (Fixed original oxyd shuffle)",
        "  Máté Lehel Juhász  (Hungarian translation)",
        "  Stian Jørgensrud  (Norwegian translation)",
        "  Samuele Kaplun  (Italian translation)",
        "  Jorma Karvonen  (Finnish translation)",
        "  Ivica Kolić  (Croatian translation)",
        "  Manuel König  (Level design, bug reports)",
        "  Jens-Christian Korth  (Level design)",
        "  Håvard Kristoffersen  (Norwegian translation)",
        "  Johannes Laire  (Level design)",
        "  Joona Laire  (Level design)",
        "  Markus Laire  (Level design)",
        "  Jürgen Lange  (Level design)",
        "  Legimet  (Fix build error with GCC 6)",
        "  Dominik Lehmann  (Level design)",
        "  Roger Leigh  (Fix invalid conversions to double)",
        "  Dominik Leipold  (Level design)",
        "  Ryan Lerch  (AppData file)",
        "  Edward Leuf  (Feedback, bug reports)",
        "  Christophe Lherieau  (French translation)",
        "  Ricky Qi Li  (Chinese translation)",
        "  Lasse Liehu  (Finnish translation)",
        "  Ingo van Lil  (Feedback, bug reports)",
        "  Hugo Locurcio  (French translation)",
        "  Frank van der Loo  (Dutch translation)",
        "  Ľuboslava  (Slovak translation)",
        "  luigif  (Italian translation)",
        "  Dzmitry Ludzik  (Belorussian and Russian translations)",
        "  Lurcane  (Level design)",
        "  Riccardo Di Maio  (Italian translation)",
        "  Wasilis Mandratzis-Walz  (Greek translation)",
        "  Dmitry Marakasov  (Fix to meditation bug)",
        "  Sérgio Marques  (Portuguese translation)",
        "  Mat Mat  (Swedish translation)",
        "  MatteoMat01  (Italian translation)",
        "  Barry & Lori Mead  (Level design)",
        "  Adnan Memija  (Bosnian translation)",
        "  Merarom  (Swedish translation)",
        "  Linda Mihalic  (English proof reading)",
        "  moonpearl  (Level design)",
        "  Carlos Moreira  (Portuguese translation)",
        "  Stephen Morley (Safalra)  (Level design)",
        "  Fumio Nakayama  (Japanese translation)",
        "  nanook  (Level design)",
        "  Krishnamurti Lelis Lima Vieira Nunes  (Portuguese translation)",
        "  Daniel Nylander  (Swedish translation)",
        "  Nikola Parsifal  (Croatian translation)",
        "  Markéta Pecoldová  (Czech translation)",
        "  Andreas Persenius  (Level design)",
        "  Karen Pouelle  (Level data clean-up)",
        "  Grzegorz Pruchniakowski  (Polish translation)",
        "  Mark Pulley  (Level design)",
        "  pzykosiz  (Level design)",
        "  Bruno Queiros  (Portuguese translation)",
        "  Quipyowert2  (Fixing leaks and bugs)",
        "  Vincenzo Reale  (Italian translation)",
        "  A. Regnander  (Swedish translation)",
        "  Thomas De Rocker  (Dutch translation)",
        "  Rudolf  (Level design)",
        "  Rui  (Portuguese translation)",
        "  Peter Santo  (Level design)",
        "  Sadie  (Level design)",
        "  Tobias Schmidbauer  (Windows installer and icon)",
        "  Lukas Schüller  (Level design)",
        "  Michael Schwarz  (Update to Enigma logo)",
        "  Stefan Schwoon  (Level design)",
        "  Achim Settelmeier  (RPM specfile)",
        "  ShadowPhrogg32642342  (Level design, Hungarian translation)",  // Dániel Borbély
        "  Gonçalo Silva  (Portuguese translation)",
        "  Manuela Silva  (Portuguese translation)",
        "  Alex Smith  (Level design)",
        "  Alan Smithee  (Level design)",
        "  Jon Sneyers  (Level design)",
        "  Spaceman  (Level design)",
        "  Ulf Stegemann  (Level design)",
        "  Jürgen Sticht  (Level design)",  // JuSt
        "  Gabe Stilez  (Hungarian translation)",
        "  Mikke Surakka  (Finnish translation)",
        "  Andrzej Szombierski  (Level design)",
        "  Tacvek  (Lua 5.1 upgrade)",
        "  James Taylor  (Level design)",
        "  Michael Terry  (.desktop file)",
        "  theschitz  (Swedish translation)",
        "  Vince38  (French translation)",
        "  Ray Wick  (Level design)",
        "  Martin Woyda-Wolf  (Level design)",
        "  Joe Wreschnig  (Manual page)",
        "  Heikki Ylipaavalniemi  (Finnish translation)",
        "  Francisco Zamorano  (Spanish translation)",
        "  Zephyr  (Level design)",
        "  郝中飞  (Chinese translation)",
        "  Unknown contributor (Slovenian translation)",
        0,
    };

    MainHelpMenu::MainHelpMenu () {
        const VMInfo *vminfo = video_engine->GetInfo();
        const bool vshrink = vminfo->width < 640 ;

        BuildVList b = vshrink? BuildVList(this, Rect(40, 40, 100, 25), 3)
                : BuildVList(this, Rect((vminfo->width - 180)/2, vminfo->tt ? 80 : 150, 180, 40), 7);
        BuildVList br = vshrink? BuildVList(this, Rect(180, 40, 100, 25), 4) : BuildVList(this, Rect(0, 0, 0, 0), 0);
        BuildVList *brp = vshrink? &br : &b;

        homepage = b.add(new StaticTextButton(N_("Homepage"), this));
        docs = b.add(new StaticTextButton(N_("Documentation"), this));
        paths = b.add(new StaticTextButton(N_("Paths"), this));
        autofolder = brp->add(new StaticTextButton(N_("Locate Auto"), this));
        scorefolder = brp->add(new StaticTextButton(N_("Locate Score"), this));
        credits = brp->add(new StaticTextButton(N_("Credits"), this));
        back = brp->add(new StaticTextButton(N_("Back"), this));
    }

    MainHelpMenu::~MainHelpMenu () {
    }

    bool MainHelpMenu::on_event (const SDL_Event &e)  {
        return false;
    }

    void MainHelpMenu::on_action(gui::Widget *w) {
        if (w == homepage) {
            video_engine->SetFullscreen(false);
            ecl::BrowseUrl("https://www.enigma-game.org");
        } else if (w == docs) {
            video_engine->SetFullscreen(false);
            ecl::BrowseUrl("file://" + app.docPath + "/index.html");
        } else if (w == paths) {
            showPaths();
        } else if (w == autofolder) {
            video_engine->SetFullscreen(false);
            ecl::ExploreFolder(ecl::BeautifyPath(app.userPath + "/levels/auto"));
        } else if (w == scorefolder) {
            video_engine->SetFullscreen(false);
            ecl::ExploreFolder(ecl::BeautifyPath(app.userPath));
        } else if (w == credits) {
            displayInfo(credit_text);
        } else if (w == back) {
            Menu::quit();
        } else
            return;
        invalidate_all();
    }

    void MainHelpMenu::draw_background(ecl::GC &gc) {
        set_caption(_("Enigma - Help Menu"));
        blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
    }

    void MainHelpMenu::tick(double dtime) {
    }

    void MainHelpMenu::showPaths() {
        const char *pathtext[25];
        std::string pathstrings[25];
        std::string work;
        Font *menufont = enigma::GetFont("menufont");
        const VMInfo *vminfo = video_engine->GetInfo();
        int width = vminfo->width - 80;
        int i = 0;

        pathtext[i++] = N_("Preferences Path:");
        work = ecl::BeautifyPath(app.prefPath);
        do {
            std::string::size_type breakPos = breakString(menufont, work, ecl::PathSeparators, width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = " ";
        pathtext[i++] = N_("User Path:");
        work = ecl::BeautifyPath(app.userPath);
        do {
            std::string::size_type breakPos = breakString(menufont, work, ecl::PathSeparators, width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = " ";
        pathtext[i++] = N_("User Image Path:");
        work = ecl::BeautifyPath(app.userImagePath);
        do {
            std::string::size_type breakPos = breakString(menufont, work, ecl::PathSeparators, width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = " ";
        pathtext[i++] = N_("System Path:");
        work = ecl::BeautifyPath(app.systemFS->getDataPath());
        do {
            std::string::size_type breakPos = breakString(menufont, work, ecl::PathSeparators, width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = " ";
        pathtext[i++] = N_("Resource Paths:");
        work = ecl::BeautifyPath(app.resourceFS->getDataPath());
        do {
            std::string::size_type breakPos = breakString(menufont, work, ecl::PathSeparators, width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = " ";
        pathtext[i++] = N_("L10n Path:");
        work = ecl::BeautifyPath(app.l10nPath);
        do {
            std::string::size_type breakPos = breakString(menufont, work, ecl::PathSeparators, width);
            pathstrings[i] = "    " + work.substr(0,breakPos);
            pathtext[i] = pathstrings[i].c_str();
            work = work.substr(breakPos);
            i++;
        } while(!work.empty() );
        pathtext[i++] = 0;
        displayInfo(pathtext);
    }

    /* -------------------- Main menu -------------------- */
    MainMenu::MainMenu()
    {
        build_menu();
    }

    void MainMenu::build_menu()
    {
        const VMInfo *vminfo = video_engine->GetInfo();
        const int vshrink = vminfo->width < 640 ? 1 : 0;
        const int vsmall = vminfo->width < 800 ? 1 : 0;
        int y[] = {75, 170, 205, 220, 220}; // y[0] and y[1] are actually not used
        BuildVList  b(this, vshrink ? Rect(40, 120, 100, 25)
                           : vsmall ? Rect(80, 200, 200, 50)
                                    : Rect((vminfo->width - 160)/2, y[vminfo->tt], 160, 40),
                            vshrink ? 4 : vsmall ? 10 : 8);
        BuildVList br(this, vshrink ? Rect(180, 120, 100, 25)
                                    : Rect(360, 200, 200, 50),
                            vshrink ? 4 : vsmall ? 10 : 8);
        BuildVList *brp = vsmall ? &br : &b;
        startgame = b.add(new StaticTextButton(N_("Start Game"), this));
        levelpack = b.add(new StaticTextButton(N_("Level Pack"), this));
        m_netgame   = b.add(new StaticTextButton(N_("Network Game"), this));
        search      = b.add(new StaticTextButton(N_("Search"), this));
        options     = brp->add(new StaticTextButton(N_("Options"), this));
#if 0
        update      = brp->add(new StaticTextButton(N_("Update"), this));
#endif
        help        = brp->add(new StaticTextButton(N_("Help"), this));
        quit        = brp->add(new StaticTextButton(N_("Quit"), this));

        const int xoffset_upper = vminfo->width - 65;
        BuildHList l_upper(this, Rect(xoffset_upper, 10, 60, 40), 5);
        if(vshrink) {
            languagemenu = new BorderlessImageButton(
                string("translation_icon_shrink"),
                string("translation_icon_shrink_hl"),
                string("translation_icon_shrink_hl"),
                true, this);
            l_upper.add(languagemenu);
        } else {
            languagemenu = new BorderlessImageButton(
                string("translation_icon"),
                string("translation_icon_hl"),
                string("translation_icon_hl"),
                true, this);
            l_upper.add(languagemenu);
        }
    }

    void MainMenu::draw_background(ecl::GC &gc)
    {
        const VMInfo *vminfo = video_engine->GetInfo();

        set_caption(_("Enigma - Main Menu"));
        sound::StartMenuMusic();

        blit(gc, vminfo->mbg_offsetx, vminfo->mbg_offsety, enigma::GetImage("menu_bg", ".jpg"));

        Font *f = enigma::GetFont("levelmenu");
        Surface * logo(enigma::GetImage((vminfo->width < 640) ? "enigma_logo3_shrink" : "enigma_logo3"));
        int x0=(vminfo->width - logo->width())/2;
        int y0[] = {30, 57, 60, 70, 80};
#ifdef ENABLE_EXPERIMENTAL
        y0[1] = 57;  // might need adaptation when more buttons are added
#endif
        blit(gc, x0, y0[vminfo->tt], logo);
        f->render (gc, 5, vminfo->height - 20, app.getVersionInfo());
    }

    bool MainMenu::on_event (const SDL_Event &e) {
        switch (e.type) {
            case SDL_KEYDOWN:
                SDL_Keycode keysym = e.key.keysym.sym;
                switch (keysym) {
                case SDLK_F1:
                case SDLK_F2: {
                    MainHelpMenu m;
                    m.manage();
                    invalidate_all();
                    return true;
                }
                default:
                    break;
                }
                break;
        }
        return false;
    }

    void MainMenu::on_action(Widget *w)
    {
        if (w == startgame) {
            LevelPackMenu m;
            m.manageLevelMenu();
        } else if (w == levelpack) {
            LevelPackMenu m;
            m.manage();
        } else if (w == search) {
            SearchMenu m;
            m.manage();
            if (m.isSearchQuit()) {
                // show search result levelpack
                LevelMenu ml;
                if (ml.manage() && !ml.isMainQuit()) {
                    // ESC in LevelMenu in cade we are a submenu of LevelMenu or
                    // Main button has been pressed in LevelMenu
                    LevelPackMenu mlp;
                    mlp.manage();
                }
            }
            invalidate_all();
        } else if (w == options) {
            ShowOptionsMenu(0, false);
            clear();
            reset_active_widget();
            build_menu();
            invalidate_all();
        } else if (w == help) {
            MainHelpMenu m;
            m.manage();

        } else if (w == m_netgame) {
            ShowNetworkMenu();
        } else if (w == quit) {
            Menu::quit();
        } else if (w == languagemenu) {
            LanguageMenu m;
            m.manage();
            invalidate_all();
        } else
            return;
        invalidate_all();
    }

    void MainMenu::tick(double /* dtime */) {
    }

/* -------------------- Functions -------------------- */

    void ShowMainMenu() {
        MainMenu m;
        m.manage();
    }

    void ShowNetworkMenu()
    {
        NetworkMenu m;
        m.manage();
    }

}} // namespace enigma::gui
