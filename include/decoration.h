#ifndef GUARD_DECORATION_H
#define GUARD_DECORATION_H

struct Decoration
{
    /*0x00*/ u8 id;
    /*0x01*/ u8 name[16];
    /*0x11*/ u8 decor_field_11;
    /*0x12*/ u8 decor_field_12;
    /*0x13*/ u8 decor_field_13;
    /*0x14*/ u16 price;
    /*0x18*/ u8 *description;
    /*0x1c*/ u16 *tiles;
};

extern struct Decoration gDecorations[0x77];
extern void sub_80FF394(u16 unkr0, u16 unkr1, u16 unkr2);
void DoPlayerPCDecoration(u8);
void sub_80FEC94(u8 arg0);
void sub_80FECB8(u8 arg0);
void sub_80FECE0(u8 arg0);

#endif // GUARD_DECORATION_H
