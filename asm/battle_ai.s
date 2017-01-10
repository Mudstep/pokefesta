	.include "constants/gba_constants.inc"
	.include "constants/species_constants.inc"
	.include "asm/macros.inc"

	.syntax unified

	.text

	thumb_func_start BattleAICmd_if_move_effect
BattleAICmd_if_move_effect: @ 81090E4
	push {r4-r7,lr}
	ldr r1, _081090FC @ =gAIScriptPtr
	ldr r0, [r1]
	ldrb r0, [r0, 0x1]
	adds r4, r1, 0
	cmp r0, 0x1
	beq _08109108
	cmp r0, 0x1
	bgt _08109100
	cmp r0, 0
	beq _0810915C
	b _081091B8
	.align 2, 0
_081090FC: .4byte gAIScriptPtr
_08109100:
	cmp r0, 0x2
	beq _0810915C
	cmp r0, 0x3
	bne _081091B8
_08109108:
	movs r3, 0
	ldr r1, _0810914C @ =gBattleMons
	ldr r0, _08109150 @ =gUnknown_02024C07
	ldrb r2, [r0]
	ldr r6, _08109154 @ =gBattleMoves
	movs r0, 0x58
	muls r0, r2
	adds r1, 0xC
	adds r2, r0, r1
	ldr r5, _08109158 @ =gAIScriptPtr
_0810911C:
	ldrh r0, [r2]
	cmp r0, 0
	beq _08109136
	adds r1, r0, 0
	lsls r0, r1, 1
	adds r0, r1
	lsls r0, 2
	adds r0, r6
	ldr r1, [r5]
	ldrb r0, [r0]
	ldrb r1, [r1, 0x2]
	cmp r0, r1
	beq _0810913E
_08109136:
	adds r2, 0x2
	adds r3, 0x1
	cmp r3, 0x3
	ble _0810911C
_0810913E:
	cmp r3, 0x4
	bne _081091A0
	ldr r0, [r4]
	adds r0, 0x7
	str r0, [r4]
	b _081091B8
	.align 2, 0
_0810914C: .4byte gBattleMons
_08109150: .4byte gUnknown_02024C07
_08109154: .4byte gBattleMoves
_08109158: .4byte gAIScriptPtr
_0810915C:
	movs r3, 0
	ldr r1, _081091C0 @ =gBattleMons
	ldr r0, _081091C4 @ =gUnknown_02024C07
	ldrb r2, [r0]
	ldr r7, _081091C8 @ =gBattleMoves
	ldr r6, _081091CC @ =0x02016a00
	ldr r5, _081091D0 @ =gUnknown_02024C08
	movs r0, 0x58
	muls r0, r2
	adds r1, 0xC
	adds r2, r0, r1
_08109172:
	lsls r1, r3, 1
	ldrh r0, [r2]
	cmp r0, 0
	beq _08109198
	ldrb r0, [r5]
	lsrs r0, 1
	lsls r0, 4
	adds r0, r1, r0
	adds r0, r6
	ldrh r1, [r0]
	lsls r0, r1, 1
	adds r0, r1
	lsls r0, 2
	adds r0, r7
	ldr r1, [r4]
	ldrb r0, [r0]
	ldrb r1, [r1, 0x2]
	cmp r0, r1
	beq _081091A0
_08109198:
	adds r2, 0x2
	adds r3, 0x1
	cmp r3, 0x7
	ble _08109172
_081091A0:
	ldr r2, [r4]
	ldrb r1, [r2, 0x3]
	ldrb r0, [r2, 0x4]
	lsls r0, 8
	orrs r1, r0
	ldrb r0, [r2, 0x5]
	lsls r0, 16
	orrs r1, r0
	ldrb r0, [r2, 0x6]
	lsls r0, 24
	orrs r1, r0
	str r1, [r4]
_081091B8:
	pop {r4-r7}
	pop {r0}
	bx r0
	.align 2, 0
_081091C0: .4byte gBattleMons
_081091C4: .4byte gUnknown_02024C07
_081091C8: .4byte gBattleMoves
_081091CC: .4byte 0x02016a00
_081091D0: .4byte gUnknown_02024C08
	thumb_func_end BattleAICmd_if_move_effect

	thumb_func_start BattleAICmd_if_not_move_effect
BattleAICmd_if_not_move_effect: @ 81091D4
	push {r4-r6,lr}
	ldr r1, _081091EC @ =gAIScriptPtr
	ldr r0, [r1]
	ldrb r0, [r0, 0x1]
	adds r4, r1, 0
	cmp r0, 0x1
	beq _081091F8
	cmp r0, 0x1
	bgt _081091F0
	cmp r0, 0
	beq _0810925C
	b _08109294
	.align 2, 0
_081091EC: .4byte gAIScriptPtr
_081091F0:
	cmp r0, 0x2
	beq _0810925C
	cmp r0, 0x3
	bne _08109294
