	.include "asm/macros.inc"
	.include "constants/constants.inc"

	.section .rodata

	.align 2
gSpriteImageTable_83ED0C0:: @ 83ED0C0
	obj_frame_tiles gSharedMem + 0x13200, 0x800
	obj_frame_tiles gSharedMem + 0x13A00, 0x800
	obj_frame_tiles gSharedMem + 0x14200, 0x800
	obj_frame_tiles gSharedMem + 0x14A00, 0x800
	obj_frame_tiles gSharedMem + 0x15200, 0x800

	.align 2
gSpriteImageTable_83ED0E8:: @ 83ED0E8
	obj_frame_tiles gSharedMem + 0x15A00, 0x300

	.align 2
gSpriteImageTable_83ED0F0:: @ 83ED0F0
	obj_frame_tiles gSharedMem + 0x15D00, 0x500

	.align 2
gSpriteImageTable_83ED0F8:: @ 83ED0F8
	obj_frame_tiles gSharedMem + 0x16200, 0x600

	.align 2
gSpriteImageTable_83ED100:: @ 83ED100
	obj_frame_tiles gSpriteImage_8E988E8, 0x80
	obj_frame_tiles gSpriteImage_8E98968, 0x80
	obj_frame_tiles gSpriteImage_8E989E8, 0x80
	obj_frame_tiles gSpriteImage_8E98A68, 0x80
	obj_frame_tiles gSpriteImage_8E98AE8, 0x80
	obj_frame_tiles gSpriteImage_8E98B68, 0x80

	.align 2
gSpriteImageTable_83ED130:: @ 83ED130
	obj_frame_tiles gSpriteImage_8E991E8, 0x200

	.align 2
gSpriteImageTable_83ED138:: @ 83ED138
	obj_frame_tiles gSpriteImage_8E99808, 0x40

	.align 2
gSpriteImageTable_83ED140:: @ 83ED140
	obj_frame_tiles gSpriteImage_8E98BE8, 0x100
	obj_frame_tiles gSpriteImage_8E98CE8, 0x100

	.align 2
gSpriteImageTable_83ED150:: @ 83ED150
	obj_frame_tiles gSpriteImage_8E993E8, 0x400

	.align 2
gSpriteImageTable_83ED158:: @ 83ED158
	obj_frame_tiles gSpriteImage_8E98DE8, 0x200
	obj_frame_tiles gSpriteImage_8E98FE8, 0x200

	.align 2
gSpriteImageTable_83ED168:: @ 83ED168
	obj_frame_tiles gSpriteImage_8E98848, 0x20

	.align 2
gSpriteImageTable_83ED170:: @ 83ED170
	obj_frame_tiles gSpriteImage_8E98868, 0x80

	.align 2
gSpriteImageTable_83ED178:: @ 83ED178
	obj_frame_tiles gSharedMem + 0x10000, 0x600

	.align 2
gSpriteImageTable_83ED180:: @ 83ED180
	obj_frame_tiles gSharedMem + 0x10600, 0x200

	.align 2
gSpriteImageTable_83ED188:: @ 83ED188
	obj_frame_tiles gSharedMem + 0x10800, 0x200

	.align 2
gSpriteImageTable_83ED190:: @ 83ED190
	obj_frame_tiles gSharedMem + 0x10A00, 0x200

	.align 2
gSpriteImageTable_83ED198:: @ 83ED198
	obj_frame_tiles gSharedMem + 0x10C00, 0x300

	.align 2
gSpriteImageTable_83ED1A0:: @ 83ED1A0
	obj_frame_tiles gSharedMem + 0x11000, 0x400

	.align 2
gSpriteImageTable_83ED1A8:: @ 83ED1A8
	obj_frame_tiles gSharedMem + 0x11400, 0x200

	.align 2
gSpriteImageTable_83ED1B0:: @ 83ED1B0
	obj_frame_tiles gSharedMem + 0x11600, 0x300

	.align 2
gSpriteImageTable_83ED1B8:: @ 83ED1B8
	obj_frame_tiles gSharedMem + 0x11900, 0x300

	.align 2
gSpriteImageTable_83ED1C0:: @ 83ED1C0
	obj_frame_tiles gSharedMem + 0x11C00, 0x200
	obj_frame_tiles gSharedMem + 0x11E00, 0x200
	obj_frame_tiles gSharedMem + 0x11E00, 0x200 @ is this a typo?

	.align 2
gSpriteImageTable_83ED1D8:: @ 83ED1D8
	obj_frame_tiles gSharedMem + 0x12000, 0x280

	.align 2
gSpriteImageTable_83ED1E0:: @ 83ED1E0
	obj_frame_tiles gSharedMem + 0x12280, 0x80
	obj_frame_tiles gSharedMem + 0x12300, 0x80
	obj_frame_tiles gSharedMem + 0x12380, 0x80
	obj_frame_tiles gSharedMem + 0x12400, 0x80
	obj_frame_tiles gSharedMem + 0x12480, 0x80

	.align 2
