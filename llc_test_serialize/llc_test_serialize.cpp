// test_gpk_serialize.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "llc_view_serialize.h"

static int testPackedU2 () {
    //for(uint16_t iUnpacked = 0; iUnpacked <             0x3FFFU  ; ++iUnpacked) { llc::packedu16 packed = iUnpacked; if_true_ef(iUnpacked != packed.Value(), "iUnpacked = %u, packed.Value() = %u.", iUnpacked, packed.Value()); if_true_e(iUnpacked > 0xFF packed.TailWidth();}
    //for(uint32_t iUnpacked = 0; iUnpacked < 0x3FFFFFFFFFFFFFFFULL; ++iUnpacked) { llc::packedu64 packed = iUnpacked; if_true_ef(iUnpacked != packed.Value(), "iUnpacked = %u, packed.Value() = %u.", iUnpacked, packed.Value()); if_true_e(iUnpacked > 0xFF packed.TailWidth();}
    for(uint32_t iUnpacked = 0; iUnpacked < 0x3FFFFFFFU; ++iUnpacked) { 
        const llc::packedu32 packed = iUnpacked; 
        if_true_if(0x40     == iUnpacked || 0x3F     == iUnpacked, "Value: 0x%X|%u, packed.Value(): 0x%X|%u, packed.TailWidth: 0x%X|%u.", iUnpacked, iUnpacked, packed.Value(), packed.Value(), packed.TailWidth, packed.TailWidth);
        if_true_if(0x4000   == iUnpacked || 0x3FFF   == iUnpacked, "Value: 0x%X|%u, packed.Value(): 0x%X|%u, packed.TailWidth: 0x%X|%u.", iUnpacked, iUnpacked, packed.Value(), packed.Value(), packed.TailWidth, packed.TailWidth);
        if_true_if(0x400000 == iUnpacked || 0x3FFFFF == iUnpacked, "Value: 0x%X|%u, packed.Value(): 0x%X|%u, packed.TailWidth: 0x%X|%u.", iUnpacked, iUnpacked, packed.Value(), packed.Value(), packed.TailWidth, packed.TailWidth);
        if_true_ef(iUnpacked != packed.Value(), "Value: %u, packed.Value(): %u.", iUnpacked, packed.Value()); 
        if_true_ef(iUnpacked >       0x3F && 1 > packed.TailWidth, "Value = %u, packed.TailWidth = %u.", iUnpacked, (uint32_t)packed.TailWidth);
        if_true_ef(iUnpacked >     0x3FFF && 2 > packed.TailWidth, "Value = %u, packed.TailWidth = %u.", iUnpacked, (uint32_t)packed.TailWidth);
        if_true_ef(iUnpacked >   0x3FFFFF && 3 > packed.TailWidth, "Value = %u, packed.TailWidth = %u.", iUnpacked, (uint32_t)packed.TailWidth);
    }
    return 0;
}

int main()
{
    info_printf("Hello World!");
    testPackedU2();
}