_081091F8:
	movs r3, 0
	ldr r1, _0810924C @ =gBattleMons
	ldr r0, _08109250 @ =gUnknown_02024C07
	ldrb r2, [r0]
	ldr r6, _08109254 @ =gBattleMoves
	movs r0, 0x58
	muls r0, r2
	adds r1, 0xC
	adds r2, r0, r1
	ldr r5, _08109258 @ =gAIScriptPtr
_0810920C:
	ldrh r0, [r2]
	cmp r0, 0
	beq _08109226
	adds r1, r0, 0
	lsls r0, r1, 1
	adds r0, r1
	lsls r0, 2
	adds r0, r6
	ldr r1, [r5]
	ldrb r0, [r0]
	ldrb r1, [r1, 0x2]
	cmp r0, r1
	beq _0810922E
_08109226:
	adds r2, 0x2
	adds r3, 0x1
	cmp r3, 0x3
	ble _0810920C
_0810922E:
	cmp r3, 0x4
	bne _0810928E
	ldr r2, [r4]
	ldrb r1, [r2, 0x3]
	ldrb r0, [r2, 0x4]
	lsls r0, 8
	orrs r1, r0
	ldrb r0, [r2, 0x5]
	lsls r0, 16
	orrs r1, r0
	ldrb r0, [r2, 0x6]
	lsls r0, 24
	orrs r1, r0
	str r1, [r4]
	b _08109294
	.align 2, 0
_0810924C: .4byte gBattleMons
_08109250: .4byte gUnknown_02024C07
_08109254: .4byte gBattleMoves
_08109258: .4byte gAIScriptPtr
_0810925C:
	movs r3, 0
	ldr r1, _0810929C @ =0x02016a00
	ldr r0, _081092A0 @ =gUnknown_02024C08
	ldrb r0, [r0]
	lsrs r0, 1
	lsls r0, 4
	adds r2, r0, r1
	ldr r5, _081092A4 @ =gBattleMoves
_0810926C:
	ldrh r0, [r2]
	cmp r0, 0
	beq _08109286
	adds r1, r0, 0
	lsls r0, r1, 1
	adds r0, r1
	lsls r0, 2
	adds r0, r5
	ldr r1, [r4]
	ldrb r0, [r0]
	ldrb r1, [r1, 0x2]
	cmp r0, r1
	beq _0810928E
_08109286:
	adds r2, 0x2
	adds r3, 0x1
	cmp r3, 0x7
	ble _0810926C
_0810928E:
	ldr r0, [r4]
	adds r0, 0x7
	str r0, [r4]
_08109294:
	pop {r4-r6}
	pop {r0}
	bx r0
	.align 2, 0
_0810929C: .4byte 0x02016a00
_081092A0: .4byte gUnknown_02024C08
_081092A4: .4byte gBattleMoves
	thumb_func_end BattleAICmd_if_not_move_effect

	thumb_func_start BattleAICmd_if_last_move_did_damage
BattleAICmd_if_last_move_did_damage: @ 81092A8
	push {r4,r5,lr}
	ldr r0, _081092BC @ =gAIScriptPtr
	ldr r1, [r0]
	ldrb r1, [r1, 0x1]
	adds r5, r0, 0
	cmp r1, 0x1
	bne _081092C4
	ldr r0, _081092C0 @ =gUnknown_02024C07
	b _081092C6
	.align 2, 0
_081092BC: .4byte gAIScriptPtr
_081092C0: .4byte gUnknown_02024C07
_081092C4:
	ldr r0, _081092E4 @ =gUnknown_02024C08
_081092C6:
	ldrb r3, [r0]
	adds r4, r5, 0
	ldr r2, [r4]
	ldrb r0, [r2, 0x2]
	cmp r0, 0
	bne _081092EC
	ldr r0, _081092E8 @ =gUnknown_02024CA8
	lsls r1, r3, 3
	subs r1, r3
	lsls r1, 2
	adds r1, r0
	ldrh r0, [r1, 0x4]
	cmp r0, 0
	beq _081092F0
	b _08109306
	.align 2, 0
_081092E4: .4byte gUnknown_02024C08
_081092E8: .4byte gUnknown_02024CA8
_081092EC:
	cmp r0, 0x1
	beq _081092F6
_081092F0:
	adds r0, r2, 0x7
	str r0, [r4]
	b _08109328
_081092F6:
	ldr r0, _08109320 @ =gUnknown_02024CA8
	lsls r1, r3, 3
	subs r1, r3
	lsls r1, 2
	adds r1, r0
	ldrh r0, [r1, 0x6]
	cmp r0, 0
	beq _08109324