gSpriteImageTable_83ED208:: @ 83ED208
	obj_frame_tiles gSharedMem + 0x12600, 0x480
	obj_frame_tiles gSharedMem + 0x12A80, 0x480

	.align 2
gSpriteImageTable_83ED218:: @ 83ED218
	obj_frame_tiles gSharedMem + 0x12F00, 0x180
	obj_frame_tiles gSharedMem + 0x13080, 0x180

	.align 2
gSpriteImageTable_83ED228:: @ 83ED228
	obj_frame_tiles gSpriteImage_8E98828, 0x20

	.align 2
gSpriteAnim_83ED230:: @ 83ED230
	obj_image_anim_frame 0, 1
	obj_image_anim_end

	.align 2
gSpriteAnim_83ED238:: @ 83ED238
	obj_image_anim_frame 0, 1
	obj_image_anim_jump 0

	.align 2
gSpriteAnim_83ED240:: @ 83ED240
	obj_image_anim_frame 0, 16
	obj_image_anim_end

	.align 2
gSpriteAnim_83ED248:: @ 83ED248
	obj_image_anim_frame 1, 16
	obj_image_anim_frame 0, 16
	obj_image_anim_jump 0

	.align 2
gSpriteAnim_83ED254:: @ 83ED254
	obj_image_anim_frame 1, 8
	obj_image_anim_frame 0, 8
	obj_image_anim_jump 0

	.align 2
gSpriteAnim_83ED260:: @ 83ED260
	obj_image_anim_frame 1, 4
	obj_image_anim_frame 0, 4
	obj_image_anim_jump 0

	.align 2
gSpriteAnim_83ED26C:: @ 83ED26C
	obj_image_anim_frame 2, 32
	obj_image_anim_frame 3, 32
	obj_image_anim_jump 0

	.align 2
gSpriteAnim_83ED278:: @ 83ED278
	obj_image_anim_frame 4, 1
	obj_image_anim_end

	.align 2
gSpriteAnim_83ED280:: @ 83ED280
	obj_image_anim_frame 0, 1
	obj_image_anim_end

	.align 2
gSpriteAnim_83ED288:: @ 83ED288
	obj_image_anim_frame 1, 1
	obj_image_anim_end

	.align 2
gSpriteAnim_83ED290:: @ 83ED290
	obj_image_anim_frame 2, 1
	obj_image_anim_end

	.align 2
gSpriteAnim_83ED298:: @ 83ED298
	obj_image_anim_frame 3, 1
	obj_image_anim_end

	.align 2
gSpriteAnim_83ED2A0:: @ 83ED2A0
	obj_image_anim_frame 4, 1
	obj_image_anim_end

	.align 2
gSpriteAnim_83ED2A8:: @ 83ED2A8
	obj_image_anim_frame 5, 1
	obj_image_anim_end

	.align 2
gSpriteAnim_83ED2B0:: @ 83ED2B0
	obj_image_anim_frame 0, 4
	obj_image_anim_frame 1, 4
	obj_image_anim_jump 0

	.align 2
gSpriteAnim_83ED2BC:: @ 83ED2BC
	obj_image_anim_frame 0, 16
	obj_image_anim_frame 1, 16
	obj_image_anim_jump 0

	.align 2
gSpriteAnim_83ED2C8:: @ 83ED2C8
	obj_image_anim_frame 0, 30
	obj_image_anim_frame 1, 30
	obj_image_anim_jump 0

	.align 2
gSpriteAnim_83ED2D4:: @ 83ED2D4
	obj_image_anim_frame 1, 1
	obj_image_anim_end

	.align 2
gSpriteAnim_83ED2DC:: @ 83ED2DC
	obj_image_anim_frame 0, 30
	obj_image_anim_frame 1, 30
	obj_image_anim_jump 0

	.align 2
gSpriteAnim_83ED2E8:: @ 83ED2E8
	obj_image_anim_frame 0, 16
	obj_image_anim_frame 1, 16
	obj_image_anim_frame 0, 16
	obj_image_anim_frame 1, 16, OBJ_IMAGE_ANIM_H_FLIP
	obj_image_anim_jump 0

	.align 2
gSpriteAnim_83ED2FC:: @ 83ED2FC
	obj_image_anim_frame 0, 1
	obj_image_anim_end

	.align 2
gSpriteAnim_83ED304:: @ 83ED304
	obj_image_anim_frame 0, 1
	obj_image_anim_end

	.align 2
gSpriteAnim_83ED30C:: @ 83ED30C
	obj_image_anim_frame 1, 1
	obj_image_anim_end

	.align 2
gSpriteAnim_83ED314:: @ 83ED314
	obj_image_anim_frame 2, 1
	obj_image_anim_end

	.align 2
gSpriteAnim_83ED31C:: @ 83ED31C
	obj_image_anim_frame 3, 1
	obj_image_anim_end

	.align 2
gSpriteAnim_83ED324:: @ 83ED324
	obj_image_anim_frame 4, 1
	obj_image_anim_end

	.align 2
gSpriteAnimTable_83ED32C:: @ 83ED32C
	.4byte gSpriteAnim_83ED230

	.align 2
gSpriteAnimTable_83ED330:: @ 83ED330
	.4byte gSpriteAnim_83ED238

	.align 2
