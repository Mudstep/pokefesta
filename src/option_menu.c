#include "gba/gba.h"
#include "main.h"
#include "menu.h"
#include "palette.h"
#include "sprite.h"
#include "task.h"

extern void SetPokemonCryStereo(u32 val);
extern void remove_some_task(void);

//Task data
enum {
    TD_MENUSELECTION,
    TD_TEXTSPEED,
    TD_BATTLESCENE,
    TD_BATTLESTYLE,
    TD_SOUND,
    TD_BUTTONMODE,
    TD_FRAMETYPE,
};

//Menu items
enum {
    MENUITEM_TEXTSPEED,
    MENUITEM_BATTLESCENE,
    MENUITEM_BATTLESTYLE,
    MENUITEM_SOUND,
    MENUITEM_BUTTONMODE,
    MENUITEM_FRAMETYPE,
    MENUITEM_CANCEL,
};

extern u8 gUnknown_0839F63C[];
extern u8 gUnknown_0839F5FC[];  //palette
extern u8 gSystemText_Normal[];
extern u8 gSystemText_LR[];
extern u8 gSystemText_LA[];
extern u8 gSystemText_Terminator[];
extern u8 gSystemText_Type[];
extern u8 gSystemText_Mono[];
extern u8 gSystemText_Stereo[];
extern u8 gSystemText_Shift[];
extern u8 gSystemText_Set[];
extern u8 gSystemText_On[];
extern u8 gSystemText_Off[];
extern u8 gSystemText_Slow[];
extern u8 gSystemText_Mid[];
extern u8 gSystemText_Fast[];
extern u8 gSystemText_OptionMenu[];
extern u8 gSystemText_TextSpeed[];
extern u8 gSystemText_BattleScene[];
extern u8 gSystemText_BattleStyle[];
extern u8 gSystemText_Sound[];
extern u8 gSystemText_ButtonMode[];
extern u8 gSystemText_Frame[];
extern u8 gSystemText_Cancel[];

static void Task_OptionMenuFadeIn(u8 taskId);
static void Task_OptionMenuProcessInput(u8 taskId);
static void Task_OptionMenuSave(u8 taskId);
static void Task_OptionMenuFadeOut(u8 taskId);
static void HighlightOptionMenuItem(u8 selection);
static u8   TextSpeed_ProcessInput(u8 selection);
static void TextSpeed_DrawChoices(u8 selection);
static u8   BattleScene_ProcessInput(u8 selection);
static void BattleScene_DrawChoices(u8 selection);
static u8   BattleStyle_ProcessInput(u8 selection);
static void BattleStyle_DrawChoices(u8 selection);
static u8   Sound_ProcessInput(u8 selection);
static void Sound_DrawChoices(u8 selection);
static u8   FrameType_ProcessInput(u8 selection);
static void FrameType_DrawChoices(u8 selection);
static u8   ButtonMode_ProcessInput(u8 selection);
static void ButtonMode_DrawChoices(u8 selection);

