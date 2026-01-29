#pragma once
#include "AquaCrop/Global.h"

namespace AquaCrop {

dp ActualRootingDepth(int32_t DAP, int32_t DaysToGermination,
    int32_t DaysToMaxRooting, int32_t DaysToHarvest,
    int32_t GDDaysToGermination, int32_t GDDaysToMaxRooting,
    dp SumGDDPrev, dp RootMin, dp RootMax, int8_t RootShape,
    modeCycle ModeCycle);

dp AdjustedRootingDepth(dp ActVal, dp PotVal, dp Tpot, dp Tact,
    dp StressLeaf, dp StressSenescence, int32_t DAP,
    int32_t DaysToGermination, int32_t DaysToMaxRooting, int32_t DaysToHarvest,
    int32_t GDDaysToGermination, int32_t GDDaysToMaxRooting,
    int32_t GDDaysToHarvest, dp SumGDDPrev, dp SumGDD,
    dp RootMin, dp RootMax, dp Ziprev, int8_t RootShape,
    modeCycle ModeCycle);

}