gSpriteAnimTable_83ED334:: @ 83ED334
	.4byte gSpriteAnim_83ED240
	.4byte gSpriteAnim_83ED248
	.4byte gSpriteAnim_83ED254
	.4byte gSpriteAnim_83ED260
	.4byte gSpriteAnim_83ED26C
	.4byte gSpriteAnim_83ED278

	.align 2
gSpriteAnimTable_83ED34C:: @ 83ED34C
	.4byte gSpriteAnim_83ED280
	.4byte gSpriteAnim_83ED288
	.4byte gSpriteAnim_83ED290
	.4byte gSpriteAnim_83ED298
	.4byte gSpriteAnim_83ED2A0
	.4byte gSpriteAnim_83ED2A8

	.align 2
gSpriteAnimTable_83ED364:: @ 83ED364
	.4byte gSpriteAnim_83ED2B0

	.align 2
gSpriteAnimTable_83ED368:: @ 83ED368
	.4byte gSpriteAnim_83ED2BC

	.align 2
gSpriteAnimTable_83ED36C:: @ 83ED36C
	.4byte gSpriteAnim_83ED2C8
	.4byte gSpriteAnim_83ED2D4

	.align 2
gSpriteAnimTable_83ED374:: @ 83ED374
	.4byte gSpriteAnim_83ED2DC

	.align 2
gSpriteAnimTable_83ED378:: @ 83ED378
	.4byte gSpriteAnim_83ED2E8
	.4byte gSpriteAnim_83ED2FC

	.align 2
gSpriteAnimTable_83ED380:: @ 83ED380
	.4byte gSpriteAnim_83ED304
	.4byte gSpriteAnim_83ED30C
	.4byte gSpriteAnim_83ED314
	.4byte gSpriteAnim_83ED31C
	.4byte gSpriteAnim_83ED324

	.align 2
gSpriteAffineAnim_83ED394:: @ 83ED394
	obj_rot_scal_anim_frame 0x10, 0x10, 0, 0
	obj_rot_scal_anim_loop 0
	obj_rot_scal_anim_frame 0x1, 0x1, 0, 1
	obj_rot_scal_anim_loop 255
	obj_rot_scal_anim_end

	.align 2
gSpriteAffineAnimTable_83ED3BC:: @ 83ED3BC
	.4byte gSpriteAffineAnim_83ED394

	.align 2
gSpriteAffineAnim_83ED3C0:: @ 83ED3C0
	obj_rot_scal_anim_frame 0x0, 0x0, 8, 32
	obj_rot_scal_anim_frame 0x0, 0x0, 6, 32
	obj_rot_scal_anim_frame 0x0, 0x0, 4, 16
	obj_rot_scal_anim_frame 0x0, 0x0, 12, 2
	obj_rot_scal_anim_frame 0x0, 0x0, -12, 4
	obj_rot_scal_anim_frame 0x0, 0x0, 12, 2
	obj_rot_scal_anim_frame 0x0, 0x0, 12, 2
	obj_rot_scal_anim_frame 0x0, 0x0, -12, 4
	obj_rot_scal_anim_frame 0x0, 0x0, 12, 2
	obj_rot_scal_anim_end

	.align 2
gSpriteAffineAnimTable_83ED410:: @ 83ED410
	.4byte gSpriteAffineAnim_83ED3C0

	.align 2
gSpriteTemplate_83ED414:: @ 83ED414
	spr_template 0, 0, gOamData_83ED0A0, gSpriteAnimTable_83ED32C, NULL, gDummySpriteAffineAnimTable, sub_8104F18

	.align 2
gSpriteTemplate_83ED42C:: @ 83ED42C
	spr_template 7, 4, gOamData_83ED088, gSpriteAnimTable_83ED32C, NULL, gDummySpriteAffineAnimTable, sub_810506C

	.align 2
gSpriteTemplate_83ED444:: @ 83ED444
	spr_template 17, 0, gOamData_83ED0B8, gSpriteAnimTable_83ED32C, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED45C:: @ 83ED45C
	spr_template 0xFFFF, 1, gOamData_83ED0B8, gSpriteAnimTable_83ED334, gSpriteImageTable_83ED0C0, gDummySpriteAffineAnimTable, sub_8105170

	.align 2
gSpriteTemplate_83ED474:: @ 83ED474
	spr_template 0xFFFF, 2, gOamData_83ED088, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED0E8, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED48C:: @ 83ED48C
	spr_template 0xFFFF, 3, gOamData_83ED088, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED0F0, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED4A4:: @ 83ED4A4
	spr_template 0xFFFF, 3, gOamData_83ED088, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED0F8, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED4BC:: @ 83ED4BC
	spr_template 0xFFFF, 4, gOamData_83ED090, gSpriteAnimTable_83ED34C, gSpriteImageTable_83ED100, gDummySpriteAffineAnimTable, sub_810535C

	.align 2
