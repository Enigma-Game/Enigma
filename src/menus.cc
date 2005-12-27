/*
 * Copyright (C) 2002,2003,2004,2005 Daniel Heck
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
 */
 
#include "menus.hh"
#include "gui/TextField.hh"
#include "client.hh"
#include "config.h"
#include "display.hh"
#include "editor.hh"
#include "help.hh"
#include "main.hh"
#include "netgame.hh"
#include "nls.hh"
#include "options.hh"
#include "oxyd.hh"
#include "server.hh"
#include "sound.hh"
#include "video.hh"
#include "world.hh"

#include <cassert>
#include <cctype>
#include <cstdlib>
#include <map>

using namespace enigma;
using namespace gui;
using namespace ecl;
using namespace std;

using levels::LevelStatus;
using levels::LevelInfo;
using levels::LevelPack;
using levels::LevelPacks;

#include "menus_internal.hh"

#ifdef _MSC_VER
#define snprintf _snprintf
#endif


/* -------------------- Helper routines -------------------- */

namespace
{
    /*! Change the video mode.  Because this opens a new screen with a
      new resolution, the display engine must be re-initialized to
      load the appropriate models. */
    void ChangeVideoMode()
    {
        world::PrepareLevel();      // make sure no references to models remain
        video::ChangeVideoMode();
        enigma::ClearImageCache();
        display::Shutdown();
        display::Init();
    }

    void SavePreview (std::string preview_name, ecl::Surface *s)
    {
        string preview_path;
        if (char *home = getenv ("HOME")) {
            preview_path = home;
            preview_path += "/.enigma/thumbs";
            if (!ecl::FolderExists (preview_path))
                ecl::FolderCreate (preview_path);
        }
        else {
            string preview_dir_path = FindDataFile("thumbs/README");
            ecl::split_path(preview_dir_path, &preview_path, 0);
        }
        
        string parent_dir;
        if (ecl::split_path(preview_path, &parent_dir, 0)) {
            preview_name = ecl::concat_paths(parent_dir, preview_name);
            enigma::Log << "Caching auto-preview as '" << preview_name << '\'' << endl;
            ecl::SavePNG (s, preview_name);
        }
    }

    /** Remove non-alphanumeric characters from `s'. */
    string remove_bad_chars(const string& s) 
    {
        string s2 = s;
        for (string::iterator c = s2.begin(); c != s2.end(); ++c) {
            if (!isalnum(*c)) *c = '_';
        }
        return s2;
    }

    string gen_preview_name(int revision, int preview_version, const string& name) 
    {
        return ecl::strf("thumbs/%i_%i_%s.png", revision, preview_version, name.c_str());
    }


    string MakePreviewName (const levels::Level &l)
    {
        levels::LevelPack *lp = l.get_levelpack();
        
        string cleanedName     = remove_bad_chars (l.get_info().filename);
        int    rev_number      = l.get_revision();
        int    preview_version = lp->get_preview_version();
        
        return gen_preview_name(rev_number, preview_version, cleanedName);
    }

    void remove_old_previews (const levels::Level &level)
    {
        levels::LevelPack *lp = level.get_levelpack();
        string name           = remove_bad_chars (level.get_info().filename);
        int    revision       = level.get_revision();
        int    preview_version = lp->get_preview_version();

        for (int r = revision; r >= 1; --r) {
            for (int v = preview_version; v >= 1; --v) {
                string preview_name = gen_preview_name(r, v, name);
                string found_name;
                if (FindFile(preview_name, found_name)) {
                    if (remove(found_name.c_str()) == -1) {
                        enigma::Log << "Error deleting outdated preview '" << found_name << '\'' << endl;
                    }
                    else {
                        enigma::Log << "Deleting outdated preview '" << found_name << '\'' << endl;
                    }
                }
            }
        }
    }
}

/* -------------------- LevelPreviewCache -------------------- */

LevelPreviewCache::LevelPreviewCache() 
: xsize(0), ysize(0) 
{}

LevelPreviewCache::~LevelPreviewCache() 
{ 
    release(); 
}

void LevelPreviewCache::release() 
{
    delete_map (cache.begin(), cache.end());
}

void LevelPreviewCache::clear() 
{ 
    release(); 
    cache.clear(); 
    imgCache.clear();
}

void LevelPreviewCache::set_size(int xs, int ys)
{
    xsize = xs; 
    ysize = ys; 
}

Surface *LevelPreviewCache::getPreview(const levels::Level &level)
{
    PreviewMap::iterator i = cache.find(level.uniqueName());
    return  (i != cache.end()) ? i->second->surface : 0;
}

Surface *LevelPreviewCache::makePreview(const levels::Level &level)
{
    if (Surface *s = getPreview (level))
        return s;

    assert(xsize != 0 && ysize != 0); // forgot to call set_size() ?

    CacheElem *ce = make_cache_elem (level);
    if (ce)
        cache[level.uniqueName()] = ce;
    return ce->surface;
}

ecl::Surface *LevelPreviewCache::newPreview (const Level &level)
{
    Surface *surface = 0;
    ecl::GC gc(video::BackBuffer());
    if (game::DrawLevelPreview (gc, level)) { 
        surface = Resample (video::BackBuffer(), 
                            video::GetInfo()->gamearea, 
                            xsize, ysize); 
    }
    return surface;
}

Surface *LevelPreviewCache::updatePreview (const levels::Level &level)
{
    if (Surface *surface = newPreview (level)) 
    {
        std::string idx = level.uniqueName();
        PreviewMap::iterator i = cache.find (idx);
        if (i != cache.end())
            delete i->second;
        cache[idx] = new CacheElem (surface, idx);

        string preview_name = MakePreviewName (level);
        imgCache.store (preview_name, surface);
        remove_old_previews (level);
        SavePreview (preview_name, surface);
        return surface;
    }
    return 0;
}


LevelPreviewCache::CacheElem *
LevelPreviewCache::make_cache_elem (const levels::Level &level)
{
    string preview_name = MakePreviewName (level);
    Surface *surface = 0;

    string preview_path;
    if (FindFile(preview_name, preview_path))
        surface = imgCache.get(preview_path);

    if (!surface) {
        surface = newPreview (level);
        if (surface) {
            imgCache.store (preview_name, surface);
            remove_old_previews (level);
            SavePreview (preview_name, surface);
        }
        else 
            surface = enigma::GetImage ("error");
    }

    if (surface)
        return new CacheElem (surface, level.uniqueName());
    return 0;
}


/* -------------------- Level Pack Menu -------------------- */

