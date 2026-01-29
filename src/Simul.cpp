#include "AquaCrop/Simul.h"
#include "AquaCrop/Global.h"
#include "AquaCrop/Utils.h"
#include "AquaCrop/ClimProcessing.h"
#include "AquaCrop/TempProcessing.h"
#include "AquaCrop/PrepareFertilitySalinity.h"
#include "AquaCrop/RootUnit.h"
#include "AquaCrop/InfoResults.h"
#include "AquaCrop/InitialSettings.h"
#include "AquaCrop/ProjectInput.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

namespace AquaCrop {

// Enums specific to this module
enum control_type : intEnum {
    control_begin_day = 0,
    control_end_day = 1,
};

// --- Forward Declarations of local functions ---
void CheckWaterSaltBalance(int32_t dayi, dp InfiltratedRain, control_type control, dp InfiltratedIrrigation, dp InfiltratedStorage, dp& Surf0, dp& ECInfilt, dp& ECdrain, dp& HorizontalWaterFlow, dp& HorizontalSaltFlow, dp& SubDrain);
void calculate_drainage();
void calculate_runoff(dp MaxDepth);
void Calculate_irrigation(dp& SubDrain, int32_t& TargetTimeVal, int32_t TargetDepthVal);
void CalculateEffectiveRainfall(dp& SubDrain);
void calculate_CapillaryRise(dp& CRwater, dp& CRsalt);
void calculate_saltcontent(dp InfiltratedRain, dp InfiltratedIrrigation, dp InfiltratedStorage, dp SubDrain, dp ECInfilt, int32_t dayi);
void CheckGermination();
void EffectSoilFertilitySalinityStress(int32_t& StressSFadjNEW, dp Coeffb0Salt, dp Coeffb1Salt, dp Coeffb2Salt, int32_t NrDayGrow, dp StressTotSaltPrev, int32_t VirtualTimeCC);
void DetermineCCiGDD(dp CCxTotal, dp CCoTotal, dp& StressLeaf, dp FracAssim, bool MobilizationON, bool StorageON, dp SumGDDAdjCC, int32_t VirtualTimeCC, dp& StressSenescence, dp& TimeSenescence, bool& NoMoreCrop, dp CDCTotal, dp GDDayFraction, dp GDDayi, dp GDDCDCTotal, int32_t GDDTadj);
void DetermineCCi(dp CCxTotal, dp CCoTotal, dp& StressLeaf, dp FracAssim, bool MobilizationON, bool StorageON, int32_t Tadj, int32_t VirtualTimeCC, dp& StressSenescence, dp& TimeSenescence, bool& NoMoreCrop, dp CDCTotal, dp DayFraction, dp GDDCDCTotal, dp& TESTVAL);
void calculate_Extra_runoff(dp& InfiltratedRain, dp& InfiltratedIrrigation, dp& InfiltratedStorage, dp& SubDrain);
void calculate_surfacestorage(dp& InfiltratedRain, dp& InfiltratedIrrigation, dp& InfiltratedStorage, dp& ECinfilt, dp SubDrain, int32_t dayi);
void calculate_infiltration(dp& InfiltratedRain, dp& InfiltratedIrrigation, dp& InfiltratedStorage, dp& SubDrain);
void PrepareStage2();
void PrepareStage1();
void AdjustEpotMulchWettedSurface(int32_t dayi, dp EpotTot, dp& Epot, dp& EvapWCsurface);
void CalculateEvaporationSurfaceWater();
void CalculateSoilEvaporationStage1();
void CalculateSoilEvaporationStage2();
void surface_transpiration(dp Coeffb0Salt, dp Coeffb1Salt, dp Coeffb2Salt);
void calculate_transpiration(dp Tpot, dp Coeffb0Salt, dp Coeffb1Salt, dp Coeffb2Salt);
void FeedbackCC();
void HorizontalInflowGWTable(dp DepthGWTmeter, dp& HorizontalSaltFlow, dp& HorizontalWaterFlow);
void ConcentrateSalts();
void AdjustpStomatalToETo(dp MeanETo, dp& pStomatULAct);

// --- Core BUDGET_module ---
void Budget_module(int32_t DayNr, int32_t TargetTimeVal, int32_t TargetDepthVal,
    int32_t VirtualTimeCC, int32_t SumInterval, int32_t DayLastCut,
    int32_t NrDayGrow, int32_t Tadj, int32_t GDDTadj, dp GDDayi,
    dp CGCref, dp GDDCGCref, dp CO2i, dp CCxTotal, dp CCoTotal, dp CDCTotal,
    dp GDDCDCTotal, dp SumGDDadjCC, dp Coeffb0Salt, dp Coeffb1Salt, dp Coeffb2Salt,
    dp StressTotSaltPrev, dp DayFraction, dp GDDayFraction, dp FracAssim,
    int32_t StressSFadjNEW, bool StorageON, bool MobilizationON,
    dp& StressLeaf, dp& StressSenescence, dp& TimeSenescence,
    bool& NoMoreCrop, dp& TESTVAL)
{
    control_type control;
    dp InfiltratedRain, InfiltratedIrrigation, InfiltratedStorage, EpotTot, SubDrain;
    int32_t DAP;
    dp ECInfilt;
    bool WaterTableInProfile;
    dp HorizontalWaterFlow, HorizontalSaltFlow;
    bool SWCtopSoilConsidered_temp;
    dp EvapWCsurf_temp, CRwater_temp, Tpot_temp, Epot_temp;
    std::vector<CompartmentIndividual> Comp_temp = Compartment;
    dp Crop_pActStom_temp;
    dp CRsalt_temp, ECdrain_temp, Surf0_temp;
    int32_t TargetTimeVal_loc = TargetTimeVal;
    int32_t StressSFadjNEW_loc = StressSFadjNEW;

    // 1. Soil water balance
    control = control_begin_day;
    ECdrain_temp = ECdrain;
    Surf0_temp = Surf0;

    InfiltratedRain = 0.0;
    InfiltratedIrrigation = 0.0;
    InfiltratedStorage = 0.0;
    HorizontalWaterFlow = 0.0;
    HorizontalSaltFlow = 0.0;
    ECInfilt = 0.0;
    SubDrain = 0.0;

    CheckWaterSaltBalance(DayNr, InfiltratedRain, control,
                          InfiltratedIrrigation, InfiltratedStorage,
                          Surf0_temp, ECInfilt, ECdrain_temp,
                          HorizontalWaterFlow, HorizontalSaltFlow,
                          SubDrain);
    ECdrain = ECdrain_temp;
    Surf0 = Surf0_temp;

    // 2. Adjustments in presence of Groundwater table
    CheckForWaterTableInProfile(ZiAqua / 100.0, Compartment, WaterTableInProfile);
    Comp_temp = Compartment;
    CalculateAdjustedFC(ZiAqua / 100.0, Comp_temp);
    Compartment = Comp_temp;

    // 3. Drainage
    calculate_drainage();

    // 4. Runoff
    if (Management.BundHeight < 0.001) {
        DaySubmerged = 0;
        if (Management.RunoffOn && Rain > 0.1) {
            calculate_runoff(simulparam.RunoffDepth);
        }
    }

    // 5. Infiltration (Rain and Irrigation)
    if (RainRecord.DataType == datatype::decadely || RainRecord.DataType == datatype::monthly) {
        CalculateEffectiveRainfall(SubDrain);
    }

    if (IrriMode_Val == IrriMode::Generate && Irrigation < ac_zero_threshold && TargetTimeVal_loc != undef_int) {
        Calculate_irrigation(SubDrain, TargetTimeVal_loc, TargetDepthVal);
    }
    if (Management.BundHeight >= 0.01) {
        calculate_surfacestorage(InfiltratedRain, InfiltratedIrrigation, InfiltratedStorage, ECInfilt, SubDrain, DayNr);
    } else {
        calculate_Extra_runoff(InfiltratedRain, InfiltratedIrrigation, InfiltratedStorage, SubDrain);
    }
    calculate_infiltration(InfiltratedRain, InfiltratedIrrigation, InfiltratedStorage, SubDrain);

    // 6. Capillary Rise
    CRwater_temp = CRwater;
    CRsalt_temp = CRsalt;
    calculate_CapillaryRise(CRwater_temp, CRsalt_temp);
    CRwater = CRwater_temp;
    CRsalt = CRsalt_temp;

    // 7. Salt balance
    calculate_saltcontent(InfiltratedRain, InfiltratedIrrigation, InfiltratedStorage, SubDrain, ECInfilt, DayNr);

    // 8. Check Germination
    if (!Simulation.Germinate && DayNr >= crop.Day1) {
        CheckGermination();
    }

    // 9. Determine effect of soil fertiltiy and soil salinity stress
    if (!NoMoreCrop) {
        EffectSoilFertilitySalinityStress(StressSFadjNEW_loc, Coeffb0Salt, Coeffb1Salt, Coeffb2Salt, NrDayGrow, StressTotSaltPrev, VirtualTimeCC);
    }

    // 10. Canopy Cover (CC)
    if (!NoMoreCrop) {
        SWCtopSoilConsidered_temp = Simulation.SWCtopSoilConsidered;
        DetermineRootZoneWC(RootingDepth, SWCtopSoilConsidered_temp);
        Simulation.SWCtopSoilConsidered = SWCtopSoilConsidered_temp;
        
        switch (crop.ModeCycle) {
            case modeCycle::GDDays:
                DetermineCCiGDD(CCxTotal, CCoTotal, StressLeaf, FracAssim,
                                MobilizationON, StorageON, SumGDDadjCC,
                                VirtualTimeCC, StressSenescence,
                                TimeSenescence, NoMoreCrop, CDCTotal,
                                GDDayFraction, GDDayi, GDDCDCTotal, GDDTadj);
                break;
            default:
                DetermineCCi(CCxTotal, CCoTotal, StressLeaf, FracAssim,
                             MobilizationON, StorageON, Tadj, VirtualTimeCC,
                             StressSenescence, TimeSenescence, NoMoreCrop,
                             CDCTotal, DayFraction, GDDCDCTotal, TESTVAL);
                break;
        }
    }

    // 11. Determine Tpot and Epot
    if (crop.ModeCycle == modeCycle::CalendarDays) {
        DAP = VirtualTimeCC;
    } else {
        DAP = SumCalendarDays(roundc(SumGDDadjCC, 1), crop.Day1,
                              crop.Tbase, crop.Tupper,
                              simulparam.Tmin, simulparam.Tmax);
        DAP = DAP + Simulation.DelayedDays;
    }

    Tpot_temp = Tpot;
    CalculateETpot(DAP, crop.DaysToGermination,
                        crop.DaysToFullCanopy, crop.DaysToSenescence,
                        crop.DaysToHarvest, DayLastCut, CCiActual,
                        ETo, crop.KcTop, crop.KcDecline,
                        crop.CCxAdjusted, crop.CCxWithered,
                        static_cast<dp>(crop.CCEffectEvapLate), CO2i,
                        GDDayi, crop.GDtranspLow, Tpot_temp, EpotTot);
    Tpot = Tpot_temp;
    Epot = EpotTot;

    Crop_pActStom_temp = crop.pActStom;
    AdjustpStomatalToETo(ETo, Crop_pActStom_temp);
    crop.pActStom = Crop_pActStom_temp;

    // 12. Evaporation
    if (!PreDay) {
        PrepareStage2();
    }
    if (Rain > 0.0 || (Irrigation > 0.0 && IrriMode_Val != IrriMode::Inet)) {
        PrepareStage1();
    }
    EvapWCsurf_temp = Simulation.EvapWCsurf;
    Epot_temp = Epot;
    AdjustEpotMulchWettedSurface(DayNr, EpotTot, Epot_temp, EvapWCsurf_temp);
    Epot = Epot_temp;
    Simulation.EvapWCsurf = EvapWCsurf_temp;
    
    if ((RainRecord.DataType == datatype::decadely || RainRecord.DataType == datatype::monthly) && simulparam.EffectiveRain.RootNrEvap > 0) {
        Epot = Epot * (std::exp((1.0/static_cast<dp>(simulparam.EffectiveRain.RootNrEvap)) * std::log((Soil.REW+1.0)/20.0)));
    }

    Eact = 0.0;
    if (Epot > 0.0) {
        if (SurfaceStorage > 0.0) {
            CalculateEvaporationSurfaceWater();
        }
        if ((std::abs(Epot - Eact) > ac_zero_threshold) && Simulation.EvapWCsurf > 0.0) {
            CalculateSoilEvaporationStage1();
        }
        if (std::abs(Epot - Eact) > ac_zero_threshold) {
            CalculateSoilEvaporationStage2();
        }
    }
    
    if ((RainRecord.DataType == datatype::decadely || RainRecord.DataType == datatype::monthly) && simulparam.EffectiveRain.RootNrEvap > 0.0) {
        Epot = Epot / (std::exp((1.0/static_cast<dp>(simulparam.EffectiveRain.RootNrEvap)) * std::log((Soil.REW+1.0)/20.0)));
    }

    // 13. Transpiration
    if (!NoMoreCrop && RootingDepth > 0.0001) {
        if (SurfaceStorage > 0.0 && (crop.AnaeroPoint == 0 || DaySubmerged < simulparam.DelayLowOxygen)) {
            surface_transpiration(Coeffb0Salt, Coeffb1Salt, Coeffb2Salt);
        } else {
            calculate_transpiration(Tpot, Coeffb0Salt, Coeffb1Salt, Coeffb2Salt);
        }
    }
    if (SurfaceStorage < ac_zero_threshold) {
        DaySubmerged = 0;
    }
    FeedbackCC();

    // 14. Adjustment to groundwater table
    if (WaterTableInProfile) {
        HorizontalInflowGWTable(ZiAqua / 100.0, HorizontalSaltFlow, HorizontalWaterFlow);
    }

    // 15. Salt concentration
    ConcentrateSalts();

    // 16. Soil water balance
    control = control_end_day;
    ECdrain_temp = ECdrain;
    Surf0_temp = Surf0;
    CheckWaterSaltBalance(DayNr, InfiltratedRain, control,
                               InfiltratedIrrigation, InfiltratedStorage,
                               Surf0_temp, ECInfilt, ECdrain_temp,
                               HorizontalWaterFlow, HorizontalSaltFlow,
                               SubDrain);
    ECdrain = ECdrain_temp;
    Surf0 = Surf0_temp;
}

void AdjustpStomatalToETo(dp MeanETo, dp& pStomatULAct) {
    if (crop.CropPMethod == pMethod::FAOCorrection) {
        pStomatULAct = crop.pdef + (5.0 - MeanETo) * simulparam.pAdjFAO;
    } else {
        pStomatULAct = crop.pdef;
    }
    if (pStomatULAct < 0.0) pStomatULAct = 0.0;
    if (pStomatULAct > 1.0) pStomatULAct = 1.0;
}

void DetermineRootZoneWC(dp RootingDepth, bool& ZtopSWCconsidered) {
    dp Ztop, Zbot, depthi, theta, theta_fc, theta_wp, theta_sat;
    int32_t compi, layeri;

    RootZoneWC.Actual = 0.0;
    RootZoneWC.FC = 0.0;
    RootZoneWC.WP = 0.0;
    RootZoneWC.SAT = 0.0;

    depthi = 0.0;
    for (compi = 1; compi <= NrCompartments; ++compi) {
        layeri = Compartment[compi - 1].Layer;
        theta = Compartment[compi - 1].theta;
        theta_fc = soillayer[layeri - 1].FC / 100.0;
        theta_wp = soillayer[layeri - 1].WP / 100.0;
        theta_sat = soillayer[layeri - 1].SAT / 100.0;

        Ztop = depthi;
        depthi += Compartment[compi - 1].Thickness;
        Zbot = depthi;

        if (Zbot <= RootingDepth) {
            RootZoneWC.Actual += theta * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
            RootZoneWC.FC += theta_fc * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
            RootZoneWC.WP += theta_wp * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
            RootZoneWC.SAT += theta_sat * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
        } else if (Ztop < RootingDepth) {
            RootZoneWC.Actual += theta * 1000.0 * (RootingDepth - Ztop) * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
            RootZoneWC.FC += theta_fc * 1000.0 * (RootingDepth - Ztop) * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
            RootZoneWC.WP += theta_wp * 1000.0 * (RootingDepth - Ztop) * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
            RootZoneWC.SAT += theta_sat * 1000.0 * (RootingDepth - Ztop) * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
        }
        if (depthi >= RootingDepth) break;
    }
}

void FeedbackCC() {
    dp CCiActual_local;
    CCiActual_local = CCiActual;
    // simplified: no weeds for now
    if (CCiActual_local > crop.CCxAdjusted) {
        CCiActual_local = crop.CCxAdjusted;
    }
    CCiActual = CCiActual_local;
}

void ConcentrateSalts() {
    int32_t compi, celli;
    dp SaltSolub;

    SaltSolub = static_cast<dp>(simulparam.SaltSolub);
    for (compi = 1; compi <= NrCompartments; ++compi) {
        for (celli = 1; celli <= soillayer[Compartment[compi - 1].Layer - 1].SCP1; ++celli) {
            SaltSolutionDeposit(Compartment[compi - 1].Thickness * 1000.0, Compartment[compi - 1].Salt[celli - 1], Compartment[compi - 1].Depo[celli - 1]);
        }
    }
}

void HorizontalInflowGWTable(dp DepthGWTmeter, dp& HorizontalSaltFlow, dp& HorizontalWaterFlow) {
    // simplified for now: no horizontal flow
    HorizontalSaltFlow = 0.0;
    HorizontalWaterFlow = 0.0;
}

void PrepareStage1() {
    Simulation.EvapWCsurf = (soillayer[0].SAT / 100.0); // simplified: reset to SAT
}

void PrepareStage2() {
    Simulation.EvapLimitON = true;
}

void AdjustEpotMulchWettedSurface(int32_t dayi, dp EpotTot, dp& Epot, dp& EvapWCsurface) {
    dp fMulch = 1.0;
    if (Management.Mulch > 0) {
        fMulch = 1.0 - (static_cast<dp>(Management.Mulch) / 100.0) * (static_cast<dp>(Management.EffectMulchInS) / 100.0);
    }
    Epot = EpotTot * fMulch;
    EvapWCsurface = Simulation.EvapWCsurf;
}

void CalculateEvaporationSurfaceWater() {

    dp Esurf;

    Esurf = Epot;

    if (Esurf > SurfaceStorage) {

        Esurf = SurfaceStorage;

    }

    SurfaceStorage -= Esurf;

    Eact += Esurf;

}



void CalculateSoilEvaporationStage1() {



    dp Estage1;



    Estage1 = Epot - Eact;



    if (Estage1 > Simulation.EvapWCsurf) {



        Estage1 = Simulation.EvapWCsurf;



    }



    Simulation.EvapWCsurf -= Estage1;



    Eact += Estage1;



}







void CalculateSoilEvaporationStage2() {







    dp Estage2, Wrel, Kr;







    int32_t layeri;















    layeri = Compartment[0].Layer;







    Wrel = (Compartment[0].theta - soillayer[layeri - 1].WP / 100.0) / (soillayer[layeri - 1].FC / 100.0 - soillayer[layeri - 1].WP / 100.0);







    Kr = SoilEvaporationReductionCoefficient(Wrel, static_cast<dp>(simulparam.EvapDeclineFactor));







    Estage2 = Kr * (Epot - Eact);







    Compartment[0].theta -= Estage2 / (1000.0 * Compartment[0].Thickness);







    Eact += Estage2;







}















void surface_transpiration(dp Coeffb0Salt, dp Coeffb1Salt, dp Coeffb2Salt) {















    dp Tsurf;















    Tsurf = Tpot;















    // simplified: assume no stress when submerged















    Tact += Tsurf;















}































void calculate_transpiration(dp Tpot, dp Coeffb0Salt, dp Coeffb1Salt, dp Coeffb2Salt) {































    dp Ks, Wrel, pULActual, pLLActual;































    int32_t layeri;































































    // simplified: assume stress based on average root zone WC































    Wrel = (RootZoneWC.Actual - RootZoneWC.WP) / (RootZoneWC.FC - RootZoneWC.WP);































    pULActual = crop.pdef;































    pLLActual = 1.0;































    Ks = KsAny(Wrel, pULActual, pLLActual, crop.KsShapeFactorStomata);































    Tact = Ks * Tpot;































    































    // adjust theta in root zone (simplified distribution)































    dp excess = Tact;































    for (int32_t compi = 1; compi <= NrCompartments; ++compi) {































        layeri = Compartment[compi - 1].Layer;































        dp Tcomp = (Compartment[compi - 1].Thickness / RootingDepth) * Tact;































        Compartment[compi - 1].theta -= Tcomp / (1000.0 * Compartment[compi - 1].Thickness);































        excess -= Tcomp;































        if (Compartment[compi - 1].theta < soillayer[layeri - 1].WP / 100.0) {































            Compartment[compi - 1].theta = soillayer[layeri - 1].WP / 100.0;































        }































    }































}































































// --- Placeholder implementations ---// Note: Actual implementation should be moved here as they are ported.

void CheckWaterSaltBalance(int32_t dayi, dp InfiltratedRain, control_type control, dp InfiltratedIrrigation, dp InfiltratedStorage, dp& Surf0, dp& ECInfilt, dp& ECdrain, dp& HorizontalWaterFlow, dp& HorizontalSaltFlow, dp& SubDrain) {
    dp Surf1, ECw;

    switch (control) {
    case control_begin_day:
        TotalWaterContent.BeginDay = 0.0; // mm
        Surf0 = SurfaceStorage; // mm
        TotalSaltContent.BeginDay = 0.0; // Mg/ha
        for (int32_t compi = 1; compi <= NrCompartments; ++compi) {
            TotalWaterContent.BeginDay += Compartment[compi-1].theta * 1000.0 *
                 Compartment[compi-1].Thickness * (1.0 -
                  soillayer[Compartment[compi-1].Layer - 1].GravelVol / 100.0);
            Compartment[compi-1].fluxout = 0.0;
            for (int32_t celli = 1; celli <= soillayer[Compartment[compi-1].Layer - 1].SCP1; ++celli) {
                TotalSaltContent.BeginDay += (Compartment[compi-1].Salt[celli-1] +
                          Compartment[compi-1].Depo[celli-1]) / 100.0; // Mg/ha
            }
        }
        Drain = 0.0;
        Runoff = 0.0;
        Tact = 0.0;
        Infiltrated = 0.0;
        ECInfilt = 0.0;
        SubDrain = 0.0;
        ECdrain = 0.0;
        HorizontalWaterFlow = 0.0;
        HorizontalSaltFlow = 0.0;
        CRwater = 0.0;
        CRsalt = 0.0;
        break;

    case control_end_day:
        Infiltrated = InfiltratedRain + InfiltratedIrrigation + InfiltratedStorage;
        for (int32_t layeri = 1; layeri <= Soil.NrSoilLayers; ++layeri) {
            soillayer[layeri-1].WaterContent = 0.0;
        }
        TotalWaterContent.EndDay = 0.0;
        Surf1 = SurfaceStorage;
        TotalSaltContent.EndDay = 0.0;

        // quality of irrigation water
        if (dayi < crop.Day1) {
            ECw = IrriECw.PreSeason;
        } else {
            ECw = Simulation.IrriECw;
            if (dayi > crop.DayN) {
                ECw = IrriECw.PostSeason;
            }
        }

        for (int32_t compi = 1; compi <= NrCompartments; ++compi) {
            TotalWaterContent.EndDay += Compartment[compi-1].theta * 1000.0 *
                 Compartment[compi-1].Thickness * (1.0 -
                  soillayer[Compartment[compi-1].Layer - 1].GravelVol / 100.0);
            soillayer[Compartment[compi-1].Layer - 1].WaterContent +=
                    Compartment[compi-1].theta * 1000.0 *
                          Compartment[compi-1].theta * (1.0 -
                       soillayer[Compartment[compi-1].Layer - 1].GravelVol / 100.0);
            for (int32_t celli = 1; celli <= soillayer[Compartment[compi-1].Layer - 1].SCP1; ++celli) {
                TotalSaltContent.EndDay += (Compartment[compi-1].Salt[celli-1] +
                      Compartment[compi-1].Depo[celli-1]) / 100.0; // Mg/ha
            }
        }
        TotalWaterContent.ErrorDay = TotalWaterContent.BeginDay + Surf0 - (TotalWaterContent.EndDay + Drain + Runoff + Eact + Tact + Surf1 - Rain - Irrigation - CRwater - HorizontalWaterFlow);
        TotalSaltContent.ErrorDay = TotalSaltContent.BeginDay - TotalSaltContent.EndDay + InfiltratedIrrigation * ECw * equiv / 100.0 + InfiltratedStorage * ECInfilt * equiv / 100.0 - Drain * ECdrain * equiv / 100.0 + CRsalt / 100.0 + HorizontalSaltFlow;
        
        SumWaBal.Epot += Epot;
        SumWaBal.Tpot += Tpot;
        SumWaBal.Rain += Rain;
        SumWaBal.Irrigation += Irrigation;
        SumWaBal.Infiltrated += Infiltrated;
        SumWaBal.Runoff += Runoff;
        SumWaBal.Drain += Drain;
        SumWaBal.Eact += Eact;
        SumWaBal.Tact += Tact;
        SumWaBal.TrW += TactWeedInfested;
        SumWaBal.CRwater += CRwater;

        if (((dayi - Simulation.DelayedDays) >= crop.Day1) && ((dayi - Simulation.DelayedDays) <= crop.DayN)) {
            if (SumWaBal.Biomass > 0.0) {
                if (CCiActual > 0.0) {
                    SumWaBal.ECropCycle += Eact;
                }
            } else {
                SumWaBal.ECropCycle += Eact;
            }
        }
        SumWaBal.CRsalt += CRsalt / 100.0;
        SumWaBal.SaltIn += (InfiltratedIrrigation * ECw + InfiltratedStorage * ECInfilt) * equiv / 100.0;
        SumWaBal.SaltOut += Drain * ECdrain * equiv / 100.0;
        break;
    }
}
dp calculate_delta_theta(dp theta_in, dp thetaAdjFC, int32_t NrLayer) {
    dp DeltaX, theta, theta_sat, theta_fc;

    theta = theta_in;
    theta_sat = soillayer[NrLayer - 1].SAT / 100.0;
    theta_fc = soillayer[NrLayer - 1].FC / 100.0;
    if (theta > theta_sat) {
        theta = theta_sat;
    }
    if (theta <= thetaAdjFC) {
        DeltaX = 0.0;
    } else {
        DeltaX = soillayer[NrLayer - 1].tau * (theta_sat - theta_fc) * (std::exp(theta - theta_fc) - 1.0) / (std::exp(theta_sat - theta_fc) - 1.0);
        if ((theta - DeltaX) < thetaAdjFC) {
            DeltaX = theta - thetaAdjFC;
        }
    }
    return DeltaX;
}

dp calculate_theta_from_delta(dp delta_theta, dp thetaAdjFC, int32_t NrLayer) {
    dp ThetaX, theta_sat, theta_fc, tau;

    theta_sat = soillayer[NrLayer - 1].SAT / 100.0;
    theta_fc = soillayer[NrLayer - 1].FC / 100.0;
    tau = soillayer[NrLayer - 1].tau;
    if (delta_theta <= 1e-12) {
        ThetaX = thetaAdjFC;
    } else if (tau > 0.0) {
        ThetaX = theta_fc + std::log(1.0 + delta_theta * (std::exp(theta_sat - theta_fc) - 1.0) / (tau * (theta_sat - theta_fc)));
        if (ThetaX < thetaAdjFC) {
            ThetaX = thetaAdjFC;
        }
    } else {
        ThetaX = theta_sat + 0.1;
    }
    return ThetaX;
}

void CheckDrainsum(int32_t layeri, dp& drainsum, dp& excess) {
    if (drainsum > soillayer[layeri - 1].InfRate) {
        excess = excess + drainsum - soillayer[layeri - 1].InfRate;
        drainsum = soillayer[layeri - 1].InfRate;
    }
}

void calculate_drainage() {
    int32_t compi, layeri, pre_nr;
    dp drainsum, delta_theta, drain_comp, drainmax, theta_x, excess;
    dp pre_thick;
    bool drainability;

    drainsum = 0.0;
    for (compi = 1; compi <= NrCompartments; ++compi) {
        layeri = Compartment[compi - 1].Layer;
        if (Compartment[compi - 1].theta > Compartment[compi - 1].FCadj / 100.0) {
            delta_theta = calculate_delta_theta(Compartment[compi - 1].theta, (Compartment[compi - 1].FCadj / 100.0), layeri);
        } else {
            delta_theta = 0.0;
        }
        drain_comp = delta_theta * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);

        excess = 0.0;
        pre_thick = 0.0;
        for (int32_t i = 1; i < compi; ++i) {
            pre_thick += Compartment[i - 1].Thickness;
        }
        drainmax = delta_theta * 1000.0 * pre_thick * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
        drainability = (drainsum <= drainmax);

        if (drainability) {
            Compartment[compi - 1].theta -= delta_theta;
            drainsum += drain_comp;
            CheckDrainsum(layeri, drainsum, excess);
        } else {
            delta_theta = drainsum / (1000.0 * pre_thick * (1.0 - soillayer[layeri - 1].GravelVol / 100.0));
            theta_x = calculate_theta_from_delta(delta_theta, (Compartment[compi - 1].FCadj / 100.0), layeri);

            if (theta_x <= soillayer[layeri - 1].SAT / 100.0) {
                Compartment[compi - 1].theta += drainsum / (1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0));
                if (Compartment[compi - 1].theta > theta_x) {
                    drainsum = (Compartment[compi - 1].theta - theta_x) * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
                    delta_theta = calculate_delta_theta(theta_x, (Compartment[compi - 1].FCadj / 100.0), layeri);
                    drainsum += delta_theta * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
                    CheckDrainsum(layeri, drainsum, excess);
                    Compartment[compi - 1].theta = theta_x - delta_theta;
                } else if (Compartment[compi - 1].theta > Compartment[compi - 1].FCadj / 100.0) {
                    delta_theta = calculate_delta_theta(Compartment[compi - 1].theta, (Compartment[compi - 1].FCadj / 100.0), layeri);
                    Compartment[compi - 1].theta -= delta_theta;
                    drainsum = delta_theta * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
                    CheckDrainsum(layeri, drainsum, excess);
                } else {
                    drainsum = 0.0;
                }
            }

            if (theta_x > soillayer[layeri - 1].SAT / 100.0) {
                Compartment[compi - 1].theta += drainsum / (1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0));
                if (Compartment[compi - 1].theta <= soillayer[layeri - 1].SAT / 100.0) {
                    if (Compartment[compi - 1].theta > Compartment[compi - 1].FCadj / 100.0) {
                        delta_theta = calculate_delta_theta(Compartment[compi - 1].theta, (Compartment[compi - 1].FCadj / 100.0), layeri);
                        Compartment[compi - 1].theta -= delta_theta;
                        drainsum = delta_theta * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
                        CheckDrainsum(layeri, drainsum, excess);
                    } else {
                        drainsum = 0.0;
                    }
                }
                if (Compartment[compi - 1].theta > soillayer[layeri - 1].SAT / 100.0) {
                    excess = (Compartment[compi - 1].theta - (soillayer[layeri - 1].SAT / 100.0)) * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
                    delta_theta = calculate_delta_theta(Compartment[compi - 1].theta, (Compartment[compi - 1].FCadj / 100.0), layeri);
                    Compartment[compi - 1].theta = soillayer[layeri - 1].SAT / 100.0 - delta_theta;
                    drain_comp = delta_theta * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
                    drainmax = delta_theta * 1000.0 * pre_thick * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
                    if (drainmax > excess) {
                        drainmax = excess;
                    }
                    excess -= drainmax;
                    drainsum = drainmax + drain_comp;
                    CheckDrainsum(layeri, drainsum, excess);
                }
            }
        }

        Compartment[compi - 1].fluxout = drainsum;

        if (excess > 0.0) {
            pre_nr = compi + 1;
            while (true) {
                pre_nr--;
                layeri = Compartment[pre_nr - 1].Layer;
                if (pre_nr < compi) {
                    Compartment[pre_nr - 1].fluxout -= excess;
                }
                Compartment[pre_nr - 1].theta += excess / (1000.0 * Compartment[pre_nr - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0));
                if (Compartment[pre_nr - 1].theta > soillayer[layeri - 1].SAT / 100.0) {
                    excess = (Compartment[pre_nr - 1].theta - soillayer[layeri - 1].SAT / 100.0) * 1000.0 * Compartment[pre_nr - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
                    Compartment[pre_nr - 1].theta = soillayer[layeri - 1].SAT / 100.0;
                } else {
                    excess = 0.0;
                }
                if (std::abs(excess) < 1e-12 || pre_nr == 1) break;
            }
        }
    }
    Drain = drainsum;
}
void calculate_weighting_factors(dp Depth, std::vector<CompartmentIndividual>& Compartment_local) {
    int32_t compi;
    dp CumDepth, xx, wx;

    CumDepth = 0.0;
    xx = 0.0;
    for (compi = 1; compi <= NrCompartments; ++compi) {
        CumDepth += Compartment_local[compi - 1].Thickness;
        if (CumDepth > Depth) {
            CumDepth = Depth;
        }
        wx = 1.016 * (1.0 - std::exp(-4.16 * CumDepth / Depth));
        Compartment_local[compi - 1].WFactor = wx - xx;
        if (Compartment_local[compi - 1].WFactor > 1.0) {
            Compartment_local[compi - 1].WFactor = 1.0;
        }
        if (Compartment_local[compi - 1].WFactor < 0.0) {
            Compartment_local[compi - 1].WFactor = 0.0;
        }
        xx = wx;
        if (CumDepth >= Depth) break;
    }
    for (int32_t i = compi + 1; i <= NrCompartments; ++i) {
        Compartment_local[i - 1].WFactor = 0.0;
    }
}

