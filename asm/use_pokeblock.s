	.include "constants/gba_constants.inc"
	.include "constants/species_constants.inc"
	.include "asm/macros.inc"

	.syntax unified

	.text

	thumb_func_start sub_8136130
sub_8136130: @ 8136130
	push {r4,r5,lr}
	ldr r4, _0813615C @ =gUnknown_02039304
	ldr r2, _08136160 @ =gUnknown_083DFEC4
	ldr r3, [r2]
	ldr r5, _08136164 @ =0x0000d164
	adds r2, r3, r5
	str r2, [r4]
	str r0, [r2, 0x8]
	str r1, [r2, 0x4]
	ldr r0, _08136168 @ =0x0000d162
	adds r3, r0
	movs r0, 0x2
	strb r0, [r3]
	ldr r0, _0813616C @ =sub_8136294
	bl launch_c3_walk_stairs_and_run_once
	ldr r0, _08136170 @ =sub_8136244
	bl SetMainCallback2
	pop {r4,r5}
	pop {r0}
	bx r0
	.align 2, 0
_0813615C: .4byte gUnknown_02039304
_08136160: .4byte gUnknown_083DFEC4
_08136164: .4byte 0x0000d164
_08136168: .4byte 0x0000d162
_0813616C: .4byte sub_8136294
_08136170: .4byte sub_8136244
	thumb_func_end sub_8136130

	thumb_func_start sub_8136174
sub_8136174: @ 8136174
	push {r4,r5,lr}
	ldr r5, _081361C4 @ =gUnknown_02039304
	ldr r1, [r5]
	ldr r0, _081361C8 @ =gUnknown_0203930C
	ldr r0, [r0]
	str r0, [r1, 0x8]
	ldr r0, _081361CC @ =gUnknown_02039308
	ldr r0, [r0]
	str r0, [r1, 0x4]
	ldr r4, _081361D0 @ =gUnknown_02039310
	ldrb r0, [r4]
	bl sub_81370E4
	strb r0, [r4]
	ldr r5, [r5]
	movs r1, 0
	lsls r0, 24
	lsrs r0, 24
	cmp r0, 0x3
	bls _0813619E
	movs r1, 0x1
_0813619E:
	adds r0, r5, 0
	adds r0, 0x56
	strb r1, [r0]
	ldr r0, _081361D4 @ =gUnknown_083DFEC4
	ldr r0, [r0]
	ldr r1, _081361D8 @ =0x0000d162
	adds r0, r1
	movs r1, 0x2
	strb r1, [r0]
	ldr r0, _081361DC @ =sub_8136294
	bl launch_c3_walk_stairs_and_run_once
	ldr r0, _081361E0 @ =sub_81361E4
	bl SetMainCallback2
	pop {r4,r5}
	pop {r0}
	bx r0
	.align 2, 0
_081361C4: .4byte gUnknown_02039304
_081361C8: .4byte gUnknown_0203930C
_081361CC: .4byte gUnknown_02039308
_081361D0: .4byte gUnknown_02039310
_081361D4: .4byte gUnknown_083DFEC4
_081361D8: .4byte 0x0000d162
_081361DC: .4byte sub_8136294
_081361E0: .4byte sub_81361E4
	thumb_func_end sub_8136174

	thumb_func_start sub_81361E4
sub_81361E4: @ 81361E4
	push {r4,lr}
	ldr r4, _08136220 @ =gUnknown_02039304
	ldr r0, [r4]
	ldr r0, [r0]
	bl _call_via_r0
	bl AnimateSprites
	bl BuildOamBuffer
	bl UpdatePaletteFade
	ldr r4, [r4]
	ldr r1, [r4]
	ldr r0, _08136224 @ =sub_81365C8
	cmp r1, r0
	bne _0813621A
	movs r0, 0x80
	lsls r0, 19
	movs r1, 0
	strh r1, [r0]
	adds r0, r4, 0
	adds r0, 0x50
	strb r1, [r0]
	ldr r0, _08136228 @ =sub_813622C
	bl SetMainCallback2
_0813621A:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08136220: .4byte gUnknown_02039304
_08136224: .4byte sub_81365C8
_08136228: .4byte sub_813622C
	thumb_func_end sub_81361E4

	thumb_func_start sub_813622C
sub_813622C: @ 813622C
	push {lr}
	bl sub_81368A4
	bl AnimateSprites
	bl BuildOamBuffer
	bl UpdatePaletteFade
	pop {r0}
	bx r0
	thumb_func_end sub_813622C

	thumb_func_start sub_8136244
sub_8136244: @ 8136244
	push {lr}
	ldr r0, _08136260 @ =gUnknown_02039304
	ldr r0, [r0]
	ldr r0, [r0]
	bl _call_via_r0
	bl AnimateSprites
	bl BuildOamBuffer
	bl UpdatePaletteFade
	pop {r0}
	bx r0
	.align 2, 0
_08136260: .4byte gUnknown_02039304
	thumb_func_end sub_8136244

	thumb_func_start sub_8136264
sub_8136264: @ 8136264
	push {lr}
	bl LoadOam
	bl ProcessSpriteCopyRequests
	bl TransferPlttBuffer
	movs r0, 0x6
	bl sub_80F5CDC
	bl sub_8089668
	pop {r0}
	bx r0
	thumb_func_end sub_8136264

	thumb_func_start launch_c3_walk_stairs_and_run_once
launch_c3_walk_stairs_and_run_once: @ 8136280
	ldr r1, _08136290 @ =gUnknown_02039304
	ldr r1, [r1]
	str r0, [r1]
	adds r1, 0x50
	movs r0, 0
	strb r0, [r1]
	bx lr
	.align 2, 0
_08136290: .4byte gUnknown_02039304
	thumb_func_end launch_c3_walk_stairs_and_run_once

	thumb_func_start sub_8136294
sub_8136294: @ 8136294
	push {r4,r5,lr}
	sub sp, 0x4
	ldr r1, _081362B4 @ =gUnknown_02039304
	ldr r0, [r1]
	adds r0, 0x50
	ldrb r0, [r0]
	adds r3, r1, 0
	cmp r0, 0x16
	bls _081362A8
	b _0813657A
_081362A8:
	lsls r0, 2
	ldr r1, _081362B8 @ =_081362BC
	adds r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_081362B4: .4byte gUnknown_02039304
_081362B8: .4byte _081362BC
	.align 2, 0
_081362BC:
	.4byte _08136318
	.4byte _08136354
	.4byte _0813635E
	.4byte _08136366
	.4byte _08136374
	.4byte _08136380
	.4byte _0813638C
	.4byte _081363B8
	.4byte _081363C8
	.4byte _081363DC
	.4byte _081363EE
	.4byte _08136404
	.4byte _08136448
	.4byte _08136464
	.4byte _08136488
	.4byte _08136494
	.4byte _0813649A
	.4byte _081364D8
	.4byte _081364FC
	.4byte _08136508
	.4byte _08136520
	.4byte _08136538
	.4byte _0813656C
_08136318:
	bl is_c1_link_related_active
	ldr r1, _08136344 @ =gUnknown_083DFEC4
	ldr r1, [r1]
	ldr r2, _08136348 @ =0x00006dac
	adds r1, r2
	strb r0, [r1]
	lsls r0, 24
	lsrs r1, r0, 24
	cmp r1, 0
	beq _08136330
	b _0813657A
_08136330:
	ldr r4, _0813634C @ =gUnknown_02039304
	ldr r0, [r4]
	adds r0, 0x55
	strb r1, [r0]
	ldr r0, _08136350 @ =sub_81365A0
	bl launch_c3_walk_stairs_and_run_once
	ldr r1, [r4]
	b _08136528
	.align 2, 0
_08136344: .4byte gUnknown_083DFEC4
_08136348: .4byte 0x00006dac
_0813634C: .4byte gUnknown_02039304
_08136350: .4byte sub_81365A0
_08136354:
	bl ResetSpriteData
	bl FreeAllSpritePalettes
	b _08136524