gSpriteTemplate_83ED4D4:: @ 83ED4D4
	spr_template 0xFFFF, 4, gOamData_83ED090, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED130, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED4EC:: @ 83ED4EC
	spr_template 0xFFFF, 4, gOamData_83ED090, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED138, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED504:: @ 83ED504
	spr_template 0xFFFF, 4, gOamData_83ED098, gSpriteAnimTable_83ED364, gSpriteImageTable_83ED140, gDummySpriteAffineAnimTable, sub_810562C

	.align 2
gSpriteTemplate_83ED51C:: @ 83ED51C
	spr_template 0xFFFF, 7, gOamData_83ED0A8, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED150, gDummySpriteAffineAnimTable, sub_8105784

	.align 2
gSpriteTemplate_83ED534:: @ 83ED534
	spr_template 0xFFFF, 5, gOamData_83ED0A0, gSpriteAnimTable_83ED368, gSpriteImageTable_83ED158, gDummySpriteAffineAnimTable, sub_8105894

	.align 2
gSpriteTemplate_83ED54C:: @ 83ED54C
	spr_template 0xFFFF, 4, gOamData_83ED080, gSpriteAnimTable_83ED330, gSpriteImageTable_83ED168, gDummySpriteAffineAnimTable, sub_810594C

	.align 2
gSpriteTemplate_83ED564:: @ 83ED564
	spr_template 0xFFFF, 4, gOamData_83ED090, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED170, gSpriteAffineAnimTable_83ED3BC, sub_8105A38

	.align 2
gSpriteTemplate_83ED57C:: @ 83ED57C
	spr_template 0xFFFF, 6, gOamData_83ED080, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED178, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED594:: @ 83ED594
	spr_template 0xFFFF, 6, gOamData_83ED080, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED180, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED5AC:: @ 83ED5AC
	spr_template 0xFFFF, 6, gOamData_83ED080, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED188, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED5C4:: @ 83ED5C4
	spr_template 18, 6, gOamData_83ED080, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED190, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED5DC:: @ 83ED5DC
	spr_template 0xFFFF, 6, gOamData_83ED0B0, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED198, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED5F4:: @ 83ED5F4
	spr_template 0xFFFF, 6, gOamData_83ED0B0, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED1A0, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED60C:: @ 83ED60C
	spr_template 19, 6, gOamData_83ED080, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED1A8, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED624:: @ 83ED624
	spr_template 20, 6, gOamData_83ED080, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED1B0, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED63C:: @ 83ED63C
	spr_template 21, 6, gOamData_83ED080, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED1B8, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED654:: @ 83ED654
	spr_template 0xFFFF, 6, gOamData_83ED0A0, gSpriteAnimTable_83ED36C, gSpriteImageTable_83ED1C0, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED66C:: @ 83ED66C
	spr_template 0xFFFF, 6, gOamData_83ED080, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED1D8, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED684:: @ 83ED684
	spr_template 0xFFFF, 6, gOamData_83ED090, gSpriteAnimTable_83ED380, gSpriteImageTable_83ED1E0, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED69C:: @ 83ED69C
	spr_template 0xFFFF, 6, gOamData_83ED080, gSpriteAnimTable_83ED378, gSpriteImageTable_83ED208, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED6B4:: @ 83ED6B4
	spr_template 0xFFFF, 6, gOamData_83ED080, gSpriteAnimTable_83ED374, gSpriteImageTable_83ED218, gDummySpriteAffineAnimTable, SpriteCallbackDummy

	.align 2
gSpriteTemplate_83ED6CC:: @ 83ED6CC
	spr_template 0xFFFF, 4, gOamData_83ED080, gSpriteAnimTable_83ED32C, gSpriteImageTable_83ED228, gSpriteAffineAnimTable_83ED410, sub_8105B70

	.align 2
gSubspriteTable_83ED6E4:: @ 83ED6E4
	subsprite -64, -64, 3,   0, 64x64
	subsprite   0, -64, 3,   0, 64x64
	subsprite -64,   0, 3,   0, 64x64
	subsprite   0,   0, 3,   0, 64x64

	.align 2
gSubspriteTables_83ED704:: @ 83ED704
	.4byte 4, gSubspriteTable_83ED6E4

	.align 2
gSubspriteTable_83ED70C:: @ 83ED70C
	subsprite -32, -12, 1,   0, 32x8
	subsprite   0, -12, 1,   4, 32x8
	subsprite -32,  -4, 1,   8, 32x8
	subsprite   0,  -4, 1,  12, 32x8
	subsprite -32,   4, 1,  16, 32x8
	subsprite   0,   4, 1,  20, 32x8

	.align 2
gSubspriteTables_83ED73C:: @ 83ED73C
	.4byte 6, gSubspriteTable_83ED70C

	.align 2
gSubspriteTable_83ED744:: @ 83ED744
	subsprite -32, -20, 1,   0, 64x32
	subsprite -32,  12, 1,  32, 32x8
	subsprite   0,  12, 1,  36, 32x8

	.align 2
gSubspriteTables_83ED75C:: @ 83ED75C
	.4byte 3, gSubspriteTable_83ED744

	.align 2