void calculate_relative_wetness_topsoil(dp& SUM, dp MaxDepth) {
    dp CumDepth, theta;
    int32_t compi, layeri;
    std::vector<CompartmentIndividual> Compartment_temp = Compartment;

    calculate_weighting_factors(MaxDepth, Compartment_temp);
    SUM = 0.0;
    CumDepth = 0.0;

    for (compi = 1; compi <= NrCompartments; ++compi) {
        layeri = Compartment_temp[compi - 1].Layer;
        CumDepth += Compartment_temp[compi - 1].Thickness;
        if (Compartment_temp[compi - 1].theta < soillayer[layeri - 1].WP / 100.0) {
            theta = soillayer[layeri - 1].WP / 100.0;
        } else {
            theta = Compartment_temp[compi - 1].theta;
        }
        SUM += Compartment_temp[compi - 1].WFactor * (theta - soillayer[layeri - 1].WP / 100.0) / (soillayer[layeri - 1].FC / 100.0 - soillayer[layeri - 1].WP / 100.0);
        if (CumDepth >= MaxDepth) break;
    }

    if (SUM < 0.0) {
        SUM = 0.0;
    }
    if (SUM > 1.0) {
        SUM = 1.0;
    }
}

void calculate_runoff(dp MaxDepth) {
    dp SUM, CNA, Shower, term, S;
    int8_t CN2, CN1, CN3;

    CN2 = static_cast<int8_t>(roundc(static_cast<dp>(Soil.CNvalue) * (100.0 + static_cast<dp>(Management.CNcorrection)) / 100.0, 1));
    if (RainRecord.DataType == datatype::daily) {
        if (simulparam.CNcorrection) {
            calculate_relative_wetness_topsoil(SUM, MaxDepth);
            DetermineCNIandIII(CN2, CN1, CN3);
            CNA = static_cast<dp>(roundc(static_cast<dp>(CN1) + (static_cast<dp>(CN3) - static_cast<dp>(CN1)) * SUM, 1));
        } else {
            CNA = static_cast<dp>(CN2);
        }
        Shower = Rain;
    } else {
        CNA = static_cast<dp>(CN2);
        Shower = (Rain * 10.0) / static_cast<dp>(simulparam.EffectiveRain.ShowersInDecade);
    }
    S = 254.0 * (100.0 / CNA - 1.0);
    term = Shower - (static_cast<dp>(simulparam.IniAbstract) / 100.0) * S;
    if (term <= 1e-12) {
        Runoff = 0.0;
    } else {
        Runoff = std::pow(term, 2) / (Shower + (1.0 - (static_cast<dp>(simulparam.IniAbstract) / 100.0)) * S);
    }
    if ((Runoff > 0.0) && ((RainRecord.DataType == datatype::decadely) || (RainRecord.DataType == datatype::monthly))) {
        if (Runoff >= Shower) {
            Runoff = Rain;
        } else {
            Runoff = Runoff * (static_cast<dp>(simulparam.EffectiveRain.ShowersInDecade) / 10.14);
            if (Runoff > Rain) {
                Runoff = Rain;
            }
        }
    }
}
void Calculate_irrigation(dp& SubDrain, int32_t& TargetTimeVal, int32_t TargetDepthVal) {
    dp depletion, RAW, TAW;

    depletion = RootZoneWC.FC - RootZoneWC.Actual;
    TAW = RootZoneWC.FC - RootZoneWC.WP;
    RAW = (static_cast<dp>(simulparam.PercRAW) / 100.0) * TAW;

    switch (GenerateTimeMode_Val) {
    case GenerateTimeMode::AllRAW:
        if (depletion >= RAW) {
            Irrigation = depletion;
        }
        break;
    case GenerateTimeMode::FixInt:
        // simplified
        break;
    default:
        break;
    }
}