LevelPackMenu::LevelPackMenu()
    : m_selection(-1)
{
#define PACKMODES 3
    struct ppm { int rectw, recth; } packmodeparam[PACKMODES] = {
        { 160, 40 },            // up to 3*10 = 30 levelpacks (640x480)
        { 150, 30 },            // up to 4*13 = 52 levelpacks
        { 120, 25 },            // up to 5*15 = 75 levelpacks
    };

    unsigned packcount = LevelPacks.size();

    for (unsigned i=0; i<packcount; ++i) {
        LevelPack *lp = LevelPacks[i];
        buttons.push_back(new UntranslatedStaticTextButton(lp->get_name(), this));
    }

    Rect buttonarea = ecl::Screen::get_instance()->size();
    bool finished   = false;

    for (int mode = 0; !finished && mode < PACKMODES; ++mode) {
        ppm&          ppm = packmodeparam[mode];
        VTableBuilder builder(this, buttonarea, Rect(0,0,ppm.rectw,ppm.recth), 5, 5);

        finished = builder.finish(buttons);
    }

    Assert <XGeneric> (finished, strf("LevelpackMenu Layout error (too many levelpacks: %i - try in higher resolution)", packcount));
#undef PACKMODES
}

void LevelPackMenu::on_action(Widget *w) {
    for (unsigned i=0; i<buttons.size(); ++i)
        if (buttons[i]==w) {
            m_selection = i;
            Menu::quit();
        }
}

void LevelPackMenu::draw_background(ecl::GC &gc) {
    video::SetCaption(("Enigma - Level Pack Menu"));
    blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
}


/* -------------------- LevelWidget -------------------- */

LevelWidget::LevelWidget(LevelPack *lp, LevelPreviewCache &cache_)
: preview_cache (cache_),
  level_pack(lp),
  ifirst(0), iselected(0),
  width (0),
  height (0),
  m_areas(),
  listener(0)
{
    const video::VMInfo &vminfo = *video::GetInfo();

    buttonw = vminfo.thumbw + 20;
    buttonh = vminfo.thumbh + 28;
}

void LevelWidget::realize (const ecl::Rect &area_)
{
    Widget::realize (area_);
    width = area_.w / buttonw;
    height = area_.h / buttonh;
}

void LevelWidget::trigger_action() {
    cache.clear();
    if (listener) {
        listener->on_action(this);
        recalc_available();
    }
}

LevelMenu * LevelWidget::get_menu() {
    if (Widget *m = Widget::get_parent())
        return dynamic_cast<LevelMenu*>(m);
    else {
        assert(0);
        return 0;
    }
}

void LevelWidget::show_text(const string& text) {
    get_menu()->show_text(text);
}

void LevelWidget::scroll_up (int nlines) 
{
    for (; nlines; --nlines) {
        if (ifirst+width*height >= level_pack->size())
            break;
        ifirst += width;
        if (iselected < ifirst)
            iselected += width;
    }
    invalidate();
}

void LevelWidget::scroll_down(int nlines) 
{
    for (; nlines; --nlines) {
        if (ifirst < width)
            break;
        ifirst -= width;
        if (iselected >= ifirst+width*height)
            iselected -= width;
    }
    invalidate();
}

void LevelWidget::page_up() {
    set_selected ((ifirst >= width*height ? ifirst - width*height : 0), 
            (iselected >= width*height ? iselected - width*height : 0));
}

void LevelWidget::page_down() 
{
    unsigned s = level_pack->size();
    size_t lastPage = (s >= width*height ? (s / width - height) * width : 0);

    // make sure last page is shown as a whole
    int first = std::min<int> (lastPage, ifirst + width*height);
    //    set_selected (first, s-1);
    set_selected (first, iselected + width*height);
}

void LevelWidget::start() { 
    set_selected (0,0); 
}

void LevelWidget::end() 
{
    unsigned s = level_pack->size();
    size_t lastPage = (s >= width*height ? (s / width - height) * width : 0);
    set_selected (lastPage, s-1);
}

void LevelWidget::recalc_available() {
    max_available = HighestAvailableLevel(level_pack);
}

void LevelWidget::change_levelpack (LevelPack *lp) {
    iselected  = 0;
    ifirst     = 0;
    level_pack = lp;
    oxyd::ChangeSoundset(options::GetInt("SoundSet"), level_pack->get_default_SoundSet());
    cache.clear();
    preview_cache.clear();
    recalc_available();
    invalidate();
    sound::SoundEvent ("menumove");
}

void LevelWidget::next_unsolved() 
{
    Level level (level_pack, iselected);

    if (levels::advance_level (level, levels::advance_unsolved))
        set_current(level.get_index());
    else
        show_text(_("No further unsolved level available!"));
}

Surface *LevelWidget::get_preview_image (const Level &level)
{
    Surface *img_error       = enigma::GetImage("error");

    const LevelInfo &levelinfo = level.get_info();

    Surface *img = 0;
    if (level_pack->may_have_previews()) {
        string fname = string("levels/") + levelinfo.filename + ".png";
        if (enigma::FindFile (fname, fname))
            img = cache.get(fname);
    }
    if (!img) {
        img = preview_cache.makePreview (level);
        if (img == 0)
            img = img_error;
    }
    return img;
}

void LevelWidget::draw_level_preview (ecl::GC &gc, const Level &level, int x, int y)
{
    Surface *img_easy        = enigma::GetImage("completed-easy");
    Surface *img_hard        = enigma::GetImage("completed");
    Surface *img_changed     = enigma::GetImage("changed");
    Surface *img_unavailable = enigma::GetImage("unavailable");
//    Surface *img_unknown     = enigma::GetImage("unknown");
    Surface *img_par         = enigma::GetImage("par");
    Surface *img_border      = enigma::GetImage("thumbborder");

    const LevelInfo &levelinfo = level.get_info();

    // Draw button with level preview

    Surface *img = get_preview_image(level);
//        img = img_unknown;

    if (level.get_index() == (unsigned) iselected) {
        blit (gc, x-4, y-4, img_border);
        blit (gc, x, y, img);
    }
    else {
        img->set_alpha (127);
        blit (gc, x, y, img);
        img->set_alpha(255);
    }

    // Draw additional icons

    int  finished      = 0;
    bool level_changed = false;
    bool par_reached   = false;

    LevelStatus levelstat;
    if (level.get_status(levelstat)) {
        finished = levelstat.finished;
        if (finished) {
            level_changed = level.get_revision() > levelstat.solved_revision;

            int par_time = level.get_par_time (options::GetDifficulty());
            int best_user_time = level.get_best_user_time (options::GetDifficulty());
            par_reached = best_user_time>0 && (par_time<0 || best_user_time <= par_time);
        }
    }

    // Shade unavailable levels
    if (LevelIsLocked(level))
        blit (gc, x, y, img_unavailable);

    // Draw solved/changed icons on top of level preview
    if (finished) {
        if (!levelinfo.has_easymode)
            finished = DIFFICULTY_EASY | DIFFICULTY_HARD;
        if (options::GetDifficulty () == DIFFICULTY_HARD) {
            if (finished & DIFFICULTY_EASY)
                blit (gc, x, y, img_easy);
            if (finished & DIFFICULTY_HARD)
                blit (gc, x+5, y, img_hard);
        }
        else {
            if (finished & DIFFICULTY_HARD)
                blit (gc, x+5, y, img_hard);
            if (finished & DIFFICULTY_EASY)
                blit (gc, x, y, img_easy);
        }
    }

    // Add warning sign if level has been changed since player solved it
    if (level_changed)
        blit (gc, x-3, y-3, img_changed);

    // Add time icon if par has been reached
    if (par_reached)
        blit(gc, x+30, y+12, img_par);

}

