	.include "constants/gba_constants.inc"
	.include "include/macros.inc"

	.syntax unified

	.text

	thumb_func_start sub_80F6074
sub_80F6074: @ 80F6074
	push {r4-r7,lr}
	ldr r1, _080F6120 @ =gPokenavStructPtr
	ldr r6, [r1]
	ldr r1, _080F6124 @ =0x00008770
	adds r3, r6, r1
	ldrh r1, [r3]
	lsls r0, 16
	asrs r7, r0, 16
	adds r2, r7, r1
	strh r2, [r3]
	ldr r5, _080F6128 @ =0x00008774
	adds r4, r6, r5
	lsls r0, r2, 16
	asrs r0, 16
	ldrh r1, [r4]
	mov r12, r1
	movs r5, 0
	ldrsh r1, [r4, r5]
	cmp r0, r1
	ble _080F60A4
	subs r0, r2, 0x1
	mov r1, r12
	subs r0, r1
	strh r0, [r3]
_080F60A4:
	ldrh r1, [r3]
	movs r2, 0
	ldrsh r0, [r3, r2]
	cmp r0, 0
	bge _080F60B6
	adds r0, r1, 0x1
	ldrh r5, [r4]
	adds r0, r5
	strh r0, [r3]
_080F60B6:
	ldr r0, _080F612C @ =0x00008772
	adds r3, r6, r0
	ldrh r0, [r3]
	adds r2, r7, r0
	strh r2, [r3]
	lsls r0, r2, 16
	asrs r0, 16
	ldrh r1, [r4]
	mov r12, r1
	movs r5, 0
	ldrsh r1, [r4, r5]
	cmp r0, r1
	ble _080F60D8
	subs r0, r2, 0x1
	mov r1, r12
	subs r0, r1
	strh r0, [r3]
_080F60D8:
	ldrh r1, [r3]
	movs r2, 0
	ldrsh r0, [r3, r2]
	cmp r0, 0
	bge _080F60EA
	adds r0, r1, 0x1
	ldrh r5, [r4]
	adds r0, r5
	strh r0, [r3]
_080F60EA:
	ldr r0, _080F6130 @ =0x0000876e
	adds r3, r6, r0
	ldrh r0, [r3]
	adds r2, r7, r0
	strh r2, [r3]
	lsls r0, r2, 16
	asrs r0, 16
	ldrh r5, [r4]
	movs r6, 0
	ldrsh r1, [r4, r6]
	cmp r0, r1
	ble _080F6108
	subs r0, r2, 0x1
	subs r0, r5
	strh r0, [r3]
_080F6108:
	ldrh r1, [r3]
	movs r2, 0
	ldrsh r0, [r3, r2]
	cmp r0, 0
	bge _080F611A
	adds r0, r1, 0x1
	ldrh r4, [r4]
	adds r0, r4
	strh r0, [r3]
_080F611A:
	pop {r4-r7}
	pop {r0}
	bx r0
	.align 2, 0
_080F6120: .4byte gPokenavStructPtr
_080F6124: .4byte 0x00008770
_080F6128: .4byte 0x00008774
_080F612C: .4byte 0x00008772
_080F6130: .4byte 0x0000876e
	thumb_func_end sub_80F6074

	thumb_func_start sub_80F6134
sub_80F6134: @ 80F6134
	push {r4-r6,lr}
	ldr r0, _080F6194 @ =gPokenavStructPtr
	ldr r3, [r0]
	ldr r1, _080F6198 @ =0x000087c9
	adds r0, r3, r1
	ldrb r0, [r0]
	cmp r0, 0
	beq _080F61E0
	ldr r2, _080F619C @ =0x000087dc
	adds r5, r3, r2
	movs r4, 0
	ldrsh r1, [r5, r4]
	ldr r6, _080F61A0 @ =0x00008774
	adds r4, r3, r6
	movs r2, 0
	ldrsh r0, [r4, r2]
	subs r0, 0x7
	cmp r1, r0
	bge _080F61AC
	subs r6, 0x8
	adds r1, r3, r6
	movs r0, 0
	strh r0, [r1]
	ldrh r0, [r5]
	ldr r1, _080F61A4 @ =0x00008770
	adds r2, r3, r1
	strh r0, [r2]
	ldrh r1, [r5]
	ldr r5, _080F61A8 @ =0x0000876e
	adds r0, r3, r5
	strh r1, [r0]
	ldrh r5, [r2]
	adds r0, r5, 0x7
	adds r6, 0x6
	adds r2, r3, r6
	strh r0, [r2]
	lsls r0, 16
	asrs r0, 16
	ldrh r3, [r4]
	movs r6, 0
	ldrsh r1, [r4, r6]
	cmp r0, r1
	ble _080F61F4
	adds r0, r5, 0x6
	subs r0, r3
	strh r0, [r2]
	b _080F61F4
	.align 2, 0
_080F6194: .4byte gPokenavStructPtr
_080F6198: .4byte 0x000087c9
_080F619C: .4byte 0x000087dc
_080F61A0: .4byte 0x00008774
_080F61A4: .4byte 0x00008770
_080F61A8: .4byte 0x0000876e
_080F61AC:
	ldrh r0, [r4]
	subs r0, 0x7
	ldr r2, _080F61D4 @ =0x00008770
	adds r1, r3, r2
	strh r0, [r1]
	ldrh r1, [r4]
	ldr r6, _080F61D8 @ =0x00008772
	adds r0, r3, r6
	strh r1, [r0]
	ldrh r1, [r5]
	subs r2, 0x2
	adds r0, r3, r2
	strh r1, [r0]
	ldrh r0, [r4]
	subs r0, r1
	movs r1, 0x7
	subs r1, r0
	ldr r4, _080F61DC @ =0x0000876c
	adds r0, r3, r4
	b _080F61F2
	.align 2, 0