// --- Placeholder implementations ---
void CalculateEffectiveRainfall(dp& SubDrain) {
    dp EffecRain = 0.0, ETcropMonth, RainMonth, DrainMax, Zr, depthi, DTheta, RestTheta;
    int32_t compi;

    if (Rain > 0.0) {
        EffecRain = Rain - Runoff;
        switch (simulparam.EffectiveRain.EffMethod) {
        case EffectiveRainMethod::percentage:
            EffecRain = (static_cast<dp>(simulparam.EffectiveRain.PercentEffRain) / 100.0) * (Rain - Runoff);
            break;
        case EffectiveRainMethod::usda:
            ETcropMonth = ((Epot + Tpot) * 30.0) / 25.4; // inch/month
            RainMonth = ((Rain - Runoff) * 30.0) / 25.4; // inch/Month
            if (RainMonth > 0.1) {
                EffecRain = (0.70917 * std::exp(0.82416 * std::log(RainMonth)) - 0.11556) * (std::exp(0.02426 * ETcropMonth * std::log(10.0))); // inch/month
            } else {
                EffecRain = RainMonth;
            }
            EffecRain = EffecRain * (25.4 / 30.0); // mm/day
            break;
        default:
            break;
        }
    }
    if (EffecRain < 0.0) {
        EffecRain = 0.0;
    }
    if (EffecRain > (Rain - Runoff)) {
        EffecRain = Rain - Runoff;
    }
    SubDrain = (Rain - Runoff) - EffecRain;

    if (SubDrain > 0.0) {
        DrainMax = soillayer[0].InfRate;
        if (SurfaceStorage > 0.0) {
            DrainMax = 0.0;
        } else {
            Zr = RootingDepth;
            if (Zr <= 1e-12) {
                Zr = static_cast<dp>(simulparam.EvapZmax) / 100.0;
            }
            compi = 0;
            depthi = 0.0;
            DTheta = (EffecRain / Zr) / 1000.0;
            while (true) {
                compi++;
                depthi += Compartment[compi - 1].Thickness;
                RestTheta = soillayer[Compartment[compi - 1].Layer - 1].SAT / 100.0 - (Compartment[compi - 1].theta + DTheta);
                if (RestTheta <= 1e-12) {
                    DrainMax = 0.0;
                }
                if (soillayer[Compartment[compi - 1].Layer - 1].InfRate < DrainMax) {
                    DrainMax = soillayer[Compartment[compi - 1].Layer - 1].InfRate;
                }
                if (depthi >= Zr || compi >= NrCompartments) break;
            }
        }
        if (SubDrain > DrainMax) {
            if (Management.BundHeight < 0.001) {
                Runoff += (SubDrain - DrainMax);
            }
            SubDrain = DrainMax;
        }
    }
}
void calculate_CapillaryRise(dp& CRwater, dp& CRsalt) {
    dp DepthGWTmeter, Ztop, Zbot, Zi, CRmax, CRactual, CRcomp, SaltCRcomp, delta_theta;
    int32_t compi, layeri;

    DepthGWTmeter = ZiAqua / 100.0;
    CRwater = 0.0;
    CRsalt = 0.0;

    if (DepthGWTmeter > 0.0) {
        Zbot = 0.0;
        for (compi = 1; compi <= NrCompartments; ++compi) {
            Ztop = Zbot;
            Zbot = Ztop + Compartment[compi - 1].Thickness;
            Zi = (Ztop + Zbot) / 2.0;
            layeri = Compartment[compi - 1].Layer;
            if (Zi < DepthGWTmeter) {
                CRmax = MaxCRatDepth(soillayer[layeri - 1].CRa, soillayer[layeri - 1].CRb, (soillayer[layeri - 1].tau * 1000.0), Zi, DepthGWTmeter);
                CRactual = CRmax; // simplified: actual CR is max CR for now
                delta_theta = soillayer[layeri - 1].SAT / 100.0 - Compartment[compi - 1].theta;
                CRcomp = delta_theta * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
                if (CRactual > CRcomp) {
                    CRactual = CRcomp;
                }
                Compartment[compi - 1].theta += CRactual / (1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0));
                CRwater += CRactual;
                SaltCRcomp = CRactual * ECiAqua * equiv / 100.0;
                SaltSolutionDeposit(Compartment[compi - 1].Thickness * 1000.0, Compartment[compi - 1].Salt[0], Compartment[compi - 1].Depo[0]);
                Compartment[compi - 1].Salt[0] += SaltCRcomp;
                CRsalt += SaltCRcomp;
            }
        }
    }
}
void Mixing(dp Dif, dp mm1, dp mm2, dp& Salt1, dp& Salt2, dp& Depo1, dp& Depo2) {
    dp EC1, EC2, ECmix;

    SaltSolutionDeposit(mm1, Salt1, Depo1);
    EC1 = Salt1 / (mm1 * equiv);
    SaltSolutionDeposit(mm2, Salt2, Depo2);
    EC2 = Salt2 / (mm2 * equiv);
    ECmix = (EC1 * mm1 + EC2 * mm2) / (mm1 + mm2);
    EC1 = EC1 + (ECmix - EC1) * Dif;
    EC2 = EC2 + (ECmix - EC2) * Dif;
    Salt1 = EC1 * mm1 * equiv;
    SaltSolutionDeposit(mm1, Salt1, Depo1);
    Salt2 = EC2 * mm2 * equiv;
    SaltSolutionDeposit(mm2, Salt2, Depo2);
}

