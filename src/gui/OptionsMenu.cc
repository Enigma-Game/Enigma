/*
 * Copyright (C) 2002,2003,2004,2005,2006 Daniel Heck
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
 
#include "gui/OptionsMenu.hh"
#include "ecl.hh"
#include "enigma.hh"
#include "lev/ScoreManager.hh"
#include "LocalToXML.hh"
#include "main.hh"
#include "nls.hh"
#include "options.hh"
#include "display.hh"
#include "oxyd.hh"
#include "SoundEngine.hh"
#include "SoundEffectManager.hh"
#include "MusicManager.hh"
#include "Utf8ToXML.hh"
#include "video.hh"
#include "XMLtoLocal.hh"
#include "XMLtoUtf8.hh"

#include <cassert>

using namespace ecl;
using namespace std;

namespace enigma { namespace gui {

/* -------------------- Buttons for Options -------------------- */

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

    class TextSpeedButton : public ValueButton {
        int get_value() const     { 
            return display::GetTextSpeed();
        }
        void set_value(int value) { 
            display::SetTextSpeed(value);
        }

        string get_text(int value) const  {
            return strf("%d", value);
        }
    public:
        TextSpeedButton()
        : ValueButton(display::MIN_TextSpeed, display::MAX_TextSpeed)
        { init(); }
    };
    
    class SoundVolumeButton : public ValueButton {
        int get_value() const     { 
            return round_nearest<int>(options::GetDouble("SoundVolume")*10.0); 
        }
        void set_value(int value) {
            options::SetOption("SoundVolume", value/10.0);
            sound::UpdateVolume();
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
            sound::UpdateVolume();
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
        void on_action(Widget *) { sound::SetInGameMusicActive(toggle()); }

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

    struct RatingsUpdateButton : public BoolOptionButton {
        RatingsUpdateButton() : BoolOptionButton("RatingsAutoUpdate", N_("Auto"), N_("Never"), this) {}
    };

    struct ScoreUploadButton : public BoolOptionButton {
        ScoreUploadButton() : BoolOptionButton("ScoreAutoUpload", N_("Always"), N_("Never"), this) {}
    };


    /* -------------------- VideoModeButton -------------------- */
    
    VideoModeButton::VideoModeButton() : ValueButton(0, 1) {
        bool isFullScreen = app.prefs->getBool("FullScreen");
        setMaxValue(video::GetNumAvailableModes(isFullScreen) - 1);
        init();
    }
    
    void VideoModeButton::reinit() {
        bool isFullScreen = app.prefs->getBool("FullScreen");
        ValueButton::setMaxValue(video::GetNumAvailableModes(isFullScreen) - 1);
    }

    int VideoModeButton::get_value() const {
        bool isFullScreen = app.prefs->getBool("FullScreen");        
        return video::GetModeNumber(static_cast<video::VideoModes>(app.selectedVideoMode), isFullScreen);
    }

    void VideoModeButton::set_value(int value) {
        bool isFullScreen = app.prefs->getBool("FullScreen");
        video::VideoModes vm = video::GetVideoMode(value, isFullScreen);
        app.selectedVideoMode = vm;

        if (vm != video::GetBestUserMode(isFullScreen)) {
            const video::VMInfo * info = video::GetInfo(vm);
            app.prefs->setProperty(isFullScreen ? "VideoModesFullscreen" : "VideoModesWindow",
                    isFullScreen ? info->fallback_fullscreen : info->fallback_window);
        }
    }

    string VideoModeButton::get_text(int value) const {
        bool isFullScreen = app.prefs->getBool("FullScreen");        
        const video::VMInfo * vi = video::GetInfo(video::GetVideoMode(value, isFullScreen));
        return vi->name;
    }


    /* -------------------- SoundSetButton -------------------- */
    
    SoundSetButton::SoundSetButton() : ValueButton(0, 1) {
        int numAvail = sound::GetOptionSoundSetCount();
        setMaxValue(numAvail - 1);
        init();
    }

    int SoundSetButton::get_value() const {
        return sound::GetOptionSoundSet();
    }

    void SoundSetButton::set_value(int value) {
        sound::SetOptionSoundSet(value);
    }

    string SoundSetButton::get_text(int value) const {
        return _(sound::GetOptionSoundSetText(value).c_str());
    }

    /* -------------------- MenuMusicButton -------------------- */
    
    MenuMusicButton::MenuMusicButton() : ValueButton(0, 1) {
        int numAvail = sound::GetOptionMenuMusicCount();
        setMaxValue(numAvail - 1);
        init();
    }

    int MenuMusicButton::get_value() const {
        return sound::GetOptionMenuMusic();
    }

    void MenuMusicButton::set_value(int value) {
        sound::SetOptionMenuMusic(value);
    }

    string MenuMusicButton::get_text(int value) const {
        return _(sound::GetOptionMenuMusicText(value).c_str());
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
    
    FullscreenButton::FullscreenButton(ActionListener *al)
    : BoolOptionButton("FullScreen", N_("Yes"), N_("No"), al)        
    {
    }
    
    /* -------------------- LanguageButton -------------------- */
    
    int LanguageButton::get_value() const
    {
        string localename; //  = ecl::DefaultMessageLocale ();
        options::GetOption ("Language", localename);
    
        int lang = 0;                  // unknown language
        for (size_t i=0; i<NUMENTRIES(nls::languages); ++i) {
            if (localename == nls::languages[i].localename)
                lang = int(i);
        }
        return lang;
    }
    
    void LanguageButton::set_value(int value)
    {
        options::SetOption ("Language", nls::languages[value].localename);
        
        if (not inInit) {
            // change language only on user action
            app.setLanguage(nls::languages[value].localename);
            myListener->on_action(this);
        }
    }
    
    string LanguageButton::get_text(int value) const
    {
        if (value == -1)
            return _("unknown");
        else
            return nls::languages[value].name;
    }
    
    LanguageButton::LanguageButton (ActionListener *al)
    : ValueButton(0, NUMENTRIES(nls::languages)-1), myListener(al)
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
        int value = round_down<int>(gamma * 5.0 + 0.1);
        return value;
    }
    
    string GammaButton::get_text(int value) const
    {
        return ecl::strf ("%d", value-5);
    }
    

    /* -------------------- Options Menu -------------------- */
    
    OptionsMenu::OptionsMenu(ecl::Surface *background_)
    : pagesVList(NULL), commandHList(NULL), optionsVList(NULL), back(NULL), 
      language(NULL), but_main_options(NULL), but_video_options(NULL),
      but_audio_options(NULL), but_config_options(NULL), fullscreen(NULL),
      videomode(NULL), userNameTF(NULL), userPathTF(NULL),
      userImagePathTF(NULL), menuMusicTF(NULL), 
      background(background_), previous_caption(video::GetCaption())
    {
        center();
        close_page();
        open_page(OPTIONS_MAIN);
    }

    OptionsMenu::~OptionsMenu() {
        video::SetCaption(previous_caption.c_str());
    }

    void OptionsMenu::open_page(OptionsPage new_page) {
        const video::VMInfo *vminfo = video::GetInfo();
        video::VideoTileType vtt = vminfo->tt;
        int vh = vminfo->area.x;
        int vv = (vminfo->height - vminfo->area.h)/2;
        static struct SpacingConfig {
            int rows;
            int button_height, optionb_width, commandb_width, pageb_width;
            int vmargin, vrow_row;
            int hmargin, hpage_option, hoption_option;
        } param[] = {
            {  // VTS_16 (320x240)
                9,
                17, 100, 70, 50,
                7, 5,
                10, 10, 10
            },
            {  // VTS_32 (640x480)
                9,
                30, 200, 140, 100,
                15, 13,
                20, 20, 20
            },
            {  // VTS_40 (800x600)
                10,
                35, 200, 140, 100,
                20, 15,
                15, 46, 15
            },
            {  // VTS_48 (960x720)  VM_1024x768
                11,
                35, 200, 140, 100,
                30, 18,
                70, 76, 20
            },
            {  // VTS_64 (1280x960)
                11,
                35, 200, 140, 100,
                40, 20,
                60, 58, 20
            }
        };
        
        // These are exactly the same preferences as in LevelPackMenu.cc
        // Left side: Availabe submenus ("pages")
        pagesVList = new VList; 
        pagesVList->set_spacing(param[vtt].vrow_row);
        pagesVList->set_alignment(HALIGN_CENTER, VALIGN_TOP);
        pagesVList->set_default_size(param[vtt].pageb_width, param[vtt].button_height);
        but_main_options = new StaticTextButton(N_("Main"), this);
        but_main_options->setHighlight(new_page == OPTIONS_MAIN);
        but_video_options = new StaticTextButton(N_("Video"), this);
        but_video_options->setHighlight(new_page == OPTIONS_VIDEO);
        but_audio_options = new StaticTextButton(N_("Audio"), this);
        but_audio_options->setHighlight(new_page == OPTIONS_AUDIO);
        but_config_options = new StaticTextButton(N_("Config"), this);
        but_config_options->setHighlight(new_page == OPTIONS_CONFIG);
        but_paths_options = new StaticTextButton(N_("Paths"), this);
        but_paths_options->setHighlight(new_page == OPTIONS_PATHS);
        pagesVList->add_back(but_main_options);
        pagesVList->add_back(new Label(""));
        pagesVList->add_back(but_video_options);
        pagesVList->add_back(but_audio_options);
        pagesVList->add_back(but_config_options);
        pagesVList->add_back(but_paths_options);
        this->add(pagesVList, Rect(param[vtt].hmargin + vh,
                                   param[vtt].vmargin + vv, 
                                   param[vtt].pageb_width,
                                   param[vtt].rows * param[vtt].button_height + 
                                       (param[vtt].rows - 1) * param[vtt].vrow_row));

        // At the bottom: Currently only "Back"
        commandHList = new HList;
        commandHList->set_spacing(param[vtt].hoption_option);
        commandHList->set_alignment(HALIGN_LEFT, VALIGN_TOP);
        commandHList->set_default_size(param[vtt].commandb_width, param[vtt].button_height);
        commandHList->add_back(back = new StaticTextButton(N_("Ok"), this));
        this->add(commandHList, Rect(vminfo->width + vh - param[vtt].hmargin
                                         - 1*param[vtt].commandb_width  // number of buttons
                                         - 0*param[vtt].hoption_option, // number - 1
                                     param[vtt].vmargin + param[vtt].rows*
                                         (param[vtt].vrow_row + param[vtt].button_height)
                                         + param[vtt].vrow_row + vv,
                                     vminfo->width-2*param[vtt].hmargin,
                                     param[vtt].button_height));

        optionsVList = new VList;
        optionsVList->set_spacing(param[vtt].vrow_row);
        optionsVList->set_alignment(HALIGN_LEFT, VALIGN_TOP);
        optionsVList->set_default_size(2*param[vtt].optionb_width
                                           + param[vtt].hoption_option,
                                       param[vtt].button_height);

        /*! All options on our pages consist of a label and a button,
          a very long (text-)button, or just a label, suited to the
          long buttons. optionsVList is a list of such rows.
          Each row itself is an HList, and has to be initialised
          with positioning data. To make things easier, we use
          Macros for initialisation. */

        HList *lb;  // a list of labels and/or buttons

#define OPTIONS_NEW_L(label) lb = new HList;\
        lb->set_spacing(param[vtt].hoption_option); \
        lb->set_alignment(HALIGN_LEFT, VALIGN_TOP); \
        lb->set_default_size(2*param[vtt].optionb_width + param[vtt].hoption_option, \
                             param[vtt].button_height); \
        lb->add_back(new Label(label, HALIGN_LEFT, VALIGN_BOTTOM)); \
        optionsVList->add_back(lb); \
// end define
#define OPTIONS_NEW_LB(label,button) lb = new HList;\
        lb->set_spacing(param[vtt].hoption_option); \
        lb->set_alignment(HALIGN_CENTER, VALIGN_TOP); \
        lb->set_default_size(param[vtt].optionb_width, param[vtt].button_height); \
        lb->add_back(new Label(label, HALIGN_RIGHT, VALIGN_CENTER)); \
        lb->add_back(button); \
        optionsVList->add_back(lb); \
// end define
#define OPTIONS_NEW_T(textbutton) lb = new HList;\
        lb->set_spacing(param[vtt].hoption_option); \
        lb->set_alignment(HALIGN_LEFT, VALIGN_TOP); \
        lb->set_default_size(2*param[vtt].optionb_width + param[vtt].hoption_option, \
                             param[vtt].button_height); \
        lb->add_back(textbutton); \
        optionsVList->add_back(lb); \
// end define

        switch (new_page) {
            case OPTIONS_MAIN:
                OPTIONS_NEW_LB(N_("Language: "), language = new LanguageButton(this))
                OPTIONS_NEW_LB(N_("Fullscreen: "), fullscreen = new FullscreenButton(this))
                OPTIONS_NEW_LB(N_("Video mode: "), videomode = new VideoModeButton())
                OPTIONS_NEW_LB(N_("Mouse speed: "), new MouseSpeedButton())
                OPTIONS_NEW_LB(N_("Sound volume: "), new SoundVolumeButton())
                OPTIONS_NEW_LB(N_("Music volume: "), new MusicVolumeButton())
                OPTIONS_NEW_LB(N_("Ratings update: "), new RatingsUpdateButton())
#ifdef ENABLE_EXPERIMENTAL
                OPTIONS_NEW_LB(N_("Score upload: "), new ScoreUploadButton())
#endif
                userNameTF = new TextField(app.state->getString("UserName"));
                userNameTF->setMaxChars(20);
                userNameTF->setInvalidChars("+");
                OPTIONS_NEW_L(N_("User name: "))
                OPTIONS_NEW_T(userNameTF)
                break;
            case OPTIONS_VIDEO:
                OPTIONS_NEW_LB(N_("Fullscreen: "), fullscreen = new FullscreenButton())
                fullscreen->set_listener(this);
                OPTIONS_NEW_LB(N_("Video mode: "), videomode = new VideoModeButton())
                OPTIONS_NEW_LB(N_("Gamma correction: "), new GammaButton())
                break;
            case OPTIONS_AUDIO:
                OPTIONS_NEW_LB(N_("Sound set: "), new SoundSetButton())
                OPTIONS_NEW_LB(N_("Menu music: "), new MenuMusicButton)
                OPTIONS_NEW_LB(N_("Sound volume: "), new SoundVolumeButton())
                OPTIONS_NEW_LB(N_("Music volume: "), new MusicVolumeButton())
                OPTIONS_NEW_LB(N_("Stereo: "), new StereoButton())
                //...("Music ingame: ", new InGameMusicButton)
                break;
            case OPTIONS_CONFIG:
                OPTIONS_NEW_LB(N_("Language: "), language = new LanguageButton(this))
                OPTIONS_NEW_LB(N_("Mouse speed: "), new MouseSpeedButton())
                OPTIONS_NEW_LB(N_("Text speed: "), new TextSpeedButton())
                OPTIONS_NEW_LB(N_("Ratings update: "), new RatingsUpdateButton())
                userNameTF = new TextField(app.state->getString("UserName"));
                userNameTF->setMaxChars(20);
                userNameTF->setInvalidChars("+");
                OPTIONS_NEW_L(N_("User name: "))
                OPTIONS_NEW_T(userNameTF)
                break;
            case OPTIONS_PATHS:
                userPathTF = new TextField(XMLtoUtf8(LocalToXML(app.userPath.c_str()).x_str()).c_str());
                OPTIONS_NEW_L(N_("User path: "))
                OPTIONS_NEW_T(userPathTF)
                userImagePathTF = new TextField(XMLtoUtf8(LocalToXML(app.userImagePath.c_str()).x_str()).c_str());
                OPTIONS_NEW_L(N_("User image path: "))
                OPTIONS_NEW_T(userImagePathTF)
                break;
        }
#undef OPTIONS_NEW_L
#undef OPTIONS_NEW_LB
#undef OPTIONS_NEW_T

        // Now add all options to the page.
        this->add(optionsVList,
           Rect(param[vtt].hmargin + vh + param[vtt].pageb_width
                    + param[vtt].hpage_option,
                param[vtt].vmargin + vv,
                2*param[vtt].optionb_width + param[vtt].hoption_option,
                param[vtt].rows * param[vtt].button_height + 
                    (param[vtt].rows - 1) * param[vtt].vrow_row));
        invalidate_all();
    }
    
    void OptionsMenu::close_page() {
        // Reset active and key_focus widgets, they will be deleted soon,
        // and we don't want any ticks for them anymore.
        reset_active_widget();
        reset_key_focus_widget();
        // Evaluate and save text field entries (if existing).
        if(userNameTF) {
            if (app.state->getString("UserName") != userNameTF->getText())
                // ensure that enigma.score is saved with new Username or to new location
                lev::ScoreManager::instance()->markModified();
            // strip off leading and trailing whitespace from user name
            std::string userName = userNameTF->getText();
            std::string::size_type firstChar = userName.find_first_not_of(" ");
            std::string::size_type lastChar = userName.find_last_not_of(" ");
            if (firstChar != std::string::npos)
                app.state->setProperty("UserName", userName.substr(firstChar, lastChar - firstChar + 1));
            else
                app.state->setProperty("UserName", std::string(""));
        }
        if(userPathTF) {
            std::string tfUserPathLocal = XMLtoLocal(Utf8ToXML(userPathTF->getText().c_str()).x_str()).c_str();
            if (app.userPath != tfUserPathLocal)
                // ensure that enigma.score is saved with new Username or to new location
                lev::ScoreManager::instance()->markModified();
            app.setUserPath(tfUserPathLocal.c_str());
        }
        if(userImagePathTF) {
            std::string tfUserImageLocal = XMLtoLocal(Utf8ToXML(userImagePathTF->getText().c_str()).x_str()).c_str();
            if (app.userImagePath != tfUserImageLocal)
                // ensure that enigma.score is saved with new Username or to new location
                lev::ScoreManager::instance()->markModified();
            app.setUserImagePath(tfUserImageLocal.c_str());
        }
        // Delete widgets.
        if (pagesVList != NULL) {
            pagesVList->clear();
            remove_child(pagesVList);
            delete pagesVList;
            pagesVList = NULL;
        }
        but_main_options = NULL;
        but_video_options = NULL;
        but_audio_options = NULL;
        but_config_options = NULL;
        if (commandHList != NULL) {
            commandHList->clear();
            remove_child(commandHList);
            delete commandHList;
            commandHList = NULL;
        }
        back = NULL;
        if (optionsVList != NULL) {
            optionsVList->clear();
            remove_child(optionsVList);
            delete optionsVList;
            optionsVList = NULL;
        }
        language = NULL;
        fullscreen = NULL;
        videomode = NULL;
        menuMusicTF = NULL;
        userNameTF = NULL;
        userPathTF = NULL;
        userImagePathTF = NULL;
    }

    void OptionsMenu::quit() {
        close_page();
        Menu::quit();
    }

    bool OptionsMenu::on_event (const SDL_Event &e)
    {
        bool handled=false;
        if (e.type == SDL_KEYUP) {
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
            quit();
        else if (w == language)
            // language changed - retranslate and redraw everything
            invalidate_all();
        else if (w == fullscreen) {
            // switch the fullscreen button and option
            fullscreen->on_action(fullscreen);
            app.selectedVideoMode = video::GetBestUserMode(app.prefs->getBool("FullScreen"));
            // update the video mode button to the modes available
            videomode->reinit();
            invalidate_all();
        } else if (w == but_main_options) {
            close_page();
            open_page(OPTIONS_MAIN);
        } else if (w == but_video_options) {
            close_page();
            open_page(OPTIONS_VIDEO);
        } else if (w == but_audio_options) {
            close_page();
            open_page(OPTIONS_AUDIO);
        } else if (w == but_config_options) {
            close_page();
            open_page(OPTIONS_CONFIG);
        } else if (w == but_paths_options) {
            close_page();
            open_page(OPTIONS_PATHS);
        }
    }
    
    void OptionsMenu::draw_background(ecl::GC &gc)
    {
        const video::VMInfo *vminfo = video::GetInfo();
        video::SetCaption(("Enigma - Options Menu"));
    //     blit(gc, 0,0, enigma::GetImage("menu_bg"));
        blit(gc, vminfo->mbg_offsetx, vminfo->mbg_offsety, background);
    }
    
/* -------------------- Functions -------------------- */

    void ShowOptionsMenu(Surface *background) {
        if (background == 0)
            background = enigma::GetImage("menu_bg", ".jpg");
        OptionsMenu m(background);
        m.manage();
    }

}} // namespace enigma::gui

