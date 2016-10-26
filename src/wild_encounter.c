#include "gba/gba.h"
#include "fieldmap.h"
#include "global.h"
#include "pokemon.h"
#include "rng.h"
#include "script.h"

struct WildPokemon {
    u8 minLevel;
    u8 maxLevel;
    u16 species;
};

struct WildPokemonInfo {
    u8 encounterRate;
    struct WildPokemon *wildPokemon;
};

struct WildPokemonHeader {
    u8 mapGroup;
    u8 mapNum;
    struct WildPokemonInfo *landMonsInfo;
    struct WildPokemonInfo *waterMonsInfo;
    struct WildPokemonInfo *rockSmashMonsInfo;
    struct WildPokemonInfo *fishingMonsInfo;
};

extern bool8 sub_805759C(u8);
extern void GetXYCoordsOneStepInFrontOfPlayer(void *, void *);
extern u32 sub_80C8448(void);
extern s16 sub_810CAE4(u8, u32);
extern bool32 GetSafariZoneFlag(void);
extern u8 TestPlayerAvatarFlags(u8);
extern u8 sub_8057468(u8);
extern u8 sub_81344CC(void);
extern u8 sub_8057494(u8);
extern void sub_8081A00(void);
extern u8 sub_8057434(u8);
extern void PlayerGetDestCoords(u16 *x, u16 *y);
extern void sub_80BEA50(u16);
extern void sav12_xor_increment(u8);
extern u8 FlagGet(u16);
extern u16 VarGet(u16);
extern int MapGridGetMetatileBehaviorAt(int x, int y);

extern u8 gWildEncountersDisabled;
extern u16 gUnknown_0839DC00[];
//extern u16 gUnknown_0839DC00[3][3];
extern u32 gUnknown_0202FF80;   //Feebas rng value
extern struct WildPokemonHeader gWildMonHeaders[];
extern struct Pokemon gEnemyParty[6];
extern struct Pokemon gPlayerParty[6];
extern u16 gScriptResult;
extern struct WildPokemon gWildFeebasRoute119Data;
extern u8 Event_RepelWoreOff[];

#define NUM_FEEBAS_SPOTS 6

u16 sub_8084984(s16 x, s16 y, u8 c);
bool8 CheckFeebas(void);
u16 FeebasRandom(void);
void FeebasSeedRng(u16 seed);
u8 PickWildMon_Grass(void);
u8 PickWildMon_Water(void);
u8 PickWildMon_Fishing(u8 rod);
u8 RandomInRange(struct WildPokemon *wildPokemon);
u16 GetCurrentMapWildMonHeader(void);
u8 PickWildMonNature(void);
void CreateWildMon(u16 species, u8 b);
bool8 GenerateWildMon(struct WildPokemonInfo *a, u8 b, u8 c);
u16 GenerateFishingWildMon(struct WildPokemonInfo *a, u8 rod);
bool8 SetUpMassOutbreakEncounter(u8 a);
bool8 DoMassOutbreakEncounterTest(void);
bool8 DoWildEncounterRateDiceRoll(u16 a);
bool8 DoWildEncounterTest(u32 a, u8 b);
bool8 DoGlobalWildEncounterDiceRoll(void);
bool8 StandardWildEncounter(u16 a, u16 b);
void RockSmashWildEncounter(void);
u8 SweetScentWildEncounter(void);
u8 GetFishingWildMonListHeader(void);
void FishingWildEncounter(u8 rod);
u16 GetLocalWildMon(u8 *a1);
u16 GetMirageIslandMon(void);
bool8 UpdateRepelCounter(void);
bool8 RepelCheck(bool8 level);
void ApplyFluteEncounterRateMod(u32 *a1);
void ApplyCleanseTagEncounterRateMod(u32 *a1);


void DisableWildEncounters(bool8 disable)
{
    gWildEncountersDisabled = disable;
}