_080F61D4: .4byte 0x00008770
_080F61D8: .4byte 0x00008772
_080F61DC: .4byte 0x0000876c
_080F61E0:
	ldr r5, _080F61FC @ =0x000087dc
	adds r2, r3, r5
	ldrh r1, [r2]
	ldr r6, _080F6200 @ =0x0000876c
	adds r0, r3, r6
	strh r1, [r0]
	ldrh r1, [r2]
	ldr r2, _080F6204 @ =0x0000876e
	adds r0, r3, r2
_080F61F2:
	strh r1, [r0]
_080F61F4:
	pop {r4-r6}
	pop {r0}
	bx r0
	.align 2, 0
_080F61FC: .4byte 0x000087dc
_080F6200: .4byte 0x0000876c
_080F6204: .4byte 0x0000876e
	thumb_func_end sub_80F6134

	thumb_func_start sub_80F6208
sub_80F6208: @ 80F6208
	push {lr}
	ldr r0, _080F623C @ =gPokenavStructPtr
	ldr r1, [r0]
	ldr r2, _080F6240 @ =0x00008fe6
	adds r0, r1, r2
	movs r2, 0
	strb r2, [r0]
	ldr r3, _080F6244 @ =0x00008fe7
	adds r0, r1, r3
	strb r2, [r0]
	ldr r0, _080F6248 @ =0x00008fe8
	adds r2, r1, r0
	movs r0, 0xFF
	strb r0, [r2]
	ldr r2, _080F624C @ =0x00006dac
	adds r1, r2
	ldrb r0, [r1]
	cmp r0, 0
	bne _080F6238
_080F622E:
	bl sub_80F6250
	lsls r0, 24
	cmp r0, 0
	bne _080F622E
_080F6238:
	pop {r0}
	bx r0
	.align 2, 0
_080F623C: .4byte gPokenavStructPtr
_080F6240: .4byte 0x00008fe6
_080F6244: .4byte 0x00008fe7
_080F6248: .4byte 0x00008fe8
_080F624C: .4byte 0x00006dac
	thumb_func_end sub_80F6208

	thumb_func_start sub_80F6250
sub_80F6250: @ 80F6250
	push {r4-r7,lr}
	mov r7, r9
	mov r6, r8
	push {r6,r7}
	ldr r0, _080F6270 @ =gPokenavStructPtr
	ldr r2, [r0]
	ldr r0, _080F6274 @ =0x00008fe8
	adds r3, r2, r0
	movs r1, 0
	ldrsb r1, [r3, r1]
	movs r0, 0x1
	negs r0, r0
	cmp r1, r0
	beq _080F6278
_080F626C:
	movs r0, 0
	b _080F637E
	.align 2, 0
_080F6270: .4byte gPokenavStructPtr
_080F6274: .4byte 0x00008fe8
_080F6278:
	ldr r1, _080F630C @ =0x00008fe6
	adds r0, r2, r1
	ldrb r0, [r0]
	cmp r0, 0x7
	beq _080F631C
	cmp r0, 0x8
	beq _080F626C
	movs r6, 0
	adds r7, r2, 0
	adds r5, r7, r1
	ldr r0, _080F6310 @ =0x00008fe7
	adds r4, r7, r0
	ldr r1, _080F6314 @ =gPokemonStorage + 0x4
	mov r8, r1
	ldr r0, _080F6318 @ =0x000041a0
	add r0, r8
	mov r9, r0
_080F629A:
	ldrb r0, [r5]
	lsls r1, r0, 2
	adds r1, r0
	lsls r0, r1, 4
	subs r0, r1
	lsls r0, 5
	ldrb r2, [r4]
	lsls r1, r2, 2
	adds r1, r2
	lsls r1, 4
	add r1, r8
	adds r0, r1
	movs r1, 0x51
	bl GetBoxMonData
	cmp r0, 0
	bne _080F636C
	ldrb r0, [r5]
	lsls r1, r0, 2
	adds r1, r0
	lsls r0, r1, 4
	subs r0, r1
	lsls r0, 5
	ldrb r2, [r4]
	lsls r1, r2, 2
	adds r1, r2
	lsls r1, 4
	add r1, r9
	adds r0, r1
	movs r1, 0x51
	bl GetBoxMonData
	adds r1, r0, 0
	cmp r1, 0
	bne _080F636C
	ldrb r0, [r4]
	adds r0, 0x1
	strb r0, [r4]
	movs r2, 0xFF
	lsls r0, 24
	lsrs r0, 24
	cmp r0, 0x1D
	bls _080F62FE
	strb r1, [r4]
	ldrb r0, [r5]
	adds r0, 0x1
	strb r0, [r5]
	ands r0, r2
	cmp r0, 0x6
	bhi _080F637C
_080F62FE:
	adds r0, r6, 0x1
	lsls r0, 16
	lsrs r6, r0, 16
	cmp r6, 0x9
	bls _080F629A
	b _080F637C
	.align 2, 0
_080F630C: .4byte 0x00008fe6
_080F6310: .4byte 0x00008fe7
_080F6314: .4byte gPokemonStorage + 0x4
_080F6318: .4byte 0x000041a0
_080F631C:
	movs r0, 0
	strb r0, [r3]
	movs r6, 0
	b _080F632A
_080F6324:
	adds r0, r6, 0x1
	lsls r0, 16
	lsrs r6, r0, 16
_080F632A:
	cmp r6, 0x5
	bhi _080F634C
	movs r0, 0x64
	muls r0, r6
	ldr r1, _080F635C @ =gPlayerParty
	adds r0, r1
	movs r1, 0x51
	bl GetMonData
	cmp r0, 0
	beq _080F6324
	ldr r0, _080F6360 @ =gPokenavStructPtr
	ldr r0, [r0]
	ldr r1, _080F6364 @ =0x00008fe8
	adds r0, r1
	movs r1, 0x1
	strb r1, [r0]