_08109306:
	ldrb r1, [r2, 0x3]
	ldrb r0, [r2, 0x4]
	lsls r0, 8
	orrs r1, r0
	ldrb r0, [r2, 0x5]
	lsls r0, 16
	orrs r1, r0
	ldrb r0, [r2, 0x6]
	lsls r0, 24
	orrs r1, r0
	str r1, [r4]
	b _08109328
	.align 2, 0
_08109320: .4byte gUnknown_02024CA8
_08109324:
	adds r0, r2, 0x7
	str r0, [r5]
_08109328:
	pop {r4,r5}
	pop {r0}
	bx r0
	thumb_func_end BattleAICmd_if_last_move_did_damage

	thumb_func_start BattleAICmd_if_encored
BattleAICmd_if_encored: @ 8109330
	push {r4,lr}
	ldr r4, _08109344 @ =gAIScriptPtr
	ldr r3, [r4]
	ldrb r0, [r3, 0x1]
	cmp r0, 0
	beq _08109348
	cmp r0, 0x1
	beq _08109370
	b _081093AC
	.align 2, 0
_08109344: .4byte gAIScriptPtr
_08109348:
	ldr r2, _08109364 @ =gUnknown_02024CA8
	ldr r0, _08109368 @ =gUnknown_02024A60
	ldrb r1, [r0]
	lsls r0, r1, 3
	subs r0, r1
	lsls r0, 2
	adds r0, r2
	ldr r1, _0810936C @ =0x02016800
	ldrh r0, [r0, 0x4]
	ldrh r1, [r1, 0x2]
	cmp r0, r1
	beq _08109388
	b _081093AC
	.align 2, 0
_08109364: .4byte gUnknown_02024CA8
_08109368: .4byte gUnknown_02024A60
_0810936C: .4byte 0x02016800
_08109370:
	ldr r2, _081093A0 @ =gUnknown_02024CA8
	ldr r0, _081093A4 @ =gUnknown_02024A60
	ldrb r1, [r0]
	lsls r0, r1, 3
	subs r0, r1
	lsls r0, 2
	adds r0, r2
	ldr r1, _081093A8 @ =0x02016800
	ldrh r0, [r0, 0x6]
	ldrh r1, [r1, 0x2]
	cmp r0, r1
	bne _081093AC
_08109388:
	ldrb r1, [r3, 0x2]
	ldrb r0, [r3, 0x3]
	lsls r0, 8
	orrs r1, r0
	ldrb r0, [r3, 0x4]
	lsls r0, 16
	orrs r1, r0
	ldrb r0, [r3, 0x5]
	lsls r0, 24
	orrs r1, r0
	str r1, [r4]
	b _081093B0
	.align 2, 0
_081093A0: .4byte gUnknown_02024CA8
_081093A4: .4byte gUnknown_02024A60
_081093A8: .4byte 0x02016800
_081093AC:
	adds r0, r3, 0x6
	str r0, [r4]
_081093B0:
	pop {r4}
	pop {r0}
	bx r0
	thumb_func_end BattleAICmd_if_encored

	thumb_func_start BattleAICmd_unk_45
BattleAICmd_unk_45: @ 81093B8
	ldr r2, _081093C4 @ =0x02016800
	ldrb r1, [r2, 0x10]
	movs r0, 0xB
	orrs r0, r1
	strb r0, [r2, 0x10]
	bx lr
	.align 2, 0
_081093C4: .4byte 0x02016800
	thumb_func_end BattleAICmd_unk_45

	thumb_func_start BattleAICmd_if_random_2
BattleAICmd_if_random_2: @ 81093C8
	push {r4,lr}
	ldr r0, _0810940C @ =0x02000000
	ldr r1, _08109410 @ =0x00016088
	adds r0, r1
	ldrb r0, [r0]
	lsls r4, r0, 2
	adds r4, r0
	lsls r4, 24
	lsrs r4, 24
	bl Random
	lsls r0, 16
	lsrs r0, 16
	movs r1, 0x64
	bl __umodsi3
	lsls r0, 24
	lsrs r0, 24
	cmp r0, r4
	bcs _08109418
	ldr r3, _08109414 @ =gAIScriptPtr
	ldr r2, [r3]
	ldrb r1, [r2, 0x1]
	ldrb r0, [r2, 0x2]
	lsls r0, 8
	orrs r1, r0
	ldrb r0, [r2, 0x3]
	lsls r0, 16
	orrs r1, r0
	ldrb r0, [r2, 0x4]
	lsls r0, 24
	orrs r1, r0
	str r1, [r3]
	b _08109420
	.align 2, 0
_0810940C: .4byte 0x02000000
_08109410: .4byte 0x00016088
_08109414: .4byte gAIScriptPtr
_08109418:
	ldr r1, _08109428 @ =gAIScriptPtr
	ldr r0, [r1]
	adds r0, 0x5
	str r0, [r1]