struct LevelWidgetConfig {
    int imgw, imgh;             // size of preview images
    int imgpadx, imgpady;       // padding of image buttons
    const char *fontname;
};

void LevelWidget::draw (ecl::GC &gc, const ecl::Rect &r)
{
    const video::VMInfo &vminfo = *video::GetInfo();
    const int imgw = vminfo.thumbw;       // Size of the preview images
    const int imgh = vminfo.thumbh;

    const int hgap = Max(0, (get_w() - width*buttonw) / (width-1));
    const int vgap = Max(0, (get_h() - height*buttonh)/ (height-1));

    unsigned i=ifirst;          // level index

    for (int y=0; y<height; y++)
    {
        for (int x=0; x<width; x++, i++)
        {
            if (i >= level_pack->size())
                goto done_painting;

            int xpos = get_x() + x*(buttonw + hgap);
            int ypos = get_y() + y*(buttonh + vgap);

            Rect buttonarea(xpos, ypos, buttonw, buttonh);
            if (!(r.overlaps(buttonarea) || r.w == 0))
                continue;       // r.w==0 if repainting whole screen

            if( (i-ifirst) >= m_areas.size())
                m_areas.push_back(buttonarea);
            else
                m_areas[(i-ifirst)] = buttonarea;

            // Draw level preview
            Level level (level_pack, i);
            int imgx = xpos+(buttonw-imgw)/2;
            int imgy = ypos + 4;
            draw_level_preview (gc, level, imgx, imgy);
                                
            // Draw level name
            Font    *smallfnt = enigma::GetFont("levelmenu");
            const char *caption = level.get_info().name.c_str();
            smallfnt->render (gc,
                              xpos + (buttonw-smallfnt->get_width(caption))/2,
                              imgy + imgh,
                              caption);
        }
    }
  done_painting:
    m_areas.resize (i-ifirst); // Remove unused areas (if any) from the list
    return;
}

void LevelWidget::set_current (size_t newsel)
{
    set_selected (ifirst, newsel);
}

void LevelWidget::set_selected (size_t newfirst, size_t newsel)
{
    // check for implicit argument convertions of negativ integers to unsigned
    assert(newfirst < 1000000);
    assert(newsel < 1000000);
    
    size_t numlevels = level_pack->size();
    newsel = Clamp<size_t> (newsel, 0, numlevels-1);

    if (newsel < newfirst)
        newfirst = (newsel/width)*width;
    if (newsel >= newfirst+width*height)
        newfirst = (newsel/width-height+1)*width;

    newfirst = Clamp<size_t> (newfirst, 0, numlevels-1);

    size_t oldsel = iselected;
    if (newfirst != ifirst) {
        ifirst    = newfirst;
        iselected = newsel;

        if (!m_areas.empty()) {
            sound::SoundEvent ("menumove");
            if (oldsel != newsel) 
                sound::SoundEvent ("menuswitch");
            invalidate();
        }
    }
    else if (newsel != iselected) {
        iselected = newsel;

        if (!m_areas.empty()) {
            sound::SoundEvent ("menuswitch");
            invalidate_area(m_areas[oldsel-ifirst]); // old selection
            invalidate_area(m_areas[iselected-ifirst]); // new selection
        }
    }
}