_0813635E:
	movs r0, 0
	bl SetVBlankCallback
	b _08136524
_08136366:
	ldr r0, _08136370 @ =gWindowConfig_81E7080
	bl SetUpWindowConfig
	b _08136524
	.align 2, 0
_08136370: .4byte gWindowConfig_81E7080
_08136374:
	ldr r0, _0813637C @ =gWindowConfig_81E7080
	bl MultistepInitMenuWindowBegin
	b _08136524
	.align 2, 0
_0813637C: .4byte gWindowConfig_81E7080
_08136380:
	bl MultistepInitMenuWindowContinue
	cmp r0, 0
	bne _0813638A
	b _0813657A
_0813638A:
	b _08136524
_0813638C:
	ldr r0, _081363AC @ =gUnknown_083DFEC4
	ldr r1, [r0]
	ldr r4, _081363B0 @ =0x000076aa
	adds r0, r1, r4
	movs r2, 0
	strb r2, [r0]
	ldr r4, _081363B4 @ =0x000087e0
	adds r0, r1, r4
	str r2, [r0]
	movs r0, 0xC3
	lsls r0, 2
	adds r1, r0
	movs r0, 0x20
	strh r0, [r1]
	ldr r1, [r3]
	b _08136528
	.align 2, 0
_081363AC: .4byte gUnknown_083DFEC4
_081363B0: .4byte 0x000076aa
_081363B4: .4byte 0x000087e0
_081363B8:
	bl sub_80F2688
	ldr r0, _081363D8 @ =gUnknown_02039304
	ldr r1, [r0]
	adds r1, 0x50
	ldrb r0, [r1]
	adds r0, 0x1
	strb r0, [r1]
_081363C8:
	bl sub_80F26BC
	lsls r0, 24
	cmp r0, 0
	beq _081363D4
	b _0813657A
_081363D4:
	b _08136524
	.align 2, 0
_081363D8: .4byte gUnknown_02039304
_081363DC:
	movs r0, 0x1
	bl sub_80F2C80
	ldr r0, _08136400 @ =gUnknown_02039304
	ldr r1, [r0]
	adds r1, 0x50
	ldrb r0, [r1]
	adds r0, 0x1
	strb r0, [r1]
_081363EE:
	movs r0, 0x1
	bl sub_80F2CBC
	lsls r0, 24
	cmp r0, 0
	beq _081363FC
	b _0813657A
_081363FC:
	b _08136524
	.align 2, 0
_08136400: .4byte gUnknown_02039304
_08136404:
	ldr r1, _08136434 @ =gKeyRepeatStartDelay
	movs r0, 0x14
	strh r0, [r1]
	bl CalculatePlayerPartyCount
	ldr r1, _08136438 @ =gUnknown_083DFEC4
	ldr r4, [r1]
	ldr r2, _0813643C @ =0x00008828
	adds r1, r4, r2
	movs r5, 0
	strb r0, [r1]
	ldr r1, _08136440 @ =0x00009344
	adds r0, r4, r1
	strb r5, [r0]
	subs r2, 0xC0
	adds r0, r4, r2
	str r5, [r0]
	bl sub_80F4BD0
	ldr r0, _08136444 @ =0x0000d160
	adds r4, r0
	strh r5, [r4]
	b _08136524
	.align 2, 0
_08136434: .4byte gKeyRepeatStartDelay
_08136438: .4byte gUnknown_083DFEC4
_0813643C: .4byte 0x00008828
_08136440: .4byte 0x00009344
_08136444: .4byte 0x0000d160
_08136448:
	bl sub_80F1778
	lsls r0, 24
	cmp r0, 0
	beq _08136454
	b _0813657A
_08136454:
	ldr r0, _08136460 @ =REG_BG2VOFS
	movs r1, 0x6
	strh r1, [r0]
	adds r0, 0x4
	strh r1, [r0]
	b _08136524
	.align 2, 0
_08136460: .4byte REG_BG2VOFS
_08136464:
	movs r0, 0
	bl sub_80F2E18
	ldr r0, _0813647C @ =gUnknown_083DFEC4
	ldr r0, [r0]
	ldr r1, _08136480 @ =0x00008768
	adds r0, r1
	ldr r1, [r0]
	ldr r0, _08136484 @ =0x0000ffd8
	strh r0, [r1, 0x26]
	b _08136524
	.align 2, 0
_0813647C: .4byte gUnknown_083DFEC4
_08136480: .4byte 0x00008768
_08136484: .4byte 0x0000ffd8
_08136488:
	bl sub_80F170C
	lsls r0, 24
	cmp r0, 0
	bne _0813657A
	b _08136524
_08136494:
	bl sub_80F33A8
	b _08136524
_0813649A:
	ldr r1, _081364C4 @ =0x0600f800
	movs r2, 0
	str r2, [sp]
	ldr r0, _081364C8 @ =0x040000d4
	mov r4, sp
	str r4, [r0]
	str r1, [r0, 0x4]
	ldr r1, _081364CC @ =0x85000200
	str r1, [r0, 0x8]
	ldr r0, [r0, 0x8]
	ldr r0, _081364D0 @ =REG_BG1VOFS
	strh r2, [r0]
	subs r0, 0x2
	strh r2, [r0]
	ldr r1, _081364D4 @ =REG_BG1CNT
	movs r2, 0xF8
	lsls r2, 5
	adds r0, r2, 0
	strh r0, [r1]
	ldr r1, [r3]
	b _08136528
	.align 2, 0
_081364C4: .4byte 0x0600f800
_081364C8: .4byte 0x040000d4
_081364CC: .4byte 0x85000200
_081364D0: .4byte REG_BG1VOFS
_081364D4: .4byte REG_BG1CNT
_081364D8:
	ldr r0, _081364F0 @ =gUnknown_083DFEC4
	ldr r1, [r0]
	ldr r4, _081364F4 @ =0x00008ff0
	adds r0, r1, r4
	ldr r2, _081364F8 @ =0x00009004
	adds r1, r2
	bl sub_80F567C
	bl sub_80F5B38
	b _08136524
	.align 2, 0
_081364F0: .4byte gUnknown_083DFEC4
_081364F4: .4byte 0x00008ff0
_081364F8: .4byte 0x00009004
_081364FC:
	bl sub_80F5B50
	lsls r0, 24
	cmp r0, 0
	bne _0813657A
	b _08136524
_08136508:
	ldr r0, _08136518 @ =gUnknown_083DFEC4
	ldr r0, [r0]
	ldr r4, _0813651C @ =0x00009004
	adds r0, r4
	bl sub_80F556C
	b _08136524
	.align 2, 0
_08136518: .4byte gUnknown_083DFEC4
_0813651C: .4byte 0x00009004
_08136520:
	bl sub_80F1934
_08136524:
	ldr r0, _08136534 @ =gUnknown_02039304
	ldr r1, [r0]
_08136528:
	adds r1, 0x50
	ldrb r0, [r1]
	adds r0, 0x1
	strb r0, [r1]
	b _0813657A
	.align 2, 0
_08136534: .4byte gUnknown_02039304
_08136538:
	ldr r1, _08136584 @ =REG_WIN0H
	movs r0, 0xF0
	strh r0, [r1]
	adds r1, 0x2
	movs r0, 0x9B
	strh r0, [r1]
	ldr r0, _08136588 @ =REG_WIN0V
	ldr r2, _0813658C @ =0x00003273
	adds r1, r2, 0
	strh r1, [r0]
	adds r0, 0x2
	strh r1, [r0]
	ldr r1, _08136590 @ =REG_WININ
	ldr r4, _08136594 @ =0x00003f3f
	adds r0, r4, 0
	strh r0, [r1]
	adds r1, 0x2
	movs r0, 0x1B
	strh r0, [r1]
	subs r1, 0x38
	movs r0, 0x28
	strh r0, [r1]
	subs r1, 0x12
	ldr r2, _08136598 @ =0x00007f40
	adds r0, r2, 0
	strh r0, [r1]