_08109420:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08109428: .4byte gAIScriptPtr
	thumb_func_end BattleAICmd_if_random_2

	thumb_func_start BattleAICmd_unk_47
BattleAICmd_unk_47: @ 810942C
	ldr r2, _08109438 @ =0x02016800
	ldrb r1, [r2, 0x10]
	movs r0, 0xD
	orrs r0, r1
	strb r0, [r2, 0x10]
	bx lr
	.align 2, 0
_08109438: .4byte 0x02016800
	thumb_func_end BattleAICmd_unk_47

	thumb_func_start BattleAICmd_get_hold_effect
BattleAICmd_get_hold_effect: @ 810943C
	push {r4,lr}
	ldr r0, _0810944C @ =gAIScriptPtr
	ldr r0, [r0]
	ldrb r0, [r0, 0x1]
	cmp r0, 0x1
	bne _08109454
	ldr r0, _08109450 @ =gUnknown_02024C07
	b _08109456
	.align 2, 0
_0810944C: .4byte gAIScriptPtr
_08109450: .4byte gUnknown_02024C07
_08109454:
	ldr r0, _0810947C @ =gUnknown_02024C08
_08109456:
	ldrb r4, [r0]
	adds r0, r4, 0
	bl battle_side_get_owner
	lsls r0, 24
	cmp r0, 0
	bne _08109488
	adds r0, r4, 0
	bl battle_get_per_side_status
	movs r1, 0x1
	ands r1, r0
	ldr r2, _08109480 @ =0x02016800
	ldr r3, _08109484 @ =0x00000222
	adds r0, r2, r3
	adds r1, r0
	ldrb r0, [r1]
	str r0, [r2, 0x8]
	b _0810949E
	.align 2, 0
_0810947C: .4byte gUnknown_02024C08
_08109480: .4byte 0x02016800
_08109484: .4byte 0x00000222
_08109488:
	ldr r1, _081094AC @ =gBattleMons
	movs r0, 0x58
	muls r0, r4
	adds r0, r1
	ldrh r0, [r0, 0x2E]
	bl ItemId_GetHoldEffect
	ldr r1, _081094B0 @ =0x02016800
	lsls r0, 24
	lsrs r0, 24
	str r0, [r1, 0x8]
_0810949E:
	ldr r1, _081094B4 @ =gAIScriptPtr
	ldr r0, [r1]
	adds r0, 0x2
	str r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_081094AC: .4byte gBattleMons
_081094B0: .4byte 0x02016800
_081094B4: .4byte gAIScriptPtr
	thumb_func_end BattleAICmd_get_hold_effect

	thumb_func_start BattleAICmd_get_gender
BattleAICmd_get_gender: @ 81094B8
	push {lr}
	ldr r0, _081094C8 @ =gAIScriptPtr
	ldr r0, [r0]
	ldrb r0, [r0, 0x1]
	cmp r0, 0x1
	bne _081094D0
	ldr r0, _081094CC @ =gUnknown_02024C07
	b _081094D2
	.align 2, 0
_081094C8: .4byte gAIScriptPtr
_081094CC: .4byte gUnknown_02024C07
_081094D0:
	ldr r0, _081094FC @ =gUnknown_02024C08
_081094D2:
	ldrb r1, [r0]
	ldr r2, _08109500 @ =gBattleMons
	movs r0, 0x58
	muls r1, r0
	adds r0, r1, r2
	ldrh r0, [r0]
	adds r2, 0x48
	adds r1, r2
	ldr r1, [r1]
	bl GetGenderFromSpeciesAndPersonality
	ldr r1, _08109504 @ =0x02016800
	lsls r0, 24
	lsrs r0, 24
	str r0, [r1, 0x8]
	ldr r1, _08109508 @ =gAIScriptPtr
	ldr r0, [r1]
	adds r0, 0x2
	str r0, [r1]
	pop {r0}
	bx r0
	.align 2, 0
_081094FC: .4byte gUnknown_02024C08
_08109500: .4byte gBattleMons
_08109504: .4byte 0x02016800
_08109508: .4byte gAIScriptPtr
	thumb_func_end BattleAICmd_get_gender

	thumb_func_start BattleAICmd_is_first_turn
BattleAICmd_is_first_turn: @ 810950C
	push {r4,lr}
	ldr r0, _08109520 @ =gAIScriptPtr
	ldr r1, [r0]
	ldrb r1, [r1, 0x1]
	adds r4, r0, 0
	cmp r1, 0x1
	bne _08109528
	ldr r0, _08109524 @ =gUnknown_02024C07
	b _0810952A
	.align 2, 0
_08109520: .4byte gAIScriptPtr
_08109524: .4byte gUnknown_02024C07
_08109528:
	ldr r0, _08109548 @ =gUnknown_02024C08