gSubspriteTable_83ED764:: @ 83ED764
	subsprite -32, -24, 1,   0, 64x32
	subsprite -32,   8, 1,  32, 32x8
	subsprite   0,   8, 1,  36, 32x8
	subsprite -32,  16, 1,  40, 32x8
	subsprite   0,  16, 1,  44, 32x8

	.align 2
gSubspriteTables_83ED78C:: @ 83ED78C
	.4byte 5, gSubspriteTable_83ED764

	.align 2
gSubspriteTable_83ED794:: @ 83ED794
	subsprite -32,  -8, 1,   0, 32x8
	subsprite   0,  -8, 1,   4, 32x8
	subsprite -32,   0, 1,   8, 32x8
	subsprite   0,   0, 1,  12, 32x8

	.align 2
gSubspriteTables_83ED7B4:: @ 83ED7B4
	.4byte 4, gSubspriteTable_83ED794

	.align 2
gSubspriteTable_83ED7BC:: @ 83ED7BC
	subsprite  -8, -12, 1,   0, 16x8
	subsprite  -8,  -4, 1,   0, 16x8
	subsprite  -8,   4, 1,   0, 16x8

	.align 2
gSubspriteTables_83ED7D4:: @ 83ED7D4
	.4byte 3, gSubspriteTable_83ED7BC

	.align 2
gSubspriteTable_83ED7DC:: @ 83ED7DC
	subsprite -32, -24, 3,   0, 64x32
	subsprite -32,   8, 3,  32, 32x8
	subsprite   0,   8, 3,  36, 32x8
	subsprite -32,  16, 3,  40, 32x8
	subsprite   0,  16, 3,  44, 32x8

	.align 2
gSubspriteTables_83ED804:: @ 83ED804
	.4byte 5, gSubspriteTable_83ED7DC

	.align 2
gSubspriteTable_83ED80C:: @ 83ED80C
	subsprite -32,  -8, 3,   0, 32x8
	subsprite   0,  -8, 3,   4, 32x8
	subsprite -32,   0, 3,   8, 32x8
	subsprite   0,   0, 3,  12, 32x8

	.align 2
gSubspriteTables_83ED82C:: @ 83ED82C
	.4byte 4, gSubspriteTable_83ED80C

	.align 2
gSubspriteTable_83ED834:: @ 83ED834
	subsprite -32,  -8, 3,   0, 32x8
	subsprite   0,  -8, 3,   4, 32x8
	subsprite -32,   0, 3,   8, 32x8
	subsprite   0,   0, 3,  12, 32x8

	.align 2
gSubspriteTables_83ED854:: @ 83ED854
	.4byte 4, gSubspriteTable_83ED834

	.align 2
gSubspriteTable_83ED85C:: @ 83ED85C
	subsprite -32,  -8, 3,   0, 32x8
	subsprite   0,  -8, 3,   4, 32x8
	subsprite -32,   0, 3,   8, 32x8
	subsprite   0,   0, 3,  12, 32x8

	.align 2
gSubspriteTables_83ED87C:: @ 83ED87C
	.4byte 4, gSubspriteTable_83ED85C

	.align 2
gSubspriteTable_83ED884:: @ 83ED884
	subsprite -32, -12, 3,   0, 32x8
	subsprite   0, -12, 3,   4, 32x8
	subsprite -32,  -4, 3,   8, 32x8
	subsprite   0,  -4, 3,  12, 32x8
	subsprite -32,   4, 3,  16, 32x8
	subsprite   0,   4, 3,  20, 32x8

	.align 2
gSubspriteTables_83ED8B4:: @ 83ED8B4
	.4byte 6, gSubspriteTable_83ED884

	.align 2
gSubspriteTable_83ED8BC:: @ 83ED8BC
	subsprite -16, -16, 3,   0, 32x32

	.align 2
gSubspriteTable_83ED8C4:: @ 83ED8C4
	subsprite  -8,  -8, 3,  16, 16x16

	.align 2
gSubspriteTables_83ED8CC:: @ 83ED8CC
	.4byte 1, gSubspriteTable_83ED8BC
	.4byte 1, gSubspriteTable_83ED8C4

	.align 2
gSubspriteTable_83ED8DC:: @ 83ED8DC
	subsprite -24, -24, 3,   0, 32x8
	subsprite   8, -24, 3,   4, 16x8
	subsprite -24, -16, 3,   6, 32x8
	subsprite   8, -16, 3,  10, 16x8
	subsprite -24,  -8, 3,  12, 32x8
	subsprite   8,  -8, 3,  16, 16x8
	subsprite -24,   0, 3,  18, 32x8
	subsprite   8,   0, 3,  22, 16x8
	subsprite -24,   8, 3,  24, 32x8
	subsprite   8,   8, 3,  28, 16x8
	subsprite -24,  16, 3,  30, 32x8
	subsprite   8,  16, 3,  34, 16x8

	.align 2
gSubspriteTables_83ED93C:: @ 83ED93C
	.4byte 12, gSubspriteTable_83ED8DC

	.align 2