_0813656C:
	ldr r0, [r3]
	adds r0, 0x55
	movs r1, 0x1
	strb r1, [r0]
	ldr r0, _0813659C @ =sub_81365C8
	bl launch_c3_walk_stairs_and_run_once
_0813657A:
	add sp, 0x4
	pop {r4,r5}
	pop {r0}
	bx r0
	.align 2, 0
_08136584: .4byte REG_WIN0H
_08136588: .4byte REG_WIN0V
_0813658C: .4byte 0x00003273
_08136590: .4byte REG_WININ
_08136594: .4byte 0x00003f3f
_08136598: .4byte 0x00007f40
_0813659C: .4byte sub_81365C8
	thumb_func_end sub_8136294

	thumb_func_start sub_81365A0
sub_81365A0: @ 81365A0
	push {r4,lr}
	ldr r1, _081365C4 @ =gUnknown_02039304
	ldr r0, [r1]
	adds r0, 0x55
	ldrb r0, [r0]
	cmp r0, 0
	bne _081365BE
	adds r4, r1, 0
_081365B0:
	bl sub_8136294
	ldr r0, [r4]
	adds r0, 0x55
	ldrb r0, [r0]
	cmp r0, 0
	beq _081365B0
_081365BE:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_081365C4: .4byte gUnknown_02039304
	thumb_func_end sub_81365A0

	thumb_func_start sub_81365C8
sub_81365C8: @ 81365C8
	push {r4,lr}
	sub sp, 0x4
	ldr r4, _081365E0 @ =gUnknown_02039304
	ldr r0, [r4]
	adds r0, 0x50
	ldrb r1, [r0]
	cmp r1, 0
	beq _081365E4
	cmp r1, 0x1
	beq _0813660C
	b _08136626
	.align 2, 0
_081365E0: .4byte gUnknown_02039304
_081365E4:
	movs r0, 0x1
	negs r0, r0
	str r1, [sp]
	movs r1, 0
	movs r2, 0x10
	movs r3, 0
	bl BeginNormalPaletteFade
	ldr r0, _08136608 @ =sub_8136264
	bl SetVBlankCallback
	ldr r1, [r4]
	adds r1, 0x50
	ldrb r0, [r1]
	adds r0, 0x1
	strb r0, [r1]
	b _08136626
	.align 2, 0
_08136608: .4byte sub_8136264
_0813660C:
	ldr r0, _08136630 @ =gPaletteFade
	ldrb r1, [r0, 0x7]
	movs r0, 0x80
	ands r0, r1
	cmp r0, 0
	bne _08136626
	bl sub_80F3C94
	bl sub_80F3D00
	ldr r0, _08136634 @ =sub_8136638
	bl launch_c3_walk_stairs_and_run_once
_08136626:
	add sp, 0x4
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08136630: .4byte gPaletteFade
_08136634: .4byte sub_8136638
	thumb_func_end sub_81365C8

	thumb_func_start sub_8136638
sub_8136638: @ 8136638
	push {r4,lr}
	ldr r0, _08136654 @ =gUnknown_02039304
	ldr r0, [r0]
	adds r0, 0x50
	ldrb r0, [r0]
	cmp r0, 0x7
	bls _08136648
	b _081367FA
_08136648:
	lsls r0, 2
	ldr r1, _08136658 @ =_0813665C
	adds r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08136654: .4byte gUnknown_02039304
_08136658: .4byte _0813665C
	.align 2, 0
_0813665C:
	.4byte _0813667C
	.4byte _08136748
	.4byte _08136754
	.4byte _08136774
	.4byte _081367FA
	.4byte _08136780
	.4byte _08136798
	.4byte _081367E0
_0813667C:
	ldr r2, _081366A4 @ =gMain
	ldrh r1, [r2, 0x2C]
	movs r0, 0x40
	ands r0, r1
	cmp r0, 0
	beq _081366AC
	movs r0, 0x5
	bl PlaySE
	movs r0, 0x1
	bl sub_80F5060
	bl move_anim_execute
	ldr r0, _081366A8 @ =gUnknown_02039304
	ldr r0, [r0]
	adds r0, 0x50
	movs r1, 0x1
	b _081367F8
	.align 2, 0
_081366A4: .4byte gMain
_081366A8: .4byte gUnknown_02039304
_081366AC:
	movs r0, 0x80
	ands r0, r1
	cmp r0, 0
	beq _081366D4
	movs r0, 0x5
	bl PlaySE
	movs r0, 0
	bl sub_80F5060
	bl move_anim_execute
	ldr r0, _081366D0 @ =gUnknown_02039304
	ldr r0, [r0]
	adds r0, 0x50
	movs r1, 0x1
	b _081367F8
	.align 2, 0
_081366D0: .4byte gUnknown_02039304
_081366D4:
	ldrh r1, [r2, 0x2E]
	movs r0, 0x2
	ands r0, r1
	cmp r0, 0
	beq _081366F4
	movs r0, 0x5
	bl PlaySE
	ldr r0, _081366F0 @ =gUnknown_02039304
	ldr r0, [r0]
	adds r0, 0x50
	movs r1, 0x3
	b _081367F8
	.align 2, 0
_081366F0: .4byte gUnknown_02039304
_081366F4:
	movs r0, 0x1
	ands r0, r1
	cmp r0, 0
	bne _081366FE
	b _081367FA
_081366FE:
	movs r0, 0x5
	bl PlaySE
	ldr r0, _08136728 @ =gUnknown_083DFEC4
	ldr r0, [r0]
	ldr r2, _0813672C @ =0x000087dc
	adds r1, r0, r2
	movs r2, 0
	ldrsh r1, [r1, r2]
	ldr r2, _08136730 @ =0x000087da
	adds r0, r2
	movs r2, 0
	ldrsh r0, [r0, r2]
	subs r0, 0x1
	cmp r1, r0
	bne _08136738
	ldr r0, _08136734 @ =gUnknown_02039304
	ldr r0, [r0]
	adds r0, 0x50
	movs r1, 0x3
	b _081367F8
	.align 2, 0
_08136728: .4byte gUnknown_083DFEC4
_0813672C: .4byte 0x000087dc
_08136730: .4byte 0x000087da
_08136734: .4byte gUnknown_02039304
_08136738:
	ldr r0, _08136744 @ =gUnknown_02039304
	ldr r0, [r0]
	adds r0, 0x50
	movs r1, 0x5
	b _081367F8
	.align 2, 0
_08136744: .4byte gUnknown_02039304
_08136748:
	bl gpu_sync_bg_show
	lsls r0, 24
	cmp r0, 0
	bne _081367FA
	b _08136784
_08136754:
	bl sub_8055870
	adds r4, r0, 0
	cmp r4, 0
	bne _081367FA
	bl sub_80F1934
	bl sub_80F3D00
	ldr r0, _08136770 @ =gUnknown_02039304
	ldr r0, [r0]
	adds r0, 0x50
	strb r4, [r0]
	b _081367FA
	.align 2, 0
_08136770: .4byte gUnknown_02039304
_08136774:
	ldr r0, _0813677C @ =sub_8136B44
	bl launch_c3_walk_stairs_and_run_once
	b _081367FA
	.align 2, 0
_0813677C: .4byte sub_8136B44
_08136780:
	bl sub_8136BB8
_08136784:
	ldr r0, _08136794 @ =gUnknown_02039304
	ldr r1, [r0]
	adds r1, 0x50
	ldrb r0, [r1]
	adds r0, 0x1
	strb r0, [r1]
	b _081367FA
	.align 2, 0
_08136794: .4byte gUnknown_02039304
_08136798:
	bl sub_8136C40
	lsls r0, 24
	asrs r1, r0, 24
	cmp r1, 0
	beq _081367B8
	cmp r1, 0
	bgt _081367B2
	movs r0, 0x1
	negs r0, r0
	cmp r1, r0
	beq _081367F0
	b _081367FA
_081367B2:
	cmp r1, 0x1
	bne _081367FA
	b _081367F0
