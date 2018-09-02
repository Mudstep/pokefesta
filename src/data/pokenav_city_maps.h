
const u8 gPokenavCityMap_Lavaridge_0[] = INCBIN_U8("graphics/pokenav/city_maps/lavaridge_0.bin.lz");
const u8 gPokenavCityMap_Fallarbor_0[] = INCBIN_U8("graphics/pokenav/city_maps/fallarbor_0.bin.lz");
const u8 gPokenavCityMap_Fortree_0[] = INCBIN_U8("graphics/pokenav/city_maps/fortree_0.bin.lz");
const u8 gPokenavCityMap_Slateport_0[] = INCBIN_U8("graphics/pokenav/city_maps/slateport_0.bin.lz");
const u8 gPokenavCityMap_Slateport_1[] = INCBIN_U8("graphics/pokenav/city_maps/slateport_1.bin.lz");
const u8 gPokenavCityMap_Rustboro_0[] = INCBIN_U8("graphics/pokenav/city_maps/rustboro_0.bin.lz");
const u8 gPokenavCityMap_Rustboro_1[] = INCBIN_U8("graphics/pokenav/city_maps/rustboro_1.bin.lz");
const u8 gPokenavCityMap_Pacifidlog_0[] = INCBIN_U8("graphics/pokenav/city_maps/pacifidlog_0.bin.lz");
const u8 gPokenavCityMap_Mauville_1[] = INCBIN_U8("graphics/pokenav/city_maps/mauville_1.bin.lz");
const u8 gPokenavCityMap_Mauville_0[] = INCBIN_U8("graphics/pokenav/city_maps/mauville_0.bin.lz");
const u8 gPokenavCityMap_Oldale_0[] = INCBIN_U8("graphics/pokenav/city_maps/oldale_0.bin.lz");
const u8 gPokenavCityMap_Lilycove_1[] = INCBIN_U8("graphics/pokenav/city_maps/lilycove_1.bin.lz");
const u8 gPokenavCityMap_Lilycove_0[] = INCBIN_U8("graphics/pokenav/city_maps/lilycove_0.bin.lz");
const u8 gPokenavCityMap_Littleroot_0[] = INCBIN_U8("graphics/pokenav/city_maps/littleroot_0.bin.lz");
const u8 gPokenavCityMap_Dewford_0[] = INCBIN_U8("graphics/pokenav/city_maps/dewford_0.bin.lz");
const u8 gPokenavCityMap_Sootopolis_0[] = INCBIN_U8("graphics/pokenav/city_maps/sootopolis_0.bin.lz");
const u8 gPokenavCityMap_EverGrande_0[] = INCBIN_U8("graphics/pokenav/city_maps/ever_grande_0.bin.lz");
const u8 gPokenavCityMap_EverGrande_1[] = INCBIN_U8("graphics/pokenav/city_maps/ever_grande_1.bin.lz");
const u8 gPokenavCityMap_Verdanturf_0[] = INCBIN_U8("graphics/pokenav/city_maps/verdanturf_0.bin.lz");
const u8 gPokenavCityMap_Mossdeep_1[] = INCBIN_U8("graphics/pokenav/city_maps/mossdeep_1.bin.lz");
const u8 gPokenavCityMap_Mossdeep_0[] = INCBIN_U8("graphics/pokenav/city_maps/mossdeep_0.bin.lz");
const u8 gPokenavCityMap_Petalburg_0[] = INCBIN_U8("graphics/pokenav/city_maps/petalburg_0.bin.lz");

const u8 *const gPokenavCityMaps[][2] = 
{
    {gPokenavCityMap_Littleroot_0, NULL},
    {gPokenavCityMap_Oldale_0, NULL},
    {gPokenavCityMap_Dewford_0, NULL},
    {gPokenavCityMap_Lavaridge_0, NULL},
    {gPokenavCityMap_Fallarbor_0, NULL},
    {gPokenavCityMap_Verdanturf_0, NULL},
    {gPokenavCityMap_Pacifidlog_0, NULL},
    {gPokenavCityMap_Petalburg_0, NULL},
    {gPokenavCityMap_Slateport_0, gPokenavCityMap_Slateport_1},
    {gPokenavCityMap_Mauville_0, gPokenavCityMap_Mauville_1},
    {gPokenavCityMap_Rustboro_0, gPokenavCityMap_Rustboro_1},
    {gPokenavCityMap_Fortree_0, NULL},
    {gPokenavCityMap_Lilycove_0, gPokenavCityMap_Lilycove_1},
    {gPokenavCityMap_Mossdeep_0, gPokenavCityMap_Mossdeep_1},
    {gPokenavCityMap_Sootopolis_0, NULL},
    {gPokenavCityMap_EverGrande_0, gPokenavCityMap_EverGrande_1},
};