_0810952A:
	ldrb r3, [r0]
	ldr r2, _0810954C @ =0x02016800
	ldr r1, _08109550 @ =gUnknown_02024CA8
	lsls r0, r3, 3
	subs r0, r3
	lsls r0, 2
	adds r0, r1
	ldrb r0, [r0, 0x16]
	str r0, [r2, 0x8]
	ldr r0, [r4]
	adds r0, 0x2
	str r0, [r4]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08109548: .4byte gUnknown_02024C08
_0810954C: .4byte 0x02016800
_08109550: .4byte gUnknown_02024CA8
	thumb_func_end BattleAICmd_is_first_turn

	thumb_func_start BattleAICmd_get_stockpile_count
BattleAICmd_get_stockpile_count: @ 8109554
	push {r4,lr}
	ldr r0, _08109568 @ =gAIScriptPtr
	ldr r1, [r0]
	ldrb r1, [r1, 0x1]
	adds r4, r0, 0
	cmp r1, 0x1
	bne _08109570
	ldr r0, _0810956C @ =gUnknown_02024C07
	b _08109572
	.align 2, 0
_08109568: .4byte gAIScriptPtr
_0810956C: .4byte gUnknown_02024C07
_08109570:
	ldr r0, _08109590 @ =gUnknown_02024C08
_08109572:
	ldrb r3, [r0]
	ldr r2, _08109594 @ =0x02016800
	ldr r1, _08109598 @ =gUnknown_02024CA8
	lsls r0, r3, 3
	subs r0, r3
	lsls r0, 2
	adds r0, r1
	ldrb r0, [r0, 0x9]
	str r0, [r2, 0x8]
	ldr r0, [r4]
	adds r0, 0x2
	str r0, [r4]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08109590: .4byte gUnknown_02024C08
_08109594: .4byte 0x02016800
_08109598: .4byte gUnknown_02024CA8
	thumb_func_end BattleAICmd_get_stockpile_count

	thumb_func_start BattleAICmd_unk_4C
BattleAICmd_unk_4C: @ 810959C
	ldr r2, _081095B4 @ =0x02016800
	ldr r0, _081095B8 @ =gBattleTypeFlags
	ldrh r1, [r0]
	movs r0, 0x1
	ands r0, r1
	str r0, [r2, 0x8]
	ldr r1, _081095BC @ =gAIScriptPtr
	ldr r0, [r1]
	adds r0, 0x1
	str r0, [r1]
	bx lr
	.align 2, 0
_081095B4: .4byte 0x02016800
_081095B8: .4byte gBattleTypeFlags
_081095BC: .4byte gAIScriptPtr
	thumb_func_end BattleAICmd_unk_4C

	thumb_func_start BattleAICmd_get_item
BattleAICmd_get_item: @ 81095C0
	push {lr}
	ldr r0, _081095D4 @ =gAIScriptPtr
	ldr r1, [r0]
	ldrb r1, [r1, 0x1]
	adds r2, r0, 0
	cmp r1, 0x1
	bne _081095DC
	ldr r0, _081095D8 @ =gUnknown_02024C07
	b _081095DE
	.align 2, 0
_081095D4: .4byte gAIScriptPtr
_081095D8: .4byte gUnknown_02024C07
_081095DC:
	ldr r0, _081095F8 @ =gUnknown_02024C08
_081095DE:
	ldrb r0, [r0]
	ldr r1, _081095FC @ =0x02016800
	lsls r0, 1
	adds r0, r1
	ldr r3, _08109600 @ =0xfffff8cc
	adds r0, r3
	ldrb r0, [r0]
	str r0, [r1, 0x8]
	ldr r0, [r2]
	adds r0, 0x2
	str r0, [r2]
	pop {r0}
	bx r0
	.align 2, 0
_081095F8: .4byte gUnknown_02024C08
_081095FC: .4byte 0x02016800
_08109600: .4byte 0xfffff8cc
	thumb_func_end BattleAICmd_get_item

	thumb_func_start BattleAICmd_unk_4E
BattleAICmd_unk_4E: @ 8109604
	ldr r3, _08109620 @ =0x02016800
	ldr r2, _08109624 @ =gBattleMoves
	ldr r1, [r3, 0x8]
	lsls r0, r1, 1
	adds r0, r1
	lsls r0, 2
	adds r0, r2
	ldrb r0, [r0, 0x2]
	str r0, [r3, 0x8]
	ldr r1, _08109628 @ =gAIScriptPtr
	ldr r0, [r1]
	adds r0, 0x1
	str r0, [r1]
	bx lr
	.align 2, 0
_08109620: .4byte 0x02016800
_08109624: .4byte gBattleMoves
_08109628: .4byte gAIScriptPtr
	thumb_func_end BattleAICmd_unk_4E

	thumb_func_start BattleAICmd_unk_4F