u16 sub_8084984(s16 x, s16 y, u8 c)
{
    //odd, but needed to match
    u16 *arr = gUnknown_0839DC00;
    u16 *ptr = &arr[c * 3];
    u16 r9 = gUnknown_0839DC00[c * 3 + 1];
    u16 r6 = gUnknown_0839DC00[c * 3 + 2];
    u16 r5 = *ptr;
   
    while(r5 <= r9)
    {
        u16 i;
       
        for(i = 0; i < gMapHeader.mapData->width; i++)
        {
            if(sub_805759C(MapGridGetMetatileBehaviorAt(i + 7, r5 + 7)) == TRUE)
            {
                r6++;
                if(x == i && y == r5)
                    return r6;
            }
            //_08084A14
        }
        r5++;
    }
 
    return r6 + 1;
}

bool8 CheckFeebas(void)
{
    u16 feebasSpots[6];
    s16 x;
    s16 y;
    u8 r7 = 0;
    u8 i;
    u16 foo;
   
    if(gSaveBlock1.location.mapGroup == 0 && gSaveBlock1.location.mapNum == 0x22)
    {
        GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
        x -= 7;
        y -= 7;
        
        // *sigh* whatever gets the code to match!
        {
        register u16 *arr asm("r0");
        if(y >= (arr = gUnknown_0839DC00)[3 * 1 + 0] && y <= arr[3 * 1 + 1])
            r7 = 1;
        if(y >= arr[3 * 2 + 0] && y <= arr[3 * 2 + 1])
            r7 = 2;
        }
        //_08084A8E
 
        if(Random() % 100 > 49) //Why not just do (Random() & 1) to get a 50% chance?
            return FALSE;
        //_08084AC8
        FeebasSeedRng(gSaveBlock1.feebasLocationSeed);
        for(i = 0; i != NUM_FEEBAS_SPOTS;)
        {
            feebasSpots[i] = FeebasRandom() % 447;
            if(feebasSpots[i] == 0)
                feebasSpots[i] = 447;
            if(feebasSpots[i] < 1 || feebasSpots[i] >= 4)
                i++;
        }
        foo = sub_8084984(x, y, r7);
        for(i = 0; i < NUM_FEEBAS_SPOTS; i++)
        {
            if(foo == feebasSpots[i])
                return TRUE;
        }        
    }
    //_08084B38
    return FALSE;
}

u16 FeebasRandom(void)
{
    gUnknown_0202FF80 = 12345 + 0x41C64E6D * gUnknown_0202FF80;
    return gUnknown_0202FF80 >> 16;
}

void FeebasSeedRng(u16 seed)
{
    gUnknown_0202FF80 = seed;
}

u8 PickWildMon_Grass(void)
{
    u8 val = Random() % 100;
    
    if(val < 20)
        return 0;
    if(val >= 20 && val < 40)
        return 1;
    if(val >= 40 && val < 50)
        return 2;
    if(val >= 50 && val < 60)
        return 3;
    if(val >= 60 && val < 70)
        return 4;
    if(val >= 70 && val < 80)
        return 5;
    if(val >= 80 && val < 85)
        return 6;
    if(val >= 85 && val < 90)
        return 7;
    if(val >= 90 && val < 94)
        return 8;
    if(val >= 94 && val < 98)
        return 9;
    if(val == 98)
        return 10;
    else
        return 11;
}

u8 PickWildMon_Water(void)
{
    u8 val = Random() % 100;
    
    if(val < 60)
        return 0;
    if(val >= 60 && val < 90)
        return 1;
    if(val >= 90 && val < 95)
        return 2;
    if(val >= 95 && val < 99)
        return 3;
    else
        return 4;
}

u8 PickWildMon_Fishing(u8 rod)
{
    u32 r5 = 0;
    u8 val = Random() % 100;
    
    switch(rod)
    {
        case 0: //old rod?
            if(val < 70)
                r5 = 0;
            else
                r5 = 1;
            return r5;
        case 1: //good rod?
            if(val < 60)
                r5 = 2;
            if(val >= 60 && val < 80)
                r5 = 3;
            if(val >= 80 && val < 100)
                r5 = 4;
            return r5;
        case 2: //super rod?
            if(val < 40)
                r5 = 5;
            if(val >= 40 && val < 80)
                r5 = 6;
            if(val >= 80 && val < 95)
                r5 = 7;
            if(val >= 95 && val < 99)
                r5 = 8;
            if(val == 99)
                r5 = 9;
            return r5;
        default:
            return r5;
    }
    
    return r5;
}