bool LevelWidget::on_event(const SDL_Event &e) 
{
    bool handled = false;

    switch (e.type) {
    case SDL_MOUSEMOTION:
        if (get_area().contains(e.motion.x, e.motion.y)) {
            int newsel=iselected;
            for (unsigned i=0; i<m_areas.size(); ++i)
                if (m_areas[i].contains(e.motion.x, e.motion.y))
                {
                    newsel = ifirst+i;
                    break;
                }
            set_current(newsel);
            handled = true;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (get_area().contains(e.button.x, e.button.y))
            handled = handle_mousedown (&e);
        break;
    case SDL_KEYDOWN:
        handled = handle_keydown (&e);
        break;
    }
    return handled;
}

bool LevelWidget::handle_mousedown (const SDL_Event *e) 
{
    switch (e->button.button) {
    case SDL_BUTTON_LEFT:
        for (unsigned i=0; i<m_areas.size(); ++i)
            if (m_areas[i].contains(e->button.x, e->button.y))
            {
                sound::SoundEvent ("menuok");
                iselected = ifirst+i;
                trigger_action();
                return true;
            }
        break;
    case SDL_BUTTON_RIGHT:
        break;
    case 4: scroll_down(1); return true;
    case 5: scroll_up(1); return true;
    }
    return false;
}

static const char *helptext_levelmenu[] = {
    N_("Escape:"),              N_("Skip to main menu"),
    "F1:",                      N_("Show this help"),
    "F5:",                      0, // see below
    N_("Arrows:"),              N_("Select level"),
    N_("Return:"),              N_("Play selected level"),
    N_("Back/Space:"),          N_("Previous/next levelpack"),
    "u",                        N_("Mark current level as Unsolved"),
//    "s",                        N_("Mark current level as Solved"),
    N_("Alt+Return:"),          N_("Switch between fullscreen and window"),
    0
};

bool LevelWidget::handle_keydown (const SDL_Event *e)
{
    Level level(level_pack, iselected);
    switch (e->key.keysym.sym) {
    case SDLK_u:
        level.mark_unsolved(); 
        invalidate(); 
        break;

    case SDLK_s:
        if (enigma::WizardMode) {
            level.mark_solved();
            invalidate();
        }
        break;

    case SDLK_t:
        // Generate new level preview for current level
        preview_cache.updatePreview (level);
        invalidate();
        break;

    case SDLK_LESS:
    case SDLK_MINUS:
        if  (level_pack->swap (iselected, iselected-1)) {
            set_current (iselected-1);
            invalidate();
        }
        break;

    case SDLK_GREATER:
    case SDLK_PLUS:
        if (level_pack->swap (iselected, iselected+1)) {
            set_current (iselected+1);
            invalidate();
        }
        break;

    case SDLK_LEFT:  set_current (iselected-1); break;
    case SDLK_RIGHT: set_current (iselected+1); break;
    case SDLK_DOWN:  set_current (iselected+width); break;
    case SDLK_UP:    set_current (iselected-width); break;
    case SDLK_PAGEDOWN: page_down(); break;
    case SDLK_PAGEUP: page_up(); break;
    case SDLK_HOME: start(); break;
    case SDLK_END: end(); break;

    case SDLK_F1: {
        if (options::GetBool("TimeHunting"))
            helptext_levelmenu[5] = N_("Select next non-par level");
        else
            helptext_levelmenu[5] = N_("Select next unsolved level");

        displayHelp(helptext_levelmenu, 200);
        get_menu()->draw_all();
        break;
    }
    case SDLK_F5:
        next_unsolved();
        break;

    case SDLK_RETURN:
        trigger_action();
        break;

    default:
        return false;           // key not handled
    }
    return true;
}

int LevelWidget::get_position() const 
{
    return (ifirst << 8) | (iselected-ifirst);
}

void LevelWidget::set_position(int pos) 
{
    int new_first = pos >> 8;
    int off       = pos & 0xff;
    int new_sel   = new_first+off;
    if (new_sel > width*height)
        set_current (new_sel);
    else
        set_selected(new_first, new_sel);
}



/* -------------------- Buttons for Options -------------------- */

namespace
{
    class MouseSpeedButton : public ValueButton {
        int get_value() const     { 
            return ecl::round_nearest<int>(options::GetMouseSpeed());
        }
        void set_value(int value) { 
            options::SetMouseSpeed (value);
        }

        string get_text(int value) const  {
            return strf("%d", value);
        }
    public:
        MouseSpeedButton()
        : ValueButton(options::MIN_MouseSpeed, options::MAX_MouseSpeed)
        { init(); }
    };

    class SoundVolumeButton : public ValueButton {
        int get_value() const     { 
            return round_nearest<int>(options::GetDouble("SoundVolume")*10.0); 
        }
        void set_value(int value) {
            options::SetOption("SoundVolume", value/10.0);
            options::UpdateVolume();
        }

        string get_text(int value) const {
            if (value == 0) {
                return _("muted");
            }
            else {
                return strf("%d", value);
            }
        }
    public:
        SoundVolumeButton() : ValueButton(0, 10) { init(); }
    };

    class MusicVolumeButton : public ValueButton {
        int get_value() const { 
            return round_nearest<int> (options::GetDouble("MusicVolume")*10.0); 
        }
        void set_value(int value) {
            options::SetOption("MusicVolume", value/10.0);
            options::UpdateVolume();
        }

        string get_text(int value) const {
            if (value == 0)
                return _("muted");
            else
                return strf("%d", value);
        }
    public:
        MusicVolumeButton() : ValueButton(0, 10) { init(); }
    };

    class InGameMusicButton : public BoolOptionButton {
        void on_action(Widget *) {
            if (toggle())
                sound::PlayMusic (options::GetString("LevelMusicFile"));
            else
                sound::StopMusic (options::GetString("LevelMusicFile"));
        }
    public:
        InGameMusicButton() :
            BoolOptionButton("InGameMusic", N_("Music in game"), N_("No music in game"), this)
        { }
    };

    struct SkipSolvedButton : public BoolOptionButton {
        SkipSolvedButton() : BoolOptionButton("SkipSolvedLevels", N_("Yes"), N_("No"), this) {}
    };

    struct TimeHuntButton : public BoolOptionButton {
        TimeHuntButton() : BoolOptionButton("TimeHunting", N_("Yes"), N_("No"), this) {}
    };

    class VideoModeButton : public TextButton {

        video::VideoModes get_mode() const {
            int mode = Clamp(options::GetInt("VideoMode"), 0, int(video::VM_COUNT));
            return static_cast<video::VideoModes>(mode);
        }
        string get_text() const {
            return GetInfo(get_mode())->name;
        }
        void on_action(Widget *) {
            int mode = get_mode();

            // cycle at most once through all available video modes
            do {
                mode += 1;
                if (mode >= video::VM_COUNT) 
                    mode = 0;

                const video::VMInfo *vminfo = GetInfo (static_cast<video::VideoModes>(mode));
                if (vminfo->available) {
                    options::SetOption("VideoMode", mode);
                    invalidate();
                    break;
                }
            } while (mode != get_mode());
        }
    public:
        VideoModeButton() : TextButton(this) { }
    };
}


/* -------------------- SoundSetButton -------------------- */

SoundSetButton::SoundSetButton()
: ValueButton(0, int(OxydLib::OxydVersion_PerOxyd)+2)
{
    init();
}

bool SoundSetButton::hasSoundSet(int value) {
    if (value<2) return true;
    return oxyd::FoundOxyd(OxydLib::OxydVersion(value-2));
}

int SoundSetButton::get_value() const 
{
    return options::GetInt("SoundSet"); 
}

void SoundSetButton::set_value(int value) {
    int old  = options::GetInt("SoundSet");
    options::SetOption("SoundSet", value);
    if (!hasSoundSet(options::GetInt("SoundSet"))) {
        if (old < options::GetInt("SoundSet")) 
            inc_value(1);
        else 
            inc_value(-1);
        return;
    }
    oxyd::ChangeSoundset(options::GetInt("SoundSet"), -1);
}

string SoundSetButton::get_text(int value) const 
{
    using namespace OxydLib;

    string sound_set;
    switch (value) {
    case 0: sound_set = _("Default"); break;
    case 1: sound_set = "Enigma"; break;
    default :
        switch (OxydVersion(options::GetInt("SoundSet")-2)) {
        case OxydVersion_Oxyd1:          sound_set = "Oxyd"; break;
        case OxydVersion_OxydMagnum:     sound_set = "Magnum"; break;
        case OxydVersion_OxydMagnumGold: sound_set = "Mag.Gold"; break;
        case OxydVersion_OxydExtra:      sound_set = "Extra"; break;
        case OxydVersion_PerOxyd:        sound_set = "Per.Oxyd"; break;
        default :
            fprintf(stderr, "Invalid soundset %i\n", value);
            break;
        }
        break;
    }
    return sound_set;
}


/* -------------------- StereoButton -------------------- */

StereoButton::StereoButton() : ValueButton(-1, 1)
{
    init();
}

int StereoButton::get_value() const 
{
    double separation = options::GetDouble("StereoSeparation");
    if (separation == 0)
        return 0;
    else
        return (separation > 0) ? 1 : -1; 
}
void StereoButton::set_value(int value)  
{
    if (value == 0) 
        options::SetOption("StereoSeparation", 0.0);
    else if (value > 0) 
        options::SetOption("StereoSeparation", 10.0);
    else 
        options::SetOption("StereoSeparation", -10.0);
}

string StereoButton::get_text(int value) const 
{
    switch (value) {
    case -1: return _("reversed");
    case 0: return _("mono");
    case 1: return _("normal");
    }
    assert(0);
    return string();
}


/* -------------------- FullscreenButton -------------------- */

FullscreenButton::FullscreenButton()
    : BoolOptionButton("FullScreen", N_("Yes"), N_("No"), this)
{
}

/* -------------------- DifficultyButton -------------------- */

DifficultyButton::DifficultyButton() : TextButton(this) {
}

string DifficultyButton::get_text() const 
{
    if (options::GetDifficulty() == DIFFICULTY_EASY)
        return _("Difficulty: Easy");
    else
        return _("Difficulty: Normal");
}

void DifficultyButton::on_action(Widget *) 
{
    int newdifficulty = (DIFFICULTY_EASY+DIFFICULTY_HARD) - options::GetDifficulty();
    options::SetOption("Difficulty", newdifficulty);
    options::MustRestartLevel = true;
    invalidate();
}

/* -------------------- TournamentButton -------------------- */

TournamentButton::TournamentButton()
    : BoolOptionButton("Tournament", N_("Tournament"), N_("Training"), this)
{
}

/* -------------------- LanguageButton -------------------- */

struct Language {
    const char *name;
    const char *localename;
};

Language languages[] = {
    { "default",                "" },
    { "Deutsch",                "de_DE" },
    { "English",                "en_EN" },
    { "Español",         "es_ES" },
    { "Français",        "fr_FR" },
    { "Italiano",               "it_IT" },
    { "Nederlands",             "nl_NL" },
};

int LanguageButton::get_value() const
{
    string localename; //  = ecl::DefaultMessageLocale ();
    options::GetOption ("Language", localename);

    int lang = 0;                  // unknown language
    for (size_t i=0; i<NUMENTRIES(languages); ++i) {
        if (localename == languages[i].localename)
            lang = int(i);
    }
    return lang;
}

void LanguageButton::set_value(int value)
{
    options::SetOption ("Language", languages[value].localename);
    
    if ( not inInit) {
        // change language only on user action
        app.setLanguage(languages[value].localename);
        myListener->on_action(this);
    }
}

string LanguageButton::get_text(int value) const
{
    if (value == -1)
        return _("unknown");
    else
        return languages[value].name;
}

LanguageButton::LanguageButton (ActionListener *al)
: ValueButton(0, NUMENTRIES(languages)-1), myListener(al)
{
    inInit = true;
    init();
    inInit = false;
}

/* -------------------- GammaButton -------------------- */


GammaButton::GammaButton()
: ValueButton(1, 10)
{
    init();
}

void GammaButton::set_value(int value) 
{ 
    double gamma = double(value) / 5.0;
    options::SetOption ("Gamma", gamma);
    video::UpdateGamma();
}

int GammaButton::get_value() const
{ 
    double gamma = options::GetDouble ("Gamma");
    int value = round_down<int>(gamma * 5.0);
    return value;
}

string GammaButton::get_text(int value) const
{
    return ecl::strf ("%d", value-5);
}



/* -------------------- Options Menu -------------------- */

OptionsMenu::OptionsMenu(ecl::Surface *background_)
: back(new StaticTextButton(N_("Back"), this)),
  fullscreen(new FullscreenButton),
  m_restartinfo (new Label("")),
  background(background_),
  previous_caption(video::GetCaption())
{
    const int spacing     = 5;
    const int big_spacing = 60;
    const int label_width = 180;
    const int but_width   = 100;
    const int but_height  = 30;

    BuildVList leftlabels (this, Rect(-label_width, 0, label_width, but_height), spacing);
    BuildVList left (this, Rect(0, 0, but_width, but_height), spacing);
    BuildVList rightlabels (this, Rect(but_width+big_spacing, 0, label_width, but_height), spacing);
    BuildVList right(this, Rect(but_width+big_spacing+label_width, 0, but_width, but_height), spacing);
    leftlabels.add (new Label(N_("Language: "), HALIGN_RIGHT));
    leftlabels.add (new Label(N_("Fullscreen: "), HALIGN_RIGHT));
    leftlabels.add (new Label(N_("Video mode: "), HALIGN_RIGHT));
    leftlabels.add (new Label(N_("Gamma correction: "), HALIGN_RIGHT));
    leftlabels.add (new Label(N_("Mouse speed: "), HALIGN_RIGHT));
    leftlabels.add (new Label(N_("Skip solved levels: "), HALIGN_RIGHT));
    leftlabels.add (new Label(N_("Time hunt: "), HALIGN_RIGHT));

    language = new LanguageButton(this);
    left.add (language);
    left.add (fullscreen);
    left.add (new VideoModeButton);
    left.add (new GammaButton);
    left.add (new MouseSpeedButton);
    left.add (new SkipSolvedButton);
    left.add (new TimeHuntButton);

    rightlabels.add (new Label(N_("Sound volume: "), HALIGN_RIGHT));
    rightlabels.add (new Label(N_("Sound set: "), HALIGN_RIGHT));
    rightlabels.add (new Label(N_("Music volume: "), HALIGN_RIGHT));
    rightlabels.add (new Label(N_("Stereo: "), HALIGN_RIGHT));

    right.add (new SoundVolumeButton);
    right.add (new SoundSetButton);
    right.add (new MusicVolumeButton);
//    right.add (new InGameMusicButton);
    right.add (new StereoButton);
//    right.add (new TextField());
    
    {
        Rect l = left.pos();
        Rect r = right.pos();

        add (m_restartinfo, Rect (l.x, l.y + 15, 400, 20));
        m_restartinfo->set_alignment (HALIGN_LEFT);
        update_info();

        l.x = (l.x+r.x)/2;
        l.y = Max(l.y, r.y)+big_spacing;

        add(back, l);
    }
}

OptionsMenu::~OptionsMenu() {
    video::SetCaption(previous_caption.c_str());
}

void OptionsMenu::update_info() 
{
    if (options::MustRestart)
        m_restartinfo->set_text (
            N_("Please restart Enigma to activate your changes!"));
    else
        m_restartinfo->set_text ("");
}

bool OptionsMenu::on_event (const SDL_Event &e)
{
    bool handled=false;
    if (e.type == SDL_MOUSEBUTTONDOWN
        && e.button.button == SDL_BUTTON_RIGHT)
    {
        Menu::quit();
        handled = true;
    }
    else if (e.type == SDL_KEYUP) {
        if ((e.key.keysym.sym==SDLK_RETURN) &&
            (e.key.keysym.mod & KMOD_ALT))
        {
            // update state of FullscreenButton :
            fullscreen->invalidate();
            handled = true;
        }
    }
    return handled;
}

void OptionsMenu::on_action(Widget *w)
{
    if (w == back)
        Menu::quit();
    else if (w == language)
        // language changed - retranslate and redraw everything
        invalidate_all();
}

void OptionsMenu::tick (double)
{
    update_info();
}

void OptionsMenu::draw_background(ecl::GC &gc)
{
    video::SetCaption(("Enigma - Options Menu"));
//     blit(gc, 0,0, enigma::GetImage("menu_bg"));
    blit(gc, 0,0, background);
}


/* -------------------- Level Menu -------------------- */

struct LevelMenuConfig {
    int buttonw, buttonh;
    int lbuttonw, lbuttonh;
    ecl::Rect previewarea;
    int thumbsy;                // y coordinate of thumbnail window
    int leftborder;

    LevelMenuConfig (const ecl::Rect &screen)
    : buttonw (140), buttonh (35),
      lbuttonw (140), lbuttonh (100),
      previewarea (10, 60, screen.w-50, screen.h-130),
      thumbsy (60),
      leftborder (10)
    {}
};

LevelMenu::LevelMenu(LevelPack *lp, unsigned long pos)
: but_unsolved   (new ImageButton("ic-unsolved", "ic-unsolved1", this)), 
  but_tournament (new TournamentButton),
  but_back       (new StaticTextButton(N_("Back"), this)),
  but_difficulty (new DifficultyButton),
  but_levelpack  (new StaticTextButton(N_("Level Pack"), this)),
  lbl_lpinfo     (new Label("")),
  lbl_statistics (new Label("")),
  lbl_levelname  (new Label("", HALIGN_LEFT)),
  lbl_levelinfo  (new Label("", HALIGN_LEFT)),
  level_pack  (lp),
  m_ilevelpack (),
  shown_text_ttl(-1.0)
{
    HList *hl;

    const video::VMInfo &vminfo = *video::GetInfo();

    // Set size of preview images
    preview_cache.set_size(vminfo.thumbw, vminfo.thumbh);

    // Levelmenu configuration
    const int Y2 = 10;          // y position for information area
    const int Y3 = vminfo.height-50; // y position for bottom button row
    LevelMenuConfig c (Rect (0, 0, vminfo.width, vminfo.height));

    but_difficulty->set_listener (this);


    // Create buttons
    hl = new HList;
    hl->set_spacing (10);
    hl->set_alignment (HALIGN_CENTER, VALIGN_TOP);
    hl->set_default_size (c.buttonw, c.buttonh);
    hl->add_back (but_levelpack);
    hl->add_back (but_unsolved);
    hl->add_back (but_difficulty);
    hl->add_back (but_back);
    this->add (hl, Rect(c.leftborder, Y3, vminfo.width-10, c.buttonh));
    
//     BuildHList hlist1(this, Rect(c.leftborder, Y3, c.buttonw, c.buttonh), 10);
//     hlist1.add (but_levelpack);
// //    hlist1.add (but_tournament);
//     hlist1.add (but_unsolved);
//     hlist1.add (but_difficulty);
//     but_difficulty->set_listener(this);
// //        hlist1.add (newgame);
//     hlist1.add (but_back);

    // Add navigation buttons
    pgup     = new ImageButton("ic-up", "ic-up1", this);
    pgdown   = new ImageButton("ic-down", "ic-down1", this);
    start    = new ImageButton("ic-top", "ic-top1", this);
    end      = new ImageButton("ic-bottom", "ic-bottom1", this);

    Rect r(vminfo.width-30, c.thumbsy, 20, 50);
    r.y = c.thumbsy;
    add (pgup, r);
    r.y += 60;
    add (pgdown, r);
    r.y = c.thumbsy + 240;
    add (start, r);
    r.y += 60;
    add (end, r);

    // Information area
    hl = new HList;
    hl->add_back (lbl_levelname, List::EXPAND);
    hl->add_back (lbl_lpinfo, List::TIGHT);
    this->add (hl, Rect (5, Y2, vminfo.width - 10, 28));

    hl = new HList;
    hl->add_back (lbl_levelinfo, List::EXPAND); //Rect (c.leftborder, Y2+20,305, 28));
    hl->add_back (lbl_statistics, List::TIGHT);
    this->add (hl, Rect (5, Y2+20, vminfo.width - 10, 28));

    // Prepare level selection widget
    levelwidget = new LevelWidget(lp, preview_cache);
    levelwidget->set_listener(this);
    levelwidget->realize (c.previewarea);
    levelwidget->set_area (c.previewarea);

    this->add (levelwidget);

    set_position(pos);
}

void LevelMenu::tick(double dtime) 
{
    static double timeaccu = 0.0;

    // info texts disappear after some time
    if (shown_text_ttl>0.0) {
        shown_text_ttl -= dtime;
        if (shown_text_ttl <= 0.0)
            shown_text = "";
    }
    timeaccu += dtime;
    if (timeaccu > 0.1) {
        update_info();
        timeaccu = 0.0;
    }

    // automatically create thumbnails in the background
//     if (int idx = levelwidget->missing_thumbnail()) {

//         invalidate_all();
//     }

}

bool LevelMenu::on_event (const SDL_Event &e) 
{
    // Pass all events to the level widget first
    bool handled=levelwidget->on_event(e);

    if (!handled) {
        if (e.type == SDL_KEYDOWN) {
            handled=true;
            switch (e.key.keysym.sym) {
            case SDLK_SPACE: next_levelpack(); break;
            case SDLK_BACKSPACE: previous_levelpack(); break;
            default: handled=false; break;
            }
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN
                 && e.button.button == SDL_BUTTON_RIGHT)
        {
            Menu::quit();
            handled=true;
        }
        else
            handled = Menu::on_event (e);
    }
    return handled;
}

void LevelMenu::on_action(Widget *w) 
{
    if (w==levelwidget) {
        int ilevel = levelwidget->selected_level();
        LevelPack *lp = LevelPacks[m_ilevelpack];

        if ((unsigned)ilevel < lp->size()) {
            Level level (lp, ilevel);
            if (!LevelIsLocked (level)) {
                game::StartGame(lp, ilevel);
                if (levels::IsHistory(lp))
                    // the played level is now the first in history
                    ilevel = 0;
                else
                    ilevel = server::CurrentLevel;
                if (lp != server::CurrentLevelPack) {
                    set_levelpack(IndexOfLevelPack(server::CurrentLevelPack));
                }
                invalidate_all();
                levelwidget->set_current(ilevel);
            }
            else
                show_text(_("You are not allowed to play this level yet."));
        }
    } else if (w == but_back) {
        Menu::quit();
    } else if (w == pgup) {
        levelwidget->page_up();
    } else if (w == pgdown) {
        levelwidget->page_down();
    } else if (w == start) {
        levelwidget->start();
    } else if (w == end) {
        levelwidget->end();
    } else if (w == but_unsolved) {
        levelwidget->next_unsolved();
    } else if (w == but_levelpack) {
        LevelPackMenu lpm;
        lpm.center();
        if (lpm.manage()) {
            set_levelpack(lpm.get_selection());
        }
        invalidate_all();
    } else if (w == but_difficulty) {
        but_difficulty->on_action(w);
        invalidate_all();
    }
}

void LevelMenu::update_info()
{
    char txt[200];
    LevelPack *lp  = LevelPacks[m_ilevelpack];
    Level level (lp, levelwidget->selected_level());
    int difficulty = options::GetDifficulty();

    if (lp->size() == 0) {
        // empty level pack
        lbl_statistics->set_text ("-");
        lbl_levelname->set_text ("-");
        lbl_levelinfo->set_text ("-");
    }
    else {
        int iselected = levelwidget->selected_level();

        // Display levelpack statistics (percentage of solved levels)
        unsigned numparhold;
        int      numsolved = CountSolvedLevels (lp, difficulty, &numparhold);

        if (options::GetBool("TimeHunting")) {
            int pct = 100*numparhold / lp->size();
            snprintf (txt, sizeof(txt), _("%d%% par"), pct);
        }
        else {
            int pct = 100*numsolved / int (lp->size());
            snprintf (txt, sizeof(txt), _("%d%% solved"), pct);
        }
        lbl_statistics->set_text(txt);

        const LevelInfo &li = level.get_info();

	// Display level name
        if (enigma::WizardMode) {
            snprintf (txt, sizeof(txt), "#%d: %s (%s)",
                      iselected+1, li.name.c_str(), li.filename.c_str());
        } else {
            snprintf (txt, sizeof(txt), "#%d: %s",
                      iselected+1, li.name.c_str());
        }
        lbl_levelname->set_text(txt);

	// Display best time
        if (shown_text.length()) {
            lbl_levelinfo->set_text(shown_text);
        }
        else {
            int    par_time       = level.get_par_time (difficulty);
            int    best_user_time = level.get_best_user_time (difficulty);
            string par_name       = level.get_par_holder (difficulty);

            string your_time;
            string par_text;

            if (best_user_time>0) {
                your_time = strf(_("Your time: %d:%02d"),
                                 best_user_time/60, best_user_time%60);

                if (par_time>0) {
                    int below = par_time-best_user_time;
                    if (below == 0)
                        par_text = _("That's par.");
                    else if (below>0)
                        par_text = strf(_("That's %d:%02d below par."),
                                        below/60, below%60);
                }
            }

            if (par_text.length() == 0 && par_time>0) {
                if (par_name.length())
                    par_text = strf(_("Par by %s: %d:%02d"), 
                                    par_name.c_str(), par_time/60, par_time%60);
                else
                    par_text = strf(_("Par: %d:%02d"), par_time/60, par_time%60);
            }

            string time_text;
            if (your_time.length()>0)   time_text = your_time+"  "+par_text;
            else                        time_text = your_time+par_text;

            lbl_levelinfo->set_text(time_text.c_str());
        }
    }
}

void LevelMenu::set_levelpack (unsigned index) 
{
    if (index < LevelPacks.size()) {
        m_ilevelpack = index;

        LevelPack *lp = LevelPacks[m_ilevelpack];
        levelwidget->change_levelpack (lp);

        char txt[100];
        snprintf (txt, sizeof(txt), _("%s: %d levels"),
                  lp->get_name().c_str(), lp->size());
        lbl_lpinfo->set_text(txt);

        update_info();
    }
}

void LevelMenu::draw_background(ecl::GC &gc) 
{
    video::SetCaption(("Enigma - Level Menu"));
    sound::PlayMusic (options::GetString("MenuMusicFile"));

    blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
}

void LevelMenu::next_levelpack() 
{
    unsigned next_pack = m_ilevelpack+1;
    if (next_pack == LevelPacks.size()) next_pack = 0;
    set_levelpack(next_pack);
}

void LevelMenu::previous_levelpack() {
    size_t prev_pack = m_ilevelpack;
    if (prev_pack == 0) prev_pack = LevelPacks.size()-1;
    else --prev_pack;
    set_levelpack(prev_pack);
}

int LevelMenu::get_position() const {
    return (m_ilevelpack << 16) | (levelwidget->get_position() & 0xffff);
}

void LevelMenu::set_position(int pos) {
    set_levelpack(pos >> 16);
    levelwidget->set_position(pos & 0xffff);
}

void LevelMenu::show_text(const string& text) {
    shown_text     = text;
    shown_text_ttl = 2.0; // show for two seconds
}



/* -------------------- NetworkMenu -------------------- */

NetworkMenu::NetworkMenu ()
{
    const video::VMInfo *vminfo = video::GetInfo();

    BuildVList b(this, Rect((vminfo->width - 150)/2,150,150,40), 5);
    m_startgame = b.add(new StaticTextButton(N_("Start Game"), this));
    m_joingame = b.add(new StaticTextButton(N_("Join Game"), this));
    m_back = b.add(new StaticTextButton(N_("Back"), this));
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
    if (w == m_startgame) {
        netgame::Start();
    } 
    else if (w == m_joingame) {
        netgame::Join("localhost", 12345);
    }
    if (w == m_back)
        Menu::quit();
}

void NetworkMenu::draw_background(ecl::GC &gc)
{
    video::SetCaption (("Enigma - Network Menu"));
    blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));
}

