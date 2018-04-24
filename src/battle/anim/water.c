#include "global.h"
#include "battle_anim.h"
#include "rom_8077ABC.h"
#include "trig.h"
#include "util.h"
#include "decompress.h"
#include "palette.h"

asm(".equ REG_BLDCNT, 0x4000050\n"
    "\t.equ REG_BG1CNT, 0x400000A\n"
    "\t.equ REG_BG1HOFS, 0x4000014");

extern s16 gBattleAnimArgs[];
extern u8 gAnimBankAttacker;
extern u8 gAnimBankTarget;
extern u16 gBattle_BG1_X;
extern u16 gBattle_BG1_Y;

extern const u8 gUnknown_08E70968[];
extern const u8 gUnknown_08E70C38[];
extern const u8 gUnknown_08E70F0C[];
extern const u8 gBattleAnimBackgroundImage_Surf[];
extern const u16 gBattleAnimBackgroundPalette_Surf[];
extern const u16 gBattleAnimBackgroundImageMuddyWater_Pal[];

static void sub_80D3874(struct Sprite *);
void sub_80D3B60(u8 taskId);
void sub_80D3D68(u8 taskId);

void sub_80D37FC(struct Sprite *sprite)
{
    sub_8078764(sprite, 1);

    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[2] = sprite->pos1.x + gBattleAnimArgs[2];
    sprite->data[4] = sprite->pos1.y + gBattleAnimArgs[4];

    sprite->callback = StartTranslateAnimSpriteByDeltas;
    StoreSpriteCallbackInData(sprite, DestroyAnimSprite);
}

void sub_80D3838(struct Sprite *sprite)
{
    if (gBattleAnimArgs[3] != 0)
    {
        sub_8078764(sprite, 1);
    }
    else
    {
        InitAnimSpritePos(sprite, 1);
    }

    sprite->data[7] = gBattleAnimArgs[2];
    sprite->callback = sub_80D3874;
}

static void sub_80D3874(struct Sprite *sprite)
{
    sprite->data[0] = (sprite->data[0] + 11) & 0xFF;
    sprite->pos2.x = Sin(sprite->data[0], 4);

    sprite->data[1] += 48;
    sprite->pos2.y = -(sprite->data[1] >> 8);

    if (--sprite->data[7] == -1)
    {
        DestroyAnimSprite(sprite);
    }
}

// The following functions are nonmatching because of unexplained behavior regarding gBattle_BG1_X and gBattle_BG1_Y.  They are therefore on the most wanted list.