//Chooses level of wild Pokemon
u8 RandomInRange(struct WildPokemon *wildPokemon)
{
    u8 min;
    u8 max;
    u8 range;
    u8 rand;
    
    //Make sure minimum level is less than maximum level
    if(wildPokemon->maxLevel >= wildPokemon->minLevel)
    {
        min = wildPokemon->minLevel;
        max = wildPokemon->maxLevel;
    }
    else
    {
        min = wildPokemon->maxLevel;
        max = wildPokemon->minLevel;
    }
    range = max - min + 1;
    rand = Random() % range;
    return min + rand;
}

u16 GetCurrentMapWildMonHeader(void)
{
    u16 i;
    
    for(i = 0; gWildMonHeaders[i].mapGroup != 0xFF; i++)
    {
        if(gWildMonHeaders[i].mapGroup == gSaveBlock1.location.mapGroup &&
        gWildMonHeaders[i].mapNum == gSaveBlock1.location.mapNum)
            return i;
    }
    return -1;
}

u8 PickWildMonNature(void)
{
    u16 foo;
    u32 r7;
    u8 arr[25];
    
    if(GetSafariZoneFlag() == TRUE && Random() % 100 < 80)
    {
        r7 = sub_80C8448();
        
        if(r7)
        {
            u8 i;
            
            for(i = 0; i < 25; i++)
                arr[i] = i;
            //_08084E04
            for(i = 0; i < 24; i++)
            {
                u8 j;
                
                for(j = i + 1; j < 25; j++)
                {
                    if(Random() & 1)
                    {
                        u8 temp = arr[i];
                        
                        arr[i] = arr[j];
                        arr[j] = temp;
                    }
                }
            }
            //_08084E3E
            for(i = 0; i < 25; i++)
            {
                if(sub_810CAE4(arr[i], r7) > 0)
                    return arr[i];
            }
        }
    }
    //_08084E5E
    return Random() % 25;
}

void CreateWildMon(u16 species, u8 b)
{
    ZeroEnemyPartyMons();
    CreateMonWithNature(&gEnemyParty[0], species, b, 0x20, PickWildMonNature());
}

bool8 GenerateWildMon(struct WildPokemonInfo *a, u8 b, u8 c)
{
    u8 mon = 0;
    u8 level;
    
    switch(b)
    {
        case 0:
            mon = PickWildMon_Grass();
            break;
        case 1:
            mon = PickWildMon_Water();
            break;
        case 2:
            mon = PickWildMon_Water();
            break;
        default:
            break;
    }
    
    level = RandomInRange(&a->wildPokemon[mon]);
    if(c == 1 && RepelCheck(level) == 0)
    {
        return 0;
    }
    else
    {
        CreateWildMon(a->wildPokemon[mon].species, level);
        return 1;
    }
}

u16 GenerateFishingWildMon(struct WildPokemonInfo *a, u8 rod)
{
    u8 mon = PickWildMon_Fishing(rod);
    u8 level = RandomInRange(&a->wildPokemon[mon]);
    
    CreateWildMon(a->wildPokemon[mon].species, level);
    return a->wildPokemon[mon].species;
}

bool8 SetUpMassOutbreakEncounter(u8 a)
{
    u16 i;
    
    if(a == 1 && RepelCheck(gSaveBlock1.outbreakPokemonLevel) == 0)
    {
        return 0;
    }
    //_08084F78
    CreateWildMon(gSaveBlock1.outbreakPokemonSpecies, gSaveBlock1.outbreakPokemonLevel);
    for(i = 0; i < 4; i++)
    {
        SetMonMoveSlot(&gEnemyParty[0], gSaveBlock1.outbreakPokemonMoves[i], i);
    }
    return 1;
}