_080F634C:
	ldr r0, _080F6360 @ =gPokenavStructPtr
	ldr r1, [r0]
	ldr r0, _080F6368 @ =0x00008fe6
	adds r1, r0
	ldrb r0, [r1]
	adds r0, 0x1
	b _080F6372
	.align 2, 0
_080F635C: .4byte gPlayerParty
_080F6360: .4byte gPokenavStructPtr
_080F6364: .4byte 0x00008fe8
_080F6368: .4byte 0x00008fe6
_080F636C:
	ldr r0, _080F6378 @ =0x00008fe8
	adds r1, r7, r0
	movs r0, 0x1
_080F6372:
	strb r0, [r1]
	b _080F626C
	.align 2, 0
_080F6378: .4byte 0x00008fe8
_080F637C:
	movs r0, 0x1
_080F637E:
	pop {r3,r4}
	mov r8, r3
	mov r9, r4
	pop {r4-r7}
	pop {r1}
	bx r1
	thumb_func_end sub_80F6250

	thumb_func_start sub_80F638C
sub_80F638C: @ 80F638C
	push {r4,lr}
	ldr r0, _080F63C0 @ =gPokenavStructPtr
	ldr r4, [r0]
	ldr r1, _080F63C4 @ =0x00008fe6
	adds r0, r4, r1
	movs r1, 0
	strb r1, [r0]
	ldr r2, _080F63C8 @ =0x00008fe7
	adds r0, r4, r2
	strb r1, [r0]
	bl sub_80F492C
	ldr r0, _080F63CC @ =0x00006dac
	adds r4, r0
	ldrb r0, [r4]
	cmp r0, 0
	bne _080F63B8
_080F63AE:
	bl sub_80F63D0
	lsls r0, 24
	cmp r0, 0
	bne _080F63AE
_080F63B8:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080F63C0: .4byte gPokenavStructPtr
_080F63C4: .4byte 0x00008fe6
_080F63C8: .4byte 0x00008fe7
_080F63CC: .4byte 0x00006dac
	thumb_func_end sub_80F638C

	thumb_func_start sub_80F63D0
sub_80F63D0: @ 80F63D0
	push {r4-r6,lr}
	sub sp, 0x4
	ldr r0, _080F647C @ =gPokenavStructPtr
	ldr r2, [r0]
	ldr r1, _080F6480 @ =0x00008fe6
	adds r0, r2, r1
	ldrb r3, [r0]
	cmp r3, 0xE
	beq _080F6498
	cmp r3, 0xF
	bne _080F63E8
	b _080F64F6
_080F63E8:
	movs r1, 0x80
	lsls r1, 23
	ldr r0, [sp]
	orrs r0, r1
	str r0, [sp]
	movs r6, 0
	ldr r0, _080F6480 @ =0x00008fe6
	adds r5, r2, r0
	ldr r1, _080F6484 @ =0x00008fe7
	adds r4, r2, r1
_080F63FC:
	ldrb r0, [r5]
	lsls r1, r0, 2
	adds r1, r0
	lsls r0, r1, 4
	subs r0, r1
	lsls r0, 5
	ldrb r2, [r4]
	lsls r1, r2, 2
	adds r1, r2
	lsls r1, 4
	ldr r2, _080F6488 @ =gPokemonStorage + 0x4
	adds r1, r2
	adds r0, r1
	movs r1, 0x51
	bl GetBoxMonData
	lsls r0, 24
	lsrs r3, r0, 24
	cmp r3, 0
	beq _080F644E
	ldrb r1, [r5]
	lsls r1, 8
	ldr r2, _080F648C @ =0xffff00ff
	ldr r0, [sp]
	ands r0, r2
	orrs r0, r1
	str r0, [sp]
	ldrb r2, [r4]
	movs r1, 0x1F
	ands r2, r1
	lsls r2, 16
	ldr r1, _080F6490 @ =0xffe0ffff
	ands r0, r1
	orrs r0, r2
	ldr r1, _080F6494 @ =0xffffff00
	ands r0, r1
	orrs r0, r3
	str r0, [sp]
	mov r0, sp
	bl sub_80F4944
_080F644E:
	ldrb r0, [r4]
	adds r0, 0x1
	movs r2, 0
	strb r0, [r4]
	movs r1, 0xFF
	lsls r0, 24
	lsrs r0, 24
	cmp r0, 0x1E
	bne _080F646E
	strb r2, [r4]
	ldrb r0, [r5]
	adds r0, 0x1
	strb r0, [r5]
	ands r0, r1
	cmp r0, 0xE
	beq _080F6508
_080F646E:
	adds r0, r6, 0x1
	lsls r0, 16
	lsrs r6, r0, 16
	cmp r6, 0xE
	bls _080F63FC
	b _080F6508
	.align 2, 0
_080F647C: .4byte gPokenavStructPtr
_080F6480: .4byte 0x00008fe6
_080F6484: .4byte 0x00008fe7
_080F6488: .4byte gPokemonStorage + 0x4
_080F648C: .4byte 0xffff00ff
_080F6490: .4byte 0xffe0ffff
_080F6494: .4byte 0xffffff00
_080F6498:
	mov r0, sp
	ldrb r1, [r0, 0x3]
	movs r2, 0x40
	orrs r1, r2
	strb r1, [r0, 0x3]
	strb r3, [r0, 0x1]
	movs r6, 0
	mov r4, sp
_080F64A8:
	movs r0, 0x64
	muls r0, r6
	ldr r1, _080F64FC @ =gPlayerParty
	adds r0, r1
	movs r1, 0x51
	bl GetMonData
	lsls r0, 24
	lsrs r3, r0, 24
	cmp r3, 0
	beq _080F64DA
	movs r0, 0x1F
	adds r2, r6, 0
	ands r2, r0
	ldrb r0, [r4, 0x2]
	movs r5, 0x20
	negs r5, r5
	adds r1, r5, 0
	ands r0, r1
	orrs r0, r2
	strb r0, [r4, 0x2]
	strb r3, [r4]
	mov r0, sp
	bl sub_80F4944