_081367B8:
	bl sub_8137058
	lsls r0, 24
	cmp r0, 0
	beq _081367D4
	bl sub_8136D60
	ldr r0, _081367D0 @ =gUnknown_02039304
	ldr r0, [r0]
	adds r0, 0x50
	movs r1, 0x7
	b _081367F8
	.align 2, 0
_081367D0: .4byte gUnknown_02039304
_081367D4:
	ldr r0, _081367DC @ =sub_8136808
	bl launch_c3_walk_stairs_and_run_once
	b _081367FA
	.align 2, 0
_081367DC: .4byte sub_8136808
_081367E0:
	ldr r0, _08136800 @ =gMain
	ldrh r1, [r0, 0x2E]
	movs r0, 0x3
	ands r0, r1
	cmp r0, 0
	beq _081367FA
	bl sub_8136D8C
_081367F0:
	ldr r0, _08136804 @ =gUnknown_02039304
	ldr r0, [r0]
	adds r0, 0x50
	movs r1, 0
_081367F8:
	strb r1, [r0]
_081367FA:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08136800: .4byte gMain
_08136804: .4byte gUnknown_02039304
	thumb_func_end sub_8136638

	thumb_func_start sub_8136808
sub_8136808: @ 8136808
	push {r4,r5,lr}
	sub sp, 0x4
	ldr r5, _08136820 @ =gUnknown_02039304
	ldr r0, [r5]
	adds r0, 0x50
	ldrb r4, [r0]
	cmp r4, 0
	beq _08136824
	cmp r4, 0x1
	beq _08136874
	b _0813688C
	.align 2, 0
_08136820: .4byte gUnknown_02039304
_08136824:
	ldr r0, _08136860 @ =gUnknown_083DFEC4
	ldr r0, [r0]
	ldr r1, _08136864 @ =0x000087dc
	adds r0, r1
	ldrb r0, [r0]
	bl sub_81370A4
	ldr r1, _08136868 @ =gUnknown_02039310
	strb r0, [r1]
	ldr r1, _0813686C @ =gUnknown_02039308
	ldr r2, [r5]
	ldr r0, [r2, 0x4]
	str r0, [r1]
	ldr r1, _08136870 @ =gUnknown_0203930C
	ldr r0, [r2, 0x8]
	str r0, [r1]
	movs r0, 0x1
	negs r0, r0
	str r4, [sp]
	movs r1, 0
	movs r2, 0
	movs r3, 0x10
	bl BeginNormalPaletteFade
	ldr r1, [r5]
	adds r1, 0x50
	ldrb r0, [r1]
	adds r0, 0x1
	strb r0, [r1]
	b _0813688C
	.align 2, 0
_08136860: .4byte gUnknown_083DFEC4
_08136864: .4byte 0x000087dc
_08136868: .4byte gUnknown_02039310
_0813686C: .4byte gUnknown_02039308
_08136870: .4byte gUnknown_0203930C
_08136874:
	ldr r0, _08136894 @ =gPaletteFade
	ldrb r1, [r0, 0x7]
	movs r0, 0x80
	ands r0, r1
	cmp r0, 0
	bne _0813688C
	ldr r0, _08136898 @ =gMain
	ldr r1, _0813689C @ =sub_8136174
	str r1, [r0, 0x8]
	ldr r0, _081368A0 @ =sub_8147ADC
	bl SetMainCallback2
_0813688C:
	add sp, 0x4
	pop {r4,r5}
	pop {r0}
	bx r0
	.align 2, 0
_08136894: .4byte gPaletteFade
_08136898: .4byte gMain
_0813689C: .4byte sub_8136174
_081368A0: .4byte sub_8147ADC
	thumb_func_end sub_8136808

	thumb_func_start sub_81368A4
sub_81368A4: @ 81368A4
	push {r4,lr}
	sub sp, 0x4
	ldr r1, _081368C4 @ =gUnknown_02039304
	ldr r0, [r1]
	adds r0, 0x50
	ldrb r0, [r0]
	adds r4, r1, 0
	cmp r0, 0x6
	bls _081368B8
	b _081369B8
_081368B8:
	lsls r0, 2
	ldr r1, _081368C8 @ =_081368CC
	adds r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_081368C4: .4byte gUnknown_02039304
_081368C8: .4byte _081368CC
	.align 2, 0
_081368CC:
	.4byte _081368E8
	.4byte _08136922
	.4byte _0813692E
	.4byte _0813694C
	.4byte _0813695A
	.4byte _0813696C
	.4byte _08136998
_081368E8:
	ldr r0, _0813690C @ =gUnknown_083DFEC4
	ldr r0, [r0]
	ldr r1, _08136910 @ =0x000087dc
	adds r0, r1
	movs r2, 0
	ldrsh r1, [r0, r2]
	ldr r0, _08136914 @ =gUnknown_02039310
	ldrb r0, [r0]
	cmp r1, r0
	beq _08136918
	ldr r0, [r4]
	adds r0, 0x56
	ldrb r0, [r0]
	bl sub_80F5060
	ldr r1, [r4]
	b _08136986
	.align 2, 0
_0813690C: .4byte gUnknown_083DFEC4
_08136910: .4byte 0x000087dc
_08136914: .4byte gUnknown_02039310
_08136918:
	ldr r0, [r4]
	adds r0, 0x50
	movs r1, 0x3
	strb r1, [r0]
	b _081369B8
_08136922:
	bl gpu_sync_bg_show
	lsls r0, 24
	cmp r0, 0
	bne _081369B8
	b _08136982
_0813692E:
	bl sub_8055870
	adds r4, r0, 0
	cmp r4, 0
	bne _081369B8
	bl sub_80F1934
	ldr r0, _08136948 @ =gUnknown_02039304
	ldr r0, [r0]
	adds r0, 0x50
	strb r4, [r0]
	b _081369B8
	.align 2, 0
_08136948: .4byte gUnknown_02039304
_0813694C:
	movs r0, 0x1
	negs r0, r0
	movs r1, 0x10
	movs r2, 0
	bl BlendPalettes
	b _08136982
_0813695A:
	movs r1, 0x80
	lsls r1, 19
	ldr r2, _08136968 @ =0x00007f40
	adds r0, r2, 0
	strh r0, [r1]
	ldr r1, [r4]
	b _08136986
	.align 2, 0
_08136968: .4byte 0x00007f40
_0813696C:
	ldr r0, _08136990 @ =sub_8136264
	bl SetVBlankCallback
	movs r0, 0x1
	negs r0, r0
	movs r1, 0
	str r1, [sp]
	movs r2, 0x10
	movs r3, 0
	bl BeginNormalPaletteFade
_08136982:
	ldr r0, _08136994 @ =gUnknown_02039304
	ldr r1, [r0]
_08136986:
	adds r1, 0x50
	ldrb r0, [r1]
	adds r0, 0x1
	strb r0, [r1]
	b _081369B8
	.align 2, 0
_08136990: .4byte sub_8136264
_08136994: .4byte gUnknown_02039304
_08136998:
	ldr r0, _081369C0 @ =gPaletteFade
	ldrb r1, [r0, 0x7]
	movs r0, 0x80
	ands r0, r1
	cmp r0, 0
	bne _081369B8
	bl sub_80F3C94
	bl sub_80F3D00
	ldr r0, _081369C4 @ =sub_81369CC
	bl launch_c3_walk_stairs_and_run_once
	ldr r0, _081369C8 @ =sub_8136244
	bl SetMainCallback2
_081369B8:
	add sp, 0x4
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_081369C0: .4byte gPaletteFade
_081369C4: .4byte sub_81369CC
_081369C8: .4byte sub_8136244
	thumb_func_end sub_81368A4

	thumb_func_start sub_81369CC
sub_81369CC: @ 81369CC
	push {r4-r6,lr}
	ldr r1, _081369E8 @ =gUnknown_02039304
	ldr r0, [r1]
	adds r0, 0x50
	ldrb r0, [r0]
	adds r4, r1, 0
	cmp r0, 0x5
	bls _081369DE
	b _08136B32
_081369DE:
	lsls r0, 2
	ldr r1, _081369EC @ =_081369F0
	adds r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_081369E8: .4byte gUnknown_02039304