gSubspriteTable_83ED944:: @ 83ED944
	subsprite -16, -12, 3,   0, 32x16
	subsprite -16,   4, 3,   8, 16x8
	subsprite   0,   4, 3,  10, 16x8

	.align 2
gSubspriteTables_83ED95C:: @ 83ED95C
	.4byte 3, gSubspriteTable_83ED944

	.align 2
gSubspriteTable_83ED964:: @ 83ED964
	subsprite  -8,  -8, 3,   0, 16x8
	subsprite  -8,   0, 3,   8, 16x8

	.align 2
gSubspriteTables_83ED974:: @ 83ED974
	.4byte 2, gSubspriteTable_83ED964

	.align 2
gSubspriteTable_83ED97C:: @ 83ED97C
	subsprite  -8,  -8, 3,   2, 16x8
	subsprite  -8,   0, 3,  10, 16x8

	.align 2
gSubspriteTables_83ED98C:: @ 83ED98C
	.4byte 2, gSubspriteTable_83ED97C

	.align 2
gSubspriteTable_83ED994:: @ 83ED994
	subsprite  -8,  -8, 3,   4, 16x8
	subsprite  -8,   0, 3,  12, 16x8

	.align 2
gSubspriteTables_83ED9A4:: @ 83ED9A4
	.4byte 2, gSubspriteTable_83ED994

	.align 2
gSubspriteTable_83ED9AC:: @ 83ED9AC
	subsprite  -8,  -8, 3,   6, 16x8
	subsprite  -8,   0, 3,  14, 16x8

	.align 2
gSubspriteTables_83ED9BC:: @ 83ED9BC
	.4byte 2, gSubspriteTable_83ED9AC

	.align 2
gSubspriteTable_83ED9C4:: @ 83ED9C4
	subsprite  -8,  -8, 3,   0, 16x8
	subsprite  -8,   0, 3,   8, 16x8

	.align 2
gSubspriteTables_83ED9D4:: @ 83ED9D4
	.4byte 2, gSubspriteTable_83ED9C4

	.align 2
gSubspriteTable_83ED9DC:: @ 83ED9DC
	subsprite  -4,  -8, 3,   2,  8x8
	subsprite  -4,   0, 3,  10,  8x8

	.align 2
gSubspriteTables_83ED9EC:: @ 83ED9EC
	.4byte 2, gSubspriteTable_83ED9DC

	.align 2
gSubspriteTable_83ED9F4:: @ 83ED9F4
	subsprite  -8,  -8, 3,   3, 16x8
	subsprite  -8,   0, 3,  11, 16x8

	.align 2
gSubspriteTables_83EDA04:: @ 83EDA04
	.4byte 2, gSubspriteTable_83ED9F4

	.align 2
gSubspriteTable_83EDA0C:: @ 83EDA0C
	subsprite  -4,  -8, 3,   5,  8x8
	subsprite  -4,   0, 3,  13,  8x8

	.align 2
gSubspriteTables_83EDA1C:: @ 83EDA1C
	.4byte 2, gSubspriteTable_83EDA0C

	.align 2
gSubspriteTable_83EDA24:: @ 83EDA24
	subsprite  -8,  -8, 3,   6, 16x8
	subsprite  -8,   0, 3,  14, 16x8

	.align 2
gSubspriteTables_83EDA34:: @ 83EDA34
	.4byte 2, gSubspriteTable_83EDA24

	.align 2
gSubspriteTable_83EDA3C:: @ 83EDA3C
	subsprite -12, -12, 3,   0, 16x8
	subsprite   4, -12, 3,   2,  8x8
	subsprite -12,  -4, 3,   8, 16x8
	subsprite   4,  -4, 3,  10,  8x8
	subsprite -12,   4, 3,  16, 16x8
	subsprite   4,   4, 3,  18,  8x8

	.align 2
gSubspriteTables_83EDA6C:: @ 83EDA6C
	.4byte 6, gSubspriteTable_83EDA3C

	.align 2
gSubspriteTable_83EDA74:: @ 83EDA74
	subsprite  -8, -12, 3,   3, 16x8
	subsprite  -8,  -4, 3,  11, 16x8
	subsprite  -8,   4, 3,  19, 16x8

	.align 2
gSubspriteTables_83EDA8C:: @ 83EDA8C
	.4byte 3, gSubspriteTable_83EDA74

	.align 2
gSubspriteTable_83EDA94:: @ 83EDA94
	subsprite -12, -12, 3,   5, 16x8
	subsprite   4, -12, 3,   7,  8x8
	subsprite -12,  -4, 3,  13, 16x8
	subsprite   4,  -4, 3,  15,  8x8
	subsprite -12,   4, 3,  21, 16x8
	subsprite   4,   4, 3,  23,  8x8

	.align 2
gSubspriteTables_83EDAC4:: @ 83EDAC4
	.4byte 6, gSubspriteTable_83EDA94

	.align 2
gSubspriteTable_83EDACC:: @ 83EDACC
	subsprite -12, -12, 3,   0, 16x8
	subsprite   4, -12, 3,   2,  8x8
	subsprite -12,  -4, 3,   8, 16x8
	subsprite   4,  -4, 3,  10,  8x8
	subsprite -12,   4, 3,  16, 16x8
	subsprite   4,   4, 3,  18,  8x8

	.align 2