bool8 DoMassOutbreakEncounterTest(void)
{
    if(gSaveBlock1.outbreakPokemonSpecies &&
    gSaveBlock1.location.mapNum == gSaveBlock1.outbreakLocationMapNum &&
    gSaveBlock1.location.mapGroup == gSaveBlock1.outbreakLocationMapGroup)
    {
        if(Random() % 100 < gSaveBlock1.outbreakPokemonProbability)
            return 1;
    }
    return 0;
}

bool8 DoWildEncounterRateDiceRoll(u16 a)
{
    if(Random() % 2880 < a)
        return TRUE;
    else
        return FALSE;
}

bool8 DoWildEncounterTest(u32 encounterRate, u8 b)
{
    encounterRate *= 16;
    
    if(TestPlayerAvatarFlags(6))
    {
        encounterRate = encounterRate * 80 / 100;
    }
    //_0808507E
    ApplyFluteEncounterRateMod(&encounterRate);
    ApplyCleanseTagEncounterRateMod(&encounterRate);
    
    if(b == 0)
    {
        // UB: Too few arguments for function 'GetMonData'
        if(!GetMonData(&gPlayerParty[0], MON_DATA_SANITY_BIT3))
        {
            u32 ability = GetMonAbility(&gPlayerParty[0]);
            if(ability == 1)
                encounterRate /= 2;
            if(ability == 0x23)
                encounterRate *= 2;
        }
    }
    //_080850BA
    if(encounterRate > 2880)
        encounterRate = 2880;
    return DoWildEncounterRateDiceRoll(encounterRate);
}

bool8 DoGlobalWildEncounterDiceRoll(void)
{
    if(Random() % 100 >= 60)
        return FALSE;
    else
        return TRUE;
}