#ifdef NONMATCHING
void sub_80D38BC(u8 taskId)
{
    struct UnknownStruct2 sp00;
    u8 taskId2;
    volatile u8 spC;

    REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_OBJ | BLDCNT_TGT2_BD;
    REG_BLDALPHA = BLDALPHA_BLEND(0, 0x10);
    ((volatile struct BgCnt *)&REG_BG1CNT)->priority = 1;
    ((volatile struct BgCnt *)&REG_BG1CNT)->screenSize = 1;
    sub_8078914(&sp00);
    Dma3FillLarge32_(0, sp00.unk0, 0x2000);
    spC = 0;
    spC = 0;
    DmaFill32Defvars(3, 0, sp00.unk4, 0x1000);
    if (!IsContest())
    {
        ((volatile struct BgCnt *)&REG_BG1CNT)->charBaseBlock = 1;
        if (GetBattlerSide(gAnimBankAttacker) == 1)
            LZDecompressVram(gUnknown_08E70968, sp00.unk4);
        else
            LZDecompressVram(gUnknown_08E70C38, sp00.unk4);
    }
    else
    {
        LZDecompressVram(gUnknown_08E70F0C, sp00.unk4);
        sub_80763FC(sp00.unk8, sp00.unk4, 0, 1);
    }
    LZDecompressVram(gBattleAnimBackgroundImage_Surf, sp00.unk0);
    if (gBattleAnimArgs[0] == 0)
        LoadCompressedPalette(gBattleAnimBackgroundPalette_Surf, sp00.unk8 * 16, 32);
    else
        LoadCompressedPalette(gBattleAnimBackgroundImageMuddyWater_Pal, sp00.unk8 * 16, 32);
    taskId2 = CreateTask(sub_80D3D68, gTasks[taskId].priority + 1);
    gTasks[taskId].data[15] = taskId2;
    gTasks[taskId2].data[0] = 0;
    gTasks[taskId2].data[1] = 0x1000;
    gTasks[taskId2].data[2] = 0x1000;
    if (IsContest())
    {
        gBattle_BG1_X = -0x50;
        gBattle_BG1_Y = -0x30;
        gTasks[taskId].data[0] = 2;
        gTasks[taskId].data[1] = 1;
        gTasks[taskId2].data[3] = 0;
    }
    else if (GetBattlerSide(gAnimBankAttacker) == 1)
    {
        gBattle_BG1_X = -0xe0;
        gBattle_BG1_Y = 0x100;
        gTasks[taskId].data[0] = 2;
        gTasks[taskId].data[1] = -1;
        gTasks[taskId2].data[3] = 1;
    }
    else
    {
        gBattle_BG1_X = 0;
        gBattle_BG1_Y = -0x30;
        gTasks[taskId].data[0] = -2;
        gTasks[taskId].data[1] = 1;
        gTasks[taskId2].data[3] = 0;
    }
    REG_BG1HOFS = gBattle_BG1_X;
    REG_BG1VOFS = gBattle_BG1_Y;
    if (gTasks[taskId2].data[3] == 0)
    {
        gTasks[taskId2].data[4] = 0x30;
        gTasks[taskId2].data[5] = 0x70;
    }
    else
    {
        gTasks[taskId2].data[4] = 0;
        gTasks[taskId2].data[5] = 0;
    }
    gTasks[taskId].func = sub_80D3B60;
}
#else
NAKED void sub_80D38BC(u8 taskId)
{
    asm_unified("    push {r4-r7,lr}\n"
                "    mov r7, r10\n"
                "    mov r6, r9\n"
                "    mov r5, r8\n"
                "    push {r5-r7}\n"
                "    sub sp, 0x14\n"
                "    lsls r0, 24\n"
                "    lsrs r0, 24\n"
                "    mov r10, r0\n"
                "    ldr r1, _080D398C @ =REG_BLDCNT\n"
                "    ldr r2, _080D3990 @ =0x00003f42\n"
                "    adds r0, r2, 0\n"
                "    strh r0, [r1]\n"
                "    adds r1, 0x2\n"
                "    movs r3, 0x80\n"
                "    lsls r3, 5\n"
                "    adds r0, r3, 0\n"
                "    strh r0, [r1]\n"
                "    ldr r2, _080D3994 @ =REG_BG1CNT\n"
                "    ldrb r1, [r2]\n"
                "    movs r0, 0x4\n"
                "    negs r0, r0\n"
                "    ands r0, r1\n"
                "    movs r1, 0x1\n"
                "    orrs r0, r1\n"
                "    strb r0, [r2]\n"
                "    ldrb r1, [r2, 0x1]\n"
                "    movs r0, 0x3F\n"
                "    ands r0, r1\n"
                "    movs r1, 0x40\n"
                "    orrs r0, r1\n"
                "    strb r0, [r2, 0x1]\n"
                "    mov r0, sp\n"
                "    bl sub_8078914\n"
                "    ldr r2, [sp]\n"
                "    movs r3, 0x80\n"
                "    lsls r3, 6\n"
                "    add r6, sp, 0x10\n"
                "    add r0, sp, 0xC\n"
                "    mov r12, r0\n"
                "    movs r5, 0\n"
                "    ldr r1, _080D3998 @ =0x040000d4\n"
                "    movs r4, 0x80\n"
                "    lsls r4, 5\n"
                "    mov r8, r6\n"
                "    ldr r7, _080D399C @ =0x85000400\n"
                "    movs r0, 0x85\n"
                "    lsls r0, 24\n"
                "    mov r9, r0\n"
                "_080D3920:\n"
                "    str r5, [sp, 0x10]\n"
                "    mov r0, r8\n"
                "    str r0, [r1]\n"
                "    str r2, [r1, 0x4]\n"
                "    str r7, [r1, 0x8]\n"
                "    ldr r0, [r1, 0x8]\n"
                "    adds r2, r4\n"
                "    subs r3, r4\n"
                "    cmp r3, r4\n"
                "    bhi _080D3920\n"
                "    str r5, [sp, 0x10]\n"
                "    str r6, [r1]\n"
                "    str r2, [r1, 0x4]\n"
                "    lsrs r0, r3, 2\n"
                "    mov r2, r9\n"
                "    orrs r0, r2\n"
                "    str r0, [r1, 0x8]\n"
                "    ldr r0, [r1, 0x8]\n"
                "    movs r0, 0\n"
                "    mov r3, r12\n"
                "    strb r0, [r3]\n"
                "    strb r0, [r3]\n"
                "    ldr r1, [sp, 0x4]\n"
                "    movs r0, 0\n"
                "    str r0, [sp, 0x10]\n"
                "    ldr r0, _080D3998 @ =0x040000d4\n"
                "    str r6, [r0]\n"
                "    str r1, [r0, 0x4]\n"
                "    ldr r1, _080D399C @ =0x85000400\n"
                "    str r1, [r0, 0x8]\n"
                "    ldr r0, [r0, 0x8]\n"
                "    bl IsContest\n"
                "    lsls r0, 24\n"
                "    cmp r0, 0\n"
                "    bne _080D39B8\n"
                "    ldr r2, _080D3994 @ =REG_BG1CNT\n"
                "    ldrb r1, [r2]\n"
                "    movs r0, 0xD\n"
                "    negs r0, r0\n"
                "    ands r0, r1\n"
                "    movs r1, 0x4\n"
                "    orrs r0, r1\n"
                "    strb r0, [r2]\n"
                "    ldr r0, _080D39A0 @ =gAnimBankAttacker\n"
                "    ldrb r0, [r0]\n"
                "    bl GetBattlerSide\n"
                "    lsls r0, 24\n"
                "    lsrs r0, 24\n"
                "    cmp r0, 0x1\n"
                "    bne _080D39A8\n"
                "    ldr r0, _080D39A4 @ =gUnknown_08E70968\n"
                "    b _080D39AA\n"
                "    .align 2, 0\n"
                "_080D398C: .4byte REG_BLDCNT\n"
                "_080D3990: .4byte 0x00003f42\n"
                "_080D3994: .4byte REG_BG1CNT\n"
                "_080D3998: .4byte 0x040000d4\n"
                "_080D399C: .4byte 0x85000400\n"
                "_080D39A0: .4byte gAnimBankAttacker\n"
                "_080D39A4: .4byte gUnknown_08E70968\n"
                "_080D39A8:\n"
                "    ldr r0, _080D39B4 @ =gUnknown_08E70C38\n"
                "_080D39AA:\n"
                "    ldr r1, [sp, 0x4]\n"
                "    bl LZDecompressVram\n"
                "    b _080D39CE\n"
                "    .align 2, 0\n"
                "_080D39B4: .4byte gUnknown_08E70C38\n"
                "_080D39B8:\n"
                "    ldr r0, _080D39F0 @ =gUnknown_08E70F0C\n"
                "    ldr r1, [sp, 0x4]\n"
                "    bl LZDecompressVram\n"
                "    mov r0, sp\n"
                "    ldrb r0, [r0, 0x8]\n"
                "    ldr r1, [sp, 0x4]\n"
                "    movs r2, 0\n"
                "    movs r3, 0x1\n"
                "    bl sub_80763FC\n"
                "_080D39CE:\n"
                "    ldr r0, _080D39F4 @ =gBattleAnimBackgroundImage_Surf\n"
                "    ldr r1, [sp]\n"
                "    bl LZDecompressVram\n"
                "    ldr r0, _080D39F8 @ =gBattleAnimArgs\n"
                "    movs r1, 0\n"
                "    ldrsh r0, [r0, r1]\n"
                "    cmp r0, 0\n"
                "    bne _080D3A00\n"
                "    ldr r0, _080D39FC @ =gBattleAnimBackgroundPalette_Surf\n"
                "    mov r1, sp\n"
                "    ldrb r1, [r1, 0x8]\n"
                "    lsls r1, 4\n"
                "    movs r2, 0x20\n"
                "    bl LoadCompressedPalette\n"
                "    b _080D3A0E\n"
                "    .align 2, 0\n"
                "_080D39F0: .4byte gUnknown_08E70F0C\n"
                "_080D39F4: .4byte gBattleAnimBackgroundImage_Surf\n"
                "_080D39F8: .4byte gBattleAnimArgs\n"
                "_080D39FC: .4byte gBattleAnimBackgroundPalette_Surf\n"
                "_080D3A00:\n"
                "    ldr r0, _080D3A78 @ =gBattleAnimBackgroundImageMuddyWater_Pal\n"
                "    mov r1, sp\n"
                "    ldrb r1, [r1, 0x8]\n"
                "    lsls r1, 4\n"
                "    movs r2, 0x20\n"
                "    bl LoadCompressedPalette\n"
                "_080D3A0E:\n"
                "    ldr r0, _080D3A7C @ =sub_80D3D68\n"
                "    ldr r4, _080D3A80 @ =gTasks\n"
                "    mov r2, r10\n"
                "    lsls r5, r2, 2\n"
                "    adds r1, r5, r2\n"
                "    lsls r1, 3\n"
                "    adds r6, r1, r4\n"
                "    ldrb r1, [r6, 0x7]\n"
                "    adds r1, 0x1\n"
                "    lsls r1, 24\n"
                "    lsrs r1, 24\n"
                "    bl CreateTask\n"
                "    lsls r0, 24\n"
                "    lsrs r0, 24\n"
                "    mov r8, r0\n"
                "    movs r3, 0\n"
                "    mov r9, r3\n"
                "    mov r0, r8\n"
                "    strh r0, [r6, 0x26]\n"
                "    mov r1, r8\n"
                "    lsls r0, r1, 2\n"
                "    add r0, r8\n"
                "    lsls r0, 3\n"
                "    adds r7, r0, r4\n"
                "    mov r2, r9\n"
                "    strh r2, [r7, 0x8]\n"
                "    movs r0, 0x80\n"
                "    lsls r0, 5\n"
                "    strh r0, [r7, 0xA]\n"
                "    strh r0, [r7, 0xC]\n"
                "    bl IsContest\n"
                "    lsls r0, 24\n"
                "    lsrs r4, r0, 24\n"
                "    cmp r4, 0\n"
                "    beq _080D3A94\n"
                "    ldr r3, _080D3A84 @ =0x0000ffb0\n"
                "    adds r0, r3, 0\n"
                "    ldr r1, _080D3A88 @ =gBattle_BG1_X\n"
                "    strh r0, [r1]\n"
                "    ldr r2, _080D3A8C @ =0x0000ffd0\n"
                "    adds r0, r2, 0\n"
                "    ldr r3, _080D3A90 @ =gBattle_BG1_Y\n"
                "    strh r0, [r3]\n"
                "    movs r0, 0x2\n"
                "    strh r0, [r6, 0x8]\n"
                "    movs r0, 0x1\n"
                "    strh r0, [r6, 0xA]\n"
                "    mov r0, r9\n"
                "    strh r0, [r7, 0xE]\n"
                "    b _080D3AEE\n"
                "    .align 2, 0\n"
                "_080D3A78: .4byte gBattleAnimBackgroundImageMuddyWater_Pal\n"
                "_080D3A7C: .4byte sub_80D3D68\n"
                "_080D3A80: .4byte gTasks\n"
                "_080D3A84: .4byte 0x0000ffb0\n"
                "_080D3A88: .4byte gBattle_BG1_X\n"
                "_080D3A8C: .4byte 0x0000ffd0\n"
                "_080D3A90: .4byte gBattle_BG1_Y\n"
                "_080D3A94:\n"
                "    ldr r0, _080D3AC4 @ =gAnimBankAttacker\n"
                "    ldrb r0, [r0]\n"
                "    bl GetBattlerSide\n"
                "    lsls r0, 24\n"
                "    lsrs r1, r0, 24\n"
                "    cmp r1, 0x1\n"
                "    bne _080D3AD8\n"
                "    ldr r2, _080D3AC8 @ =0x0000ff20\n"
                "    adds r0, r2, 0\n"
                "    ldr r3, _080D3ACC @ =gBattle_BG1_X\n"
                "    strh r0, [r3]\n"
                "    movs r2, 0x80\n"
                "    lsls r2, 1\n"
                "    adds r0, r2, 0\n"
                "    ldr r3, _080D3AD0 @ =gBattle_BG1_Y\n"
                "    strh r0, [r3]\n"
                "    movs r0, 0x2\n"
                "    strh r0, [r6, 0x8]\n"
                "    ldr r0, _080D3AD4 @ =0x0000ffff\n"
                "    strh r0, [r6, 0xA]\n"
                "    strh r1, [r7, 0xE]\n"
                "    b _080D3AEE\n"
                "    .align 2, 0\n"
                "_080D3AC4: .4byte gAnimBankAttacker\n"
                "_080D3AC8: .4byte 0x0000ff20\n"
                "_080D3ACC: .4byte gBattle_BG1_X\n"
                "_080D3AD0: .4byte gBattle_BG1_Y\n"
                "_080D3AD4: .4byte 0x0000ffff\n"
                "_080D3AD8:\n"
                "    ldr r0, _080D3B1C @ =gBattle_BG1_X\n"
                "    strh r4, [r0]\n"
                "    ldr r1, _080D3B20 @ =0x0000ffd0\n"
                "    adds r0, r1, 0\n"
                "    ldr r2, _080D3B24 @ =gBattle_BG1_Y\n"
                "    strh r0, [r2]\n"
                "    ldr r0, _080D3B28 @ =0x0000fffe\n"
                "    strh r0, [r6, 0x8]\n"
                "    movs r0, 0x1\n"
                "    strh r0, [r6, 0xA]\n"
                "    strh r4, [r7, 0xE]\n"
                "_080D3AEE:\n"
                "    ldr r1, _080D3B2C @ =REG_BG1HOFS\n"
                "    ldr r3, _080D3B1C @ =gBattle_BG1_X\n"
                "    ldrh r0, [r3]\n"
                "    strh r0, [r1]\n"
                "    adds r1, 0x2\n"
                "    ldr r2, _080D3B24 @ =gBattle_BG1_Y\n"
                "    ldrh r0, [r2]\n"
                "    strh r0, [r1]\n"
                "    ldr r1, _080D3B30 @ =gTasks\n"
                "    mov r3, r8\n"
                "    lsls r0, r3, 2\n"
                "    add r0, r8\n"
                "    lsls r0, 3\n"
                "    adds r2, r0, r1\n"
                "    movs r3, 0xE\n"
                "    ldrsh r0, [r2, r3]\n"
                "    cmp r0, 0\n"
                "    bne _080D3B34\n"
                "    movs r0, 0x30\n"
                "    strh r0, [r2, 0x10]\n"
                "    movs r0, 0x70\n"
                "    b _080D3B38\n"
                "    .align 2, 0\n"
                "_080D3B1C: .4byte gBattle_BG1_X\n"
                "_080D3B20: .4byte 0x0000ffd0\n"
                "_080D3B24: .4byte gBattle_BG1_Y\n"
                "_080D3B28: .4byte 0x0000fffe\n"
                "_080D3B2C: .4byte REG_BG1HOFS\n"
                "_080D3B30: .4byte gTasks\n"
                "_080D3B34:\n"
                "    movs r0, 0\n"
                "    strh r0, [r2, 0x10]\n"
                "_080D3B38:\n"
                "    strh r0, [r2, 0x12]\n"
                "    mov r2, r10\n"
                "    adds r0, r5, r2\n"
                "    lsls r0, 3\n"
                "    adds r0, r1\n"
                "    movs r1, 0x1\n"
                "    strh r1, [r0, 0x14]\n"
                "    ldr r1, _080D3B5C @ =sub_80D3B60\n"
                "    str r1, [r0]\n"
                "    add sp, 0x14\n"
                "    pop {r3-r5}\n"
                "    mov r8, r3\n"
                "    mov r9, r4\n"
                "    mov r10, r5\n"
                "    pop {r4-r7}\n"
                "    pop {r0}\n"
                "    bx r0\n"
                "    .align 2, 0\n"
                "_080D3B5C: .4byte sub_80D3B60");
}
#endif // NONMATCHING