void NetworkMenu::tick(double dtime)
{
}



/* -------------------- Main menu -------------------- */

MainMenu::MainMenu() 
{
    build_menu();
}

void MainMenu::build_menu() 
{
    const video::VMInfo *vminfo = video::GetInfo();

    BuildVList b(this, Rect((vminfo->width - 150)/2,150,150,40), 5);
    m_startgame = b.add(new StaticTextButton(N_("Start Game"), this));
#ifdef ENABLE_EXPERIMENTAL
    m_netgame   = b.add (new StaticTextButton (N_("Network Game"), this));
    leveled     = b.add(new StaticTextButton(N_("Editor"), this));
#endif
//    manual      = b.add(new StaticTextButton("Manual", this));
    options     = b.add(new StaticTextButton(N_("Options"), this));
    credits     = b.add(new StaticTextButton(N_("Credits"), this));
    quit        = b.add(new StaticTextButton(N_("Quit"), this));
}

void MainMenu::draw_background(ecl::GC &gc) 
{
    const video::VMInfo *vminfo = video::GetInfo();

    video::SetCaption (("Enigma - Main Menu"));
    sound::PlayMusic (options::GetString("MenuMusicFile"));

    blit(gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));

    Font *f = enigma::GetFont("levelmenu");
    Surface * logo(enigma::GetImage("enigma_logo3"));
    int x0=(vminfo->width - logo->width())/2;
    int y0=30;
    blit(gc, x0, y0, logo);
    f->render (gc, 5, vminfo->height - 20, "v" PACKAGE_VERSION);
}