_080F64DA:
	adds r0, r6, 0x1
	lsls r0, 16
	lsrs r6, r0, 16
	cmp r6, 0x5
	bls _080F64A8
	bl sub_80F49F4
	ldr r0, _080F6500 @ =gPokenavStructPtr
	ldr r1, [r0]
	ldr r0, _080F6504 @ =0x00008fe6
	adds r1, r0
	ldrb r0, [r1]
	adds r0, 0x1
	strb r0, [r1]
_080F64F6:
	movs r0, 0
	b _080F650A
	.align 2, 0
_080F64FC: .4byte gPlayerParty
_080F6500: .4byte gPokenavStructPtr
_080F6504: .4byte 0x00008fe6
_080F6508:
	movs r0, 0x1
_080F650A:
	add sp, 0x4
	pop {r4-r6}
	pop {r1}
	bx r1
	thumb_func_end sub_80F63D0

	thumb_func_start sub_80F6514
sub_80F6514: @ 80F6514
	push {r4-r7,lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5-r7}
	sub sp, 0xC
	mov r10, r0
	lsls r1, 16
	lsrs r1, 16
	str r1, [sp]
	lsls r2, 24
	lsrs r2, 24
	str r2, [sp, 0x4]
	mov r5, r10
	ldr r0, _080F65A4 @ =gPokenavStructPtr
	ldr r0, [r0]
	lsls r1, 2
	adds r0, r1
	ldr r2, _080F65A8 @ =0x0000893d
	adds r1, r0, r2
	ldrb r6, [r1]
	mov r8, r6
	ldr r1, _080F65AC @ =0x0000893e
	adds r0, r1
	ldrb r0, [r0]
	lsls r0, 27
	lsrs r4, r0, 27
	adds r7, r4, 0
	adds r0, r6, 0
	adds r1, r4, 0
	movs r2, 0x2D
	movs r3, 0
	bl sub_80F44B0
	cmp r0, 0
	beq _080F655E
	b _080F66CA
_080F655E:
	adds r0, r6, 0
	adds r1, r4, 0
	movs r2, 0x2
	adds r3, r5, 0
	bl sub_80F44B0
	adds r0, r5, 0
	bl StringGetEnd10
	adds r0, r6, 0
	adds r1, r4, 0
	movs r2, 0xB
	movs r3, 0
	bl sub_80F44B0
	lsls r0, 16
	lsrs r0, 16
	mov r9, r0
	cmp r6, 0xE
	bne _080F65B4
	movs r0, 0x64
	muls r4, r0
	ldr r0, _080F65B0 @ =gPlayerParty
	adds r4, r0
	adds r0, r4, 0
	movs r1, 0x38
	bl GetMonData
	lsls r0, 16
	lsrs r0, 16
	str r0, [sp, 0x8]
	adds r0, r4, 0
	bl GetMonGender
	b _080F65EA
	.align 2, 0
_080F65A4: .4byte gPokenavStructPtr
_080F65A8: .4byte 0x0000893d
_080F65AC: .4byte 0x0000893e
_080F65B0: .4byte gPlayerParty
_080F65B4:
	mov r2, r8
	lsls r1, r2, 2
	add r1, r8
	lsls r0, r1, 4
	subs r0, r1
	lsls r0, 5
	lsls r1, r7, 2
	adds r1, r7
	lsls r1, 4
	ldr r2, _080F6620 @ =gPokemonStorage + 0x4
	adds r1, r2
	adds r0, r1
	bl GetLevelFromBoxMonExp
	lsls r0, 24
	lsrs r0, 24
	str r0, [sp, 0x8]
	mov r0, r8
	adds r1, r7, 0
	movs r2, 0
	movs r3, 0
	bl sub_80F44B0
	adds r1, r0, 0
	mov r0, r9
	bl GetGenderFromSpeciesAndPersonality
_080F65EA:
	lsls r0, 24
	lsrs r4, r0, 24
	mov r0, r9
	mov r1, r10
	bl ShouldHideGenderIcon
	cmp r0, 0
	beq _080F65FC
	movs r4, 0xFF
_080F65FC:
	adds r0, r5, 0
	bl StringLength
	lsls r0, 16
	lsrs r0, 16
	adds r5, r0
	movs r1, 0xFC
	strb r1, [r5]
	movs r0, 0x13
	strb r0, [r5, 0x1]
	movs r0, 0x3F
	strb r0, [r5, 0x2]
	adds r5, 0x3
	cmp r4, 0
	beq _080F6624
	cmp r4, 0xFE
	beq _080F663C
	b _080F6656
	.align 2, 0
_080F6620: .4byte gPokemonStorage + 0x4
_080F6624:
	strb r1, [r5]
	movs r0, 0x1
	strb r0, [r5, 0x1]
	movs r0, 0xC
	strb r0, [r5, 0x2]
	strb r1, [r5, 0x3]
	movs r0, 0x3
	strb r0, [r5, 0x4]
	movs r0, 0xD
	strb r0, [r5, 0x5]
	movs r0, 0xB5
	b _080F6652
_080F663C:
	strb r1, [r5]
	movs r0, 0x1
	strb r0, [r5, 0x1]
	movs r0, 0xA
	strb r0, [r5, 0x2]
	strb r1, [r5, 0x3]
	movs r0, 0x3
	strb r0, [r5, 0x4]
	movs r0, 0xB
	strb r0, [r5, 0x5]
	movs r0, 0xB6
_080F6652:
	strb r0, [r5, 0x6]
	adds r5, 0x7
