#ifndef GUARD_ITEM_USE_H
#define GUARD_ITEM_USE_H

void ItemUseOnFieldCB_Bike(u8);
void ItemUseOnFieldCB_Rod(u8);
void ItemUseOnFieldCB_Itemfinder(u8);
void RunItemfinderResults(u8);
void ExitItemfinder(u8);
bool8 ItemfinderCheckForHiddenItems(struct MapEvents *events, u8 taskId);
void sub_80C9720(u8);
void sub_80C9838(u8, s16, s16);
u8 GetPlayerDirectionTowardsHiddenItem(s16, s16);
void SetPlayerDirectionTowardsItem(u8);
void DisplayItemRespondingMessageAndExitItemfinder(u8);
void RotatePlayerAndExitItemfinder(u8);
void sub_80C9D00(u8);
void sub_80C9D74(u8);
void ItemUseOutOfBattle_TMHM(u8);
void sub_80C9EE4(u8);
void sub_80C9F10(u8);
void sub_80C9F80(u8);
void sub_80C9FC0(u8);
void task08_080A1C44(u8);
u8 sub_80CA1C8(void);
void ItemUseOutOfBattle_EvolutionStone(u8);
void ItemUseOutOfBattle_CannotUse(u8);

#endif // GUARD_ITEM_USE_H