_081369EC: .4byte _081369F0
	.align 2, 0
_081369F0:
	.4byte _08136A08
	.4byte _08136A48
	.4byte _08136A5C
	.4byte _08136AAC
	.4byte _08136AEC
	.4byte _08136B0E
_08136A08:
	ldr r3, [r4]
	ldr r2, _08136A38 @ =gPlayerParty
	str r2, [r3, 0xC]
	ldr r0, _08136A3C @ =gUnknown_083DFEC4
	ldr r1, [r0]
	ldr r5, _08136A40 @ =0x000087dc
	adds r0, r1, r5
	movs r5, 0
	ldrsh r0, [r0, r5]
	lsls r0, 2
	adds r1, r0
	ldr r0, _08136A44 @ =0x0000893e
	adds r1, r0
	ldrb r0, [r1]
	lsls r0, 27
	lsrs r0, 27
	movs r1, 0x64
	muls r0, r1
	adds r0, r2
	str r0, [r3, 0xC]
	bl move_anim_execute
	b _08136B02
	.align 2, 0
_08136A38: .4byte gPlayerParty
_08136A3C: .4byte gUnknown_083DFEC4
_08136A40: .4byte 0x000087dc
_08136A44: .4byte 0x0000893e
_08136A48:
	ldr r0, _08136A58 @ =gMain
	ldrh r1, [r0, 0x2E]
	movs r0, 0x3
	ands r0, r1
	cmp r0, 0
	beq _08136B32
	b _08136B02
	.align 2, 0
_08136A58: .4byte gMain
_08136A5C:
	bl sub_8136EF0
	ldr r6, _08136A98 @ =gUnknown_02039304
	ldr r0, [r6]
	adds r0, 0x5C
	ldr r1, _08136A9C @ =gUnknown_083DFEC4
	ldr r4, [r1]
	ldr r1, _08136AA0 @ =0x00009040
	adds r5, r4, r1
	adds r1, r5, 0
	bl sub_80F567C
	ldr r1, _08136AA4 @ =0x00008fe9
	adds r0, r4, r1
	movs r1, 0
	ldrsb r1, [r0, r1]
	lsls r0, r1, 2
	adds r0, r1
	lsls r0, 2
	ldr r1, _08136AA8 @ =0x00009004
	adds r0, r1
	adds r4, r0
	adds r0, r4, 0
	adds r1, r5, 0
	bl sub_80F5550
	bl sub_8137138
	ldr r1, [r6]
	b _08136B04
	.align 2, 0
_08136A98: .4byte gUnknown_02039304
_08136A9C: .4byte gUnknown_083DFEC4
_08136AA0: .4byte 0x00009040
_08136AA4: .4byte 0x00008fe9
_08136AA8: .4byte 0x00009004
_08136AAC:
	bl sub_80F555C
	lsls r0, 24
	lsrs r4, r0, 24
	cmp r4, 0
	bne _08136B32
	ldr r0, _08136AE0 @ =gUnknown_083DFEC4
	ldr r0, [r0]
	ldr r5, _08136AE4 @ =0x000087dc
	adds r0, r5
	ldrb r0, [r0]
	bl sub_81370A4
	lsls r0, 24
	lsrs r0, 24
	bl sub_80F7224
	bl sub_80F3D00
	ldr r1, _08136AE8 @ =gUnknown_02039304
	ldr r0, [r1]
	adds r0, 0x52
	strb r4, [r0]
	ldr r1, [r1]
	b _08136B04
	.align 2, 0
_08136AE0: .4byte gUnknown_083DFEC4
_08136AE4: .4byte 0x000087dc
_08136AE8: .4byte gUnknown_02039304
_08136AEC:
	ldr r1, [r4]
	adds r1, 0x52
	ldrb r0, [r1]
	adds r0, 0x1
	strb r0, [r1]
	lsls r0, 24
	lsrs r0, 24
	cmp r0, 0x10
	bls _08136B32
	bl sub_8136C6C
_08136B02:
	ldr r1, [r4]
_08136B04:
	adds r1, 0x50
	ldrb r0, [r1]
	adds r0, 0x1
	strb r0, [r1]
	b _08136B32
_08136B0E:
	ldr r0, _08136B38 @ =gMain
	ldrh r1, [r0, 0x2E]
	movs r0, 0x3
	ands r0, r1
	cmp r0, 0
	beq _08136B32
	bl sub_8136D00
	lsls r0, 24
	cmp r0, 0
	bne _08136B32
	ldr r0, _08136B3C @ =gScriptItemId
	ldrb r0, [r0]
	bl sub_810CA6C
	ldr r0, _08136B40 @ =sub_8136B44
	bl launch_c3_walk_stairs_and_run_once
_08136B32:
	pop {r4-r6}
	pop {r0}
	bx r0
	.align 2, 0
_08136B38: .4byte gMain
_08136B3C: .4byte gScriptItemId
_08136B40: .4byte sub_8136B44
	thumb_func_end sub_81369CC

	thumb_func_start sub_8136B44
sub_8136B44: @ 8136B44
	push {r4,lr}
	sub sp, 0x4
	ldr r4, _08136B60 @ =gUnknown_02039304
	ldr r0, [r4]
	adds r2, r0, 0
	adds r2, 0x50
	ldrb r1, [r2]
	cmp r1, 0x1
	beq _08136B80
	cmp r1, 0x1
	bgt _08136B64
	cmp r1, 0
	beq _08136B6E
	b _08136BAE
	.align 2, 0
_08136B60: .4byte gUnknown_02039304
_08136B64:
	cmp r1, 0x2
	beq _08136B98
	cmp r1, 0x3
	beq _08136BA8
	b _08136BAE
_08136B6E:
	movs r0, 0x1
	negs r0, r0
	str r1, [sp]
	movs r1, 0
	movs r2, 0
	movs r3, 0x10
	bl BeginNormalPaletteFade
	b _08136B9C
_08136B80:
	ldr r0, _08136B94 @ =gPaletteFade
	ldrb r1, [r0, 0x7]
	movs r0, 0x80
	ands r0, r1
	cmp r0, 0
	bne _08136BAE
	movs r0, 0x2
	strb r0, [r2]
	b _08136BAE
	.align 2, 0
_08136B94: .4byte gPaletteFade
_08136B98:
	bl sub_80F5BDC
_08136B9C:
	ldr r1, [r4]
	adds r1, 0x50
	ldrb r0, [r1]
	adds r0, 0x1
	strb r0, [r1]
	b _08136BAE
_08136BA8:
	ldr r0, [r0, 0x4]
	bl SetMainCallback2
_08136BAE:
	add sp, 0x4
	pop {r4}
	pop {r0}
	bx r0
	thumb_func_end sub_8136B44

	thumb_func_start sub_8136BB8
sub_8136BB8: @ 8136BB8
	push {r4,lr}
	ldr r0, _08136C28 @ =gUnknown_083DFEC4
	ldr r0, [r0]
	ldr r1, _08136C2C @ =0x000087dc
	adds r0, r1
	ldrb r0, [r0]
	bl sub_81370A4
	lsls r0, 24
	lsrs r0, 24
	movs r1, 0x64
	muls r0, r1
	ldr r1, _08136C30 @ =gPlayerParty
	adds r0, r1
	ldr r4, _08136C34 @ =gUnknown_02039304
	ldr r2, [r4]
	adds r2, 0x10
	movs r1, 0x2
	bl GetMonData
	ldr r0, [r4]
	adds r0, 0x10
	bl StringGetEnd10
	ldr r0, [r4]
	adds r0, 0x10
	ldr r1, _08136C38 @ =gOtherText_GetsAPokeBlock
	bl StringAppend
	ldr r0, _08136C3C @ =gWindowConfig_81E709C
	bl BasicInitMenuWindow
	movs r0, 0
	movs r1, 0x10
	movs r2, 0x1D
	movs r3, 0x13
	bl MenuDrawTextWindow
	ldr r0, [r4]
	adds r0, 0x10
	movs r1, 0x1
	movs r2, 0x11
	bl MenuPrint
	movs r0, 0x17
	movs r1, 0xA
	movs r2, 0x1
	bl DisplayYesNoMenu
	movs r0, 0
	bl MoveMenuCursor
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08136C28: .4byte gUnknown_083DFEC4
_08136C2C: .4byte 0x000087dc
_08136C30: .4byte gPlayerParty
_08136C34: .4byte gUnknown_02039304
_08136C38: .4byte gOtherText_GetsAPokeBlock
_08136C3C: .4byte gWindowConfig_81E709C
	thumb_func_end sub_8136BB8

	thumb_func_start sub_8136C40