_080F6656:
	movs r4, 0xFC
	strb r4, [r5]
	movs r1, 0x1
	strb r1, [r5, 0x1]
	strb r1, [r5, 0x2]
	strb r4, [r5, 0x3]
	movs r0, 0x3
	strb r0, [r5, 0x4]
	movs r0, 0x5
	strb r0, [r5, 0x5]
	adds r5, 0x6
	strb r4, [r5]
	movs r6, 0x13
	strb r6, [r5, 0x1]
	movs r0, 0x46
	strb r0, [r5, 0x2]
	adds r5, 0x3
	movs r0, 0xBA
	strb r0, [r5]
	strb r4, [r5, 0x1]
	movs r0, 0x11
	strb r0, [r5, 0x2]
	strb r1, [r5, 0x3]
	movs r0, 0x34
	strb r0, [r5, 0x4]
	adds r5, 0x5
	adds r0, r5, 0
	ldr r1, [sp, 0x8]
	bl ConvertIntToDecimalString
	adds r5, r0, 0
	ldr r0, [sp, 0x4]
	cmp r0, 0x1
	bne _080F66C0
	ldr r0, _080F66B8 @ =gPokenavStructPtr
	ldr r0, [r0]
	ldr r2, [sp]
	lsls r1, r2, 2
	adds r0, r1
	ldr r1, _080F66BC @ =0x0000893c
	adds r0, r1
	ldrb r1, [r0]
	adds r0, r5, 0
	movs r2, 0x80
	movs r3, 0x1
	bl AlignInt1InMenuWindow
	adds r5, r0, 0
	b _080F66CE
	.align 2, 0
_080F66B8: .4byte gPokenavStructPtr
_080F66BC: .4byte 0x0000893c
_080F66C0:
	strb r4, [r5]
	strb r6, [r5, 0x1]
	movs r0, 0x67
	strb r0, [r5, 0x2]
	adds r5, 0x3
_080F66CA:
	movs r0, 0xFF
	strb r0, [r5]
_080F66CE:
	adds r0, r5, 0
	add sp, 0xC
	pop {r3-r5}
	mov r8, r3
	mov r9, r4
	mov r10, r5
	pop {r4-r7}
	pop {r1}
	bx r1
	thumb_func_end sub_80F6514

	thumb_func_start sub_80F66E0
sub_80F66E0: @ 80F66E0
	push {r4-r7,lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5-r7}
	sub sp, 0x4
	ldr r0, _080F676C @ =gPokenavStructPtr
	mov r8, r0
	ldr r4, [r0]
	ldr r1, _080F6770 @ =0x00008829
	adds r0, r4, r1
	ldr r2, _080F6774 @ =0x0000876e
	adds r5, r4, r2
	ldrh r1, [r5]
	movs r2, 0
	bl sub_80F6514
	movs r3, 0
	ldrsh r0, [r5, r3]
	movs r1, 0
	bl sub_80F4824
	ldrh r1, [r5]
	ldr r6, _080F6778 @ =0x000087dc
	adds r0, r4, r6
	movs r6, 0
	strh r1, [r0]
	ldr r1, _080F677C @ =0x0000bc8e
	adds r0, r4, r1
	strb r6, [r0]
	movs r2, 0
	mov r9, r2
	movs r3, 0
	ldrsh r1, [r5, r3]
	lsls r1, 2
	adds r1, r4, r1
	ldr r5, _080F6780 @ =0x0000893d
	adds r0, r1, r5
	ldrb r0, [r0]
	ldr r2, _080F6784 @ =0x0000893e
	adds r1, r2
	ldrb r1, [r1]
	lsls r1, 27
	lsrs r1, 27
	movs r2, 0x52
	movs r3, 0
	bl sub_80F44B0
	adds r7, r0, 0
	ldr r3, _080F6788 @ =0x0000bc8f
	adds r4, r3
	strb r6, [r4]
	movs r5, 0
	mov r12, r5
	mov r10, r8
_080F674E:
	ldr r1, _080F678C @ =gUnknown_083E499C
	mov r6, r12
	lsls r0, r6, 1
	adds r0, r1
	ldrh r0, [r0]
	cmp r0, 0x36
	bgt _080F6790
	cmp r0, 0x32
	blt _080F6790
	movs r0, 0x7
	adds r2, r7, 0
	ands r2, r0
	lsrs r7, 3
	movs r0, 0x4
	b _080F679A
	.align 2, 0
_080F676C: .4byte gPokenavStructPtr
_080F6770: .4byte 0x00008829
_080F6774: .4byte 0x0000876e
_080F6778: .4byte 0x000087dc
_080F677C: .4byte 0x0000bc8e
_080F6780: .4byte 0x0000893d
_080F6784: .4byte 0x0000893e
_080F6788: .4byte 0x0000bc8f
_080F678C: .4byte gUnknown_083E499C
_080F6790:
	movs r0, 0x1
	adds r2, r7, 0
	ands r2, r0
	lsrs r7, 1
	movs r0, 0x1
_080F679A:
	movs r4, 0
	adds r1, r2, 0
	add r0, r9
	mov r8, r0
	cmp r4, r1
	bcs _080F67D4
	mov r3, r10
	ldr r0, [r3]
	ldr r5, _080F6828 @ =0x0000bc8e
	adds r3, r0, r5
	ldr r6, _080F682C @ =0x0000bc4c
	adds r6, r0, r6
	str r6, [sp]
	adds r5, r1, 0
_080F67B6:
	ldrb r0, [r3]
	adds r1, r0, 0x1
	strb r1, [r3]
	lsls r0, 24
	lsrs r0, 24
	ldr r1, [sp]
	adds r0, r1, r0
	mov r6, r9
	adds r1, r6, r4
	strb r1, [r0]
	adds r0, r4, 0x1
	lsls r0, 16
	lsrs r4, r0, 16
	cmp r4, r5
	bcc _080F67B6
