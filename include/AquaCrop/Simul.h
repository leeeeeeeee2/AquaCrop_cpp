#pragma once

#include "AquaCrop/Global.h"

namespace AquaCrop {

void Budget_module(int32_t DayNr, int32_t TargetTimeVal, int32_t TargetDepthVal,
    int32_t VirtualTimeCC, int32_t SumInterval, int32_t DayLastCut,
    int32_t NrDayGrow, int32_t Tadj, int32_t GDDTadj, dp GDDayi,
    dp CGCref, dp GDDCGCref, dp CO2i, dp CCxTotal, dp CCoTotal, dp CDCTotal,
    dp GDDCDCTotal, dp SumGDDadjCC, dp Coeffb0Salt, dp Coeffb1Salt, dp Coeffb2Salt,
    dp StressTotSaltPrev, dp DayFraction, dp GDDayFraction, dp FracAssim,
    int32_t StressSFadjNEW, bool StorageON, bool MobilizationON,
    dp& StressLeaf, dp& StressSenescence, dp& TimeSenescence,
    bool& NoMoreCrop, dp& TESTVAL);

void DeterminePotentialBiomass(int32_t VirtualTimeCC, dp SumGDDadjCC,
    dp CO2i, dp GDDayi, dp& CCxWitheredTpotNoS, dp& BiomassUnlim);

void DetermineBiomassAndYield(int32_t DayNr, dp ETo, dp Tmin, dp Tmax, dp CO2i,
    dp GDDayi, dp Tact, dp SumKcTop, dp CGCref, dp GDDCGCref,
    dp Coeffb0, dp Coeffb1, dp Coeffb2, dp FracBiomassPotSF,
    dp Coeffb0Salt, dp Coeffb1Salt, dp Coeffb2Salt, dp StressTot_Salt,
    dp SumGDDadjCC, dp CCiActual, dp FracAssim, int32_t VirtualTimeCC,
    int32_t SumInterval, dp& Biomass, dp& BiomassPot, dp& BiomassUnlim,
    dp& BiomassTot, dp& YieldPart, dp WPi, dp HItimesBEF,
    dp ScorAT1, dp ScorAT2, dp HItimesAT1, dp HItimesAT2, dp HItimesAT,
    dp alfaHI, dp alfaHIAdj, dp& SumKcTopStress, dp& SumKci,
    dp& WeedRCi, dp& CCiActualWeedInfested, dp& TactWeedInfested,
    int32_t& StressSFadjNEW, int32_t& PreviousStressLevel,
    bool& Store, bool& Mobilize, dp& ToMobilize, dp& Bmobilized,
    dp& Bin, dp& Bout, dp& TESTVALY);

} // namespace AquaCrop