void MainMenu::on_action(Widget *w) 
{
    if (w == m_startgame) {
        LevelMenu m(LevelPacks[0], options::GetInt("LevelMenuPosition"));
        m.manage();
        options::SetOption("LevelMenuPosition", m.get_position());
        invalidate_all();
//     }
//     else if (w == manual) {
//         show_help ();
    } else if (w == credits) {
        show_credits ();
    } else if (w == options) {
        ShowOptionsMenu(0);

#ifdef ENABLE_EXPERIMENTAL
    } else if (w == m_netgame) {
        ShowNetworkMenu();
    } else if (w == leveled) {
        editor::Run();
#endif
    } else if (w == quit) {
        Menu::quit();
    } else
        return;
    invalidate_all();
}

void MainMenu::tick(double /* dtime */) 
{
    bool option_fullscreen = options::GetInt ("FullScreen") != 0;
    if (options::GetInt ("VideoMode") != video::GetVideoMode()
        || option_fullscreen != video::IsFullScreen())
    {
        ChangeVideoMode();
        clear();
        reset_active_widget ();
        build_menu();
        invalidate_all();
    }
}


void MainMenu::show_text( const char *text[]) 
{
    Screen *scr = video::GetScreen ();
    GC gc (scr->get_surface());
    blit (gc, 0,0, enigma::GetImage("menu_bg", ".jpg"));


    Font *f = enigma::GetFont("menufont");
    for (int i=0; text[i]; ++i)
    {
        const char *t = _(text[i]);
        f->render (gc, 40, 20+i*f->get_height(), t);
    }
    scr->update_all ();
    scr->flush_updates();

    SDL_Event e;
    for (;;) {
        SDL_WaitEvent(&e);
        if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
            break;
    }
}