gSubspriteTables_83EDAFC:: @ 83EDAFC
	.4byte 6, gSubspriteTable_83EDACC

	.align 2
gSubspriteTable_83EDB04:: @ 83EDB04
	subsprite  -8, -12, 3,   3, 16x8
	subsprite  -8,  -4, 3,  11, 16x8
	subsprite  -8,   4, 3,  19, 16x8

	.align 2
gSubspriteTables_83EDB1C:: @ 83EDB1C
	.4byte 3, gSubspriteTable_83EDB04

	.align 2
gSubspriteTable_83EDB24:: @ 83EDB24
	subsprite -12, -12, 3,   5, 16x8
	subsprite   4, -12, 3,   7,  8x8
	subsprite -12,  -4, 3,  13, 16x8
	subsprite   4,  -4, 3,  15,  8x8
	subsprite -12,   4, 3,  21, 16x8
	subsprite   4,   4, 3,  23,  8x8

	.align 2
gSubspriteTables_83EDB54:: @ 83EDB54
	.4byte 6, gSubspriteTable_83EDB24

	.align 2
gUnknown_083EDB5C:: @ 83EDB5C
	.4byte gSpriteTemplate_83ED57C
	.4byte gSpriteTemplate_83ED594
	.4byte gSpriteTemplate_83ED5AC
	.4byte gSpriteTemplate_83ED5DC
	.4byte gSpriteTemplate_83ED5F4
	.4byte gSpriteTemplate_83ED654
	.4byte gSpriteTemplate_83ED66C
	.4byte gSpriteTemplate_83ED684
	.4byte gSpriteTemplate_83ED69C
	.4byte gSpriteTemplate_83ED6B4
	.4byte gSpriteTemplate_83ED5C4
	.4byte gSpriteTemplate_83ED5C4
	.4byte gSpriteTemplate_83ED5C4
	.4byte gSpriteTemplate_83ED5C4
	.4byte gSpriteTemplate_83ED60C
	.4byte gSpriteTemplate_83ED60C
	.4byte gSpriteTemplate_83ED60C
	.4byte gSpriteTemplate_83ED60C
	.4byte gSpriteTemplate_83ED60C
	.4byte gSpriteTemplate_83ED624
	.4byte gSpriteTemplate_83ED624
	.4byte gSpriteTemplate_83ED624
	.4byte gSpriteTemplate_83ED63C
	.4byte gSpriteTemplate_83ED63C
	.4byte gSpriteTemplate_83ED63C
	.4byte gDummySpriteTemplate

	.align 2
gUnknown_083EDBC4:: @ 83EDBC4
	.4byte gSubspriteTables_83ED804
	.4byte gSubspriteTables_83ED82C
	.4byte gSubspriteTables_83ED854
	.4byte gSubspriteTables_83ED8B4
	.4byte NULL
	.4byte NULL
	.4byte gSubspriteTables_83ED8CC
	.4byte NULL
	.4byte gSubspriteTables_83ED93C
	.4byte gSubspriteTables_83ED95C
	.4byte gSubspriteTables_83ED974
	.4byte gSubspriteTables_83ED98C
	.4byte gSubspriteTables_83ED9A4
	.4byte gSubspriteTables_83ED9BC
	.4byte gSubspriteTables_83ED9D4
	.4byte gSubspriteTables_83ED9EC
	.4byte gSubspriteTables_83EDA04
	.4byte gSubspriteTables_83EDA1C
	.4byte gSubspriteTables_83EDA34
	.4byte gSubspriteTables_83EDA6C
	.4byte gSubspriteTables_83EDA8C
	.4byte gSubspriteTables_83EDAC4
	.4byte gSubspriteTables_83EDAFC
	.4byte gSubspriteTables_83EDB1C
	.4byte gSubspriteTables_83EDB54
	.4byte NULL

	.align 2
gUnknown_083EDC2C:: @ 83EDC2C
	obj_tiles gSlotMachineReelSymbol1Tiles, 0x200, 0
	obj_tiles gSlotMachineReelSymbol2Tiles, 0x200, 1
	obj_tiles gSlotMachineReelSymbol3Tiles, 0x200, 2
	obj_tiles gSlotMachineReelSymbol4Tiles, 0x200, 3
	obj_tiles gSlotMachineReelSymbol5Tiles, 0x200, 4
	obj_tiles gSlotMachineReelSymbol6Tiles, 0x200, 5
	obj_tiles gSlotMachineReelSymbol7Tiles, 0x200, 6
	obj_tiles gSlotMachineNumber0Tiles, 0x40, 7
	obj_tiles gSlotMachineNumber1Tiles, 0x40, 8
	obj_tiles gSlotMachineNumber2Tiles, 0x40, 9
	obj_tiles gSlotMachineNumber3Tiles, 0x40, 10
	obj_tiles gSlotMachineNumber4Tiles, 0x40, 11
	obj_tiles gSlotMachineNumber5Tiles, 0x40, 12
	obj_tiles gSlotMachineNumber6Tiles, 0x40, 13
	obj_tiles gSlotMachineNumber7Tiles, 0x40, 14
	obj_tiles gSlotMachineNumber8Tiles, 0x40, 15
	obj_tiles gSlotMachineNumber9Tiles, 0x40, 16
	obj_tiles gSharedMem + 0x10A00, 0x200, 18
	obj_tiles gSharedMem + 0x11400, 0x200, 19
	obj_tiles gSharedMem + 0x11600, 0x300, 20
	obj_tiles gSharedMem + 0x11900, 0x300, 21
	null_obj_tiles

	.align 2