BattleAICmd_unk_4F: @ 810962C
	ldr r3, _08109648 @ =0x02016800
	ldr r2, _0810964C @ =gBattleMoves
	ldr r1, [r3, 0x8]
	lsls r0, r1, 1
	adds r0, r1
	lsls r0, 2
	adds r0, r2
	ldrb r0, [r0, 0x1]
	str r0, [r3, 0x8]
	ldr r1, _08109650 @ =gAIScriptPtr
	ldr r0, [r1]
	adds r0, 0x1
	str r0, [r1]
	bx lr
	.align 2, 0
_08109648: .4byte 0x02016800
_0810964C: .4byte gBattleMoves
_08109650: .4byte gAIScriptPtr
	thumb_func_end BattleAICmd_unk_4F

	thumb_func_start BattleAICmd_unk_50
BattleAICmd_unk_50: @ 8109654
	ldr r3, _08109670 @ =0x02016800
	ldr r2, _08109674 @ =gBattleMoves
	ldr r1, [r3, 0x8]
	lsls r0, r1, 1
	adds r0, r1
	lsls r0, 2
	adds r0, r2
	ldrb r0, [r0]
	str r0, [r3, 0x8]
	ldr r1, _08109678 @ =gAIScriptPtr
	ldr r0, [r1]
	adds r0, 0x1
	str r0, [r1]
	bx lr
	.align 2, 0
_08109670: .4byte 0x02016800
_08109674: .4byte gBattleMoves
_08109678: .4byte gAIScriptPtr
	thumb_func_end BattleAICmd_unk_50

	thumb_func_start BattleAICmd_get_protect_count
BattleAICmd_get_protect_count: @ 810967C
	push {r4,lr}
	ldr r0, _08109690 @ =gAIScriptPtr
	ldr r1, [r0]
	ldrb r1, [r1, 0x1]
	adds r4, r0, 0
	cmp r1, 0x1
	bne _08109698
	ldr r0, _08109694 @ =gUnknown_02024C07
	b _0810969A
	.align 2, 0
_08109690: .4byte gAIScriptPtr
_08109694: .4byte gUnknown_02024C07
_08109698:
	ldr r0, _081096B8 @ =gUnknown_02024C08
_0810969A:
	ldrb r3, [r0]
	ldr r2, _081096BC @ =0x02016800
	ldr r1, _081096C0 @ =gUnknown_02024CA8
	lsls r0, r3, 3
	subs r0, r3
	lsls r0, 2
	adds r0, r1
	ldrb r0, [r0, 0x8]
	str r0, [r2, 0x8]
	ldr r0, [r4]
	adds r0, 0x2
	str r0, [r4]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_081096B8: .4byte gUnknown_02024C08
_081096BC: .4byte 0x02016800
_081096C0: .4byte gUnknown_02024CA8
	thumb_func_end BattleAICmd_get_protect_count

	thumb_func_start BattleAICmd_unk_52
BattleAICmd_unk_52: @ 81096C4
	bx lr
	thumb_func_end BattleAICmd_unk_52

	thumb_func_start BattleAICmd_unk_53
BattleAICmd_unk_53: @ 81096C8
	bx lr
	thumb_func_end BattleAICmd_unk_53

	thumb_func_start BattleAICmd_unk_54
BattleAICmd_unk_54: @ 81096CC
	bx lr
	thumb_func_end BattleAICmd_unk_54

	thumb_func_start BattleAICmd_unk_55
BattleAICmd_unk_55: @ 81096D0
	bx lr
	thumb_func_end BattleAICmd_unk_55

	thumb_func_start BattleAICmd_unk_56
BattleAICmd_unk_56: @ 81096D4
	bx lr
	thumb_func_end BattleAICmd_unk_56

	thumb_func_start BattleAICmd_unk_57
BattleAICmd_unk_57: @ 81096D8
	bx lr
	thumb_func_end BattleAICmd_unk_57

	thumb_func_start BattleAICmd_call
BattleAICmd_call: @ 81096DC
	push {r4,lr}
	ldr r4, _08109708 @ =gAIScriptPtr
	ldr r0, [r4]
	adds r0, 0x5
	bl sub_81098C4
	ldr r2, [r4]
	ldrb r1, [r2, 0x1]
	ldrb r0, [r2, 0x2]
	lsls r0, 8
	orrs r1, r0
	ldrb r0, [r2, 0x3]
	lsls r0, 16
	orrs r1, r0
	ldrb r0, [r2, 0x4]
	lsls r0, 24
	orrs r1, r0
	str r1, [r4]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08109708: .4byte gAIScriptPtr
	thumb_func_end BattleAICmd_call

	thumb_func_start BattleAICmd_jump