_080F67D4:
	cmp r2, 0
	beq _080F67EC
	mov r0, r9
	cmp r0, 0x18
	bls _080F67EC
	mov r2, r10
	ldr r1, [r2]
	ldr r3, _080F6830 @ =0x0000bc8f
	adds r1, r3
	ldrb r0, [r1]
	adds r0, 0x1
	strb r0, [r1]
_080F67EC:
	mov r5, r8
	lsls r0, r5, 24
	lsrs r0, 24
	mov r9, r0
	mov r0, r12
	adds r0, 0x1
	lsls r0, 16
	lsrs r0, 16
	mov r12, r0
	cmp r0, 0x10
	bls _080F674E
	ldr r6, _080F6834 @ =gPokenavStructPtr
	ldr r2, [r6]
	ldr r1, _080F6828 @ =0x0000bc8e
	adds r0, r2, r1
	ldr r3, _080F6830 @ =0x0000bc8f
	adds r1, r2, r3
	ldrb r0, [r0]
	ldrb r1, [r1]
	cmp r0, r1
	beq _080F6840
	ldr r5, _080F6838 @ =0x0000bc90
	adds r0, r2, r5
	movs r1, 0
	strb r1, [r0]
	ldr r6, _080F683C @ =0x0000bc91
	adds r0, r2, r6
	strb r1, [r0]
	b _080F6850
	.align 2, 0
_080F6828: .4byte 0x0000bc8e
_080F682C: .4byte 0x0000bc4c
_080F6830: .4byte 0x0000bc8f
_080F6834: .4byte gPokenavStructPtr
_080F6838: .4byte 0x0000bc90
_080F683C: .4byte 0x0000bc91
_080F6840:
	ldr r0, _080F6888 @ =0x0000bc90
	adds r1, r2, r0
	movs r0, 0
	strb r0, [r1]
	ldr r3, _080F688C @ =0x0000bc91
	adds r1, r2, r3
	movs r0, 0x3
	strb r0, [r1]
_080F6850:
	ldr r5, _080F6890 @ =gPokenavStructPtr
	ldr r1, [r5]
	ldr r6, _080F6894 @ =0x0000bc8e
	adds r0, r1, r6
	ldr r2, _080F6898 @ =0x0000bc8f
	adds r1, r2
	ldrb r0, [r0]
	ldrb r1, [r1]
	subs r0, r1
	lsls r0, 24
	lsrs r2, r0, 24
	movs r3, 0
	mov r12, r3
	adds r3, r5, 0
	ldr r1, _080F689C @ =0x0000bc96
	movs r4, 0x9
_080F6870:
	cmp r2, 0x8
	bls _080F68A0
	ldr r0, [r3]
	adds r0, r1
	add r0, r12
	strb r4, [r0]
	adds r0, r2, 0
	subs r0, 0x9
	lsls r0, 24
	lsrs r2, r0, 24
	b _080F68AA
	.align 2, 0
_080F6888: .4byte 0x0000bc90
_080F688C: .4byte 0x0000bc91
_080F6890: .4byte gPokenavStructPtr
_080F6894: .4byte 0x0000bc8e
_080F6898: .4byte 0x0000bc8f
_080F689C: .4byte 0x0000bc96
_080F68A0:
	ldr r0, [r3]
	adds r0, r1
	add r0, r12
	strb r2, [r0]
	movs r2, 0
_080F68AA:
	mov r0, r12
	adds r0, 0x1
	lsls r0, 16
	lsrs r0, 16
	mov r12, r0
	cmp r0, 0x2
	bls _080F6870
	ldr r5, _080F68DC @ =gPokenavStructPtr
	ldr r0, [r5]
	ldr r6, _080F68E0 @ =0x0000bc96
	adds r1, r0, r6
	add r1, r12
	ldr r2, _080F68E4 @ =0x0000bc8f
	adds r0, r2
	ldrb r0, [r0]
	strb r0, [r1]
	add sp, 0x4
	pop {r3-r5}
	mov r8, r3
	mov r9, r4
	mov r10, r5
	pop {r4-r7}
	pop {r0}
	bx r0
	.align 2, 0
_080F68DC: .4byte gPokenavStructPtr
_080F68E0: .4byte 0x0000bc96
_080F68E4: .4byte 0x0000bc8f
	thumb_func_end sub_80F66E0

	thumb_func_start sub_80F68E8
sub_80F68E8: @ 80F68E8
	push {r4-r7,lr}
	ldr r3, _080F6918 @ =gPokenavStructPtr
	ldr r0, [r3]
	ldr r2, _080F691C @ =0x0000bc90
	adds r1, r0, r2
	ldrb r5, [r1]
	ldr r6, _080F6920 @ =0x0000bc91
	adds r0, r6
	ldrb r4, [r0]
	movs r0, 0x1
	mov r12, r0
	ldr r2, _080F6924 @ =gMain
	ldrh r1, [r2, 0x30]
	movs r0, 0x40
	ands r0, r1
	adds r6, r3, 0
	adds r7, r2, 0
	cmp r0, 0
	beq _080F692E
	lsls r0, r4, 24
	cmp r0, 0
	ble _080F692E
	b _080F69BC
	.align 2, 0
_080F6918: .4byte gPokenavStructPtr
_080F691C: .4byte 0x0000bc90
_080F6920: .4byte 0x0000bc91
_080F6924: .4byte gMain
_080F6928:
	ldr r1, _080F69B0 @ =0x0000bc91
	adds r0, r2, r1
	ldrb r4, [r0]
_080F692E:
	ldrh r1, [r7, 0x30]
	movs r0, 0x80
	ands r0, r1
	cmp r0, 0
	beq _080F6978
	lsls r0, r4, 24
	asrs r0, 24
	cmp r0, 0x2
	bgt _080F6978
	ldr r3, _080F69B4 @ =gPokenavStructPtr
	ldr r2, _080F69B8 @ =0x0000bc96
