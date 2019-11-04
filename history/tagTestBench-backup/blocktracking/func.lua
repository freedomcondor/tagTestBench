------------------------------------------------------------------------------
--	Weixu ZHU (Harry)
--		zhuweixu_harry@126.com
--	Version: 1.1
------------------------------------------------------------------------------

trackingPath = ';../blocktracking/'
solveSquPath = 'solveSqu/'
mathPath = 'math/'
luaFile = '?.lua'
package.path = package.path .. trackingPath .. luaFile
package.path = package.path .. trackingPath .. solveSquPath .. luaFile
package.path = package.path .. trackingPath .. mathPath .. luaFile

require("calcPos")
require("tracking")
Vec3 = require("Vector3")

--[[
function input:
for every frame, build a taglist, which is a table, to lua
taglist, as the para of func
{
	n = <a number> the number of tags
	1 = <a table> which is a tag
	    {
   			center = {x = **, y = **}
			corners = <a table>
					{
						1 = {x = **, y = **}
						2
						3
						4
					}
		}
	2
	3
	4
	...
}
--]]

local halfTag = 0.012
local halfBox = 0.0275
-- expected unit is meter
local tags = {n = 0,label = {}}
local boxes = {n = 0,label = {}}
local structures = {n = 0,label = {}}

function blocktracking(tags_seeing)
	-- Calc position of tags ------------------------------------
	for i = 1, tags_seeing.n do
		tags_seeing[i].halfL = halfTag
		tags_seeing[i].camera_flag = tags_seeing.camera_flag
		local pos = calTagPos(tags_seeing[i])
			-- calTagPos returns a table (for each tag)
				-- {rotation = {x=,y=,z=}  <a vector> the direction vector of the tag, 
					-- clockwise : to point outside the box
					-- anti-clockwise : to point inside the box
				--	translation = {x=,y=,z=} <a vector> the position of the the tag
				--	quaternion = {x,y,z,w} <a quaternion> the quaternion rotation of the tag
				-- }
		tags_seeing[i].rotation = pos.rotation
		tags_seeing[i].translation = pos.translation
		tags_seeing[i].quaternion = pos.quaternion
	end
	trackingTags(tags,tags_seeing)
	-- Calc postion of boxes ----------------------------------
	tags.halfBox = halfBox
	local boxes_seeing = calcBoxPos(tags)
	trackingBoxes(boxes,boxes_seeing)
	--[[
		boxes has
		{
			n
			1 = {
					translation
					rotation
					quaternion
					average

					nTags
					1 = <atag> {translation, rotation, quaternion}
					2
				}
		}
	--]]
	-- Calc structures  ----------------------------------
	boxes_seeing.halfBox = halfBox
	boxes.halfBox = halfBox
	local structures_seeing = calcStructure(boxes) 

								--[[
								print("-----------------------------")
								print("structures n : ",structures_seeing.n)
								for i = 1, boxes.n do
									print("box",boxes[i].label,"its scale = ",boxes[i].groupscale)
								end
								print("-----------------------------")
								--]]

	return {tags = tags,boxes = boxes}
	--return tags, boxes, structures_seeing
end