BattleAICmd_jump: @ 810970C
	ldr r3, _08109728 @ =gAIScriptPtr
	ldr r2, [r3]
	ldrb r1, [r2, 0x1]
	ldrb r0, [r2, 0x2]
	lsls r0, 8
	orrs r1, r0
	ldrb r0, [r2, 0x3]
	lsls r0, 16
	orrs r1, r0
	ldrb r0, [r2, 0x4]
	lsls r0, 24
	orrs r1, r0
	str r1, [r3]
	bx lr
	.align 2, 0
_08109728: .4byte gAIScriptPtr
	thumb_func_end BattleAICmd_jump

	thumb_func_start BattleAICmd_unk_5A
BattleAICmd_unk_5A: @ 810972C
	push {lr}
	bl sub_8109908
	lsls r0, 24
	cmp r0, 0
	bne _08109742
	ldr r2, _08109748 @ =0x02016800
	ldrb r1, [r2, 0x10]
	movs r0, 0x1
	orrs r0, r1
	strb r0, [r2, 0x10]
_08109742:
	pop {r0}
	bx r0
	.align 2, 0
_08109748: .4byte 0x02016800
	thumb_func_end BattleAICmd_unk_5A

	thumb_func_start BattleAICmd_if_level_cond
BattleAICmd_if_level_cond: @ 810974C
	push {r4,r5,lr}
	ldr r5, _08109764 @ =gAIScriptPtr
	ldr r4, [r5]
	ldrb r0, [r4, 0x1]
	cmp r0, 0x1
	beq _081097A0
	cmp r0, 0x1
	bgt _08109768
	cmp r0, 0
	beq _0810976E
	b _0810981C
	.align 2, 0
_08109764: .4byte gAIScriptPtr
_08109768:
	cmp r0, 0x2
	beq _081097D0
	b _0810981C
_0810976E:
	ldr r3, _08109794 @ =gBattleMons
	ldr r0, _08109798 @ =gUnknown_02024C07
	ldrb r0, [r0]
	movs r2, 0x58
	adds r1, r0, 0
	muls r1, r2
	adds r1, r3
	adds r1, 0x2A
	ldr r0, _0810979C @ =gUnknown_02024C08
	ldrb r0, [r0]
	muls r0, r2
	adds r0, r3
	adds r0, 0x2A
	ldrb r1, [r1]
	ldrb r0, [r0]
	cmp r1, r0
	bhi _081097F2
	b _08109818
	.align 2, 0
_08109794: .4byte gBattleMons
_08109798: .4byte gUnknown_02024C07
_0810979C: .4byte gUnknown_02024C08
_081097A0:
	ldr r3, _081097C4 @ =gBattleMons
	ldr r0, _081097C8 @ =gUnknown_02024C07
	ldrb r0, [r0]
	movs r2, 0x58
	adds r1, r0, 0
	muls r1, r2
	adds r1, r3
	adds r1, 0x2A
	ldr r0, _081097CC @ =gUnknown_02024C08
	ldrb r0, [r0]
	muls r0, r2
	adds r0, r3
	adds r0, 0x2A
	ldrb r1, [r1]
	ldrb r0, [r0]
	cmp r1, r0
	bcc _081097F2
	b _08109818
	.align 2, 0
_081097C4: .4byte gBattleMons
_081097C8: .4byte gUnknown_02024C07
_081097CC: .4byte gUnknown_02024C08
_081097D0:
	ldr r3, _0810980C @ =gBattleMons
	ldr r0, _08109810 @ =gUnknown_02024C07
	ldrb r0, [r0]
	movs r2, 0x58
	adds r1, r0, 0
	muls r1, r2
	adds r1, r3
	adds r1, 0x2A
	ldr r0, _08109814 @ =gUnknown_02024C08
	ldrb r0, [r0]
	muls r0, r2
	adds r0, r3
	adds r0, 0x2A
	ldrb r1, [r1]
	ldrb r0, [r0]
	cmp r1, r0
	bne _08109818
_081097F2:
	ldrb r1, [r4, 0x2]
	ldrb r0, [r4, 0x3]
	lsls r0, 8
	orrs r1, r0
	ldrb r0, [r4, 0x4]
	lsls r0, 16
	orrs r1, r0
	ldrb r0, [r4, 0x5]
	lsls r0, 24
	orrs r1, r0
	str r1, [r5]
	b _0810981C
	.align 2, 0
_0810980C: .4byte gBattleMons
_08109810: .4byte gUnknown_02024C07
_08109814: .4byte gUnknown_02024C08
_08109818:
	adds r0, r4, 0x6
	str r0, [r5]
_0810981C:
	pop {r4,r5}
	pop {r0}
	bx r0
	thumb_func_end BattleAICmd_if_level_cond

	thumb_func_start BattleAICmd_if_taunted