_080F6944:
	lsls r0, r4, 24
	asrs r0, 24
	cmp r0, 0x2
	bgt _080F6960
	adds r0, 0x1
	lsls r0, 24
	ldr r1, [r3]
	lsrs r4, r0, 24
	asrs r0, 24
	adds r1, r2
	adds r1, r0
	ldrb r0, [r1]
	cmp r0, 0
	beq _080F6944
_080F6960:
	ldr r2, [r6]
	lsls r0, r4, 24
	asrs r0, 24
	ldr r3, _080F69B8 @ =0x0000bc96
	adds r1, r2, r3
	adds r1, r0
	ldrb r0, [r1]
	cmp r0, 0
	bne _080F69EC
	ldr r1, _080F69B0 @ =0x0000bc91
	adds r0, r2, r1
	ldrb r4, [r0]
_080F6978:
	ldrh r2, [r7, 0x30]
	movs r0, 0x20
	ands r0, r2
	cmp r0, 0
	beq _080F698A
	lsls r0, r5, 24
	asrs r0, 24
	cmp r0, 0
	bgt _080F6A3C
_080F698A:
	movs r0, 0x10
	ands r0, r2
	cmp r0, 0
	beq _080F69AA
	lsls r0, r5, 24
	asrs r2, r0, 24
	ldr r1, [r6]
	lsls r0, r4, 24
	asrs r0, 24
	ldr r3, _080F69B8 @ =0x0000bc96
	adds r1, r3
	adds r1, r0
	ldrb r0, [r1]
	subs r0, 0x1
	cmp r2, r0
	blt _080F6A38
_080F69AA:
	movs r0, 0
	mov r12, r0
	b _080F69FC
	.align 2, 0
_080F69B0: .4byte 0x0000bc91
_080F69B4: .4byte gPokenavStructPtr
_080F69B8: .4byte 0x0000bc96
_080F69BC:
	lsls r0, r4, 24
	asrs r0, 24
	cmp r0, 0
	ble _080F69DA
	subs r0, 0x1
	lsls r0, 24
	ldr r1, [r6]
	lsrs r4, r0, 24
	asrs r0, 24
	ldr r2, _080F6A2C @ =0x0000bc96
	adds r1, r2
	adds r1, r0
	ldrb r0, [r1]
	cmp r0, 0
	beq _080F69BC
_080F69DA:
	ldr r2, [r6]
	lsls r0, r4, 24
	asrs r0, 24
	ldr r3, _080F6A2C @ =0x0000bc96
	adds r1, r2, r3
	adds r1, r0
	ldrb r0, [r1]
	cmp r0, 0
	beq _080F6928
_080F69EC:
	lsls r0, r5, 24
	asrs r0, 24
	ldrb r1, [r1]
	cmp r0, r1
	blt _080F69FC
	subs r0, r1, 0x1
_080F69F8:
	lsls r0, 24
	lsrs r5, r0, 24
_080F69FC:
	mov r0, r12
	cmp r0, 0
	beq _080F6A44
	lsls r0, r5, 24
	asrs r0, 24
	ldr r3, [r6]
	ldr r1, _080F6A30 @ =0x0000bc90
	adds r2, r3, r1
	ldrb r6, [r2]
	cmp r0, r6
	bne _080F6A20
	lsls r0, r4, 24
	asrs r0, 24
	ldr r6, _080F6A34 @ =0x0000bc91
	adds r1, r3, r6
	ldrb r1, [r1]
	cmp r0, r1
	beq _080F6A40
_080F6A20:
	strb r5, [r2]
	ldr r1, _080F6A34 @ =0x0000bc91
	adds r0, r3, r1
	strb r4, [r0]
	b _080F6A44
	.align 2, 0
_080F6A2C: .4byte 0x0000bc96
_080F6A30: .4byte 0x0000bc90
_080F6A34: .4byte 0x0000bc91
_080F6A38:
	adds r0, r2, 0x1
	b _080F69F8
_080F6A3C:
	subs r0, 0x1
	b _080F69F8
_080F6A40:
	movs r2, 0
	mov r12, r2
_080F6A44:
	mov r0, r12
	pop {r4-r7}
	pop {r1}
	bx r1
	thumb_func_end sub_80F68E8

	thumb_func_start sub_80F6A4C
sub_80F6A4C: @ 80F6A4C
	push {r4,r5,lr}
	lsls r0, 24
	lsrs r0, 24
	adds r4, r0, 0
	ldr r0, _080F6AC4 @ =gPokenavStructPtr
	ldr r3, [r0]
	ldr r0, _080F6AC8 @ =0x0000876e
	adds r2, r3, r0
	lsls r0, r4, 24
	asrs r0, 24
	ldrh r1, [r2]
	adds r0, r1
	strh r0, [r2]
	lsls r0, 16
	cmp r0, 0
	bge _080F6A74
	ldr r5, _080F6ACC @ =0x00008774
	adds r0, r3, r5
	ldrh r0, [r0]
	strh r0, [r2]
_080F6A74:
	ldr r1, _080F6ACC @ =0x00008774
	adds r0, r3, r1
	movs r5, 0
	ldrsh r1, [r2, r5]
	movs r5, 0
	ldrsh r0, [r0, r5]
	cmp r1, r0
	ble _080F6A88
	movs r0, 0
	strh r0, [r2]