void MoveSaltTo(CompartmentIndividual& Compx, int32_t celx, dp DS) {
    dp mmx;
    int32_t celx_local = celx;

    if (DS >= 0.0) {
        Compx.Salt[celx_local-1] += DS;
        mmx = soillayer[Compx.Layer - 1].Dx * 1000.0 * Compx.Thickness * (1.0 - soillayer[Compx.Layer - 1].GravelVol / 100.0);
        if (celx_local == (int32_t)soillayer[Compx.Layer - 1].SCP1) {
            mmx = 2.0 * mmx;
        }
        SaltSolutionDeposit(mmx, Compx.Salt[celx_local - 1], Compx.Depo[celx_local - 1]);
    } else {
        celx_local = (int32_t)soillayer[Compx.Layer - 1].SCP1;
        Compx.Salt[celx_local-1] += DS;
        mmx = 2.0 * soillayer[Compx.Layer - 1].Dx * 1000.0 * Compx.Thickness * (1.0 - soillayer[Compx.Layer - 1].GravelVol / 100.0);
        SaltSolutionDeposit(mmx, Compx.Salt[celx_local - 1], Compx.Depo[celx_local - 1]);
        mmx = mmx / 2.0;
        while (Compx.Salt[celx_local - 1] < 0.0) {
            if (celx_local == 1) {
                // Should not happen based on Fortran logic but handle it
                break;
            }
            Compx.Salt[celx_local - 2] += Compx.Salt[celx_local - 1];
            Compx.Salt[celx_local - 1] = 0.0;
            celx_local--;
            SaltSolutionDeposit(mmx, Compx.Salt[celx_local - 1], Compx.Depo[celx_local - 1]);
        }
    }
}

