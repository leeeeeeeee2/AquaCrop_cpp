#include "AquaCrop/PrepareFertilitySalinity.h"

namespace AquaCrop {

void ReferenceStressBiomassRelationship(
    int32_t DaysToCCini, int32_t GDDaysToCCini, int32_t DaysToGermination,
    int32_t DaysToFullCanopy, int32_t DaysToSenescence, int32_t DaysToHarvest,
    int32_t DaysToFlowering, int32_t LengthFlowering,
    int32_t GDDaysToGermination, int32_t GDDaysToFullCanopy,
    int32_t GDDaysToSenescence, int32_t GDDaysToHarvest,
    int32_t WPy, int32_t HI, dp CCo, dp CCx, dp CGC, dp GDDCGC,
    dp CDC, dp GDDCDC, dp KcTop, dp KcDecline, dp CCEffectEvapLate,
    dp Tbase, dp Tupper, dp Tmin, dp Tmax, dp GDtranspLow,
    dp WP, dp dHIdt, int32_t Day1, bool DeterminancyLinked,
    rep_Shapes StressResponse, subkind CropSubkind, modeCycle ModeCycle,
    dp& Coeffb0, dp& Coeffb1, dp& Coeffb2,
    dp& X10, dp& X20, dp& X30, dp& X40, dp& X50, dp& X60, dp& X70,
    int32_t GDDaysToFlowering, int32_t GDDLengthFlowering,
    int32_t GDDaysToHIo, plant Planting, int32_t DaysToHIo)
{
    // Placeholder
    Coeffb0 = 0.0;
    Coeffb1 = 0.0;
    Coeffb2 = 0.0;
}

void ReferenceCCxSaltStressRelationship(
    int32_t DaysToCCini, int32_t GDDaysToCCini, int32_t DaysToGermination,
    int32_t DaysToFullCanopy, int32_t DaysToSenescence, int32_t DaysToHarvest,
    int32_t DaysToFlowering, int32_t LengthFlowering,
    int32_t GDDaysToFlowering, int32_t GDDLengthFlowering,
    int32_t GDDaysToGermination, int32_t GDDaysToFullCanopy,
    int32_t GDDaysToSenescence, int32_t GDDaysToHarvest,
    int32_t WPy, int32_t HI, dp CCo, dp CCx, dp CGC, dp GDDCGC,
    dp CDC, dp GDDCDC, dp KcTop, dp KcDecline, dp CCEffectEvapLate,
    dp Tbase, dp Tupper, dp Tmin, dp Tmax, dp GDtranspLow,
    dp WP, dp dHIdt, int32_t Day1, bool DeterminancyLinked,
    subkind CropSubkind, modeCycle ModeCycle, int8_t CCsaltDistortion,
    dp& Coeffb0Salt, dp& Coeffb1Salt, dp& Coeffb2Salt,
    dp& X10, dp& X20, dp& X30, dp& X40, dp& X50, dp& X60, dp& X70, dp& X80, dp& X90,
    int32_t GDDaysToHIo, plant Planting, int32_t DaysToHIo)
{
    // Placeholder
    Coeffb0Salt = 0.0;
    Coeffb1Salt = 0.0;
    Coeffb2Salt = 0.0;
}

}