BattleAICmd_if_taunted: @ 8109824
	push {lr}
	ldr r2, _08109858 @ =gUnknown_02024CA8
	ldr r0, _0810985C @ =gUnknown_02024C08
	ldrb r1, [r0]
	lsls r0, r1, 3
	subs r0, r1
	lsls r0, 2
	adds r0, r2
	ldrb r0, [r0, 0x13]
	lsls r0, 28
	cmp r0, 0
	beq _08109864
	ldr r3, _08109860 @ =gAIScriptPtr
	ldr r2, [r3]
	ldrb r1, [r2, 0x1]
	ldrb r0, [r2, 0x2]
	lsls r0, 8
	orrs r1, r0
	ldrb r0, [r2, 0x3]
	lsls r0, 16
	orrs r1, r0
	ldrb r0, [r2, 0x4]
	lsls r0, 24
	orrs r1, r0
	str r1, [r3]
	b _0810986C
	.align 2, 0
_08109858: .4byte gUnknown_02024CA8
_0810985C: .4byte gUnknown_02024C08
_08109860: .4byte gAIScriptPtr
_08109864:
	ldr r1, _08109870 @ =gAIScriptPtr
	ldr r0, [r1]
	adds r0, 0x5
	str r0, [r1]
_0810986C:
	pop {r0}
	bx r0
	.align 2, 0
_08109870: .4byte gAIScriptPtr
	thumb_func_end BattleAICmd_if_taunted

	thumb_func_start BattleAICmd_if_not_taunted
BattleAICmd_if_not_taunted: @ 8109874
	push {lr}
	ldr r2, _081098A8 @ =gUnknown_02024CA8
	ldr r0, _081098AC @ =gUnknown_02024C08
	ldrb r1, [r0]
	lsls r0, r1, 3
	subs r0, r1
	lsls r0, 2
	adds r0, r2
	ldrb r0, [r0, 0x13]
	lsls r0, 28
	cmp r0, 0
	bne _081098B4
	ldr r3, _081098B0 @ =gAIScriptPtr
	ldr r2, [r3]
	ldrb r1, [r2, 0x1]
	ldrb r0, [r2, 0x2]
	lsls r0, 8
	orrs r1, r0
	ldrb r0, [r2, 0x3]
	lsls r0, 16
	orrs r1, r0
	ldrb r0, [r2, 0x4]
	lsls r0, 24
	orrs r1, r0
	str r1, [r3]
	b _081098BC
	.align 2, 0
_081098A8: .4byte gUnknown_02024CA8
_081098AC: .4byte gUnknown_02024C08
_081098B0: .4byte gAIScriptPtr
_081098B4:
	ldr r1, _081098C0 @ =gAIScriptPtr
	ldr r0, [r1]
	adds r0, 0x5
	str r0, [r1]
_081098BC:
	pop {r0}
	bx r0
	.align 2, 0
_081098C0: .4byte gAIScriptPtr
	thumb_func_end BattleAICmd_if_not_taunted

	thumb_func_start sub_81098C4
sub_81098C4: @ 81098C4
	push {r4,lr}
	ldr r4, _081098E0 @ =0x02016c00
	adds r3, r4, 0
	adds r3, 0x20
	ldrb r1, [r3]
	adds r2, r1, 0x1
	strb r2, [r3]
	lsls r1, 24
	lsrs r1, 22
	adds r1, r4
	str r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_081098E0: .4byte 0x02016c00
	thumb_func_end sub_81098C4

	thumb_func_start unref_sub_81098E4
unref_sub_81098E4: @ 81098E4
	ldr r3, _08109900 @ =0x02016c00
	adds r2, r3, 0
	adds r2, 0x20
	ldrb r0, [r2]
	adds r1, r0, 0x1
	strb r1, [r2]
	lsls r0, 24
	lsrs r0, 22
	adds r0, r3
	ldr r1, _08109904 @ =gAIScriptPtr
	ldr r1, [r1]
	str r1, [r0]
	bx lr
	.align 2, 0
_08109900: .4byte 0x02016c00
_08109904: .4byte gAIScriptPtr
	thumb_func_end unref_sub_81098E4

	thumb_func_start sub_8109908
sub_8109908: @ 8109908
	push {lr}
	ldr r3, _0810991C @ =0x02016c00
	adds r2, r3, 0
	adds r2, 0x20
	ldrb r0, [r2]
	cmp r0, 0
	bne _08109920
	movs r0, 0
	b _08109932
	.align 2, 0
_0810991C: .4byte 0x02016c00
_08109920:
	subs r0, 0x1
	strb r0, [r2]
	ldr r1, _08109938 @ =gAIScriptPtr
	ldrb r0, [r2]
	lsls r0, 2
	adds r0, r3
	ldr r0, [r0]
	str r0, [r1]
	movs r0, 0x1
_08109932:
	pop {r1}
	bx r1
	.align 2, 0
_08109938: .4byte gAIScriptPtr
	thumb_func_end sub_8109908

	.align 2, 0 @ Don't pad with nop.