void calculate_saltcontent(dp InfiltratedRain, dp InfiltratedIrrigation, dp InfiltratedStorage, dp SubDrain, dp ECInfilt, int32_t dayi) {
    dp SaltIN, SaltOUT, mmIN, DeltaTheta, Theta, SAT, mm1, mm2, Dx, limit, Dif, UL;
    dp Zr, depthi, ECsubdrain, ECcel, DeltaZ, ECsw1, ECsw2, ECsw, SM1, SM2, DS1, DS2, DS;
    int32_t compi, celi, celiM1, Ni;
    dp ECw;

    mmIN = InfiltratedRain + InfiltratedIrrigation + InfiltratedStorage;

    if (dayi < crop.Day1) {
        ECw = IrriECw.PreSeason;
    } else {
        ECw = Simulation.IrriECw;
        if (dayi > crop.DayN) {
            ECw = IrriECw.PostSeason;
        }
    }

    SaltIN = InfiltratedIrrigation * ECw * equiv + InfiltratedStorage * ECstorage * equiv;
    SaltInfiltr = SaltIN / 100.0;
    SaltOUT = 0.0;

    for (compi = 1; compi <= NrCompartments; ++compi) {
        SAT = soillayer[Compartment[compi - 1].Layer - 1].SAT / 100.0;
        UL = soillayer[Compartment[compi - 1].Layer - 1].UL;
        Dx = soillayer[Compartment[compi - 1].Layer - 1].Dx;

        DeltaTheta = mmIN / (1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0));
        Theta = Compartment[compi - 1].theta - DeltaTheta + Compartment[compi - 1].fluxout / (1000.0 * Compartment[compi - 1].Thickness);

        Theta += DeltaTheta;
        if (Theta <= UL) {
            celi = 0;
            while (Theta > Dx * celi) {
                celi++;
            }
        } else {
            celi = (int32_t)soillayer[Compartment[compi - 1].Layer - 1].SCP1;
        }
        if (celi == 0) celi = 1;

        if (DeltaTheta > 0.0) {
            Compartment[compi - 1].Salt[celi - 1] += SaltIN;
        }

        if (celi > 1) {
            for (Ni = 1; Ni <= (celi - 1); ++Ni) {
                mm1 = Dx * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0);
                if (Ni < (int32_t)soillayer[Compartment[compi - 1].Layer - 1].SC) {
                    mm2 = mm1;
                } else if (Theta > SAT) {
                    mm2 = (Theta - UL) * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0);
                } else {
                    mm2 = (SAT - UL) * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0);
                }
                Dif = soillayer[Compartment[compi - 1].Layer - 1].SaltMobility[Ni - 1];
                Mixing(Dif, mm1, mm2, Compartment[compi - 1].Salt[Ni - 1], Compartment[compi - 1].Salt[Ni], Compartment[compi - 1].Depo[Ni - 1], Compartment[compi - 1].Depo[Ni]);
            }
        }

        SaltOUT = 0.0;
        if (Compartment[compi - 1].fluxout > 0.0) {
            DeltaTheta = Compartment[compi - 1].fluxout / (1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0));
            while (DeltaTheta > 0.0) {
                if (celi < (int32_t)soillayer[Compartment[compi - 1].Layer - 1].SCP1) {
                    limit = (celi - 1.0) * Dx;
                } else {
                    limit = UL;
                }
                if ((Theta - DeltaTheta) < limit) {
                    SaltOUT += Compartment[compi - 1].Salt[celi - 1] + Compartment[compi - 1].Depo[celi - 1];
                    Compartment[compi - 1].Salt[celi - 1] = 0.0;
                    mm1 = (Theta - limit) * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0);
                    if (SaltOUT > (static_cast<dp>(simulparam.SaltSolub) * mm1)) {
                        Compartment[compi - 1].Depo[celi - 1] = SaltOUT - (static_cast<dp>(simulparam.SaltSolub) * mm1);
                        SaltOUT = static_cast<dp>(simulparam.SaltSolub) * mm1;
                    } else {
                        Compartment[compi - 1].Depo[celi - 1] = 0.0;
                    }
                    DeltaTheta -= (Theta - limit);
                    Theta = limit;
                    celi--;
                } else {
                    SaltOUT += (Compartment[compi - 1].Salt[celi - 1] + Compartment[compi - 1].Depo[celi - 1]) * (DeltaTheta / (Theta - limit));
                    Compartment[compi - 1].Salt[celi - 1] *= (1.0 - DeltaTheta / (Theta - limit));
                    Compartment[compi - 1].Depo[celi - 1] *= (1.0 - DeltaTheta / (Theta - limit));
                    mm1 = DeltaTheta * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0);
                    if (SaltOUT > (static_cast<dp>(simulparam.SaltSolub) * mm1)) {
                        Compartment[compi - 1].Depo[celi - 1] += (SaltOUT - static_cast<dp>(simulparam.SaltSolub) * mm1);
                        SaltOUT = static_cast<dp>(simulparam.SaltSolub) * mm1;
                    }
                    DeltaTheta = 0.0;
                    mm1 = soillayer[Compartment[compi - 1].Layer - 1].Dx * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0);
                    if (celi == (int32_t)soillayer[Compartment[compi - 1].Layer - 1].SCP1) {
                        mm1 = 2.0 * mm1;
                    }
                    SaltSolutionDeposit(mm1, Compartment[compi - 1].Salt[celi - 1], Compartment[compi - 1].Depo[celi - 1]);
                }
            }
        }
        mmIN = Compartment[compi - 1].fluxout;
        SaltIN = SaltOUT;
    }

    if (Drain > 0.001) {
        ECdrain = SaltOUT / (Drain * equiv);
    }

    if (NrCompartments > 0) {
        celi = ActiveCells(Compartment[0]);
        SM2 = soillayer[Compartment[0].Layer - 1].SaltMobility[celi - 1] / 4.0;
        ECsw2 = ECswComp(Compartment[0], false);
        mm2 = Compartment[0].theta * 1000.0 * Compartment[0].Thickness * (1.0 - soillayer[Compartment[0].Layer - 1].GravelVol / 100.0);
        for (compi = 2; compi <= NrCompartments; ++compi) {
            celiM1 = celi;
            SM1 = SM2;
            ECsw1 = ECsw2;
            mm1 = mm2;
            celi = ActiveCells(Compartment[compi - 1]);
            SM2 = soillayer[Compartment[compi - 1].Layer - 1].SaltMobility[celi - 1] / 4.0;
            ECsw2 = ECswComp(Compartment[compi - 1], false);
            mm2 = Compartment[compi - 1].theta * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0);
            ECsw = (ECsw1 * mm1 + ECsw2 * mm2) / (mm1 + mm2);
            DS1 = (ECsw1 - (ECsw1 + (ECsw - ECsw1) * SM1)) * mm1 * equiv;
            DS2 = (ECsw2 - (ECsw2 + (ECsw - ECsw2) * SM2)) * mm2 * equiv;
            if (std::abs(DS2) < std::abs(DS1)) {
                DS = std::abs(DS2);
            } else {
                DS = std::abs(DS1);
            }
            if (DS > 0.0) {
                if (ECsw1 > ECsw) {
                    DS = DS * (-1.0);
                }
                MoveSaltTo(Compartment[compi - 2], celiM1, DS);
                DS = DS * (-1.0);
                MoveSaltTo(Compartment[compi - 1], celi, DS);
            }
        }
    }

    if (SubDrain > 0.0 && NrCompartments > 0) {
        Zr = RootingDepth;
        if (Zr >= 1e-12) {
            Zr = static_cast<dp>(simulparam.EvapZmax) / 100.0;
        }
        compi = 0;
        depthi = 0.0;
        ECsubdrain = 0.0;

        while (true) {
            compi++;
            depthi += Compartment[compi - 1].Thickness;
            if (depthi <= Zr) {
                DeltaZ = Compartment[compi - 1].Thickness;
            } else {
                DeltaZ = Compartment[compi - 1].Thickness - (depthi - Zr);
            }
            celi = ActiveCells(Compartment[compi - 1]);
            if (celi < (int32_t)soillayer[Compartment[compi - 1].Layer - 1].SCP1) {
                mm1 = soillayer[Compartment[compi - 1].Layer - 1].Dx * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0);
            } else {
                mm1 = 2.0 * soillayer[Compartment[compi - 1].Layer - 1].Dx * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0);
            }
            ECcel = Compartment[compi - 1].Salt[celi - 1] / (mm1 * equiv);
            ECsubdrain = (ECcel * mm1 * (DeltaZ / Compartment[compi - 1].Thickness) + ECsubdrain * SubDrain) / (mm1 * (DeltaZ / Compartment[compi - 1].Thickness) + SubDrain);
            Compartment[compi - 1].Salt[celi - 1] = (1.0 - (DeltaZ / Compartment[compi - 1].Thickness)) * Compartment[compi - 1].Salt[celi - 1] + (DeltaZ / Compartment[compi - 1].Thickness) * ECsubdrain * mm1 * equiv;
            SaltSolutionDeposit(mm1, Compartment[compi - 1].Salt[celi - 1], Compartment[compi - 1].Depo[celi - 1]);
            if (depthi >= Zr || compi >= NrCompartments) break;
        }

        if (compi >= NrCompartments) {
            SaltOUT = ECdrain * (Drain * equiv) + ECsubdrain * SubDrain * equiv;
            if (Drain > 0.001) ECdrain = SaltOUT / (Drain * equiv);
        } else {
            compi++;
            celi = ActiveCells(Compartment[compi - 1]);
            if (celi < (int32_t)soillayer[Compartment[compi - 1].Layer - 1].SCP1) {
                mm1 = soillayer[Compartment[compi - 1].Layer - 1].Dx * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0);
            } else {
                mm1 = 2.0 * soillayer[Compartment[compi - 1].Layer - 1].Dx * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0);
            }
            Compartment[compi - 1].Salt[celi - 1] += ECsubdrain * SubDrain * equiv;
            SaltSolutionDeposit(mm1, Compartment[compi - 1].Salt[celi - 1], Compartment[compi - 1].Depo[celi - 1]);
        }
    }
}
void CheckGermination() {
    dp SUM, MaxDepth;
    bool Considered;

    MaxDepth = static_cast<dp>(simulparam.EvapZmax) / 100.0;
    calculate_relative_wetness_topsoil(SUM, MaxDepth);
    if (SUM >= static_cast<dp>(simulparam.TAWGermination) / 100.0) {
        Simulation.Germinate = true;
    }
}
void EffectSoilFertilitySalinityStress(int32_t& StressSFadjNEW, dp Coeffb0Salt, dp Coeffb1Salt, dp Coeffb2Salt, int32_t NrDayGrow, dp StressTotSaltPrev, int32_t VirtualTimeCC) {
    dp ZrECe, ZrECsw, ZrECswFC, ZrKsSalt, SaltStress;
    rep_EffectStress EffectStressSF;

    DetermineRootZoneSaltContent(RootingDepth, ZrECe, ZrECsw, ZrECswFC, ZrKsSalt);
    SaltStress = (1.0 - ZrKsSalt) * 100.0;

    Simulation.EffectStress.RedKsSto = static_cast<int8_t>(roundc(Coeffb0Salt + Coeffb1Salt * SaltStress + Coeffb2Salt * SaltStress * SaltStress, 1));
    if (Simulation.EffectStress.RedKsSto > 100) Simulation.EffectStress.RedKsSto = 100;
    if (Simulation.EffectStress.RedKsSto < 0) Simulation.EffectStress.RedKsSto = 0;

    CropStressParametersSoilFertility(crop.StressResponse, Management.FertilityStress, EffectStressSF);
    Simulation.EffectStress.RedCGC = EffectStressSF.RedCGC;
    Simulation.EffectStress.RedCCX = EffectStressSF.RedCCX;
    Simulation.EffectStress.RedWP = EffectStressSF.RedWP;
    Simulation.EffectStress.CDecline = EffectStressSF.CDecline;
}
void DetermineCCi(dp CCxTotal, dp CCoTotal, dp& StressLeaf, dp FracAssim, bool MobilizationON, bool StorageON, int32_t Tadj, int32_t VirtualTimeCC, dp& StressSenescence, dp& TimeSenescence, bool& NoMoreCrop, dp CDCTotal, dp DayFraction, dp GDDCDCTotal, dp& TESTVAL) {
    dp CCi;
    int32_t DAP = VirtualTimeCC;

    CCi = CanopyCoverNoStressSF(DAP, crop.DaysToGermination, crop.DaysToSenescence, crop.DaysToHarvest, crop.GDDaysToGermination, crop.GDDaysToSenescence, crop.GDDaysToHarvest, CCoTotal, CCxTotal, crop.CGC, crop.CDC, crop.GDDCGC, crop.GDDCDC, Simulation.SumGDD, modeCycle::CalendarDays, Simulation.EffectStress.RedCGC, Simulation.EffectStress.RedCCX);
    CCiActual = CCi;
}

