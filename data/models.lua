------------------------------------------------------------------------
-- Copyright (C) 2002,2003 Daniel Heck
--
-- This program is free software; you can redistribute it and/or
-- modify it under the terms of the GNU General Public License
-- as published by the Free Software Foundation; either version 2
-- of the License, or (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License along
-- with this program; if not, write to the Free Software Foundation, Inc.,
-- 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
--
------------------------------------------------------------------------

-- This file contains common routines for defining new Enigma models.
-- It is used by all other model*.lua files.

-- Display the loading progress
function Progress(percent, text)
    local fontname   = "levelmenu"
    local scr        = video.GetScreen()
    local d          = scr:get_surface()
    local background = enigma.GetImage ("menu_bg", ".jpg")
    local logo       = enigma.GetImage("enigma_logo3")
    local x          = (d:width()  - logo:width())/2
    local y          = (d:height() - logo:height())/2
    local gs         = ecl.GS:new(d:size())
    local font2      = enigma.GetFont("menufontsel")

    d:blit(gs, 0, 0, background) -- offset missing!
    d:blit(gs, x , y-logo:height(), logo)

    if text then
        font2:render(d, (d:width() - font2:get_width(text))/2, y, text)
    end
--     font:render(d, x, y-10, strrep(".", 50))
--     font2:render(d, x, y-10, strrep(".", percent/2))
    scr:update_all();
    scr:flush_updates();
    gs:delete()
end

-- Return an unique modelname.
modeltag = 0
function UniqueModelname()
    modeltag = modeltag + 1
    return "xmodel" .. modeltag
end

--------------------------------------
--   Basic Single Image Functions   --
--------------------------------------

-- Define a new image model.
-- * 'name' is the name of the model being created
-- * 'opt' contains optional information about the image file.
--   The following fields can be set in 'opt':
--    * filename
--    * xoff,yoff (hotspot coordinates inside the image)
-- Suitable default values are used for all options
function DefImage(name, opt)
    opt = opt or {}
    name = name or UniqueModelname()
    local fname = opt.filename or name
    local err = display.DefineImage(name, fname, opt.xoff or 0, opt.yoff or 0, opt.padding or 0)
    if err ~= 0 then
        error ("Could not define model "..name..": error loading "..fname)
    end
    return name
end

-- Define multiple image models at once.  Use the same options for all
-- of them. The namelist contains the modelnames to be created.
function DefImages(namelist, opt)
    opt = opt or {}
    for i,name in pairs(namelist) do
	    DefImage(name,opt)
    end
end

-------------------------------------
--   Basic Subdivision Functions   --
-------------------------------------

-- Define many image models from one single big image file.
-- Inputarguments are the imagefilename and the options.
-- The defined models are named 'modelname1', 'modelname2' ...
function DefSubimages(name, options)
    local opts = options or { }
    local w = opts.w or 1
    local h = opts.h or 1
    local imgw = opts.imgw or TileSize
    local imgh = opts.imgh or TileSize
    local xoff = opts.xoff or 0
    local yoff = opts.yoff or 0
    local modelname = opts.modelname or name
    local padding = options.padding or 0
    local cnt = options.startindex or 1
    local imagelist = {}
    local r = ecl.Rect:new(0,0,0,0)
    for x=1,w do
        for y=1,h do
            r.x,r.y,r.w,r.h = imgw*(x-1),imgh*(y-1),imgw,imgh
            r.x = r.x + padding
            r.y = r.y + padding
            r.w = r.w - 2*padding
            r.h = r.h - 2*padding
            table.insert(imagelist, modelname..cnt)
            err = display.DefineSubImage(modelname..cnt, name, xoff+padding,yoff+padding, r)
            if err ~= 0 then
                error ("Could not define "..modelname..cnt..": error loading "..name)
            end
            cnt = cnt+1
        end
    end
    r:delete()
    return imagelist
end

-- Generates an image model by cutting it out of a larger image.
-- Arguments are the imagefilename, the modelname and the position
-- where to cut a square with length TileSize. The position is
-- measured in multiples of TileSize.
function DefTile(imagename, modelname, x, y)
    local image = GetSurface (imagename)
    local r=ecl.Rect:new(x * TileSize, y*TileSize, TileSize, TileSize)
    local subsurface = CropSurface (image, r)
    DefineImageModel (modelname, subsurface)
    r:delete()
end

-- Generate multiple image models by tiling a big image into many
-- smaller subimages.  The parameters are currently hardcoded.
-- Arguments are the filename of the image and a list of
-- modelnames to be generated out of this image.
function DefTiles(imagename, modelnames)
    local xoff = 0
    local yoff = 0
    local tilew = TileSize
    local tileh = TileSize

    local image = GetSurface (imagename)
    local imgw = image:width()
    local r=ecl.Rect:new(0,0,TileSize,TileSize)
    for i,mname in pairs(modelnames) do
        r.x,r.y,r.w,r.h = xoff,yoff,tilew,tileh
        DefTile(imagename, mname, xoff/TileSize, yoff/TileSize)
        xoff = xoff + tilew
        if xoff >= imgw then
            xoff = 0
            yoff = yoff + tileh
        end
    end
    r:delete()
end

-----------------------------------
--   Basic Animation Functions   --
-----------------------------------

-- Generate a list of frame names by appending increasing numerical
-- prefixes to a base name.  For example, 'FrameNames("hello", 1,2)'
-- yields {"hello_0001", "hello_0002"}.
-- [Filenames like this are created by Gimp's "Video/Split Image.."
-- tool.]
function FrameNames(prefix, first, last)
    local fn = {}
    for i=first,last do
	    table.insert(fn, prefix .. format("_%04d", i))
    end
    return fn
end

function AddFrameNames(fn, prefix, first, last)
    for i=first,last do
	    table.insert(fn, i, prefix .. format("_%04d", i))
    end
end

-- Build a list of frames from a list of model names and a constant
-- duration.
function BuildFrames(names, msec)
    local a={}
    for i,n in pairs(names) do a[i] = {n, msec} end
    return a
end

-- Build a list of frames from (1) a list of names and (2) a list of
-- frame durations.  These two list are assumed to have the same
-- number of entries, or, more generally, to have the same index set.
function ComposeFrames(namelist, mseclist)
    local a={}
    for i=1,getn(namelist) do a[i] = {namelist[i], mseclist[i]} end
    return a
end

-- Given a list of frames, this function generates a new framelist
-- with for a ping-pong style animation.  (For example, an "abcd"
-- style animation would result in an "abcddcba"-style one.)
function PingPong(framelist)
    local a=framelist
    local n=getn(framelist)
    for i = 0,n-1 do
	    a[n+i+1] = framelist[n-i]
    end
    return a
end

-- Given a list of frames, this function generates a new framelist
-- with repeated framesequence.  (For example, an "abcd"
-- style animation with a cnt of 3 would result in an "abcdabcdabcd"-style one.)
function RepeatFrames(framelist, blocksize, cnt)
    local a={}
    for i=1,getn(framelist),blocksize do
	    for j=1,cnt do
	        for k=i,i+blocksize-1 do
		        table.insert(a,framelist[k])
	        end
	    end
    end
    return a
end

-- Given a list of frames, this function generates a new framelist
-- with repeated framesequence.  (For example, an "abcd"
-- style animation with a cnt of 3 would result in an "abcdabcdabcd"-style one.)
function RepeatAnim(framelist, cnt)
    return RepeatFrames(framelist, getn(framelist), cnt or 2)
end

-- Given a list of frames, this function generates a new framelist
-- with reversed frame order.  (For example, an "abcd"
-- style animation would result in an "dcba"-style one.)
function ReverseFrames(framelist)
    local a={}
    for i=getn(framelist),1,-1 do
	    table.insert(a, framelist[i])
    end
    return a
end

-- Define an animation from a list of images.  `frames' is a
-- framelist, as built with `FrameNames()', but the model names in
-- this list are interpreted as image filenames.
function DefAnimImages(name, frames, opt)
    opt = opt or {}
    local loopbool = opt.loop and true or false
    display.DefineAnim(name, loopbool)
    for i=1,getn(frames) do
	    local frame=frames[i]
	    opt.filename = frame[1]
	    local immodel = DefImage(nil, opt)
	    display.AddFrame(name, immodel, frame[2])
    end
end

-- Define an animation from a list of models.
-- * 'name' is the name of the animation being created
-- * 'frames' is the list of modelnames used
function DefAnim(name, frames, loop)
    local loopbool = loop and true or false
    display.DefineAnim(name,loopbool)
    for i=1,getn(frames) do
	    local frame = frames[i]
	    display.AddFrame(name, frame[1], frame[2])
    end
end

-- Define a new Animation.
-- * 'name' is the name of the animation being created
-- * 'opt' contains optional information about the animation.
function NewAnim(name,opts)
    local imagefile = opts.img or name
    local h = opts.h or 1
    local w = opts.w or 1
    local speed = opts.speed or 100
    local loop = opts.loop or 0

    local frames = DefSubimages(imagefile, {["h"]=h, ["w"]=w})
    if opts.pingpong then frames=PingPong(frames) end
    DefAnim(name, BuildFrames(frames, speed), loop)
end

--------------------------
--   Sprite functions   --
--------------------------

-- padding is calculated as:
-- padding = (1.25 - 2*Actorradius)/2
function SpriteImages(spriteimg, n, offsetfactor, padding)
    local factor = offsetfactor or 0.5
    local offset = -SpriteSize * factor
    local pad = (padding or 0) * TileSize
    return DefSubimages(spriteimg,
                         {h = n, imgw=SpriteSize, imgh=SpriteSize,
                             xoff = offset, yoff = offset, padding=pad})
end

function SpriteImage(spriteimg, offsetfactor, padding)
    local factor = offsetfactor or 0.5
    local offset = -SpriteSize * factor
    local padding = (padding or 0) * TileSize
    return DefImage(spriteimg, {xoff = offset, yoff = offset, padding=padding})
end

function SpriteWithShadow()

end

function SpriteAnim(name, images, shadows, framelen, loop)
    local frames={}
    local nframes = getn(images)
    for i=1,getn(images) do
        DefShModel(name..i, images[i], shadows[mod (i, getn(shadows))] )
        table.insert(frames, name..i)
    end
    DefAnim (name, BuildFrames(frames, framelen), loop)
end

function Sprite(descr)
    local imgfile = descr.imgfile or descr.name
    local loop    = descr.loop or false
    local img     = SpriteImages(imgfile, descr.nimages, 0.5, descr.padding)
    DefAnim(descr.name, BuildFrames(img, descr.framelen), loop)
end

---------------------------------
--   Floor related Functions   --
---------------------------------

-- Defines a random floor tile out of a given list.
function DefRandFloor(name, imagelist)
    display.DefineRandModel(name, getn(imagelist), imagelist)
end

---------------------------------
--   Stone related Functions   --
---------------------------------

-- Define a stone together with its shadow model:
function DefStone(name, shmodel, opt)
    opt = opt or {}
    shmodel = shmodel or "sh-solid"
    opt.filename = opt.filename or name
    display.DefineShadedModel(name, DefImage(nil, opt), shmodel)
end

-- Define a shaded model named `name' with texture model `fg' and
-- shadow model `bg'.  Both are real models (not names of image files), so
-- you can combine animated images with shadows etc.
function DefShModel(name, fg, bg)
    display.DefineShadedModel(name, fg, bg)
end

function DefSolidStone(name, front)
    DefShModel(name, front, "sh-solid")
end

function DefRoundStone(name, front)
    DefShModel(name, front, "sh-round")
end

function DefFloatingStone(name, front)
    DefShModel(name, front, "sh-floating")
end

-- Animated Stones --
function DefSolidStoneWithAnim(name, npictures, frametime)
    local n=DefSubimages(name, {h=npictures})
    DefAnim(name.."-animfg", BuildFrames(n,frametime))
    DefSolidStone(name.."-anim", name.."-animfg")
    DefSolidStone(name, n[1])
end

function DefRoundStoneWithAnim(name, npictures, frametime)
    local n=DefSubimages(name, {h=npictures})
    DefAnim(name.."-animfg", BuildFrames(n,frametime))
    DefRoundStone(name.."-anim", name.."-animfg")
    DefRoundStone(name, n[1])
end

-- Aliases --
function DefAlias(name, othername)
    display.DefineAlias(name,othername)
end

-- Define an overlay --
function DefOverlay(name, imglist)
    display.DefineOverlayImage(name, getn(imglist), imglist)
end

-- Define a multiple composite --
-- This function will compose a list of images
-- and store the result in "name".
-- As this function needs lots of memory, it's use is deprecated.
function DefMultipleComposite(name, imglist)
    -- Assert imglist >= 1!
    local listsize = table.getn(imglist)
    if listsize == 0 then
        DefAlias(name, "invisible")
    elseif listsize == 1 then
        DefAlias(name, imglist[1])
    elseif listsize == 2 then
        display.DefineComposite(name, imglist[1], imglist[2])
    elseif listsize > 2 then
        display.DefineComposite(name.."-2", imglist[1], imglist[2])
        for i = 3, table.getn(imglist)-1 do
            display.DefineComposite(name.."-"..i, name.."-"..(i-1), imglist[i])
        end
        display.DefineComposite(name, name.."-"..(table.getn(imglist)-1), imglist[table.getn(imglist)])
    end
end