static void MainCB(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void CB2_InitOptionMenu(void)
{
    switch(gMain.state)
    {
        default:
        case 0:
        {
            u8 *addr;
            u32 size;
            
            SetVBlankCallback(NULL);
            REG_DISPCNT = 0;
            REG_BG2CNT = 0;
            REG_BG1CNT = 0;
            REG_BG0CNT = 0;
            REG_BG2HOFS = 0;
            REG_BG2VOFS = 0;
            REG_BG1HOFS = 0;
            REG_BG1VOFS = 0;
            REG_BG0HOFS = 0;
            REG_BG0VOFS = 0;
            addr = (u8 *)VRAM;
            size = 0x18000;
            while(1)
            {
                DmaFill16(3, 0, addr, 0x1000);
                addr += 0x1000;
                size -= 0x1000;
                if(size <= 0x1000)
                {
                    DmaFill16(3, 0, addr, size);
                    break;
                }
            }
            DmaClear32(3, OAM, OAM_SIZE);
            DmaClear16(3, PLTT, PLTT_SIZE);
            gMain.state++;
            break;
        }
        case 1:
            ResetPaletteFade();
            remove_some_task();
            ResetTasks();
            ResetSpriteData();
            gMain.state++;
            break;
        case 2:
            SetUpWindowConfig(&gWindowConfig_81E71B4);
            gMain.state++;
            break;
        case 3:
            MultistepInitMenuWindowBegin(&gWindowConfig_81E71B4);
            gMain.state++;
            break;
        case 4:
            if(!MultistepInitMenuWindowContinue())
                return;
            gMain.state++;
            break;
        case 5:
            LoadPalette(gUnknown_0839F5FC, 0x80, 0x40);
            CpuCopy16(gUnknown_0839F63C, (void *)0x0600BEE0, 0x40);
            gMain.state++;
            break;
        case 6:
            BeginNormalPaletteFade(-1, 0, 0x10, 0, 0);
            gMain.state++;
            break;
        case 7:
        {
            u16 savedIme;
            
            REG_WIN0H = 0;
            REG_WIN0V = 0;
            REG_WIN1H = 0;
            REG_WIN1V = 0;
            REG_WININ = 0x1111;
            REG_WINOUT = 0x31;
            REG_BLDCNT = 0xE1;
            REG_BLDALPHA = 0;
            REG_BLDY = 7;
            savedIme = REG_IME;
            REG_IME = 0;
            REG_IE |= INTR_FLAG_VBLANK;
            REG_IME = savedIme;
            REG_DISPSTAT |= DISPSTAT_VBLANK_INTR;
            SetVBlankCallback(VBlankCB);
            REG_DISPCNT = DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_OBJ_ON |
              DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
            gMain.state++;
            break;
        }
        case 8:
        {
            u8 taskId = CreateTask(Task_OptionMenuFadeIn, 0);
            
            gTasks[taskId].data[TD_MENUSELECTION] = 0;
            gTasks[taskId].data[TD_TEXTSPEED] = gSaveBlock2.optionsTextSpeed;
            gTasks[taskId].data[TD_BATTLESCENE] = gSaveBlock2.optionsBattleSceneOff;
            gTasks[taskId].data[TD_BATTLESTYLE] = gSaveBlock2.optionsBattleStyle;
            gTasks[taskId].data[TD_SOUND] = gSaveBlock2.optionsSound;
            gTasks[taskId].data[TD_BUTTONMODE] = gSaveBlock2.optionsButtonMode;
            gTasks[taskId].data[TD_FRAMETYPE] = gSaveBlock2.optionsWindowFrameType;
            
            MenuDrawTextWindow(2, 0, 27, 3);
            MenuDrawTextWindow(2, 4, 27, 19);
            
            MenuPrint(gSystemText_OptionMenu, 4, 1);
            MenuPrint(gSystemText_TextSpeed, 4, 5);
            MenuPrint(gSystemText_BattleScene, 4, 7);
            MenuPrint(gSystemText_BattleStyle, 4, 9);
            MenuPrint(gSystemText_Sound, 4, 11);
            MenuPrint(gSystemText_ButtonMode, 4, 13);
            MenuPrint(gSystemText_Frame, 4, 15);
            MenuPrint(gSystemText_Cancel, 4, 17);
            
            TextSpeed_DrawChoices(gTasks[taskId].data[TD_TEXTSPEED]);
            BattleScene_DrawChoices(gTasks[taskId].data[TD_BATTLESCENE]);
            BattleStyle_DrawChoices(gTasks[taskId].data[TD_BATTLESTYLE]);
            Sound_DrawChoices(gTasks[taskId].data[TD_SOUND]);
            ButtonMode_DrawChoices(gTasks[taskId].data[TD_BUTTONMODE]);
            FrameType_DrawChoices(gTasks[taskId].data[TD_FRAMETYPE]);
            
            REG_WIN0H = WIN_RANGE(17, 223);
            REG_WIN0V = WIN_RANGE(1, 31);
            
            HighlightOptionMenuItem(gTasks[taskId].data[TD_MENUSELECTION]);
            gMain.state++;
            break;
        }
        case 9:
            SetMainCallback2(MainCB);
            return;
    }
}

static void Task_OptionMenuFadeIn(u8 taskId)
{
    if(!gPaletteFade.active)
    {
        gTasks[taskId].func = Task_OptionMenuProcessInput;
    }
}

static void Task_OptionMenuProcessInput(u8 taskId)
{
    if(gMain.newKeys & A_BUTTON)
    {
        if(gTasks[taskId].data[TD_MENUSELECTION] == MENUITEM_CANCEL)
            gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if(gMain.newKeys & B_BUTTON)
    {
        gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if(gMain.newKeys & DPAD_UP)
    {
        if(gTasks[taskId].data[TD_MENUSELECTION] > 0)
            gTasks[taskId].data[TD_MENUSELECTION]--;
        else
            gTasks[taskId].data[TD_MENUSELECTION] = 6;
        HighlightOptionMenuItem(gTasks[taskId].data[TD_MENUSELECTION]);
    }
    else if(gMain.newKeys & DPAD_DOWN)
    {
        if(gTasks[taskId].data[TD_MENUSELECTION] <= 5)
            gTasks[taskId].data[TD_MENUSELECTION]++;
        else
            gTasks[taskId].data[TD_MENUSELECTION] = 0;
        HighlightOptionMenuItem(gTasks[taskId].data[TD_MENUSELECTION]);
    }
    else
    {
        switch(gTasks[taskId].data[TD_MENUSELECTION])
        {
            case 0:
                gTasks[taskId].data[TD_TEXTSPEED] = TextSpeed_ProcessInput(gTasks[taskId].data[TD_TEXTSPEED]);
                TextSpeed_DrawChoices(gTasks[taskId].data[TD_TEXTSPEED]);
                break;
            case 1:
                gTasks[taskId].data[TD_BATTLESCENE] = BattleScene_ProcessInput(gTasks[taskId].data[TD_BATTLESCENE]);
                BattleScene_DrawChoices(gTasks[taskId].data[TD_BATTLESCENE]);
                break;
            case 2:
                gTasks[taskId].data[TD_BATTLESTYLE] = BattleStyle_ProcessInput(gTasks[taskId].data[TD_BATTLESTYLE]);
                BattleStyle_DrawChoices(gTasks[taskId].data[TD_BATTLESTYLE]);
                break;
            case 3:
                gTasks[taskId].data[TD_SOUND] = Sound_ProcessInput(gTasks[taskId].data[TD_SOUND]);
                Sound_DrawChoices(gTasks[taskId].data[TD_SOUND]);
                break;
            case 4:
                gTasks[taskId].data[TD_BUTTONMODE] = ButtonMode_ProcessInput(gTasks[taskId].data[TD_BUTTONMODE]);
                ButtonMode_DrawChoices(gTasks[taskId].data[TD_BUTTONMODE]);
                break;
            case 5:
                gTasks[taskId].data[TD_FRAMETYPE] = FrameType_ProcessInput(gTasks[taskId].data[TD_FRAMETYPE]);
                FrameType_DrawChoices(gTasks[taskId].data[TD_FRAMETYPE]);
                break;
        }
    }
}

#ifdef NONMATCHING
static void Task_OptionMenuSave(u8 taskId)
{
    gSaveBlock2.optionsTextSpeed = gTasks[taskId].data[TD_TEXTSPEED];
    gSaveBlock2.optionsBattleSceneOff = gTasks[taskId].data[TD_BATTLESCENE];
    gSaveBlock2.optionsBattleStyle = gTasks[taskId].data[TD_BATTLESTYLE];
    gSaveBlock2.optionsSound = gTasks[taskId].data[TD_SOUND];
    gSaveBlock2.optionsButtonMode = gTasks[taskId].data[TD_BUTTONMODE];
    gSaveBlock2.optionsWindowFrameType = gTasks[taskId].data[TD_FRAMETYPE];
    
    BeginNormalPaletteFade(-1, 0, 0, 0x10, 0);
    gTasks[taskId].func = Task_OptionMenuFadeOut;
}
#else
__attribute__((naked))
static void Task_OptionMenuSave(u8 taskId)
{
    asm("push {r4-r6,lr}            \n\
    sub sp, #4                      \n\
    lsl r0, #24                     \n\
    lsr r0, #24                     \n\
    ldr r5, =gSaveBlock2            \n\
    ldr r1, =gTasks                 \n\
    lsl r4, r0, #2                  \n\
    add r4, r0                      \n\
    lsl r4, #3                      \n\
    add r4, r1                      \n\
    mov r1, #7                      \n\
    ldrb r0, [r4, #0xA]             \n\
    and r0, r1                      \n\
    ldrb r1, [r5, #0x14]            \n\
    mov r3, #8                      \n\
    neg r3, r3                      \n\
    and r3, r1                      \n\
    orr r3, r0                      \n\
    strb r3, [r5, #0x14]            \n\
    mov r6, #1                      \n\
    ldrb r0, [r4, #0xC]             \n\
    and r0, r6                      \n\
    lsl r0, #2                      \n\
    ldrb r1, [r5, #0x15]            \n\
    mov r2, #5                      \n\
    neg r2, r2                      \n\
    and r2, r1                      \n\
    orr r2, r0                      \n\
    strb r2, [r5, #0x15]            \n\
    ldrb r1, [r4, #0xE]             \n\
    and r1, r6                      \n\
    lsl r1, #1                      \n\
    mov r0, #3                      \n\
    neg r0, r0                      \n\
    and r0, r2                      \n\
    orr r0, r1                      \n\
    strb r0, [r5, #0x15]            \n\
    ldrb r1, [r4, #0x10]            \n\
    and r1, r6                      \n\
    mov r2, #2                      \n\
    neg r2, r2                      \n\
    and r0, r2                      \n\
    orr r0, r1                      \n\
    strb r0, [r5, #0x15]            \n\
    ldrh r0, [r4, #0x12]            \n\
    mov r2, #0                      \n\
    strb r0, [r5, #0x13]            \n\
    ldrb r0, [r4, #0x14]            \n\
    lsl r0, #3                      \n\
    mov r1, #7                      \n\
    and r3, r1                      \n\
    orr r3, r0                      \n\
    strb r3, [r5, #0x14]            \n\
    mov r0, #1                      \n\
    neg r0, r0                      \n\
    str r2, [sp]                    \n\
    mov r1, #0                      \n\
    mov r3, #16                     \n\
    bl BeginNormalPaletteFade       \n\
    ldr r0, =Task_OptionMenuFadeOut \n\
    str r0, [r4]                    \n\
    add sp, #4                      \n\
    pop {r4-r6}                     \n\
    pop {r0}                        \n\
    bx r0                           \n\
    .pool\n");
}
#endif

static void Task_OptionMenuFadeOut(u8 taskId)
{
    if(!gPaletteFade.active)
    {
        DestroyTask(taskId);
        SetMainCallback2(gMain.field_8);
    }
}

//This version uses addition '+' instead of OR '|'.
#define WIN_RANGE_(a, b) (((a) << 8) + (b))

static void HighlightOptionMenuItem(u8 index)
{
    REG_WIN1H = WIN_RANGE(24, 215);
    REG_WIN1V = WIN_RANGE_(index * 16 + 40, index * 16 + 56);
}

static void DrawOptionMenuChoice(u8 *text, u8 x, u8 y, u8 style)
{
    u8 dst[16];
    u16 i;
    
    for(i = 0; *text != EOS && i <= 14; i++)
        dst[i] = *(text++);
    
    dst[2] = style;
    dst[i] = EOS;
    sub_80729D8(dst, x, y, 1);
}

static u8 TextSpeed_ProcessInput(u8 selection)
{
    if(gMain.newKeys & DPAD_RIGHT)
    {
        if(selection <= 1)
            selection++;
        else
            selection = 0;
    }
    if(gMain.newKeys & DPAD_LEFT)
    {
        if(selection != 0)
            selection--;
        else
            selection = 2;
    }
    return selection;
}

static void TextSpeed_DrawChoices(u8 selection)
{
    u8 styles[3];
    
    styles[0] = 0xF;
    styles[1] = 0xF;
    styles[2] = 0xF;
    styles[selection] = 0x8;
    
    DrawOptionMenuChoice(gSystemText_Slow, 120, 40, styles[0]);
    DrawOptionMenuChoice(gSystemText_Mid, 155, 40, styles[1]);
    DrawOptionMenuChoice(gSystemText_Fast, 184, 40, styles[2]);
}

static u8 BattleScene_ProcessInput(u8 selection)
{
    if(gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT))
        selection ^= 1;
    return selection;
}

static void BattleScene_DrawChoices(u8 selection)
{
    u8 styles[2];
    
    styles[0] = 0xF;
    styles[1] = 0xF;
    styles[selection] = 0x8;
    
    DrawOptionMenuChoice(gSystemText_On, 120, 56, styles[0]);
    DrawOptionMenuChoice(gSystemText_Off, 190, 56, styles[1]);
}

static u8 BattleStyle_ProcessInput(u8 selection)
{
    if(gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT))
        selection ^= 1;
    return selection;
}

static void BattleStyle_DrawChoices(u8 selection)
{
    u8 styles[2];
    
    styles[0] = 0xF;
    styles[1] = 0xF;
    styles[selection] = 0x8;
    
    DrawOptionMenuChoice(gSystemText_Shift, 120, 72, styles[0]);
    DrawOptionMenuChoice(gSystemText_Set, 190, 72, styles[1]);
}

static u8 Sound_ProcessInput(u8 selection)
{
    if(gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        SetPokemonCryStereo(selection);
    }
    return selection;
}

static void Sound_DrawChoices(u8 selection)
{
    u8 styles[3];
    
    styles[0] = 0xF;
    styles[1] = 0xF;
    styles[selection] = 0x8;
    
    DrawOptionMenuChoice(gSystemText_Mono, 120, 88, styles[0]);
    DrawOptionMenuChoice(gSystemText_Stereo, 172, 88, styles[1]);
}

static u8 FrameType_ProcessInput(u8 selection)
{
    if(gMain.newKeys & DPAD_RIGHT)
    {
        if(selection <= 18)
            selection++;
        else
            selection = 0;
        MenuLoadTextWindowGraphics_OverrideFrameType(selection);
    }
    if(gMain.newKeys & DPAD_LEFT)
    {
        if(selection != 0)
            selection--;
        else
            selection = 19;
        MenuLoadTextWindowGraphics_OverrideFrameType(selection);
    }
    return selection;
}

#define CHAR_0 0xA1 //Character code of '0' character

static void FrameType_DrawChoices(u8 selection)
{
    u8 text[8];
    u8 n = selection + 1;
    u16 i;
    
    for(i = 0; gSystemText_Terminator[i] != EOS && i <= 5; i++)
        text[i] = gSystemText_Terminator[i];
    
    //Convert number to decimal string
    if(n / 10 != 0)
    {
        text[i] = n / 10 + CHAR_0;
        i++;
        text[i] = n % 10 + CHAR_0;
        i++;
    }
    else
    {
        text[i] = n % 10 + CHAR_0;
        i++;
        text[i] = CHAR_SPACE;
        i++;
    }
    
    text[i] = EOS;
    MenuPrint(gSystemText_Type, 15, 15);
    MenuPrint(text, 18, 15);
}

static u8 ButtonMode_ProcessInput(u8 selection)
{
    if(gMain.newKeys & DPAD_RIGHT)
    {
        if(selection <= 1)
            selection++;
        else
            selection = 0;
    }
    if(gMain.newKeys & DPAD_LEFT)
    {
        if(selection != 0)
            selection--;
        else
            selection = 2;
    }
    return selection;
}

static void ButtonMode_DrawChoices(u8 selection)
{
    u8 styles[3];
    
    styles[0] = 0xF;
    styles[1] = 0xF;
    styles[2] = 0xF;
    styles[selection] = 0x8;
    
    DrawOptionMenuChoice(gSystemText_Normal, 120, 104, styles[0]);
    DrawOptionMenuChoice(gSystemText_LR, 166, 104, styles[1]);
    DrawOptionMenuChoice(gSystemText_LA, 188, 104, styles[2]);
}