gUnknown_083EDCDC:: @ 83EDCDC
	obj_tiles gSharedMem + 0x10000, 2048, 0x0011

	.align 2
gUnknown_083EDCE4:: @ 83EDCE4
	.4byte gUnknownPalette_08E997E8

UnknownPalette_83EDCE8:
	.ifdef SAPPHIRE
	.incbin "graphics/unknown/sapphire_83EDD40.gbapal"
	.else
	.incbin "graphics/unknown/ruby_83EDCE8.gbapal"
	.endif

	.align 2
gUnknown_083EDD08:: @ 83EDD08
	.4byte UnknownPalette_83EDCE8 + 10 * 2
	.4byte UnknownPalette_83EDCE8 + 11 * 2
	.4byte UnknownPalette_83EDCE8 + 12 * 2
	.4byte UnknownPalette_83EDCE8 + 13 * 2
	.4byte UnknownPalette_83EDCE8 + 14 * 2

	.align 2
gUnknown_083EDD1C:: @ 83EDD1C
	.4byte gUnknown_08E95A18 + 148
	.4byte gUnknown_08E95A18 + 150
	.4byte gUnknown_08E95A18 + 152
	.4byte gUnknown_08E95A18 + 154
	.4byte gUnknown_08E95A18 + 156

gUnknown_083EDD30:: @ 83EDD30
	.byte 0x4A, 0x4B, 0x4C, 0x4E, 0x4D

gUnknown_083EDD35:: @ 83EDD35
	.byte 0, 0, 1, 2, 3, 4

gUnknown_083EDD3B:: @ 83EDD3B
	.byte 1, 2, 2

	.align 1
Unknown_83EDD3E:
	.ifdef SAPPHIRE
	.incbin "graphics/unknown/sapphire_83EDD96.gbapal"
	.else
	.incbin "graphics/unknown/ruby_83EDD3E.gbapal"
	.endif

	.align 1
Unknown_83EDD5E:
	.ifdef SAPPHIRE
	.incbin "graphics/unknown/sapphire_83EDDB6.gbapal"
	.else
	.incbin "graphics/unknown/ruby_83EDD5E.gbapal"
	.endif

	.align 1
Unknown_83EDD7E:
	.ifdef SAPPHIRE
	.incbin "graphics/unknown/sapphire_83EDDD6.gbapal"
	.else
	.incbin "graphics/unknown/ruby_83EDD7E.gbapal"
	.endif

	.align 2
gUnknown_083EDDA0:: @ 83EDDA0
	.4byte Unknown_83EDD3E
	.4byte Unknown_83EDD5E
	.4byte Unknown_83EDD7E

	.align 2
gUnknown_083EDDAC:: @ 83EDDAC
	.4byte gUnknown_08E95A18 + 32

	.align 1
Palette_83EDDB0:
	.incbin "graphics/slot_machine/83EDDB0.gbapal"

	.align 1
Palette_83EDDD0:
	.incbin "graphics/slot_machine/83EDDD0.gbapal"

	.align 1
Palette_83EDDF0:
	.incbin "graphics/slot_machine/83EDDF0.gbapal"

	.align 2
gUnknown_083EDE10:: @ 83EDE10
	.4byte Palette_83EDDB0
	.4byte Palette_83EDDD0
	.4byte Palette_83EDDF0
	.4byte gSlotMachineSpritePalette6

	.align 2
gUnknown_083EDE20:: @ 83EDE20
	.4byte gSlotMachineSpritePalette6

	.align 1
gPalette_83EDE24:: @ 83EDE24
	.incbin "graphics/slot_machine/83EDE24_pal.bin"

	.align 2
gSlotMachineSpritePalettes:: @ 83EDE44
	obj_pal gSlotMachineSpritePalette0, 0
	obj_pal gSlotMachineSpritePalette1, 1
	obj_pal gSlotMachineSpritePalette2, 2
	obj_pal gSlotMachineSpritePalette3, 3
	obj_pal gSlotMachineSpritePalette4, 4
	obj_pal gSlotMachineSpritePalette5, 5
	obj_pal gSlotMachineSpritePalette6, 6
	obj_pal gSlotMachineSpritePalette4, 7
	null_obj_pal

	.align 2
gUnknown_083EDE8C:: @ 83EDE8C
	.incbin "graphics/slot_machine/reel_time.4bpp.lz"

	.align 2
gReelTimeWindowTilemap:: @ 83EEFE0
	.incbin "graphics/slot_machine/reel_time_window_map.bin"