void MainMenu::show_credits () 
{
    static const char *credit_text[] = {
	N_("Project maintainer:"),
	"  DANIEL HECK",
	" ",
        N_("Main developers:"),
        "  SIEGFRIED FENNIG, MARTIN HAWLISCH, PETR MACHATA,",
        "  JACOB SCOTT, SVEN SIGGELKOW, RALF WESTRAM",
        " ",
        N_("Special Thanks To:"),
        N_("  JOHANNES FORTMANN (graphics), JEREMY SAWICKI (oxydlib),"),
        N_("  MEINOLF SCHNEIDER (game idea, level design), ANDREW \"NECROS\" SEGA (menu music)"),
        N_("  NAT PRYCE & JOHN 'WB' SNEYERS (levels)"),
	" ",
	N_("Please see the manual for more detailed credits."),
        " ",
	N_("Home Page: http://www.nongnu.org/enigma"),
	N_("Contact: enigma-devel@nongnu.org"),
        " ",
        N_("Enigma is free software and may be distributed under the"),
        N_("terms of the GNU General Public License, version 2.  See"),
        N_("the accompanying COPYING.GPL for details."),
        " ",
        N_("Copyright (C) 2002-2005 Daniel Heck and contributors."),
        0
    };

    show_text(credit_text);
}