void DetermineCCiGDD(dp CCxTotal, dp CCoTotal, dp& StressLeaf, dp FracAssim, bool MobilizationON, bool StorageON, dp SumGDDAdjCC, int32_t VirtualTimeCC, dp& StressSenescence, dp& TimeSenescence, bool& NoMoreCrop, dp CDCTotal, dp GDDayFraction, dp GDDayi, dp GDDCDCTotal, int32_t GDDTadj) {
    dp CCi;
    CCi = CanopyCoverNoStressSF(VirtualTimeCC, crop.DaysToGermination, crop.DaysToSenescence, crop.DaysToHarvest, crop.GDDaysToGermination, crop.GDDaysToSenescence, crop.GDDaysToHarvest, CCoTotal, CCxTotal, crop.CGC, crop.CDC, crop.GDDCGC, crop.GDDCDC, SumGDDAdjCC, modeCycle::GDDays, Simulation.EffectStress.RedCGC, Simulation.EffectStress.RedCCX);
    CCiActual = CCi;
}

// --- Placeholder implementations ---
void calculate_Extra_runoff(dp& InfiltratedRain, dp& InfiltratedIrrigation, dp& InfiltratedStorage, dp& SubDrain) {
    dp excess;
    excess = (Rain - Runoff - SubDrain) + Irrigation - soillayer[0].InfRate;
    if (excess > 0.0) {
        Runoff += excess;
        if (Irrigation > excess) {
            Irrigation -= excess;
        } else {
            excess -= Irrigation;
            Irrigation = 0.0;
            SubDrain -= excess;
        }
    }
}