sub_8136C40: @ 8136C40
	push {r4,lr}
	bl ProcessMenuInputNoWrap
	lsls r0, 24
	asrs r4, r0, 24
	movs r1, 0x80
	lsls r1, 17
	adds r0, r1
	lsrs r0, 24
	cmp r0, 0x2
	bhi _08136C60
	bl MenuZeroFillScreen
	ldr r0, _08136C68 @ =gWindowConfig_81E7080
	bl BasicInitMenuWindow
_08136C60:
	adds r0, r4, 0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08136C68: .4byte gWindowConfig_81E7080
	thumb_func_end sub_8136C40

	thumb_func_start sub_8136C6C
sub_8136C6C: @ 8136C6C
	push {lr}
	ldr r0, _08136C8C @ =gWindowConfig_81E709C
	bl BasicInitMenuWindow
	movs r0, 0
	movs r1, 0x10
	movs r2, 0x1D
	movs r3, 0x13
	bl MenuDrawTextWindow
	ldr r3, _08136C90 @ =gUnknown_02039304
	ldr r0, [r3]
	adds r0, 0x53
	movs r1, 0
	b _08136C9C
	.align 2, 0
_08136C8C: .4byte gWindowConfig_81E709C
_08136C90: .4byte gUnknown_02039304
_08136C94:
	ldr r0, [r3]
	adds r0, 0x53
	ldrb r1, [r0]
	adds r1, 0x1
_08136C9C:
	strb r1, [r0]
	ldr r2, [r3]
	adds r1, r2, 0
	adds r1, 0x53
	ldrb r0, [r1]
	cmp r0, 0x4
	bhi _08136CDC
	adds r0, r2, 0
	adds r0, 0x61
	ldrb r1, [r1]
	adds r0, r1
	ldrb r0, [r0]
	cmp r0, 0
	beq _08136C94
	ldr r0, _08136CD8 @ =gUnknown_02039304
	ldr r2, [r0]
	adds r1, r2, 0
	adds r1, 0x53
	ldrb r0, [r1]
	cmp r0, 0x4
	bhi _08136CDC
	adds r0, r2, 0
	adds r0, 0x10
	ldrb r1, [r1]
	adds r2, 0x61
	adds r2, r1
	ldrb r2, [r2]
	bl sub_8136DC0
	b _08136CEE
	.align 2, 0
_08136CD8: .4byte gUnknown_02039304
_08136CDC:
	ldr r0, _08136CFC @ =gUnknown_02039304
	ldr r1, [r0]
	adds r0, r1, 0
	adds r0, 0x10
	adds r1, 0x53
	ldrb r1, [r1]
	movs r2, 0
	bl sub_8136DC0
_08136CEE:
	ldr r0, _08136CFC @ =gUnknown_02039304
	ldr r0, [r0]
	adds r0, 0x10
	bl sub_8136DA0
	pop {r0}
	bx r0
	.align 2, 0
_08136CFC: .4byte gUnknown_02039304
	thumb_func_end sub_8136C6C

	thumb_func_start sub_8136D00
sub_8136D00: @ 8136D00
	push {r4,r5,lr}
	ldr r4, _08136D30 @ =gUnknown_02039304
	adds r3, r4, 0
	movs r5, 0x5
_08136D08:
	ldr r0, [r3]
	adds r0, 0x53
	ldrb r1, [r0]
	adds r1, 0x1
	strb r1, [r0]
	ldr r2, [r3]
	adds r1, r2, 0
	adds r1, 0x53
	ldrb r0, [r1]
	cmp r0, 0x4
	bhi _08136D34
	adds r0, r2, 0
	adds r0, 0x61
	ldrb r1, [r1]
	adds r0, r1
	ldrb r0, [r0]
	cmp r0, 0
	bne _08136D3A
	b _08136D08
	.align 2, 0
_08136D30: .4byte gUnknown_02039304
_08136D34:
	strb r5, [r1]
	movs r0, 0
	b _08136D5A
_08136D3A:
	ldr r2, [r4]
	adds r0, r2, 0
	adds r0, 0x10
	adds r1, r2, 0
	adds r1, 0x53
	ldrb r1, [r1]
	adds r2, 0x61
	adds r2, r1
	ldrb r2, [r2]
	bl sub_8136DC0
	ldr r0, [r4]
	adds r0, 0x10
	bl sub_8136DA0
	movs r0, 0x1
_08136D5A:
	pop {r4,r5}
	pop {r1}
	bx r1
	thumb_func_end sub_8136D00

	thumb_func_start sub_8136D60
sub_8136D60: @ 8136D60
	push {lr}
	ldr r0, _08136D84 @ =gWindowConfig_81E709C
	bl BasicInitMenuWindow
	movs r0, 0
	movs r1, 0x10
	movs r2, 0x1D
	movs r3, 0x13
	bl MenuDrawTextWindow
	ldr r0, _08136D88 @ =gOtherText_WontEat
	movs r1, 0x1
	movs r2, 0x11
	bl MenuPrint
	pop {r0}
	bx r0
	.align 2, 0
_08136D84: .4byte gWindowConfig_81E709C
_08136D88: .4byte gOtherText_WontEat
	thumb_func_end sub_8136D60

	thumb_func_start sub_8136D8C
sub_8136D8C: @ 8136D8C
	push {lr}
	bl MenuZeroFillScreen
	ldr r0, _08136D9C @ =gWindowConfig_81E7080
	bl BasicInitMenuWindow
	pop {r0}
	bx r0
	.align 2, 0
_08136D9C: .4byte gWindowConfig_81E7080
	thumb_func_end sub_8136D8C

	thumb_func_start sub_8136DA0
sub_8136DA0: @ 8136DA0
	push {r4,lr}
	adds r4, r0, 0
	movs r0, 0
	movs r1, 0x10
	movs r2, 0x1D
	movs r3, 0x13
	bl MenuDrawTextWindow
	adds r0, r4, 0
	movs r1, 0x1
	movs r2, 0x11
	bl MenuPrint
	pop {r4}
	pop {r0}
	bx r0
	thumb_func_end sub_8136DA0

	thumb_func_start sub_8136DC0
sub_8136DC0: @ 8136DC0
	push {r4,lr}
	adds r4, r0, 0
	lsls r1, 24
	lsrs r3, r1, 24
	lsls r2, 16
	lsrs r0, r2, 16
	asrs r2, 16
	cmp r2, 0
	beq _08136DFC
	cmp r2, 0
	ble _08136DD8
	movs r0, 0
_08136DD8:
	lsls r0, 16
	ldr r1, _08136DF4 @ =gUnknown_08406134
	lsls r0, r3, 2
	adds r0, r1
	ldr r1, [r0]
	adds r0, r4, 0
	bl StringCopy
	ldr r1, _08136DF8 @ =gOtherText_WasEnhanced
	adds r0, r4, 0
	bl StringAppend
	b _08136E04
	.align 2, 0
_08136DF4: .4byte gUnknown_08406134
_08136DF8: .4byte gOtherText_WasEnhanced
_08136DFC:
	ldr r1, _08136E0C @ =gOtherText_NothingChanged
	adds r0, r4, 0
	bl StringCopy
_08136E04:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08136E0C: .4byte gOtherText_NothingChanged
	thumb_func_end sub_8136DC0

	thumb_func_start sub_8136E10