void MainMenu::show_help () 
{
    static const char *screen1[] = {
        "Introduction:",
        "",
        "The idea behind Enigma is simple: In most levels your job is to find",
        "pairs of \"Oxyd\" stones (you will recognize them when you see them)",
        "with matching colors. You have to open all oxyd stones but they only",
        "stay open when opening two stones of the same color one after",
        "another. Just play the first levels in the \"Oxyd & Co\" group and you",
        "will get the idea.",
        "In some other levels, called \"meditation landscapes\" you have a",
        "different job: You control a couple of small white marbles",
        "simultaneously and have to put each of them into a pit on the floor.",
        "",
        "Moving around:",
        "",
        "You control the marble by moving around the mouse into the desired",
        "direction. But be careful, because the marble has some mass and",
        "the floor some friction, the marble needs some time to accelerate or",
        "stop.",
        "Larger levels scroll when you reach the outermost part of the visible",
        "part on the screen.",
        "",
        0
    };
    static const char *screen2[] = {
        "The Floor:",
        "",
        "On most types of floor you can move around riskless but the speed",
        "of your marble may vary.",
        "When moving into abyss or water you will die.",
        "Ice is very slippery.",
        "You cannot control your marble in space.",
        "",
        "Items and Inventory:",
        "",
        "In many levels you can see different items. You can pick them up",
        "by simply rolling over them. The items are then stored in your",
        "inventory, which you control using the left and right mouse button.",
        "The leftmost item will be activated when pressing the left mouse",
        "button and you can rotate the items using the right mouse button.",
        "Some items can mutate when hit by a laser or crushed by a stone.",
        "",
        0
    };
    static const char *screen3[] = {
        "Stones:",
        "",
        "Most stones  are nothing more than walls.",
        "Many stones are movable when hit strong enough.",
        "The wooden stone will build new floor if moved into water, space or",
        "abyss.",
        "Others can be destroyed using a hammer, dynamite or a laser.",
        "Doors can be opened using a switch or trigger hidden somewhere",
        "around in the level.",
        "Some magic stones can be changed when hit using a magic wand.",
        "And some depend on the color of your marble.",
        "",
        0
    };

    show_text (screen1);
    show_text (screen2);
    show_text (screen3);
}


/* -------------------- Functions -------------------- */

void enigma::ShowMainMenu() {
    MainMenu m;
    m.manage();
}

void enigma::ShowOptionsMenu(Surface *background) {
    if (background == 0)
        background = enigma::GetImage("menu_bg", ".jpg");
    OptionsMenu m(background);
    m.center();
    m.manage();
}

void enigma::ShowNetworkMenu() 
{
    NetworkMenu m;
    m.manage();
}
