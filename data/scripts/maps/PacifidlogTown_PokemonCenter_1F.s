PacifidlogTown_PokemonCenter_1F_MapScripts:: @ 81540FF
	map_script 3, PacifidlogTown_PokemonCenter_1F_MapScript1_154105
	.byte 0

PacifidlogTown_PokemonCenter_1F_MapScript1_154105:: @ 8154105
	sethealplace 19
	end

@ 8154109
	setvar 0x800b, 1
	call VerdanturfTown_PokemonCenter_1F_EventScript_19FD5B
	waittext
	waitbutton
	release
	end

PacifidlogTown_PokemonCenter_1F_EventScript_154117:: @ 8154117
	msgbox PacifidlogTown_PokemonCenter_1F_Text_178E47, 2
	end

PacifidlogTown_PokemonCenter_1F_EventScript_154120:: @ 8154120
	msgbox PacifidlogTown_PokemonCenter_1F_Text_178E79, 2
	end

PacifidlogTown_PokemonCenter_1F_EventScript_154129:: @ 8154129
	msgbox PacifidlogTown_PokemonCenter_1F_Text_178ED9, 2
	end