sub_8136E10: @ 8136E10
	push {r4-r7,lr}
	adds r6, r0, 0
	adds r5, r1, 0
	movs r4, 0
	ldr r7, _08136E3C @ =gUnknown_08406118
_08136E1A:
	lsls r0, r4, 2
	adds r0, r7
	ldr r1, [r0]
	adds r0, r6, 0
	bl GetMonData
	adds r1, r5, r4
	strb r0, [r1]
	adds r0, r4, 0x1
	lsls r0, 16
	lsrs r4, r0, 16
	cmp r4, 0x4
	bls _08136E1A
	pop {r4-r7}
	pop {r0}
	bx r0
	.align 2, 0
_08136E3C: .4byte gUnknown_08406118
	thumb_func_end sub_8136E10

	thumb_func_start sub_8136E40
sub_8136E40: @ 8136E40
	push {r4-r7,lr}
	mov r7, r8
	push {r7}
	sub sp, 0x4
	mov r8, r0
	adds r5, r1, 0
	adds r0, r5, 0
	movs r1, 0x30
	bl GetMonData
	cmp r0, 0xFF
	beq _08136EDC
	mov r0, r8
	adds r1, r5, 0
	bl sub_8136F74
	movs r4, 0
	mov r7, sp
_08136E64:
	ldr r1, _08136EE8 @ =gUnknown_08406118
	lsls r0, r4, 2
	adds r0, r1
	ldr r6, [r0]
	adds r0, r5, 0
	adds r1, r6, 0
	bl GetMonData
	lsls r0, 24
	lsrs r0, 24
	strb r0, [r7]
	ldr r1, _08136EEC @ =gUnknown_02039304
	ldr r1, [r1]
	lsls r2, r4, 1
	adds r1, 0x66
	adds r1, r2
	ldrh r1, [r1]
	adds r0, r1
	lsls r0, 16
	lsrs r1, r0, 16
	cmp r0, 0
	bge _08136E92
	movs r1, 0
_08136E92:
	lsls r0, r1, 16
	asrs r0, 16
	cmp r0, 0xFF
	ble _08136E9C
	movs r1, 0xFF
_08136E9C:
	strb r1, [r7]
	adds r0, r5, 0
	adds r1, r6, 0
	mov r2, sp
	bl SetMonData
	adds r0, r4, 0x1
	lsls r0, 16
	lsrs r4, r0, 16
	cmp r4, 0x4
	bls _08136E64
	adds r0, r5, 0
	movs r1, 0x30
	bl GetMonData
	lsls r0, 24
	lsrs r0, 24
	mov r2, r8
	ldrb r2, [r2, 0x6]
	adds r1, r0, r2
	lsls r0, r1, 16
	asrs r0, 16
	cmp r0, 0xFF
	ble _08136ECE
	movs r1, 0xFF
_08136ECE:
	mov r0, sp
	strb r1, [r0]
	adds r0, r5, 0
	movs r1, 0x30
	mov r2, sp
	bl SetMonData
_08136EDC:
	add sp, 0x4
	pop {r3}
	mov r8, r3
	pop {r4-r7}
	pop {r0}
	bx r0
	.align 2, 0
_08136EE8: .4byte gUnknown_08406118
_08136EEC: .4byte gUnknown_02039304
	thumb_func_end sub_8136E40

	thumb_func_start sub_8136EF0
sub_8136EF0: @ 8136EF0
	push {r4,r5,lr}
	ldr r5, _08136F60 @ =gPlayerParty
	ldr r0, _08136F64 @ =gUnknown_083DFEC4
	ldr r1, [r0]
	ldr r2, _08136F68 @ =0x000087dc
	adds r0, r1, r2
	movs r2, 0
	ldrsh r0, [r0, r2]
	lsls r0, 2
	adds r1, r0
	ldr r0, _08136F6C @ =0x0000893e
	adds r1, r0
	ldrb r0, [r1]
	lsls r0, 27
	lsrs r0, 27
	movs r1, 0x64
	muls r0, r1
	adds r5, r0, r5
	ldr r4, _08136F70 @ =gUnknown_02039304
	ldr r1, [r4]
	adds r1, 0x57
	adds r0, r5, 0
	bl sub_8136E10
	ldr r0, [r4]
	ldr r0, [r0, 0x8]
	adds r1, r5, 0
	bl sub_8136E40
	ldr r1, [r4]
	adds r1, 0x5C
	adds r0, r5, 0
	bl sub_8136E10
	movs r3, 0
_08136F36:
	ldr r0, [r4]
	adds r2, r0, 0
	adds r2, 0x61
	adds r2, r3
	adds r1, r0, 0
	adds r1, 0x5C
	adds r1, r3
	adds r0, 0x57
	adds r0, r3
	ldrb r1, [r1]
	ldrb r0, [r0]
	subs r1, r0
	strb r1, [r2]
	adds r0, r3, 0x1
	lsls r0, 16
	lsrs r3, r0, 16
	cmp r3, 0x4
	bls _08136F36
	pop {r4,r5}
	pop {r0}
	bx r0
	.align 2, 0
_08136F60: .4byte gPlayerParty
_08136F64: .4byte gUnknown_083DFEC4
_08136F68: .4byte 0x000087dc
_08136F6C: .4byte 0x0000893e
_08136F70: .4byte gUnknown_02039304
	thumb_func_end sub_8136EF0

	thumb_func_start sub_8136F74
sub_8136F74: @ 8136F74
	push {r4-r7,lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5-r7}
	mov r10, r1
	ldr r1, _08136FBC @ =gUnknown_02039304
	ldr r3, [r1]
	ldrb r1, [r0, 0x1]
	adds r2, r3, 0
	adds r2, 0x66
	strh r1, [r2]
	ldrb r2, [r0, 0x5]
	adds r1, r3, 0
	adds r1, 0x68
	strh r2, [r1]
	ldrb r1, [r0, 0x4]
	adds r2, r3, 0
	adds r2, 0x6A
	strh r1, [r2]
	ldrb r2, [r0, 0x3]
	adds r1, r3, 0
	adds r1, 0x6C
	strh r2, [r1]
	ldrb r1, [r0, 0x2]
	adds r0, r3, 0
	adds r0, 0x6E
	strh r1, [r0]
	ldr r0, _08136FC0 @ =gUnknown_02039312
	movs r1, 0
	ldrsh r0, [r0, r1]
	cmp r0, 0
	ble _08136FC4
	movs r0, 0x1
	b _08136FCA
	.align 2, 0
_08136FBC: .4byte gUnknown_02039304
_08136FC0: .4byte gUnknown_02039312
_08136FC4:
	cmp r0, 0
	bge _08137040
	movs r0, 0xFF
_08136FCA:
	movs r2, 0
	lsls r0, 24
	mov r9, r0
	ldr r0, _08137050 @ =gUnknown_02039304
	mov r8, r0
_08136FD4:
	mov r0, r8
	ldr r1, [r0]
	lsls r0, r2, 24
	asrs r6, r0, 24
	lsls r7, r6, 1
	adds r1, 0x66
	adds r1, r7
	movs r2, 0
	ldrsh r4, [r1, r2]
	adds r0, r4, 0
	movs r1, 0xA
	bl __divsi3
	lsls r0, 24
	lsrs r5, r0, 24
	adds r0, r4, 0
	movs r1, 0xA
	bl __modsi3
	lsls r0, 16
	asrs r0, 16
	cmp r0, 0x4
	ble _0813700C
	lsls r0, r5, 24
	movs r1, 0x80
	lsls r1, 17
	adds r0, r1
	lsrs r5, r0, 24
_0813700C:
	ldr r0, _08137054 @ =gUnknown_0840612C
	adds r0, r6, r0
	ldrb r1, [r0]
	mov r0, r10
	bl sub_8040A54
	lsls r0, 24
	asrs r2, r0, 24
	cmp r0, r9
	bne _08137034
	mov r1, r8
	ldr r0, [r1]
	adds r0, 0x66
	adds r0, r7
	lsls r1, r5, 24
	asrs r1, 24
	muls r1, r2
	ldrh r2, [r0]
	adds r1, r2
	strh r1, [r0]