void calculate_surfacestorage(dp& InfiltratedRain, dp& InfiltratedIrrigation, dp& InfiltratedStorage, dp& ECinfilt, dp SubDrain, int32_t dayi) {
    dp excess, ECw;

    if (dayi < crop.Day1) {
        ECw = IrriECw.PreSeason;
    } else {
        ECw = Simulation.IrriECw;
        if (dayi > crop.DayN) {
            ECw = IrriECw.PostSeason;
        }
    }

    excess = (Rain - Runoff - SubDrain) + Irrigation + SurfaceStorage - Management.BundHeight;
    if (excess > 0.0) {
        Runoff += excess;
        SurfaceStorage = Management.BundHeight;
    } else {
        SurfaceStorage = (Rain - Runoff - SubDrain) + Irrigation + SurfaceStorage;
    }
    
    InfiltratedStorage = 0.0;
    if (SurfaceStorage > soillayer[0].InfRate) {
        InfiltratedStorage = soillayer[0].InfRate;
    } else {
        InfiltratedStorage = SurfaceStorage;
    }
    SurfaceStorage -= InfiltratedStorage;
    ECinfilt = ECw; // simplified
}

// --- Placeholder implementations ---
dp calculate_factor(int32_t layeri, int32_t compi) {
    dp delta_theta_SAT;

    delta_theta_SAT = calculate_delta_theta(soillayer[layeri - 1].SAT / 100.0,
                                            soillayer[layeri - 1].FC / 100.0,
                                            layeri);
    if (delta_theta_SAT > 0.0) {
        return soillayer[layeri - 1].InfRate / (delta_theta_SAT * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0));
    } else {
        return 1.0;
    }
}

