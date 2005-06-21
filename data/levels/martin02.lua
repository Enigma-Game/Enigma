--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 77
levelh = 37

create_world( levelw, levelh)

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-rock2", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		end
	end
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(  0, "#############################################################################")
renderLine(  1, "#o      #   #               #           #           #    #                  #")
renderLine(  2, "####### # # ### ########### # ######### # ######### # ## #          o       #")
renderLine(  3, "#     # # #   #     #     #   #   #     # #       #   #o #                  #")
renderLine(  4, "# # # # # ### ### # # ######### ### ##### # ##### ###### #                  #")
renderLine(  5, "# # # # # # #   # # #           #   #     # #     # #    #                  #")
renderLine(  6, "# # ### # # ### ### # ####### # # ### ##### # ##### # ####                  #")
renderLine(  7, "# #   # # #   # #   #   #     # #     #     #       #    #                  #")
renderLine(  8, "# ### # # # # # # ##### # ##### ####### ########### #### #                  #")
renderLine(  9, "# #   # # # # # #   #   #   # # #       #         #    # #                  #")
renderLine( 10, "### ### # # ### ### # ##### # # # ####### ##### ##### ## #                  #")
renderLine( 11, "#   #   # #   #   # # #     # # # #     # #     #   # #  #                  #")
renderLine( 12, "# ### ### # # ### # ### ##### # # ### # ### ### # # # # ##                  #")
renderLine( 13, "# #   #   # #     #   # #     # #   # #   # #   # # # #  #                  #")
renderLine( 14, "# # ### ######### ### # # ### # ### # ### # ##### # # ## #                  #")
renderLine( 15, "#   #           #   # # # #   #   # # # #   #     #   #  #                  #")
renderLine( 16, "# ### ######### ### # # ### # ### # # # ### # ######### ##                  #")
renderLine( 17, "#   # #         # #   #   # # #   # # #   # # #       #  #                  #")
renderLine( 18, "### ### ######### ##### # # ### ### # # # # # ### ###### #                  #")
renderLine( 19, "# #     #         #   # # #     #o  # # #   #   #      # #                  #")
renderLine( 20, "# ####### ####### # # # # ### ##### # ######### ###### # #                  #")
renderLine( 21, "#         #   #   # # # #   #       #         #   #    # #                  #")
renderLine( 22, "# ######### # # ### # # ### ######### ##### # ### # #### #                  #")
renderLine( 23, "#   #       # # #   # #   #   #     # # #   #   # #    # #                  #")
renderLine( 24, "### # ### ##### ### # ### ### # ### # # # ####### ## # # #                  #")
renderLine( 25, "#   # # # #   #     #   #   # #   # #   #            # # #                  #")
renderLine( 26, "# ### # # # # ######### ### # ### ### ##### ####### ## # #                  #")
renderLine( 27, "# #   #   # #         #   # #   #     #   # #   #      # #                  #")
renderLine( 28, "# # ####### ####### # ### # ### ### ### # ### # ######## #                  #")
renderLine( 29, "# #       #   #   # #   # # # #   # #   #     #   #      #                  #")
renderLine( 30, "# ##### # # ### # # ### # # # ### ### ########### # ######                  #")
renderLine( 31, "#     # #   #   #   #   # # #   #     #     #     #    # #                  #")
renderLine( 32, "# ### ####### ########### # ### ####### ### # ### #### # #                  #")
renderLine( 33, "# #   #     #             #       #       # # #      #   #                  #")
renderLine( 34, "# # ### ### ##################### ####### # # ########## #                  #")
renderLine( 35, "#o#       #                               #o#                               #")
renderLine( 36, "#############################################################################")



oxyd_shuffle()

set_actor("ac-blackball", 70, 16)
display.SetFollowMode(display.FOLLOW_SCROLLING)