#ifdef NONMATCHING
//ToDo: fix control flow
bool8 StandardWildEncounter(u16 a, u16 b)
{
    u16 unk;
    
    if(gWildEncountersDisabled != TRUE)
    {
        unk = GetCurrentMapWildMonHeader();
        
        if(unk != 0xFFFF)
        {
            if(sub_8057468(a) == 1)
            {
                if(gWildMonHeaders[unk].landMonsInfo)
                {
                    if(b != a && !DoGlobalWildEncounterDiceRoll())
                        return 0;
                    
                    if(DoWildEncounterTest(gWildMonHeaders[unk].landMonsInfo->encounterRate, 0) == TRUE)
                    {
                        if(sub_81344CC() == TRUE)
                        {
                            //_0808524A
                            if(RepelCheck(gSaveBlock1.roamer.level))
                            {
                                sub_8081A5C();
                                return 1;
                            }
                        }
                        else
                        {
                            if(DoMassOutbreakEncounterTest() == TRUE && SetUpMassOutbreakEncounter(1) == TRUE
                            || GenerateWildMon(gWildMonHeaders[unk].landMonsInfo, 0, 1) == 1)
                            {
                                //_0808527A
                                asm("");
                                sub_8081A00();
                                return 1;
                            }
                        }
                    }
                }
            }
            else
            {
                //_080851D8
                if(sub_8057494(a) == TRUE || TestPlayerAvatarFlags(8) && sub_8057434(a) == TRUE)
                {
                    if(gWildMonHeaders[unk].waterMonsInfo)
                    {
                        if(b != a && !DoGlobalWildEncounterDiceRoll())
                            return 0;
                        
                        if(DoWildEncounterTest(gWildMonHeaders[unk].waterMonsInfo->encounterRate, 0) == TRUE)
                        {
                            if(sub_81344CC() == TRUE)
                            {
                                //_0808524A
                                if(RepelCheck(gSaveBlock1.roamer.level))
                                {
                                    sub_8081A5C();
                                    return 1;
                                }
                            }
                            else
                            {
                                //_08085268
                                if(GenerateWildMon(gWildMonHeaders[unk].waterMonsInfo, 1, 1) == 1)
                                {
                                    //_0808527A
                                    sub_8081A00();
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
#else
__attribute__((naked))
bool8 StandardWildEncounter(u16 a, u16 b)
{
    asm(".syntax unified\n\
	push {r4-r7,lr}\n\
	mov r7, r9\n\
	mov r6, r8\n\
	push {r6,r7}\n\
	lsls r0, 16\n\
	lsrs r6, r0, 16\n\
	mov r9, r6\n\
	lsls r1, 16\n\
	lsrs r7, r1, 16\n\
	mov r8, r7\n\
	ldr r0, _080851CC\n\
	ldrb r0, [r0]\n\
	cmp r0, 0x1\n\
	bne _08085122\n\
	b _08085282\n\
_08085122:\n\
	bl GetCurrentMapWildMonHeader\n\
	lsls r0, 16\n\
	lsrs r5, r0, 16\n\
	ldr r0, _080851D0\n\
	cmp r5, r0\n\
	bne _08085132\n\
	b _08085282\n\
_08085132:\n\
	lsls r0, r6, 24\n\
	lsrs r4, r0, 24\n\
	adds r0, r4, 0\n\
	bl sub_8057468\n\
	lsls r0, 24\n\
	lsrs r0, 24\n\
	cmp r0, 0x1\n\
	bne _080851D8\n\
	ldr r1, _080851D4\n\
	lsls r2, r5, 2\n\
	adds r0, r2, r5\n\
	lsls r0, 2\n\
	adds r1, 0x4\n\
	adds r0, r1\n\
	ldr r0, [r0]\n\
	adds r4, r2, 0\n\
	cmp r0, 0\n\
	bne _0808515A\n\
	b _08085282\n\
_0808515A:\n\
	cmp r7, r6\n\
	beq _0808516A\n\
	bl DoGlobalWildEncounterDiceRoll\n\
	lsls r0, 24\n\
	cmp r0, 0\n\
	bne _0808516A\n\
	b _08085282\n\
_0808516A:\n\
	ldr r1, _080851D4\n\
	adds r0, r4, r5\n\
	lsls r0, 2\n\
	adds r1, 0x4\n\
	adds r0, r1\n\
	ldr r0, [r0]\n\
	ldrb r0, [r0]\n\
	movs r1, 0\n\
	bl DoWildEncounterTest\n\
	lsls r0, 24\n\
	lsrs r0, 24\n\
	cmp r0, 0x1\n\
	beq _08085188\n\
	b _08085282\n\
_08085188:\n\
	bl sub_81344CC\n\
	lsls r0, 24\n\
	lsrs r0, 24\n\
	cmp r0, 0x1\n\
	beq _0808524A\n\
	bl DoMassOutbreakEncounterTest\n\
	lsls r0, 24\n\
	lsrs r0, 24\n\
	cmp r0, 0x1\n\
	bne _080851AE\n\
	movs r0, 0x1\n\
	bl SetUpMassOutbreakEncounter\n\
	lsls r0, 24\n\
	lsrs r0, 24\n\
	cmp r0, 0x1\n\
	beq _0808527A\n\
_080851AE:\n\
	ldr r1, _080851D4\n\
	adds r0, r4, r5\n\
	lsls r0, 2\n\
	adds r1, 0x4\n\
	adds r0, r1\n\
	ldr r0, [r0]\n\
	movs r1, 0\n\
	movs r2, 0x1\n\
	bl GenerateWildMon\n\
	lsls r0, 24\n\
	lsrs r0, 24\n\
	cmp r0, 0x1\n\
	beq _0808527A\n\
	b _08085282\n\
	.align 2, 0\n\
_080851CC: .4byte gWildEncountersDisabled\n\
_080851D0: .4byte 0x0000ffff\n\
_080851D4: .4byte gWildMonHeaders\n\
_080851D8:\n\
	adds r0, r4, 0\n\
	bl sub_8057494\n\
	lsls r0, 24\n\
	lsrs r0, 24\n\
	cmp r0, 0x1\n\
	beq _08085200\n\
	movs r0, 0x8\n\
	bl TestPlayerAvatarFlags\n\
	lsls r0, 24\n\
	cmp r0, 0\n\
	beq _08085282\n\
	adds r0, r4, 0\n\
	bl sub_8057434\n\
	lsls r0, 24\n\
	lsrs r0, 24\n\
	cmp r0, 0x1\n\
	bne _08085282\n\
_08085200:\n\
	ldr r1, _08085260\n\
	lsls r2, r5, 2\n\
	adds r0, r2, r5\n\
	lsls r0, 2\n\
	adds r1, 0x8\n\
	adds r0, r1\n\
	ldr r0, [r0]\n\
	adds r4, r2, 0\n\
	cmp r0, 0\n\
	beq _08085282\n\
	cmp r8, r9\n\
	beq _08085222\n\
	bl DoGlobalWildEncounterDiceRoll\n\
	lsls r0, 24\n\
	cmp r0, 0\n\
	beq _08085282\n\
_08085222:\n\
	ldr r1, _08085260\n\
	adds r0, r4, r5\n\
	lsls r0, 2\n\
	adds r1, 0x8\n\
	adds r4, r0, r1\n\
	ldr r0, [r4]\n\
	ldrb r0, [r0]\n\
	movs r1, 0\n\
	bl DoWildEncounterTest\n\
	lsls r0, 24\n\
	lsrs r0, 24\n\
	cmp r0, 0x1\n\
	bne _08085282\n\
	bl sub_81344CC\n\
	lsls r0, 24\n\
	lsrs r0, 24\n\
	cmp r0, 0x1\n\
	bne _08085268\n\
_0808524A:\n\
	ldr r0, _08085264\n\
	ldrb r0, [r0, 0xC]\n\
	bl RepelCheck\n\
	lsls r0, 24\n\
	cmp r0, 0\n\
	beq _08085282\n\
	bl sub_8081A5C\n\
	movs r0, 0x1\n\
	b _08085284\n\
	.align 2, 0\n\
_08085260: .4byte gWildMonHeaders\n\
_08085264: .4byte gSaveBlock1 + 0x3144\n\
_08085268:\n\
	ldr r0, [r4]\n\
	movs r1, 0x1\n\
	movs r2, 0x1\n\
	bl GenerateWildMon\n\
	lsls r0, 24\n\
	lsrs r0, 24\n\
	cmp r0, 0x1\n\
	bne _08085282\n\
_0808527A:\n\
	bl sub_8081A00\n\
	movs r0, 0x1\n\
	b _08085284\n\
_08085282:\n\
	movs r0, 0\n\
_08085284:\n\
	pop {r3,r4}\n\
	mov r8, r3\n\
	mov r9, r4\n\
	pop {r4-r7}\n\
	pop {r1}\n\
	bx r1\n\
    .syntax divided\n");
}
#endif

void RockSmashWildEncounter(void)
{
    u16 headerNum = GetCurrentMapWildMonHeader();
    
    if(headerNum != 0xFFFF)
    {
        struct WildPokemonInfo *wildPokemonInfo = gWildMonHeaders[headerNum].rockSmashMonsInfo;
        
        if(wildPokemonInfo == NULL)
        {
            gScriptResult = 0;
            return;
        }
        else if(DoWildEncounterTest(wildPokemonInfo->encounterRate, 1) == TRUE &&
        GenerateWildMon(wildPokemonInfo, 2, 1) == 1)
        {
            sub_8081A00();
            gScriptResult = 1;
            return;
        }
    }
    gScriptResult = 0;
    return;
}

u8 SweetScentWildEncounter(void)
{
    s16 x;
    s16 y;
    u16 headerNum;
    struct WildPokemonInfo *wildPokemonInfo;
    
    PlayerGetDestCoords(&x, &y);
    //headerNum = GetCurrentMapWildMonHeader();
    if((headerNum = GetCurrentMapWildMonHeader()) != 0xFFFF)
    {
        if(sub_8057468(MapGridGetMetatileBehaviorAt(x, y)) == 1)
        {
            wildPokemonInfo = gWildMonHeaders[headerNum].landMonsInfo;
            
            if(wildPokemonInfo == NULL)
                return 0;
            
            if(sub_81344CC() == 1)
            {
                //_080853BC
                sub_8081A5C();
                return 1;
            }
            
            if(DoMassOutbreakEncounterTest() == 1)
                SetUpMassOutbreakEncounter(0);
            else
            //_08085374
                GenerateWildMon(wildPokemonInfo, 0, 0);
            //_080853D2
            sub_8081A00();
            return 1;
        }
        //_08085380
        else
        {
            if(sub_8057494(MapGridGetMetatileBehaviorAt(x, y)) == 1)
            {
                wildPokemonInfo = gWildMonHeaders[headerNum].waterMonsInfo;
                
                if(wildPokemonInfo == NULL)
                    return 0;
                
                if(sub_81344CC() == 1)
                {
                    sub_8081A5C();
                    return 1;
                }
                else
                //_080853C8
                GenerateWildMon(wildPokemonInfo, 1, 0);
                sub_8081A00();
                return 1;
            }
        }
    }
    return 0;
}

u8 GetFishingWildMonListHeader(void)
{
    u16 headerNum = GetCurrentMapWildMonHeader();
    
    if(headerNum != 0xFFFF && gWildMonHeaders[headerNum].fishingMonsInfo != NULL)
        return 1;
    else
        return 0;
}

void FishingWildEncounter(u8 rod)
{
    u16 species;
    
    if(CheckFeebas() == TRUE)
    {
        u8 level = RandomInRange(&gWildFeebasRoute119Data);
        
        species = gWildFeebasRoute119Data.species;
        CreateWildMon(species, level);
    }
    else
    {
        species = GenerateFishingWildMon(
          gWildMonHeaders[GetCurrentMapWildMonHeader()].fishingMonsInfo,
          rod);
    }
    sav12_xor_increment(12);
    sub_80BEA50(species);
    sub_8081A00();
}

#ifdef NONMATCHING
u16 GetLocalWildMon(u8 *a1)
{
    u16 headerNum;
    
    *a1 = 0;
    headerNum = GetCurrentMapWildMonHeader();
    if(headerNum != 0xFFFF)
    {
        struct WildPokemonInfo *landMonsInfo = gWildMonHeaders[headerNum].landMonsInfo;
        struct WildPokemonInfo *waterMonsInfo = gWildMonHeaders[headerNum].waterMonsInfo;
        
        if(landMonsInfo || waterMonsInfo)
        {
            if(waterMonsInfo && landMonsInfo == NULL)
            {
                //_080854CE
                *a1 = 1;
                return waterMonsInfo->wildPokemon[PickWildMon_Water()].species;
            }
            else if(landMonsInfo && waterMonsInfo)
            {
                //_080854DC
                if(Random() % 100 < 80)
                    //jump to _08085500
                    return landMonsInfo->wildPokemon[PickWildMon_Grass()].species;
                else
                {
                    *a1 = 1;
                    return waterMonsInfo->wildPokemon[PickWildMon_Water()].species;
                }
            }
            else if(landMonsInfo && waterMonsInfo == NULL)
            {
                //_08085500:
                return landMonsInfo->wildPokemon[PickWildMon_Grass()].species;
            }
        }
    }
    return 0;
}
#else
__attribute__((naked))
u16 GetLocalWildMon(u8 *a1)
{
    asm(".syntax unified\n\
	push {r4-r6,lr}\n\
	adds r6, r0, 0\n\
	movs r0, 0\n\
	strb r0, [r6]\n\
	bl GetCurrentMapWildMonHeader\n\
	lsls r0, 16\n\
	lsrs r3, r0, 16\n\
	ldr r0, _080854C0\n\
	cmp r3, r0\n\
	beq _080854BA\n\
	ldr r2, _080854C4\n\
	lsls r1, r3, 2\n\
	adds r1, r3\n\
	lsls r1, 2\n\
	adds r0, r2, 0x4\n\
	adds r0, r1, r0\n\
	ldr r5, [r0]\n\
	adds r2, 0x8\n\
	adds r1, r2\n\
	ldr r4, [r1]\n\
	cmp r5, 0\n\
	bne _080854C8\n\
	cmp r4, 0\n\
	bne _080854CE\n\
_080854BA:\n\
	movs r0, 0\n\
	b _0808550E\n\
	.align 2, 0\n\
_080854C0: .4byte 0x0000ffff\n\
_080854C4: .4byte gWildMonHeaders\n\
_080854C8:\n\
	cmp r4, 0\n\
	bne _080854DC\n\
	b _08085500\n\
_080854CE:\n\
	movs r0, 0x1\n\
	strb r0, [r6]\n\
	bl PickWildMon_Water\n\
	lsls r0, 24\n\
	ldr r1, [r4, 0x4]\n\
	b _08085508\n\
_080854DC:\n\
	bl Random\n\
	lsls r0, 16\n\
	lsrs r0, 16\n\
	movs r1, 0x64\n\
	bl __umodsi3\n\
	lsls r0, 16\n\
	lsrs r0, 16\n\
	cmp r0, 0x4F\n\
	bls _08085500\n\
	movs r0, 0x1\n\
	strb r0, [r6]\n\
	bl PickWildMon_Water\n\
	lsls r0, 24\n\
	ldr r1, [r4, 0x4]\n\
	b _08085508\n\
_08085500:\n\
	bl PickWildMon_Grass\n\
	lsls r0, 24\n\
	ldr r1, [r5, 0x4]\n\
_08085508:\n\
	lsrs r0, 22\n\
	adds r0, r1\n\
	ldrh r0, [r0, 0x2]\n\
_0808550E:\n\
	pop {r4-r6}\n\
	pop {r1}\n\
	bx r1\n\
    .syntax divided\n");
}
#endif

u16 GetMirageIslandMon(void)
{
    u16 headerNum = GetCurrentMapWildMonHeader();
    
    if(headerNum != 0xFFFF)
    {
        struct WildPokemonInfo *waterMonsInfo = gWildMonHeaders[headerNum].waterMonsInfo;
        
        if(waterMonsInfo)
            return waterMonsInfo->wildPokemon[PickWildMon_Water()].species;
    }
    return 0;
}

bool8 UpdateRepelCounter(void)
{
    u16 steps = VarGet(0x4021);
    
    if(steps != 0)
    {
        steps--;
        VarSet(0x4021, steps);
        if(steps == 0)
        {
            ScriptContext1_SetupScript(Event_RepelWoreOff);
            return TRUE;
        }
    }
    return FALSE;
}

//Returns FALSE if Repel prevents wild Pokemon at the specified level from appearing
bool8 RepelCheck(bool8 level)
{
    u8 i;
    
    if(!VarGet(0x4021))
        return TRUE;
    else
    {
        for(i = 0; i < 6; i++)
        {
            // UB: Too few arguments for function 'GetMonData'
            if(GetMonData(&gPlayerParty[i], MON_DATA_HP) && !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG))
            {
                if(level < (u8)GetMonData(&gPlayerParty[i], MON_DATA_LEVEL))
                    return FALSE;
                else
                    return TRUE;
            }
        }
        return FALSE;
    }
}

void ApplyFluteEncounterRateMod(u32 *a1)
{
    if(FlagGet(0x84D) == 1)
        *a1 = *a1 + *a1 / 2;
    else if(FlagGet(0x84E) == 1)
        *a1 = *a1 / 2;
}

void ApplyCleanseTagEncounterRateMod(u32 *a1)
{
    // UB: Too few arguments for function 'GetMonData'
    if(GetMonData(&gPlayerParty[0], MON_DATA_HELD_ITEM) == 0xBE)
        *a1 = *a1 * 2 / 3;
}