void calculate_infiltration(dp& InfiltratedRain, dp& InfiltratedIrrigation, dp& InfiltratedStorage, dp& SubDrain) {
    int32_t compi, layeri, pre_comp;
    dp RunoffIni, amount_still_to_store, factor, delta_theta_nul, delta_theta_SAT, theta_nul, drain_max, diff, excess;
    dp EffecRain, Zr, depthi, DeltaZ, StorableMM;

    if (RainRecord.DataType == datatype::daily) {
        amount_still_to_store = InfiltratedRain + InfiltratedIrrigation + InfiltratedStorage;
        EffecRain = 0.0;
    } else {
        amount_still_to_store = InfiltratedIrrigation + InfiltratedStorage;
        EffecRain = InfiltratedRain - SubDrain;
    }

    if (amount_still_to_store > 0.0) {
        RunoffIni = Runoff;
        compi = 0;

        while (true) {
            compi++;
            layeri = Compartment[compi - 1].Layer;

            factor = calculate_factor(layeri, compi);

            delta_theta_nul = amount_still_to_store / (1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0));
            delta_theta_SAT = calculate_delta_theta(soillayer[layeri - 1].SAT / 100.0, soillayer[layeri - 1].FC / 100.0, layeri);

            if (delta_theta_nul < delta_theta_SAT) {
                theta_nul = calculate_theta_from_delta(delta_theta_nul, soillayer[layeri - 1].FC / 100.0, layeri);
                if (theta_nul <= (Compartment[compi - 1].FCadj / 100.0)) {
                    theta_nul = Compartment[compi - 1].FCadj / 100.0;
                    delta_theta_nul = calculate_delta_theta(theta_nul, soillayer[layeri - 1].FC / 100.0, layeri);
                }
                if (theta_nul > soillayer[layeri - 1].SAT / 100.0) {
                    theta_nul = soillayer[layeri - 1].SAT / 100.0;
                }
            } else {
                theta_nul = soillayer[layeri - 1].SAT / 100.0;
                delta_theta_nul = delta_theta_SAT;
            }

            drain_max = factor * delta_theta_nul * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
            if ((Compartment[compi - 1].fluxout + drain_max) > soillayer[layeri - 1].InfRate) {
                drain_max = soillayer[layeri - 1].InfRate - Compartment[compi - 1].fluxout;
            }

            diff = theta_nul - Compartment[compi - 1].theta;
            if (diff > 0.0) {
                Compartment[compi - 1].theta += amount_still_to_store / (1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0));
                if (Compartment[compi - 1].theta > theta_nul) {
                    amount_still_to_store = (Compartment[compi - 1].theta - theta_nul) * 1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
                    Compartment[compi - 1].theta = theta_nul;
                } else {
                    amount_still_to_store = 0.0;
                }
            }
            Compartment[compi - 1].fluxout += amount_still_to_store;

            excess = amount_still_to_store - drain_max;
            if (excess < 0.0) excess = 0.0;
            amount_still_to_store -= excess;

            if (excess > 0.0) {
                pre_comp = compi + 1;
                while (true) {
                    pre_comp--;
                    layeri = Compartment[pre_comp - 1].Layer;
                    Compartment[pre_comp - 1].fluxout -= excess;
                    Compartment[pre_comp - 1].theta += excess / (1000.0 * Compartment[pre_comp - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0));
                    if (Compartment[pre_comp - 1].theta > soillayer[layeri - 1].SAT / 100.0) {
                        excess = (Compartment[pre_comp - 1].theta - soillayer[layeri - 1].SAT / 100.0) * 1000.0 * Compartment[pre_comp - 1].Thickness * (1.0 - soillayer[layeri - 1].GravelVol / 100.0);
                        Compartment[pre_comp - 1].theta = soillayer[layeri - 1].SAT / 100.0;
                    } else {
                        excess = 0.0;
                    }
                    if (excess < 1e-12 || pre_comp == 1) break;
                }
                if (excess > 0.0) Runoff += excess;
            }

            if (amount_still_to_store <= 1e-12 || compi == NrCompartments) break;
        }
        if (amount_still_to_store > 0.0) Drain += amount_still_to_store;

        if (Runoff > RunoffIni) {
            if (Management.BundHeight >= 0.01) {
                SurfaceStorage += (Runoff - RunoffIni);
                InfiltratedStorage -= (Runoff - RunoffIni);
                if (SurfaceStorage > Management.BundHeight * 1000.0) {
                    Runoff = RunoffIni + (SurfaceStorage - Management.BundHeight * 1000.0);
                    SurfaceStorage = Management.BundHeight * 1000.0;
                } else {
                    Runoff = RunoffIni;
                }
            } else {
                InfiltratedRain -= (Runoff - RunoffIni);
                if (InfiltratedRain < 0.0) {
                    InfiltratedIrrigation += InfiltratedRain;
                    InfiltratedRain = 0.0;
                }
            }
        }
    }

    if (SubDrain > 0.0) {
        amount_still_to_store = SubDrain;
        Zr = RootingDepth;
        if (Zr <= 0.0) Zr = static_cast<dp>(simulparam.EvapZmax) / 100.0;
        compi = 0;
        depthi = 0.0;
        while (true) {
            compi++;
            depthi += Compartment[compi - 1].Thickness;
            if (depthi >= Zr || compi >= NrCompartments) break;
        }
        if (depthi > Zr) DeltaZ = (depthi - Zr); else DeltaZ = 0.0;

        while (amount_still_to_store > 0.0 && (compi < NrCompartments || DeltaZ > 0.0)) {
            if (std::abs(DeltaZ) < 1e-12) {
                compi++;
                DeltaZ = Compartment[compi - 1].Thickness;
            }
            StorableMM = (soillayer[Compartment[compi - 1].Layer - 1].SAT / 100.0 - Compartment[compi - 1].theta) * 1000.0 * DeltaZ * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0);
            if (StorableMM > amount_still_to_store) {
                Compartment[compi - 1].theta += amount_still_to_store / (1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0));
                amount_still_to_store = 0.0;
            } else {
                amount_still_to_store -= StorableMM;
                Compartment[compi - 1].theta += StorableMM / (1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0));
            }
            DeltaZ = 0.0;
            if (amount_still_to_store > soillayer[Compartment[compi - 1].Layer - 1].InfRate) {
                SubDrain -= (amount_still_to_store - soillayer[Compartment[compi - 1].Layer - 1].InfRate);
                EffecRain += (amount_still_to_store - soillayer[Compartment[compi - 1].Layer - 1].InfRate);
                amount_still_to_store = soillayer[Compartment[compi - 1].Layer - 1].InfRate;
            }
        }
        if (amount_still_to_store > 0.0) Drain += amount_still_to_store;
    }

    if (EffecRain > 0.0) {
        Zr = RootingDepth;
        if (Zr <= 1e-12) Zr = static_cast<dp>(simulparam.EvapZmax) / 100.0;
        amount_still_to_store = EffecRain;

        compi = 0;
        depthi = 0.0;
        while (true) {
            compi++;
            depthi += Compartment[compi - 1].Thickness;
            if (depthi <= Zr) DeltaZ = Compartment[compi - 1].Thickness; else DeltaZ = Compartment[compi - 1].Thickness - (depthi - Zr);
            StorableMM = (Compartment[compi - 1].FCadj / 100.0 - Compartment[compi - 1].theta) * 1000.0 * DeltaZ * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0);
            if (StorableMM < 0.0) StorableMM = 0.0;
            if (StorableMM > amount_still_to_store) {
                Compartment[compi - 1].theta += amount_still_to_store / (1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0));
                amount_still_to_store = 0.0;
            } else if (StorableMM > 0.0) {
                Compartment[compi - 1].theta += StorableMM / (1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0));
                amount_still_to_store -= StorableMM;
            }
            if (depthi >= Zr || compi >= NrCompartments || amount_still_to_store <= 1e-12) break;
        }

        if (amount_still_to_store > 0.0) {
            while (true) {
                if (depthi > Zr) DeltaZ = Compartment[compi - 1].Thickness - (depthi - Zr); else DeltaZ = Compartment[compi - 1].Thickness;
                StorableMM = (soillayer[Compartment[compi - 1].Layer - 1].SAT / 100.0 - Compartment[compi - 1].theta) * 1000.0 * DeltaZ * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0);
                if (StorableMM < 0.0) StorableMM = 0.0;
                if (StorableMM > amount_still_to_store) {
                    Compartment[compi - 1].theta += amount_still_to_store / (1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0));
                    amount_still_to_store = 0.0;
                } else if (StorableMM > 0.0) {
                    Compartment[compi - 1].theta += StorableMM / (1000.0 * Compartment[compi - 1].Thickness * (1.0 - soillayer[Compartment[compi - 1].Layer - 1].GravelVol / 100.0));
                    amount_still_to_store -= StorableMM;
                }
                compi--;
                if (compi > 0) depthi -= Compartment[compi].Thickness;
                if (compi == 0 || amount_still_to_store <= 1e-12) break;
            }
        }

        if (amount_still_to_store > 0.0) {
            if (InfiltratedRain > 0.0) InfiltratedRain -= amount_still_to_store;
            if (Management.BundHeight >= 0.01) {
                SurfaceStorage += amount_still_to_store;
                if (SurfaceStorage > Management.BundHeight * 1000.0) {
                    Runoff += (SurfaceStorage - Management.BundHeight * 1000.0);
                    SurfaceStorage = Management.BundHeight * 1000.0;
                }
            } else {
                Runoff += amount_still_to_store;
            }
        }
    }
}

void DeterminePotentialBiomass(int32_t VirtualTimeCC, dp SumGDDadjCC,
    dp CO2i, dp GDDayi, dp& CCxWitheredTpotNoS, dp& BiomassUnlim)
{
    dp CCi, TpotNoS, EpotNoS, WPi;
    int32_t DAP;

    if (crop.ModeCycle == modeCycle::CalendarDays) {
        DAP = VirtualTimeCC;
    } else {
        DAP = SumCalendarDays(roundc(SumGDDadjCC, 1), crop.Day1, crop.Tbase, crop.Tupper, simulparam.Tmin, simulparam.Tmax);
    }

    CCi = CanopyCoverNoStressSF(DAP, crop.DaysToGermination, crop.DaysToSenescence, crop.DaysToHarvest, crop.GDDaysToGermination, crop.GDDaysToSenescence, crop.GDDaysToHarvest, crop.CCoAdjusted, crop.CCxAdjusted, crop.CGC, crop.CDC, crop.GDDCGC, crop.GDDCDC, SumGDDadjCC, crop.ModeCycle, 0, 0);

    if (CCi > CCxWitheredTpotNoS) {
        CCxWitheredTpotNoS = CCi;
    }

    CalculateETpot(DAP, crop.DaysToGermination, crop.DaysToFullCanopy, crop.DaysToSenescence, crop.DaysToHarvest, 0, CCi, ETo, crop.KcTop, crop.KcDecline, crop.CCxAdjusted, CCxWitheredTpotNoS, crop.CCEffectEvapLate, CO2i, GDDayi, crop.GDtranspLow, TpotNoS, EpotNoS);

    WPi = crop.WP * fAdjustedForCO2(CO2i, crop.WP, crop.AdaptedToCO2);
    BiomassUnlim = WPi * (TpotNoS / ETo);
}

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
    dp& Bin, dp& Bout, dp& TESTVALY)
{
    dp WPi_adj, HIday;
    int32_t DAP, HIfinal_int;
    int8_t PercentLagPhase;

    WPi_adj = WPi * fAdjustedForCO2(CO2i, WPi, crop.AdaptedToCO2);
    Biomass = WPi_adj * (Tact / ETo);
    BiomassTot += Biomass;

    if (crop.ModeCycle == modeCycle::CalendarDays) {
        DAP = VirtualTimeCC;
    } else {
        DAP = SumCalendarDays(roundc(SumGDDadjCC, 1), crop.Day1, crop.Tbase, crop.Tupper, simulparam.Tmin, simulparam.Tmax);
    }

    HIday = HarvestIndexDay(DAP, crop.DaysToFlowering, crop.HI, crop.dHIdt, CCiActual, crop.CCxAdjusted, crop.CCxWithered, simulparam.PercCCxHIfinal, crop.Planting, PercentLagPhase, HIfinal_int);
    YieldPart = BiomassTot * (HIday / 100.0);
}

} // namespace AquaCrop