_08137034:
	adds r0, r6, 0x1
	lsls r0, 24
	lsrs r2, r0, 24
	asrs r0, 24
	cmp r0, 0x4
	ble _08136FD4
_08137040:
	pop {r3-r5}
	mov r8, r3
	mov r9, r4
	mov r10, r5
	pop {r4-r7}
	pop {r0}
	bx r0
	.align 2, 0
_08137050: .4byte gUnknown_02039304
_08137054: .4byte gUnknown_0840612C
	thumb_func_end sub_8136F74

	thumb_func_start sub_8137058
sub_8137058: @ 8137058
	push {lr}
	ldr r0, _0813708C @ =gPlayerParty
	ldr r1, _08137090 @ =gUnknown_083DFEC4
	ldr r2, [r1]
	ldr r3, _08137094 @ =0x000087dc
	adds r1, r2, r3
	movs r3, 0
	ldrsh r1, [r1, r3]
	lsls r1, 2
	adds r2, r1
	ldr r1, _08137098 @ =0x0000893e
	adds r2, r1
	ldrb r1, [r2]
	lsls r1, 27
	lsrs r1, 27
	movs r2, 0x64
	muls r1, r2
	adds r0, r1, r0
	movs r1, 0x30
	bl GetMonData
	cmp r0, 0xFF
	beq _0813709C
	movs r0, 0
	b _0813709E
	.align 2, 0
_0813708C: .4byte gPlayerParty
_08137090: .4byte gUnknown_083DFEC4
_08137094: .4byte 0x000087dc
_08137098: .4byte 0x0000893e
_0813709C:
	movs r0, 0x1
_0813709E:
	pop {r1}
	bx r1
	thumb_func_end sub_8137058

	thumb_func_start sub_81370A4
sub_81370A4: @ 81370A4
	push {r4,r5,lr}
	lsls r0, 24
	lsrs r5, r0, 24
	movs r4, 0
_081370AC:
	movs r0, 0x64
	muls r0, r4
	ldr r1, _081370C8 @ =gPlayerParty
	adds r0, r1
	movs r1, 0x2D
	bl GetMonData
	cmp r0, 0
	bne _081370D2
	cmp r5, 0
	bne _081370CC
	adds r0, r4, 0
	b _081370DE
	.align 2, 0
_081370C8: .4byte gPlayerParty
_081370CC:
	subs r0, r5, 0x1
	lsls r0, 24
	lsrs r5, r0, 24
_081370D2:
	adds r0, r4, 0x1
	lsls r0, 24
	lsrs r4, r0, 24
	cmp r4, 0x5
	bls _081370AC
	movs r0, 0
_081370DE:
	pop {r4,r5}
	pop {r1}
	bx r1
	thumb_func_end sub_81370A4

	thumb_func_start sub_81370E4
sub_81370E4: @ 81370E4
	push {r4-r6,lr}
	lsls r0, 24
	lsrs r5, r0, 24
	movs r4, 0
	movs r6, 0
	cmp r6, r5
	bcs _08137114
_081370F2:
	movs r0, 0x64
	muls r0, r4
	ldr r1, _08137120 @ =gPlayerParty
	adds r0, r1
	movs r1, 0x2D
	bl GetMonData
	cmp r0, 0
	beq _0813710A
	adds r0, r6, 0x1
	lsls r0, 24
	lsrs r6, r0, 24
_0813710A:
	adds r0, r4, 0x1
	lsls r0, 24
	lsrs r4, r0, 24
	cmp r4, r5
	bcc _081370F2
_08137114:
	subs r0, r5, r6
	lsls r0, 24
	lsrs r0, 24
	pop {r4-r6}
	pop {r1}
	bx r1
	.align 2, 0
_08137120: .4byte gPlayerParty
	thumb_func_end sub_81370E4

	thumb_func_start sub_8137124
sub_8137124: @ 8137124
	push {lr}
	lsls r0, 24
	lsrs r0, 24
	bl sub_81370A4
	lsls r0, 24
	lsrs r0, 24
	pop {r1}
	bx r1
	thumb_func_end sub_8137124

	thumb_func_start sub_8137138
sub_8137138: @ 8137138
	push {r4-r7,lr}
	mov r7, r8
	push {r7}
	ldr r0, _081371C0 @ =gSpriteSheet_ConditionUpDown
	bl LoadSpriteSheet
	ldr r0, _081371C4 @ =gSpritePalette_ConditionUpDown
	bl LoadSpritePalette
	ldr r2, _081371C8 @ =gUnknown_02039304
	ldr r0, [r2]
	adds r0, 0x54
	movs r1, 0
	strb r1, [r0]
	movs r4, 0
	adds r5, r2, 0
	ldr r6, _081371CC @ =gUnknown_08406158
	adds r0, r6, 0x2
	mov r8, r0
	ldr r7, _081371D0 @ =gSprites + 0x1C
_08137160:
	ldr r0, [r5]
	adds r0, 0x61
	adds r0, r4
	ldrb r0, [r0]
	cmp r0, 0
	beq _081371AC
	lsls r2, r4, 2
	adds r0, r2, r6
	movs r3, 0
	ldrsh r1, [r0, r3]
	add r2, r8
	movs r0, 0
	ldrsh r2, [r2, r0]
	ldr r0, _081371D4 @ =gSpriteTemplate_840618C
	movs r3, 0
	bl CreateSprite
	lsls r0, 24
	lsrs r1, r0, 24
	cmp r1, 0x40
	beq _081371AC
	ldr r0, [r5]
	adds r0, 0x61
	adds r0, r4
	ldrb r0, [r0]
	cmp r0, 0
	beq _081371A2
	lsls r0, r1, 4
	adds r0, r1
	lsls r0, 2
	adds r0, r7
	ldr r1, _081371D8 @ =sub_81371DC
	str r1, [r0]
_081371A2:
	ldr r1, [r5]
	adds r1, 0x54
	ldrb r0, [r1]
	adds r0, 0x1
	strb r0, [r1]
_081371AC:
	adds r0, r4, 0x1
	lsls r0, 16
	lsrs r4, r0, 16
	cmp r4, 0x4
	bls _08137160
	pop {r3}
	mov r8, r3
	pop {r4-r7}
	pop {r0}
	bx r0
	.align 2, 0
_081371C0: .4byte gSpriteSheet_ConditionUpDown
_081371C4: .4byte gSpritePalette_ConditionUpDown
_081371C8: .4byte gUnknown_02039304
_081371CC: .4byte gUnknown_08406158
_081371D0: .4byte gSprites + 0x1C
_081371D4: .4byte gSpriteTemplate_840618C
_081371D8: .4byte sub_81371DC
	thumb_func_end sub_8137138

	thumb_func_start sub_81371DC
sub_81371DC: @ 81371DC
	push {lr}
	adds r1, r0, 0
	movs r2, 0x2E
	ldrsh r0, [r1, r2]
	cmp r0, 0x5
	bgt _081371EE
	ldrh r0, [r1, 0x26]
	subs r0, 0x2
	b _081371F6
_081371EE:
	cmp r0, 0xB
	bgt _081371F8
	ldrh r0, [r1, 0x26]
	adds r0, 0x2
_081371F6:
	strh r0, [r1, 0x26]
_081371F8:
	ldrh r0, [r1, 0x2E]
	adds r0, 0x1
	strh r0, [r1, 0x2E]
	lsls r0, 16
	asrs r0, 16
	cmp r0, 0x3C
	ble _08137218
	adds r0, r1, 0
	bl DestroySprite
	ldr r0, _0813721C @ =gUnknown_02039304
	ldr r1, [r0]
	adds r1, 0x54
	ldrb r0, [r1]
	subs r0, 0x1
	strb r0, [r1]
_08137218:
	pop {r0}
	bx r0
	.align 2, 0
_0813721C: .4byte gUnknown_02039304
	thumb_func_end sub_81371DC
	
	.align 2, 0 @ Don't pad with nop.