_080F6A88:
	ldr r1, _080F6AD0 @ =0x0000bc94
	adds r0, r3, r1
	strb r4, [r0]
	ldrh r1, [r2]
	ldr r2, _080F6AD4 @ =0x000087dc
	adds r0, r3, r2
	strh r1, [r0]
	ldr r1, _080F6AD8 @ =REG_WININ
	ldr r5, _080F6ADC @ =0x00003f37
	adds r0, r5, 0
	strh r0, [r1]
	adds r1, 0x2
	ldr r2, _080F6AE0 @ =0x00003f3f
	adds r0, r2, 0
	strh r0, [r1]
	subs r1, 0xA
	ldr r5, _080F6AE4 @ =0x000058f0
	adds r0, r5, 0
	strh r0, [r1]
	adds r1, 0x4
	ldr r2, _080F6AE8 @ =0x00002060
	adds r0, r2, 0
	strh r0, [r1]
	ldr r5, _080F6AEC @ =0x000087de
	adds r1, r3, r5
	movs r0, 0
	strb r0, [r1]
	pop {r4,r5}
	pop {r0}
	bx r0
	.align 2, 0
_080F6AC4: .4byte gPokenavStructPtr
_080F6AC8: .4byte 0x0000876e
_080F6ACC: .4byte 0x00008774
_080F6AD0: .4byte 0x0000bc94
_080F6AD4: .4byte 0x000087dc
_080F6AD8: .4byte REG_WININ
_080F6ADC: .4byte 0x00003f37
_080F6AE0: .4byte 0x00003f3f
_080F6AE4: .4byte 0x000058f0
_080F6AE8: .4byte 0x00002060
_080F6AEC: .4byte 0x000087de
	thumb_func_end sub_80F6A4C

	thumb_func_start sub_80F6AF0
sub_80F6AF0: @ 80F6AF0
	push {r4,lr}
	ldr r1, _080F6B10 @ =gPokenavStructPtr
	ldr r0, [r1]
	ldr r2, _080F6B14 @ =0x000087de
	adds r0, r2
	ldrb r0, [r0]
	adds r4, r1, 0
	cmp r0, 0x6
	bls _080F6B04
	b _080F6C08
_080F6B04:
	lsls r0, 2
	ldr r1, _080F6B18 @ =_080F6B1C
	adds r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080F6B10: .4byte gPokenavStructPtr
_080F6B14: .4byte 0x000087de
_080F6B18: .4byte _080F6B1C
	.align 2, 0
_080F6B1C:
	.4byte _080F6B38
	.4byte _080F6B54
	.4byte _080F6B6E
	.4byte _080F6B84
	.4byte _080F6B9C
	.4byte _080F6BC0
	.4byte _080F6BE0
_080F6B38:
	bl sub_80F173C
	lsls r0, 24
	cmp r0, 0
	bne _080F6C18
	ldr r0, _080F6B4C @ =gPokenavStructPtr
	ldr r1, [r0]
	ldr r3, _080F6B50 @ =0x000087de
	adds r1, r3
	b _080F6BCE
	.align 2, 0
_080F6B4C: .4byte gPokenavStructPtr
_080F6B50: .4byte 0x000087de
_080F6B54:
	movs r2, 0x80
	lsls r2, 19
	ldrh r0, [r2]
	movs r3, 0x80
	lsls r3, 6
	adds r1, r3, 0
	orrs r0, r1
	strh r0, [r2]
	bl sub_80F1480
	bl sub_80F66E0
	b _080F6BC6
_080F6B6E:
	bl DrawMonRibbonIcons
	ldr r0, _080F6B7C @ =gPokenavStructPtr
	ldr r1, [r0]
	ldr r2, _080F6B80 @ =0x000087de
	adds r1, r2
	b _080F6BCE
	.align 2, 0
_080F6B7C: .4byte gPokenavStructPtr
_080F6B80: .4byte 0x000087de
_080F6B84:
	bl sub_80F13FC
	ldr r0, _080F6B94 @ =gPokenavStructPtr
	ldr r1, [r0]
	ldr r3, _080F6B98 @ =0x000087de
	adds r1, r3
	b _080F6BCE
	.align 2, 0
_080F6B94: .4byte gPokenavStructPtr
_080F6B98: .4byte 0x000087de
_080F6B9C:
	ldr r4, [r4]
	ldr r1, _080F6BB8 @ =0x0000876e
	adds r0, r4, r1
	movs r2, 0
	ldrsh r0, [r0, r2]
	movs r1, 0
	bl sub_80F4824
	ldr r3, _080F6BBC @ =0x000087de
	adds r4, r3
	ldrb r0, [r4]
	adds r0, 0x1
	strb r0, [r4]
	b _080F6C18
	.align 2, 0
_080F6BB8: .4byte 0x0000876e
_080F6BBC: .4byte 0x000087de
_080F6BC0:
	movs r0, 0
	bl sub_80F2E18
_080F6BC6:
	ldr r0, _080F6BD8 @ =gPokenavStructPtr
	ldr r1, [r0]
	ldr r0, _080F6BDC @ =0x000087de
	adds r1, r0
_080F6BCE:
	ldrb r0, [r1]
	adds r0, 0x1
	strb r0, [r1]
	b _080F6C18
	.align 2, 0
_080F6BD8: .4byte gPokenavStructPtr
_080F6BDC: .4byte 0x000087de
_080F6BE0:
	bl sub_80F170C
	lsls r0, 24
	cmp r0, 0
	bne _080F6C18
	bl sub_80F1438
	movs r2, 0x80
	lsls r2, 19
	ldrh r1, [r2]
	ldr r0, _080F6C0C @ =0x0000dfff
	ands r0, r1
	strh r0, [r2]
	ldr r0, _080F6C10 @ =gPokenavStructPtr
	ldr r1, [r0]
	ldr r2, _080F6C14 @ =0x000087de
	adds r1, r2
	ldrb r0, [r1]
	adds r0, 0x1
	strb r0, [r1]
_080F6C08:
	movs r0, 0
	b _080F6C1A
	.align 2, 0
_080F6C0C: .4byte 0x0000dfff
_080F6C10: .4byte gPokenavStructPtr
_080F6C14: .4byte 0x000087de
_080F6C18:
	movs r0, 0x1
_080F6C1A:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_80F6AF0

	.align 2, 0 @ Don't pad with nop.