#ifdef NONMATCHING
void sub_80D3B60(u8 taskId)
{
    vu8 sp0;
    struct UnknownStruct2 substruct;
    u8 i;
    u16 tmp;

    // Desired output: *gBattle_BG1_X is loaded into r1, then gBattle_BG1_X is loaded into r2 almost immediately after.  Similar with gBattle_BG1_Y in the next line.
    gBattle_BG1_X += gTasks[taskId].data[0];
    gBattle_BG1_Y += gTasks[taskId].data[1];
    sub_8078914(&substruct);
    gTasks[taskId].data[2] += gTasks[taskId].data[1];
    gTasks[taskId].data[5]++;
    if (gTasks[taskId].data[5] == 4)
    {
        tmp = gPlttBufferFaded[16 * substruct.unk8 + 7];
        for (i = 6; i > 0; i--)
        {
            gPlttBufferFaded[((16 * substruct.unk8 + 1) + i)] = gPlttBufferFaded[((16 * substruct.unk8 + 1) + i) - 1];
        }
        gPlttBufferFaded[16 * substruct.unk8 + 1] = tmp;
        gTasks[taskId].data[5] = 0;
    }
    gTasks[taskId].data[6]++;
    if (gTasks[taskId].data[6] > 1)
    {
        gTasks[taskId].data[6] = 0;
        gTasks[taskId].data[3]++;
        if (gTasks[taskId].data[3] < 14)
        {
            gTasks[gTasks[taskId].data[15]].data[1] = ((16 - gTasks[taskId].data[3]) << 8) | gTasks[taskId].data[3];
            gTasks[taskId].data[4]++;
        }
        if (gTasks[taskId].data[3] > 54)
        {
            gTasks[taskId].data[4]--;
            gTasks[gTasks[taskId].data[15]].data[1] = ((16 - gTasks[taskId].data[4]) << 8) | gTasks[taskId].data[4];
        }
    }
    if ((gTasks[gTasks[taskId].data[15]].data[1] & 0x1F) == 0)
    {
        DmaFill32Large(3, 0, substruct.unk0, 0x10000, 0x1000);
        sp0 = 0;
        sp0 = 0;
        DmaFill32Defvars(3, 0, substruct.unk4, 0x1000);
        if (!IsContest())
        {
            ((volatile struct BgCnt *)REG_ADDR_BG1CNT)->charBaseBlock = 0;
        }
        gBattle_BG1_X = 0;
        gBattle_BG1_Y = 0;
        REG_BLDCNT = 0;
        REG_BLDALPHA = 0;
        gTasks[gTasks[taskId].data[15]].data[1] = -1;
        DestroyAnimVisualTask(taskId);
    }
}
#else
NAKED void sub_80D3B60(u8 taskId)
{
    asm_unified("\tpush {r4-r7,lr}\n"
                "\tmov r7, r10\n"
                "\tmov r6, r9\n"
                "\tmov r5, r8\n"
                "\tpush {r5-r7}\n"
                "\tsub sp, 0x18\n"
                "\tlsls r0, 24\n"
                "\tlsrs r7, r0, 24\n"
                "\tldr r0, _080D3D44 @ =gTasks\n"
                "\tlsls r4, r7, 2\n"
                "\tadds r4, r7\n"
                "\tlsls r4, 3\n"
                "\tadds r4, r0\n"
                "\tldrh r0, [r4, 0x8]\n"
                "\tldr r1, _080D3D48 @ =gBattle_BG1_X\n"
                "\tldrh r1, [r1]\n"
                "\tadds r0, r1\n"
                "\tldr r2, _080D3D48 @ =gBattle_BG1_X\n"
                "\tstrh r0, [r2]\n"
                "\tldrh r0, [r4, 0xA]\n"
                "\tldr r3, _080D3D4C @ =gBattle_BG1_Y\n"
                "\tldrh r3, [r3]\n"
                "\tadds r0, r3\n"
                "\tldr r1, _080D3D4C @ =gBattle_BG1_Y\n"
                "\tstrh r0, [r1]\n"
                "\tadd r5, sp, 0x4\n"
                "\tadds r0, r5, 0\n"
                "\tbl sub_8078914\n"
                "\tldrh r0, [r4, 0xA]\n"
                "\tldrh r2, [r4, 0xC]\n"
                "\tadds r0, r2\n"
                "\tstrh r0, [r4, 0xC]\n"
                "\tldrh r0, [r4, 0x12]\n"
                "\tadds r0, 0x1\n"
                "\tstrh r0, [r4, 0x12]\n"
                "\tlsls r0, 16\n"
                "\tasrs r0, 16\n"
                "\tmov r10, r5\n"
                "\tcmp r0, 0x4\n"
                "\tbne _080D3C02\n"
                "\tldr r1, _080D3D50 @ =gPlttBufferFaded\n"
                "\tldrb r0, [r5, 0x8]\n"
                "\tlsls r0, 4\n"
                "\tadds r0, 0x7\n"
                "\tlsls r0, 1\n"
                "\tadds r0, r1\n"
                "\tldrh r6, [r0]\n"
                "\tmovs r2, 0x6\n"
                "\tadds r5, r1, 0\n"
                "\tadds r3, r5, 0\n"
                "\tmov r4, r10\n"
                "_080D3BC8:\n"
                "\tldrb r0, [r4, 0x8]\n"
                "\tlsls r0, 4\n"
                "\tadds r1, r2, 0x1\n"
                "\tadds r0, r1\n"
                "\tlsls r1, r0, 1\n"
                "\tadds r1, r3\n"
                "\tsubs r0, 0x1\n"
                "\tlsls r0, 1\n"
                "\tadds r0, r3\n"
                "\tldrh r0, [r0]\n"
                "\tstrh r0, [r1]\n"
                "\tsubs r0, r2, 0x1\n"
                "\tlsls r0, 24\n"
                "\tlsrs r2, r0, 24\n"
                "\tcmp r2, 0\n"
                "\tbne _080D3BC8\n"
                "\tmov r3, r10\n"
                "\tldrb r0, [r3, 0x8]\n"
                "\tlsls r0, 4\n"
                "\tadds r0, 0x1\n"
                "\tlsls r0, 1\n"
                "\tadds r0, r5\n"
                "\tstrh r6, [r0]\n"
                "\tldr r1, _080D3D44 @ =gTasks\n"
                "\tlsls r0, r7, 2\n"
                "\tadds r0, r7\n"
                "\tlsls r0, 3\n"
                "\tadds r0, r1\n"
                "\tstrh r2, [r0, 0x12]\n"
                "_080D3C02:\n"
                "\tldr r1, _080D3D44 @ =gTasks\n"
                "\tlsls r2, r7, 2\n"
                "\tadds r0, r2, r7\n"
                "\tlsls r0, 3\n"
                "\tadds r3, r0, r1\n"
                "\tldrh r0, [r3, 0x14]\n"
                "\tadds r0, 0x1\n"
                "\tstrh r0, [r3, 0x14]\n"
                "\tlsls r0, 16\n"
                "\tasrs r0, 16\n"
                "\tadds r4, r1, 0\n"
                "\tstr r2, [sp, 0x14]\n"
                "\tcmp r0, 0x1\n"
                "\tble _080D3C70\n"
                "\tmovs r0, 0\n"
                "\tstrh r0, [r3, 0x14]\n"
                "\tldrh r0, [r3, 0xE]\n"
                "\tadds r2, r0, 0x1\n"
                "\tstrh r2, [r3, 0xE]\n"
                "\tlsls r0, r2, 16\n"
                "\tasrs r0, 16\n"
                "\tcmp r0, 0xD\n"
                "\tbgt _080D3C4C\n"
                "\tmovs r1, 0x26\n"
                "\tldrsh r0, [r3, r1]\n"
                "\tlsls r1, r0, 2\n"
                "\tadds r1, r0\n"
                "\tlsls r1, 3\n"
                "\tadds r1, r4\n"
                "\tmovs r0, 0x10\n"
                "\tsubs r0, r2\n"
                "\tlsls r0, 8\n"
                "\torrs r2, r0\n"
                "\tstrh r2, [r1, 0xA]\n"
                "\tldrh r0, [r3, 0x10]\n"
                "\tadds r0, 0x1\n"
                "\tstrh r0, [r3, 0x10]\n"
                "_080D3C4C:\n"
                "\tmovs r2, 0xE\n"
                "\tldrsh r0, [r3, r2]\n"
                "\tcmp r0, 0x36\n"
                "\tble _080D3C70\n"
                "\tldrh r2, [r3, 0x10]\n"
                "\tsubs r2, 0x1\n"
                "\tstrh r2, [r3, 0x10]\n"
                "\tmovs r1, 0x26\n"
                "\tldrsh r0, [r3, r1]\n"
                "\tlsls r1, r0, 2\n"
                "\tadds r1, r0\n"
                "\tlsls r1, 3\n"
                "\tadds r1, r4\n"
                "\tmovs r0, 0x10\n"
                "\tsubs r0, r2\n"
                "\tlsls r0, 8\n"
                "\torrs r2, r0\n"
                "\tstrh r2, [r1, 0xA]\n"
                "_080D3C70:\n"
                "\tldr r2, [sp, 0x14]\n"
                "\tadds r0, r2, r7\n"
                "\tlsls r0, 3\n"
                "\tadds r0, r4\n"
                "\tmovs r3, 0x26\n"
                "\tldrsh r1, [r0, r3]\n"
                "\tlsls r0, r1, 2\n"
                "\tadds r0, r1\n"
                "\tlsls r0, 3\n"
                "\tadds r0, r4\n"
                "\tldrh r1, [r0, 0xA]\n"
                "\tmovs r0, 0x1F\n"
                "\tands r0, r1\n"
                "\tcmp r0, 0\n"
                "\tbne _080D3D32\n"
                "\tldr r2, [sp, 0x4]\n"
                "\tmovs r3, 0x80\n"
                "\tlsls r3, 6\n"
                "\tadd r6, sp, 0x10\n"
                "\tmovs r5, 0\n"
                "\tldr r1, _080D3D54 @ =0x040000d4\n"
                "\tmovs r4, 0x80\n"
                "\tlsls r4, 5\n"
                "\tmov r8, r6\n"
                "\tldr r0, _080D3D58 @ =0x85000400\n"
                "\tmov r12, r0\n"
                "\tmovs r0, 0x85\n"
                "\tlsls r0, 24\n"
                "\tmov r9, r0\n"
                "_080D3CAA:\n"
                "\tstr r5, [sp, 0x10]\n"
                "\tmov r0, r8\n"
                "\tstr r0, [r1]\n"
                "\tstr r2, [r1, 0x4]\n"
                "\tmov r0, r12\n"
                "\tstr r0, [r1, 0x8]\n"
                "\tldr r0, [r1, 0x8]\n"
                "\tadds r2, r4\n"
                "\tsubs r3, r4\n"
                "\tcmp r3, r4\n"
                "\tbhi _080D3CAA\n"
                "\tstr r5, [sp, 0x10]\n"
                "\tstr r6, [r1]\n"
                "\tstr r2, [r1, 0x4]\n"
                "\tlsrs r0, r3, 2\n"
                "\tmov r2, r9\n"
                "\torrs r0, r2\n"
                "\tstr r0, [r1, 0x8]\n"
                "\tldr r0, [r1, 0x8]\n"
                "\tmov r0, sp\n"
                "\tmovs r1, 0\n"
                "\tstrb r1, [r0]\n"
                "\tstrb r1, [r0]\n"
                "\tmov r3, r10\n"
                "\tldr r1, [r3, 0x4]\n"
                "\tmovs r4, 0\n"
                "\tstr r4, [sp, 0x10]\n"
                "\tldr r0, _080D3D54 @ =0x040000d4\n"
                "\tstr r6, [r0]\n"
                "\tstr r1, [r0, 0x4]\n"
                "\tldr r1, _080D3D58 @ =0x85000400\n"
                "\tstr r1, [r0, 0x8]\n"
                "\tldr r0, [r0, 0x8]\n"
                "\tbl IsContest\n"
                "\tlsls r0, 24\n"
                "\tcmp r0, 0\n"
                "\tbne _080D3D02\n"
                "\tldr r2, _080D3D5C @ =REG_BG1CNT\n"
                "\tldrb r1, [r2]\n"
                "\tmovs r0, 0xD\n"
                "\tnegs r0, r0\n"
                "\tands r0, r1\n"
                "\tstrb r0, [r2]\n"
                "_080D3D02:\n"
                "\tldr r0, _080D3D48 @ =gBattle_BG1_X\n"
                "\tstrh r4, [r0]\n"
                "\tldr r1, _080D3D4C @ =gBattle_BG1_Y\n"
                "\tstrh r4, [r1]\n"
                "\tldr r0, _080D3D60 @ =REG_BLDCNT\n"
                "\tstrh r4, [r0]\n"
                "\tadds r0, 0x2\n"
                "\tstrh r4, [r0]\n"
                "\tldr r2, _080D3D44 @ =gTasks\n"
                "\tldr r3, [sp, 0x14]\n"
                "\tadds r0, r3, r7\n"
                "\tlsls r0, 3\n"
                "\tadds r0, r2\n"
                "\tmovs r3, 0x26\n"
                "\tldrsh r1, [r0, r3]\n"
                "\tlsls r0, r1, 2\n"
                "\tadds r0, r1\n"
                "\tlsls r0, 3\n"
                "\tadds r0, r2\n"
                "\tldr r1, _080D3D64 @ =0x0000ffff\n"
                "\tstrh r1, [r0, 0x26]\n"
                "\tadds r0, r7, 0\n"
                "\tbl DestroyAnimVisualTask\n"
                "_080D3D32:\n"
                "\tadd sp, 0x18\n"
                "\tpop {r3-r5}\n"
                "\tmov r8, r3\n"
                "\tmov r9, r4\n"
                "\tmov r10, r5\n"
                "\tpop {r4-r7}\n"
                "\tpop {r0}\n"
                "\tbx r0\n"
                "\t.align 2, 0\n"
                "_080D3D44: .4byte gTasks\n"
                "_080D3D48: .4byte gBattle_BG1_X\n"
                "_080D3D4C: .4byte gBattle_BG1_Y\n"
                "_080D3D50: .4byte gPlttBufferFaded\n"
                "_080D3D54: .4byte 0x040000d4\n"
                "_080D3D58: .4byte 0x85000400\n"
                "_080D3D5C: .4byte REG_BG1CNT\n"
                "_080D3D60: .4byte REG_BLDCNT\n"
                "_080D3D64: .4byte 0x0000ffff");
}
#endif // NONMATCHING
