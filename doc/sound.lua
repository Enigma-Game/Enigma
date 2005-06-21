--
-- This file demonstrates how to change Enigma's default sound set.
--
-- Sound effects are triggered by so-called "sound events".
-- These sound events usually have a name (like "dooropen") and an
-- associated location (the coordinates of the door) which
-- affect the way a sound effect is played.
--
-- The sound event is converted into a real sound effect using
-- tables similar to the one below.  Each entry in the table is
-- either a string like "st-coinslot", which is interpreted as
-- referring to file "st-coinslot.wav" or a list of sound
-- attributes enclosed in curly braces "{ ... }".
--
-- Here is a complete example of such an attribute list:
--
--      dooropen = { file="open-door", volume=0.9, priority=4, global=0 },
--
-- The meaning of these attributes is as follows:
--
--      file     - The name of the sound file for this event, without
--                 the".wav" extension.
--
--      volume   - The sound volume: 1.0 is standard, 0.0 is silent.
--
--      priority - If many effects are active at the same time, high-priority
--                 effects can replace lower-priority effects. Use an integer
--                 between 1 and 10.
--
--      global   - Either 1 or 0.  If 1, no stereo effects are applied and
--                 there is no attenuation.  Used for menu sound, level end
--                 sounds, etc.
--
-- To design a new sound set, proceed as follows.
--
-- 1) Create a new folder containing this file (named "sound.lua")
--    and the wav files you want to use.  
--
-- 2) Move this new folder into Enigma's "sound" folder.  The directory
--    structure should look something like this
--
--      (enigma data folder)/sound/my_sounds/
--                                          /sound.lua
--                                          /a.wav
--                                          /b.wav
--                                          ...
--    [On Unix systems, you can use ~/.enigma/sound/ for testing purposes.]
--
-- 3) Run Enigma.  Since this file sets does not map any sound effect to a
--    wav file, you should hear nothing
--
-- 4) Edit the contents of this file to your liking.  If you need inspiration,
--    take a look at "startup.lua" shipped with Enigma, which contains the
--    default sound table.
--
-- If you have questions, don't hesitate to ask.  Have fun!
--

AddSoundTable ("Enigma", {
    [""]           = "",        -- empty sound
    ballcollision  = "",
    blackbomb      = "",
    blockerdown    = "",
    blockerup      = "",
    booze          = "",
    bumper         = "",
    cloth          = "",
    coinslotoff    = "",
    coinsloton     = "",
    crack          = "",
    doorclose      = "",
    dooropen       = "",
    drown          = "",
    dynamite       = "",
    electric       = "",
    exit           = "",
    extinguish     = "",
    fakeoxyd       = "",
    falldown       = "",
    fart           = "",
    finished       = "",
    floordestroy   = "",
    fourswitch     = "",
    glass          = "",
    hitfloor       = "",
    impulse        = "",
    intro          = "",
    invrotate      = "",
    itemtransform  = "",
    jump           = "",
    jumppad        = "",
    landmine       = "",
    laserloop      = "",
    laseron        = "",
    laseroff       = "",
    lock           = "",
    magneton       = "",
    magnetoff      = "",
    mail           = "",
    menuexit        = "",
    menumove        = "",
    menuok          = "",
    menustop        = "",
    menuswitch      = "",
    metal          = "",
    mirrorturn     = "",
    movebig        = "",
    moveslow       = "",
    movesmall      = "",
    oxydclose      = "",
    oxydopen       = "",
    oxydopened     = "",
    pickup         = "",
    puller         = "",
    puzzlerotate   = "",
    rubberband     = "",
    scissors       = "",
    seedgrow       = "",
    shatter        = "",
    shattersmall   = "",
    shogunoff      = "",
    shogunon       = "",
    skull          = "",
    spade          = "",
    squish         = "",
    stone          = "",
    stonedestroy   = "",
    stonepaint     = "",
    stonetransform = "",
    swamp          = "",
    switchmarbles  = "",
    switchoff      = "",
    switchon       = "",
    switchplayer   = "",
    sword          = "",
    thief          = "",
    triggerdown    = "",
    triggerup      = "",
    turnstileleft  = "",
    turnstileright = "",
    umbrellaoff    = "",
    umbrellaon     = "",
    umbrellawarn   = "",
    unlock         = "",
    vortexclose    = "",
    vortexopen     = "",
    warp           = "",
    whitebomb      = "",
    wood           = "",
    yinyang        = "",
})
