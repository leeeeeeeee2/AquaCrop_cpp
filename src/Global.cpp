#include "AquaCrop/Global.h"
#include "AquaCrop/Utils.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <vector>
#include <sstream>

namespace AquaCrop {

bool FileExists(const std::string& full_name)
{
    std::ifstream f(full_name);
    return f.good();
}

// Global variables (Definitions)
std::string RainFile;
std::string RainFileFull;
std::string RainDescription;
std::string EToFile;
std::string EToFileFull;
std::string EToDescription;
std::string CalendarFile;
std::string CalendarFileFull;
std::string CalendarDescription;
std::string CO2File;
std::string CO2FileFull;
std::string CO2Description;
std::string IrriFile;
std::string IrriFileFull;
std::string CropFile;
std::string CropFileFull;
std::string CropDescription;
std::string PathNameProg;
std::string PathNameOutp;
std::string PathNameSimul;
std::string ProfFile;
std::string ProfFilefull;
std::string ProfDescription;
std::string ManFile;
std::string ManFilefull;
std::string ObservationsFile;
std::string ObservationsFilefull;
std::string ObservationsDescription;
std::string OffSeasonFile;
std::string OffSeasonFilefull;
std::string OutputName;
std::string GroundWaterFile;
std::string GroundWaterFilefull;
std::string ClimateFile;
std::string ClimateFileFull;
std::string ClimateDescription;
std::string IrriDescription;
std::string ClimFile;
std::string SWCiniFile;
std::string SWCiniFileFull;
std::string SWCiniDescription;
std::string ProjectDescription;
std::string ProjectFile;
std::string ProjectFileFull;
std::string MultipleProjectDescription;
std::string MultipleProjectFile;
std::string TemperatureFile;
std::string TemperatureFileFull;
std::string TemperatureDescription;
std::string MultipleProjectFileFull;
std::string FullFileNameProgramParameters;
std::string ManDescription;
std::string ClimDescription;
std::string OffSeasonDescription;
std::string GroundwaterDescription;
std::string TnxReferenceFile;
std::string TnxReferenceFileFull;
std::string TnxReference365DaysFile;
std::string TnxReference365DaysFileFull;

rep_IrriECw IrriECw;
rep_Manag Management;
rep_PerennialPeriod perennialperiod;
rep_param simulparam;
rep_Cuttings Cuttings;
rep_Onset onset;
rep_EndSeason endseason;
rep_Crop crop;
rep_Content TotalSaltContent;
rep_Content TotalWaterContent;
rep_EffectiveRain effectiverain;
rep_soil Soil;
rep_RootZoneWC RootZoneWC;
rep_CropFileSet CropFileSet;
rep_sum SumWaBal;
rep_RootZoneSalt RootZoneSalt;
rep_clim TemperatureRecord, ClimRecord, RainRecord, EToRecord;
rep_sim Simulation;

GenerateTimeMode GenerateTimeMode_Val;
GenerateDepthMode GenerateDepthMode_Val;
IrriMode IrriMode_Val;
IrriMethod IrriMethod_Val;

int32_t TnxReferenceYear;
int32_t DaySubmerged;
int32_t MaxPlotNew;
int32_t NrCompartments;
int32_t IrriFirstDayNr;
dp ZiAqua = 0.0;

// Placeholder implementations for missing declarations in Global.h
dp GetManagement_BundHeight() { return Management.BundHeight; }
bool GetManagement_RunoffON() { return Management.RunoffOn; }
dp GetRain() { return Rain; }
datatype GetRainRecord_DataType() { return RainRecord.DataType; }
dp GetSimulParam_RunoffDepth() { return simulparam.RunoffDepth; }
IrriMode GetIrriMode() { return IrriMode_Val; }
dp GetIrrigation() { return Irrigation; }
void SetDaySubmerged(int32_t val) { DaySubmerged = val; }
dp GetSimulParam_DelayLowOxygen() { return (dp)simulparam.DelayLowOxygen; }
dp GetRootingDepth() { return RootingDepth; }

int8_t IniPercTAW;
int8_t MaxPlotTr;
int8_t OutputAggregate;

int fTnxReference;
int fTnxReference_iostat;
int fTnxReference365Days;
int fTnxReference365Days_iostat;

dp CCiActual;
dp CCiprev;
dp CCiTopEarlySen;
dp CRsalt;
dp CRwater;
dp ECdrain = 0.0;
dp ECiAqua;
dp ECstorage;
dp Eact;
dp Epot;
dp ETo;
dp Drain;
dp Infiltrated;
dp Irrigation;
dp Rain;
dp RootingDepth;
dp Runoff;
dp SaltInfiltr;
dp Surf0 = 0.0;
dp SurfaceStorage;
dp Tact;
dp Tpot;
dp TactWeedInfested;
dp Tmax;
dp Tmin;
dp TmaxCropReference;
dp TminCropReference;
dp TmaxTnxReference365Days;
dp TminTnxReference365Days;

std::vector<sp> TmaxRun(366);
std::vector<sp> TminRun(366);
std::vector<sp> TmaxTnxReference12MonthsRun(12);
std::vector<sp> TminTnxReference12MonthsRun(12);
std::vector<sp> TmaxCropReferenceRun(365);
std::vector<sp> TminCropReferenceRun(365);
std::vector<sp> TmaxTnxReference365DaysRun(365);
std::vector<sp> TminTnxReference365DaysRun(365);

bool EvapoEntireSoilSurface;
bool PreDay, OutDaily, Out8Irri;
bool Out1Wabal;
bool Out2Crop;
bool Out3Prof;
bool Out4Salt;
bool Out5CompWC;
bool Out6CompEC;
bool Out7Clim;
bool Part1Mult, Part2Eval;

std::string PathNameList, PathNameParam;

std::vector<CompartmentIndividual> Compartment(max_No_compartments);
std::vector<SoilLayerIndividual> soillayer(max_SoilLayers);

std::vector<rep_DayEventInt> IrriBeforeSeason(5);
std::vector<rep_DayEventInt> IrriAfterSeason(5);

// Function implementations
dp DeduceAquaCropVersion(const std::string& FullNameXXFile)
{
    std::ifstream file(FullNameXXFile);
    dp VersionNr = 0.0;
    if (file.is_open())
    {
        std::string line;
        std::getline(file, line); // Description
        file >> VersionNr;
    }
    return VersionNr;
}

sp RootMaxInSoilProfile(dp ZmaxCrop, int8_t TheNrSoilLayers, const std::vector<SoilLayerIndividual>& TheSoilLayer)
{
    dp Zmax = ZmaxCrop;
    dp Zsoil = 0.0;
    int layi = 0;

    while (layi < TheNrSoilLayers && Zmax > 0.0)
    {
        layi++;
        if (TheSoilLayer[layi-1].Penetrability < 100 && roundc(Zsoil * 1000.0, 1) < roundc(ZmaxCrop * 1000.0, 1))
        {
            Zmax = static_cast<dp>(undef_int);
        }
        Zsoil += TheSoilLayer[layi-1].Thickness;
    }

    if (Zmax < 0.0)
    {
        ZrAdjustedToRestrictiveLayers(ZmaxCrop, TheNrSoilLayers, TheSoilLayer, Zmax);
    }

    return static_cast<sp>(Zmax);
}

void ZrAdjustedToRestrictiveLayers(dp ZrIN, int8_t TheNrSoilLayers, const std::vector<SoilLayerIndividual>& TheLayer, dp& ZrOUT)
{
    int Layi;
    dp Zsoil, ZrAdj, ZrRemain, DeltaZ, ZrTest;
    bool TheEnd;

    ZrOUT = ZrIN;

    Layi = 1;
    Zsoil = TheLayer[Layi-1].Thickness;
    ZrAdj = 0.0;
    ZrRemain = ZrIN;
    DeltaZ = Zsoil;
    TheEnd = false;

    while (!TheEnd)
    {
        ZrTest = ZrAdj + ZrRemain * (static_cast<dp>(TheLayer[Layi-1].Penetrability) / 100.0);

        if (Layi == TheNrSoilLayers || TheLayer[Layi-1].Penetrability == 0 || roundc(ZrTest * 10000.0, 1) <= roundc(Zsoil * 10000.0, 1))
        {
            TheEnd = true;
            ZrOUT = ZrTest;
        }
        else
        {
            ZrAdj = Zsoil;
            ZrRemain = ZrRemain - DeltaZ / (static_cast<dp>(TheLayer[Layi-1].Penetrability) / 100.0);
            Layi++;
            Zsoil = Zsoil + TheLayer[Layi-1].Thickness;
            DeltaZ = TheLayer[Layi-1].Thickness;
        }
    }
}

void DeclareInitialCondAtFCandNoSalt()
{
    int32_t layeri, compi, celli, ind;

    SWCiniFile = "(None)";
    SWCiniFileFull = SWCiniFile;
    SWCiniDescription = "Soil water profile at Field Capacity";
    Simulation.IniSWC.AtDepths = false;
    Simulation.IniSWC.NrLoc = Soil.NrSoilLayers;

    for (layeri = 1; layeri <= Soil.NrSoilLayers; ++layeri)
    {
        Simulation.IniSWC.Loc[layeri-1] = soillayer[layeri-1].Thickness;
        Simulation.IniSWC.VolProc[layeri-1] = soillayer[layeri-1].FC;
        Simulation.IniSWC.SaltECe[layeri-1] = 0.0;
    }
    Simulation.IniSWC.AtFC = true;

    for (layeri = static_cast<int32_t>(Soil.NrSoilLayers) + 1; layeri <= max_No_compartments; ++layeri)
    {
        Simulation.IniSWC.Loc[layeri-1] = undef_double;
        Simulation.IniSWC.VolProc[layeri-1] = undef_double;
        Simulation.IniSWC.SaltECe[layeri-1] = undef_double;
    }

    for (compi = 1; compi <= NrCompartments; ++compi)
    {
        if (Compartment[compi-1].Layer == 0)
        {
            ind = 1;
        }
        else
        {
            ind = Compartment[compi-1].Layer;
        }
        for (celli = 1; celli <= soillayer[ind-1].SCP1; ++celli)
        {
            Compartment[compi-1].Salt[celli-1] = 0.0;
            Compartment[compi-1].Depo[celli-1] = 0.0;
        }
    }
}

void set_layer_undef(SoilLayerIndividual& LayerData)
{
    LayerData.Description = "";
    LayerData.Thickness = undef_double;
    LayerData.SAT = undef_double;
    LayerData.FC = undef_double;
    LayerData.WP = undef_double;
    LayerData.tau = undef_double;
    LayerData.InfRate = undef_double;
    LayerData.Penetrability = undef_int;
    LayerData.GravelMass = undef_int;
    LayerData.GravelVol = 0.0;
    LayerData.Macro = undef_int;
    LayerData.UL = undef_double;
    LayerData.Dx = undef_double;
    for (int i = 0; i < 11; ++i)
    {
        LayerData.SaltMobility[i] = undef_double;
    }
    LayerData.SoilClass = undef_int;
    LayerData.CRa = 0.0;
    LayerData.CRb = 0.0;
    LayerData.WaterContent = undef_double;
}

void CropStressParametersSoilFertility(const rep_Shapes& CropSResp, int32_t StressLevel, rep_EffectStress& StressOUT)
{
    dp Ksi, pULActual, pLLActual;

    pLLActual = 1.0;

    // decline canopy growth coefficient (CGC)
    pULActual = 0.0;
    Ksi = KsAny(static_cast<dp>(StressLevel) / 100.0, pULActual, pLLActual, CropSResp.ShapeCGC);
    StressOUT.RedCGC = static_cast<int8_t>(roundc((1.0 - Ksi) * 100.0, 1));
    // decline maximum canopy cover (CCx)
    pULActual = 0.0;
    Ksi = KsAny(static_cast<dp>(StressLevel) / 100.0, pULActual, pLLActual, CropSResp.ShapeCCX);
    StressOUT.RedCCX = static_cast<int8_t>(roundc((1.0 - Ksi) * 100.0, 1));
    // decline crop water productivity (WP)
    pULActual = 0.0;
    Ksi = KsAny(static_cast<dp>(StressLevel) / 100.0, pULActual, pLLActual, CropSResp.ShapeWP);
    StressOUT.RedWP = static_cast<int8_t>(roundc((1.0 - Ksi) * 100.0, 1));
    // decline Canopy Cover (CDecline)
    pULActual = 0.0;
    Ksi = KsAny(static_cast<dp>(StressLevel) / 100.0, pULActual, pLLActual, CropSResp.ShapeCDecline);
    StressOUT.CDecline = 1.0 - Ksi;
    // inducing stomatal closure (KsSto) not applicable
    Ksi = 1.0;
    StressOUT.RedKsSto = static_cast<int8_t>(roundc((1.0 - Ksi) * 100.0, 1));
}

dp TimeRootFunction(dp t, int8_t ShapeFactor, dp tmax, dp t0)
{
    return std::exp((10.0 / static_cast<dp>(ShapeFactor)) * std::log((t - t0) / (tmax - t0)));
}

dp TimeToReachZroot(dp Zi, dp Zo, dp Zx, int8_t ShapeRootDeepening, int32_t Lo, int32_t LZxAdj)
{
    dp ti = static_cast<dp>(undef_int);

    if (roundc(Zi * 100.0, 1) >= roundc(Zx * 100.0, 1))
    {
        ti = static_cast<dp>(LZxAdj);
    }
    else
    {
        if (((Zo + 0.0001) < Zx) && (static_cast<dp>(LZxAdj) > static_cast<dp>(Lo) / 2.0) && (LZxAdj > 0) && (ShapeRootDeepening > 0))
        {
            dp T1 = std::exp((static_cast<dp>(ShapeRootDeepening) / 10.0) * std::log((Zi - Zo) / (Zx - Zo)));
            ti = T1 * (static_cast<dp>(LZxAdj) - static_cast<dp>(Lo) / 2.0) + static_cast<dp>(Lo) / 2.0;
        }
    }
    return ti;
}

namespace { // Anonymous namespace for helper functions
dp CanopyCoverNoStressDaysSF(int32_t DAP, int32_t L0, int32_t L123, int32_t LMaturity, dp CCo, dp CCx, dp CGC, dp CDC, int8_t SFRedCGC, int8_t SFRedCCx)
{
    dp CC = 0.0;
    int32_t t = DAP - Simulation.DelayedDays;

    if (t >= 1 && t <= LMaturity && CCo > 1e-9)
    {
        if (t <= L0)
        {
            CC = 0.0;
        }
        else if (t < L123)
        {
            CC = CCatTime(t - L0, CCo, (1.0 - static_cast<dp>(SFRedCGC) / 100.0) * CGC, (1.0 - static_cast<dp>(SFRedCCx) / 100.0) * CCx);
        }
        else
        {
            if (CCx < 0.001)
            {
                CC = 0.0;
            }
            else
            {
                dp CCxAdj = CCatTime(L123 - L0, CCo, (1.0 - static_cast<dp>(SFRedCGC) / 100.0) * CGC, (1.0 - static_cast<dp>(SFRedCCx) / 100.0) * CCx);
                dp CDCadj = CDC * (CCxAdj + 2.29) / (CCx + 2.29);
                if (CCxAdj < 0.001)
                {
                    CC = 0.0;
                }
                else
                {
                    CC = CCxAdj * (1.0 - 0.05 * (std::exp(static_cast<dp>(t - L123) * 3.33 * CDCadj / (CCxAdj + 2.29)) - 1.0));
                }
            }
        }
    }

    if (CC > 1.0) CC = 1.0;
    if (CC < 1e-9) CC = 0.0;

    return CC;
}
} // namespace

dp CanopyCoverNoStressSF(int32_t DAP, int32_t L0, int32_t L123, int32_t LMaturity, int32_t GDDL0, int32_t GDDL123, int32_t GDDLMaturity, dp CCo, dp CCx, dp CGC, dp CDC, dp GDDCGC, dp GDDCDC, dp SumGDD, modeCycle TypeDays, int8_t SFRedCGC, int8_t SFRedCCx)
{
    if (TypeDays == modeCycle::GDDays)
    {
        return CanopyCoverNoStressGDDaysSF(GDDL0, GDDL123, GDDLMaturity, SumGDD, CCo, CCx, CGC, CDC, GDDCGC, GDDCDC, SFRedCGC, SFRedCCx);
    }
    else
    {
        return CanopyCoverNoStressDaysSF(DAP, L0, L123, LMaturity, CCo, CCx, CGC, CDC, SFRedCGC, SFRedCCx);
    }
}

dp CCiNoWaterStressSF(int32_t Dayi, int32_t L0, int32_t L12SF, int32_t L123, int32_t L1234, int32_t GDDL0, int32_t GDDL12SF, int32_t GDDL123, int32_t GDDL1234, dp CCo, dp CCx, dp CGC, dp GDDCGC, dp CDC, dp GDDCDC, dp SumGDD, dp RatDGDD, int8_t SFRedCGC, int8_t SFRedCCx, dp SFCDecline, modeCycle TheModeCycle)
{
    dp CCi, CCibis, CCxAdj, CDCadj, GDDCDCadj;

    CCi = CanopyCoverNoStressSF(Dayi, L0, L123, L1234, GDDL0, GDDL123, GDDL1234, CCo, CCx, CGC, CDC, GDDCGC, GDDCDC, SumGDD, TheModeCycle, SFRedCGC, SFRedCCx);

    if ((Dayi > L12SF) && (SFCDecline > ac_zero_threshold) && (L12SF < L123))
    {
        if (Dayi < L123)
        {
            if (TheModeCycle == modeCycle::CalendarDays)
            {
                CCi -= (SFCDecline / 100.0) * std::exp(2.0 * std::log(static_cast<dp>(Dayi - L12SF))) / static_cast<dp>(L123 - L12SF);
            }
            else
            {
                if ((SumGDD > static_cast<dp>(GDDL12SF)) && (static_cast<dp>(GDDL123) > static_cast<dp>(GDDL12SF)))
                {
                    CCi -= (RatDGDD * SFCDecline / 100.0) * std::exp(2.0 * std::log(SumGDD - static_cast<dp>(GDDL12SF))) / static_cast<dp>(GDDL123 - GDDL12SF);
                }
            }
            if (CCi < 0.0) CCi = 0.0;
        }
        else
        {
            if (TheModeCycle == modeCycle::CalendarDays)
            {
                CCi = CCatTime(L123 - L0, CCo, (CGC * (1.0 - static_cast<dp>(SFRedCGC) / 100.0)), ((1.0 - static_cast<dp>(SFRedCCx) / 100.0) * CCx));
                CCibis = CCi - (SFCDecline / 100.0) * (std::exp(2.0 * std::log(static_cast<dp>(Dayi - L12SF))) / static_cast<dp>(L123 - L12SF));
                if (CCibis < 0.0)
                {
                    CCi = 0.0;
                }
                else
                {
                    CCi -= ((SFCDecline / 100.0) * static_cast<dp>(L123 - L12SF));
                }
                if (CCi < 0.001)
                {
                    CCi = 0.0;
                }
                else
                {
                    CCxAdj = CCi;
                    CDCadj = CDC * (CCxAdj + 2.29) / (CCx + 2.29);
                    if (Dayi < (L123 + LengthCanopyDecline(CCxAdj, CDCadj)))
                    {
                        CCi = CCxAdj * (1.0 - 0.05 * (std::exp(static_cast<dp>(Dayi - L123) * 3.33 * CDCadj / (CCxAdj + 2.29)) - 1.0));
                        if (CCibis < CCi) CCi = CCibis;
                    }
                    else
                    {
                        CCi = 0.0;
                    }
                }
            }
            else
            {
                CCi = CCatTime(static_cast<int32_t>(static_cast<dp>(GDDL123) - static_cast<dp>(GDDL0)), CCo, (GDDCGC * (1.0 - static_cast<dp>(SFRedCGC) / 100.0)), ((1.0 - static_cast<dp>(SFRedCCx) / 100.0) * CCx));
                if ((SumGDD > static_cast<dp>(GDDL12SF)) && (static_cast<dp>(GDDL123) > static_cast<dp>(GDDL12SF)))
                {
                    CCibis = CCi - (RatDGDD * SFCDecline / 100.0) * (std::exp(2.0 * std::log(SumGDD - static_cast<dp>(GDDL12SF))) / static_cast<dp>(GDDL123 - GDDL12SF));
                }
                else
                {
                    CCibis = CCi;
                }
                if (CCibis < 0.0)
                {
                    CCi = 0.0;
                }
                else
                {
                    CCi -= ((RatDGDD * SFCDecline / 100.0) * static_cast<dp>(GDDL123 - GDDL12SF));
                }
                if (CCi < 0.001)
                {
                    CCi = 0.0;
                }
                else
                {
                    CCxAdj = CCi;
                    GDDCDCadj = GDDCDC * (CCxAdj + 2.29) / (CCx + 2.29);
                    if (SumGDD < (static_cast<dp>(GDDL123) + static_cast<dp>(LengthCanopyDecline(CCxAdj, GDDCDCadj))))
                    {
                        CCi = CCxAdj * (1.0 - 0.05 * (std::exp((SumGDD - static_cast<dp>(GDDL123)) * 3.33 * GDDCDCadj / (CCxAdj + 2.29)) - 1.0));
                        if (CCibis < CCi) CCi = CCibis;
                    }
                    else
                    {
                        CCi = 0.0;
                    }
                }
            }
            if (CCi < 0.0) CCi = 0.0;
        }
    }
    return CCi;
}

dp FromGravelMassToGravelVolume(dp PorosityPercent, int8_t GravelMassPercent)
{
    const dp MineralBD = 2.65;
    dp MatrixBD, SoilBD;

    if (static_cast<int32_t>(GravelMassPercent) > 0)
    {
        MatrixBD = MineralBD * (1.0 - PorosityPercent / 100.0);
        SoilBD = 100.0 / (static_cast<dp>(GravelMassPercent) / MineralBD + (100.0 - static_cast<dp>(GravelMassPercent)) / MatrixBD);
        return static_cast<dp>(GravelMassPercent) * (SoilBD / MineralBD);
    }
    else
    {
        return 0.0;
    }
}

void CheckForWaterTableInProfile(dp DepthGWTmeter, const std::vector<CompartmentIndividual>& ProfileComp, bool& WaterTableInProfile)
{
    dp Ztot = 0.0;
    int32_t compi = 0;
    WaterTableInProfile = false;

    if (DepthGWTmeter >= 0.0)
    {
        while (!WaterTableInProfile && compi < NrCompartments)
        {
            compi++;
            Ztot += ProfileComp[compi-1].Thickness;
            dp Zi = Ztot - ProfileComp[compi-1].Thickness / 2.0;
            if (Zi >= DepthGWTmeter)
            {
                WaterTableInProfile = true;
            }
        }
    }
}

dp GetWeedRC(int32_t TheDay, dp GDDayi, dp fCCx, int8_t TempWeedRCinput, int8_t TempWeedAdj, int32_t& TempWeedDeltaRC, int32_t L12SF, int32_t TempL123, int32_t GDDL12SF, int32_t TempGDDL123, modeCycle TheModeCycle)
{
    dp WeedRCDayCalc = static_cast<dp>(TempWeedRCinput);

    if (static_cast<int32_t>(TempWeedRCinput) > 0 && TempWeedDeltaRC != 0)
    {
        if (TempWeedDeltaRC != 0 && fCCx < 0.999)
        {
            if (fCCx < 0.005)
            {
                TempWeedDeltaRC = 0;
            }
            else
            {
                TempWeedDeltaRC = static_cast<int32_t>(roundc(static_cast<dp>(TempWeedDeltaRC) * std::exp(std::log(fCCx) * (1.0 + static_cast<dp>(TempWeedAdj) / 100.0)), 1));
            }
        }

        if (TheModeCycle == modeCycle::CalendarDays)
        {
            if (TheDay > L12SF)
            {
                if (TheDay >= TempL123)
                {
                    WeedRCDayCalc = static_cast<dp>(TempWeedRCinput) * (1.0 + static_cast<dp>(TempWeedDeltaRC) / 100.0);
                }
                else
                {
                    WeedRCDayCalc = static_cast<dp>(TempWeedRCinput) * (1.0 + (static_cast<dp>(TempWeedDeltaRC) / 100.0) * static_cast<dp>(TheDay - L12SF) / static_cast<dp>(TempL123 - L12SF));
                }
            }
        }
        else
        {
            if (GDDayi > static_cast<dp>(GDDL12SF))
            {
                if (GDDayi > static_cast<dp>(TempGDDL123))
                {
                    WeedRCDayCalc = static_cast<dp>(TempWeedRCinput) * (1.0 + static_cast<dp>(TempWeedDeltaRC) / 100.0);
                }
                else
                {
                    WeedRCDayCalc = static_cast<dp>(TempWeedRCinput) * (1.0 + (static_cast<dp>(TempWeedDeltaRC) / 100.0) * (GDDayi-static_cast<dp>(GDDL12SF)) / static_cast<dp>(TempGDDL123-GDDL12SF));
                }
            }
        }

        if (fCCx < 0.999)
        {
            if (fCCx < 1.0 && fCCx > 0.0 && WeedRCDayCalc > 98.0)
            {
                WeedRCDayCalc = 98.0;
            }
            if (WeedRCDayCalc < 0.0)
            {
                WeedRCDayCalc = 0.0;
            }
            if (fCCx <= 0.0)
            {
                WeedRCDayCalc = 100.0;
            }
        }
    }

    return WeedRCDayCalc;
}

void DetermineLengthGrowthStages(dp CCoVal, dp CCxVal, dp CDCVal, int32_t L0, int32_t TotalLength, bool CGCgiven, int32_t TheDaysToCCini, plant ThePlanting, int32_t& Length123, std::vector<int32_t>& StLength, int32_t& Length12, dp& CGCVal)
{
    dp CCxVal_scaled;
    dp CCToReach;
    int32_t L12Adj;

    if (Length123 < Length12)
    {
        Length123 = Length12;
    }

    if (CCoVal >= CCxVal || Length12 <= L0)
    {
        Length12 = 0;
        StLength[0] = 0;
        StLength[1] = 0;
        CGCVal = static_cast<dp>(undef_int);
    }
    else
    {
        if (!CGCgiven)
        {
            CGCVal = (std::log((0.25 * CCxVal / CCoVal) / (1.0 - 0.98)) / static_cast<dp>(Length12 - L0));
            if (CGCVal > 0.40)
            {
                CGCVal = 0.40;
                CCxVal_scaled = 0.98 * CCxVal;
                Length12 = DaysToReachCCwithGivenCGC(CCxVal_scaled, CCoVal, CCxVal, CGCVal, L0);
                if (Length123 < Length12)
                {
                    Length123 = Length12;
                }
            }
        }
        CCToReach = 0.10;
        StLength[0] = DaysToReachCCwithGivenCGC(CCToReach, CCoVal, CCxVal, CGCVal, L0);
        StLength[1] = Length12 - StLength[0];
    }
    L12Adj = Length12;

    if (ThePlanting == plant::regrowth)
    {
        if (TheDaysToCCini == undef_int)
        {
            L12Adj = 0;
            StLength[0] = 0;
            StLength[1] = 0;
        }
        else
        {
            if (TheDaysToCCini == 0)
            {
                L12Adj = Length12 - L0;
                StLength[0] = StLength[0] - L0;
            }
            else
            {
                L12Adj = Length12 - (L0 + TheDaysToCCini);
                StLength[0] = StLength[0] - (L0 + TheDaysToCCini);
            }
            if (StLength[0] < 0)
            {
                StLength[0] = 0;
            }
            StLength[1] = L12Adj - StLength[0];
        }
    }

    StLength[2] = Length123 - L12Adj;
    StLength[3] = LengthCanopyDecline(CCxVal, CDCVal);

    if (StLength[0] > TotalLength)
    {
        StLength[0] = TotalLength;
        StLength[1] = 0;
        StLength[2] = 0;
        StLength[3] = 0;
    }
    else if ((StLength[0] + StLength[1]) > TotalLength)
    {
        StLength[1] = TotalLength - StLength[0];
        StLength[2] = 0;
        StLength[3] = 0;
    }
    else if ((StLength[0] + StLength[1] + StLength[2]) > TotalLength)
    {
        StLength[2] = TotalLength - StLength[0] - StLength[1];
        StLength[3] = 0;
    }
    else if ((StLength[0] + StLength[1] + StLength[2] + StLength[3]) > TotalLength)
    {
        StLength[3] = TotalLength - StLength[0] - StLength[1] - StLength[2];
    }
}

int32_t TimeToCCini(plant ThePlantingType, int32_t TheCropPlantingDens, dp TheSizeSeedling, dp TheSizePlant, dp TheCropCCx, dp TheCropCGC)
{
    int32_t ElapsedTime;
    dp TheCropCCo;
    dp TheCropCCini;

    if (ThePlantingType == plant::seed || ThePlantingType == plant::transplant || TheSizeSeedling >= TheSizePlant)
    {
        ElapsedTime = 0;
    }
    else
    {
        TheCropCCo = (static_cast<dp>(TheCropPlantingDens) / 10000.0) * (TheSizeSeedling / 10000.0);
        TheCropCCini = (static_cast<dp>(TheCropPlantingDens) / 10000.0) * (TheSizePlant / 10000.0);
        if (TheCropCCini >= (0.98 * TheCropCCx))
        {
            ElapsedTime = undef_int;
        }
        else
        {
            ElapsedTime = DaysToReachCCwithGivenCGC(TheCropCCini, TheCropCCo, TheCropCCx, TheCropCGC, 0);
        }
    }
    return ElapsedTime;
}

dp MultiplierCCxSelfThinning(int32_t Yeari, int32_t Yearx, dp ShapeFactor)
{
    dp fCCx = 1.0;
    if (Yeari >= 2 && Yearx >= 2 && roundc(100.0 * ShapeFactor, 1) != 0)
    {
        dp Year0 = 1.0 + static_cast<dp>(Yearx - 1) * std::exp(ShapeFactor * std::log(10.0));
        if (static_cast<dp>(Yeari) >= Year0)
        {
            fCCx = 0.0;
        }
        else
        {
            fCCx = 0.9 + 0.1 * (1.0 - std::exp((1.0 / ShapeFactor) * std::log((static_cast<dp>(Yeari) - 1.0) / (static_cast<dp>(Yearx) - 1.0))));
        }
        if (fCCx < 0.0)
        {
            fCCx = 0.0;
        }
    }
    return fCCx;
}

int32_t DaysToReachCCwithGivenCGC(dp CCToReach, dp CCoVal, dp CCxVal, dp CGCVal, int32_t L0)
{
    dp L;
    dp CCToReach_local = CCToReach;

    if (CCoVal > CCToReach_local || CCoVal >= CCxVal)
    {
        L = 0.0;
    }
    else
    {
        if (CCToReach_local > (0.98 * CCxVal))
        {
            CCToReach_local = 0.98 * CCxVal;
        }
        if (CCToReach_local <= CCxVal / 2.0)
        {
            L = std::log(CCToReach_local / CCoVal) / CGCVal;
        }
        else
        {
            L = std::log((0.25 * CCxVal * CCxVal / CCoVal) / (CCxVal - CCToReach_local)) / CGCVal;
        }
    }
    return L0 + roundc(L, 1);
}

int32_t LengthCanopyDecline(dp CCx, dp CDC)
{
    int32_t ND = 0;
    if (CCx > 0.0)
    {
        if (CDC <= 1e-9)
        {
            ND = undef_int;
        }
        else
        {
            ND = roundc((((CCx + 2.29) / (CDC * 3.33)) * std::log(1.0 + 1.0 / 0.05) + 0.50), 1);
        }
    }
    return ND;
}

dp HarvestIndexGrowthCoefficient(dp HImax, dp dHIdt)
{
    dp HIo = 1.0;
    dp HIvar, HIGC, t;

    if (HImax > HIo)
    {
        t = HImax / dHIdt;
        HIGC = 0.001;
        do
        {
            HIGC += 0.001;
            HIvar = (HIo * HImax) / (HIo + (HImax - HIo) * std::exp(-HIGC * t));
        } while (HIvar <= (0.98 * HImax));

        if (HIvar >= HImax)
        {
            HIGC -= 0.001;
        }
    }
    else
    {
        HIGC = static_cast<dp>(undef_int);
    }
    return HIGC;
}

dp TauFromKsat(dp Ksat)
{
    if (std::abs(Ksat) < 1e-9)
    {
        return 0.0;
    }
    else
    {
        int32_t TauTemp = roundc(100.0 * 0.0866 * std::exp(0.35 * std::log(Ksat)), 1);
        if (TauTemp < 0) TauTemp = 0;
        if (TauTemp > 100) TauTemp = 100;
        return static_cast<dp>(TauTemp) / 100.0;
    }
}

int8_t NumberSoilClass(dp SatvolPro, dp FCvolPro, dp PWPvolPro, dp Ksatmm)
{
    if (SatvolPro <= 55.0)
    {
        if (PWPvolPro >= 20.0)
        {
            if (SatvolPro >= 49.0 && FCvolPro >= 40.0)
            {
                return 4; // silty clayey soils
            }
            else
            {
                return 3; // sandy clayey soils
            }
        }
        else
        {
            if (FCvolPro < 23.0)
            {
                return 1; // sandy soils
            }
            else
            {
                if (PWPvolPro > 16.0 && Ksatmm < 100.0)
                {
                    return 3; // sandy clayey soils
                }
                else
                {
                    if (PWPvolPro < 6.0 && FCvolPro < 28.0 && Ksatmm > 750.0)
                    {
                        return 1; // sandy soils
                    }
                    else
                    {
                        return 2; // loamy soils
                    }
                }
            }
        }
    }
    else
    {
        return 4; // silty clayey soils
    }
}

void DeriveSmaxTopBottom(dp SxTopQ, dp SxBotQ, dp& SxTop, dp& SxBot)
{
    dp x, V1, V2, V11, V22;

    V1 = SxTopQ;
    V2 = SxBotQ;
    if (std::abs(V1 - V2) < 1e-12)
    {
        SxTop = V1;
        SxBot = V2;
    }
    else
    {
        if (SxTopQ < SxBotQ)
        {
            V1 = SxBotQ;
            V2 = SxTopQ;
        }
        x = 3.0 * V2 / (V1 - V2);
        if (x < 0.5)
        {
            V11 = (4.0 / 3.5) * V1;
            V22 = 0.0;
        }
        else
        {
            V11 = (x + 3.5) * V1 / (x + 3.0);
            V22 = (x - 0.5) * V2 / x;
        }
        if (SxTopQ > SxBotQ)
        {
            SxTop = V11;
            SxBot = V22;
        }
        else
        {
            SxTop = V22;
            SxBot = V11;
        }
    }
}

namespace { // Anonymous namespace for helper functions
dp GetKs(dp T0, dp T1, dp Tin)
{
    const dp Mo = 0.02;
    const dp Mx = 1.0;
    dp MRate, Ksi, Trel;

    Trel = (Tin - T0) / (T1 - T0);
    MRate = -1.0 * (std::log((Mo * Mx - 0.98 * Mo) / (0.98 * (Mx - Mo))));
    Ksi = (Mo * Mx) / (Mo + (Mx - Mo) * std::exp(-MRate * Trel));
    Ksi = Ksi - Mo * (1.0 - Trel);
    return Ksi;
}
} // namespace

dp KsTemperature(dp T0, dp T1, dp Tin)
{
    dp M = 1.0;
    int a;

    if (roundc(T0, 1) != undef_int && roundc(T1, 1) != undef_int && std::abs(T0 - T1) > 1e-9)
    {
        if (T0 < T1)
        {
            a = 1;
        }
        else
        {
            a = -1;
        }
        if (static_cast<dp>(a) * Tin > static_cast<dp>(a) * T0 && static_cast<dp>(a) * Tin < static_cast<dp>(a) * T1)
        {
            M = GetKs(T0, T1, Tin);
            if (M < 0.0) M = 0.0;
            if (M > 1.0) M = 1.0;
        }
        else
        {
            if (static_cast<dp>(a) * Tin <= static_cast<dp>(a) * T0) M = 0.0;
            if (static_cast<dp>(a) * Tin >= static_cast<dp>(a) * T1) M = 1.0;
        }
    }
    return M;
}

dp KsSalinity(bool SalinityResponsConsidered, int8_t ECeN, int8_t ECeX, dp ECeVAR, dp KsShapeSalinity)
{
    dp M = 1.0;
    dp tmp_var;

    if (SalinityResponsConsidered)
    {
        if (ECeVAR > static_cast<dp>(ECeN) && ECeVAR < static_cast<dp>(ECeX))
        {
            if (roundc(KsShapeSalinity * 10.0, 1) != 0 && roundc(KsShapeSalinity * 10.0, 1) != 990)
            {
                tmp_var = static_cast<dp>(ECeN);
                M = KsAny(ECeVAR, tmp_var, static_cast<dp>(ECeX), KsShapeSalinity);
            }
            else
            {
                if (roundc(KsShapeSalinity * 10.0, 1) == 0)
                {
                    M = 1.0 - (ECeVAR - static_cast<dp>(ECeN)) / (static_cast<dp>(ECeX) - static_cast<dp>(ECeN));
                }
                else
                {
                    M = KsTemperature(static_cast<dp>(ECeX), static_cast<dp>(ECeN), ECeVAR);
                }
            }
        }
        else
        {
            if (ECeVAR <= static_cast<dp>(ECeN)) M = 1.0;
            if (ECeVAR >= static_cast<dp>(ECeX)) M = 0.0;
        }
    }

    if (M > 1.0) M = 1.0;
    if (M < 0.0) M = 0.0;

    return M;
}

void TimeToMaxCanopySF(dp CCo, dp CGC, dp CCx, int32_t L0, int32_t L12, int32_t L123, int32_t LToFlor, int32_t LFlor, bool DeterminantCrop, int32_t& L12SF, int8_t& RedCGC, int8_t& RedCCx, int32_t& ClassSF)
{
    dp CCToReach;
    int32_t L12SFmax;

    if (ClassSF == 0 || (static_cast<int32_t>(RedCCx) == 0 && static_cast<int32_t>(RedCGC) == 0))
    {
        L12SF = L12;
    }
    else
    {
        CCToReach = 0.98 * (1.0 - static_cast<dp>(RedCCx) / 100.0) * CCx;
        L12SF = DaysToReachCCwithGivenCGC(CCToReach, CCo, ((1.0 - static_cast<dp>(RedCCx) / 100.0) * CCx), (CGC * (1.0 - static_cast<dp>(RedCGC) / 100.0)), L0);

        if (DeterminantCrop)
        {
            L12SFmax = LToFlor + roundc(static_cast<dp>(LFlor) / 2.0, 1);
        }
        else
        {
            L12SFmax = L123;
        }

        if (L12SF > L12SFmax)
        {
            while (L12SF > L12SFmax && static_cast<int32_t>(RedCGC) > 0)
            {
                RedCGC--;
                L12SF = DaysToReachCCwithGivenCGC(CCToReach, CCo, ((1.0 - static_cast<dp>(RedCCx) / 100.0) * CCx), (CGC * (1.0 - static_cast<dp>(RedCGC) / 100.0)), L0);
            }
            while (L12SF > L12SFmax && ((1.0 - static_cast<dp>(RedCCx) / 100.0) * CCx) > 0.10 && static_cast<int32_t>(RedCCx) <= 50)
            {
                RedCCx++;
                CCToReach = 0.98 * (1.0 - static_cast<dp>(RedCCx) / 100.0) * CCx;
                L12SF = DaysToReachCCwithGivenCGC(CCToReach, CCo, ((1.0 - static_cast<dp>(RedCCx) / 100.0) * CCx), (CGC * (1.0 - static_cast<dp>(RedCGC) / 100.0)), L0);
            }
        }
    }
}

dp SoilEvaporationReductionCoefficient(dp Wrel, dp Edecline)
{
    if (Wrel <= 0.00001)
    {
        return 0.0;
    }
    else if (Wrel >= 0.99999)
    {
        return 1.0;
    }
    else
    {
        return (std::exp(Edecline * Wrel) - 1.0) / (std::exp(Edecline) - 1.0);
    }
}

dp MaxCRatDepth(dp ParamCRa, dp ParamCRb, dp Ksat, dp Zi, dp DepthGWT)
{
    dp CRmax = 0.0;
    if (Ksat > 0.0 && DepthGWT > 0.0 && (DepthGWT - Zi) < 4.0)
    {
        if (Zi >= DepthGWT)
        {
            CRmax = 99.0;
        }
        else
        {
            CRmax = std::exp((std::log(DepthGWT - Zi) - ParamCRb) / ParamCRa);
            if (CRmax > 99.0)
            {
                CRmax = 99.0;
            }
        }
    }
    return CRmax;
}

dp CCmultiplierWeed(int8_t ProcentWeedCover, dp CCxCrop, dp FshapeWeed)
{
    dp fWeed;

    if (static_cast<int32_t>(ProcentWeedCover) > 0 && CCxCrop < 0.9999 && CCxCrop > 0.001)
    {
        if (static_cast<int32_t>(ProcentWeedCover) == 100)
        {
            fWeed = 1.0 / CCxCrop;
        }
        else
        {
            fWeed = 1.0 - (1.0 - 1.0 / CCxCrop) * (std::exp(FshapeWeed * static_cast<dp>(ProcentWeedCover) / 100.0) - 1.0) / (std::exp(FshapeWeed) - 1.0);
            if (fWeed > (1.0 / CCxCrop))
            {
                fWeed = 1.0 / CCxCrop;
            }
        }
    }
    else
    {
        fWeed = 1.0;
    }
    return fWeed;
}

dp CCmultiplierWeedAdjusted(int8_t ProcentWeedCover, dp CCxCrop, dp& FshapeWeed, dp fCCx, int8_t Yeari, int8_t MWeedAdj, int8_t& RCadj)
{
    dp fWeedi, CCxTot100, CCxTot0, CCxTotM, fweedMax, RCadjD, FshapeMinimum;

    fWeedi = 1.0;
    RCadj = ProcentWeedCover;
    if (static_cast<int32_t>(ProcentWeedCover) > 0)
    {
        fWeedi = CCmultiplierWeed(ProcentWeedCover, CCxCrop, FshapeWeed);
        if (crop.CropSubkind == subkind::Forage && static_cast<int32_t>(Yeari) > 1 && fCCx < 0.995)
        {
            FshapeMinimum = 10.0 - 20.0 * ((std::exp(fCCx * 3.0) - 1.0) / (std::exp(3.0) - 1.0) + std::sqrt(static_cast<dp>(MWeedAdj) / 100.0));
            if (roundc(FshapeMinimum * 10.0, 1) == 0) FshapeMinimum = 0.1;
            if (FshapeWeed < FshapeMinimum) FshapeWeed = FshapeMinimum;

            fWeedi = CCmultiplierWeed(ProcentWeedCover, CCxCrop, FshapeWeed);
            CCxTot100 = fWeedi * CCxCrop;

            if (fCCx > 0.005)
            {
                fWeedi = CCmultiplierWeed(static_cast<int8_t>(roundc(fCCx * static_cast<dp>(ProcentWeedCover), 1)), (fCCx * CCxCrop), FshapeWeed);
            }
            else
            {
                fWeedi = 1.0;
            }
            CCxTot0 = fWeedi * (fCCx * CCxCrop);
            CCxTotM = CCxTot0 + (CCxTot100 - CCxTot0) * static_cast<dp>(MWeedAdj) / 100.0;

            if (CCxTotM < (fCCx * CCxCrop * (1.0 - static_cast<dp>(ProcentWeedCover) / 100.0)))
            {
                CCxTotM = fCCx * CCxCrop * (1.0 - static_cast<dp>(ProcentWeedCover) / 100.0);
            }
            if (fCCx > 0.005)
            {
                fWeedi = CCxTotM / (fCCx * CCxCrop);
                fweedMax = 1.0 / (fCCx * CCxCrop);
                if (roundc(fWeedi * 1000.0, 1) > roundc(fweedMax * 1000.0, 1))
                {
                    fWeedi = fweedMax;
                }
            }

            RCadjD = static_cast<dp>(ProcentWeedCover) + (1.0 - fCCx) * CCxCrop * static_cast<dp>(MWeedAdj);
            if (fCCx > 0.005)
            {
                if (RCadjD < (100.0 * (CCxTotM - fCCx * CCxCrop) / CCxTotM))
                {
                    RCadjD = 100.0 * (CCxTotM - fCCx * CCxCrop) / CCxTotM;
                }
                if (RCadjD > (100.0 * (1.0 - (fCCx * CCxCrop * (1.0 - static_cast<dp>(ProcentWeedCover) / 100.0) / CCxTotM))))
                {
                    RCadjD = 100.0 * (1.0 - fCCx * CCxCrop * (1.0 - static_cast<dp>(ProcentWeedCover) / 100.0) / CCxTotM);
                }
            }
            RCadj = static_cast<int8_t>(roundc(RCadjD, 1));
            if (static_cast<int32_t>(RCadj) > 100) RCadj = 100;
        }
    }
    return fWeedi;
}

dp BMRange(int32_t HIadj)
{
    dp BMR;
    if (HIadj <= 0)
    {
        BMR = 0.0;
    }
    else
    {
        BMR = (std::log(static_cast<dp>(HIadj)) / 0.0562) / 100.0;
    }
    if (BMR > 1.0) BMR = 1.0;
    return BMR;
}

dp HImultiplier(dp RatioBM, dp RangeBM, int8_t HIadj)
{
    dp Rini, Rmax, Rend;

    Rini = 1.0 - RangeBM;
    Rend = 1.0;
    Rmax = Rini + (2.0 / 3.0) * (Rend - Rini);

    if (RatioBM <= Rini)
    {
        return 1.0;
    }
    else if (RatioBM <= Rmax)
    {
        return 1.0 + (1.0 + std::sin(PI * (1.5 - (RatioBM - Rini) / (Rmax - Rini)))) * (static_cast<dp>(HIadj) / 200.0);
    }
    else if (RatioBM <= Rend)
    {
        return 1.0 + (1.0 + std::sin(PI * (0.5 + (RatioBM - Rmax) / (Rend - Rmax)))) * (static_cast<dp>(HIadj) / 200.0);
    }
    else
    {
        return 1.0;
    }
}

dp AdjustedKsStoToECsw(int8_t ECeMin, int8_t ECeMax, int32_t ResponseECsw, dp ECei, dp ECswi, dp ECswFCi, dp Wrel, dp Coeffb0Salt, dp Coeffb1Salt, dp Coeffb2Salt, dp KsStoIN)
{
    dp ECswRel, LocalKsShapeFactorSalt, KsSalti, SaltStressi, StoClosure, KsStoOut;

    if (ResponseECsw > 0 && Wrel > 1e-9 && Simulation.SalinityConsidered)
    {
        ECswRel = ECswi - (ECswFCi - ECei) + (static_cast<dp>(ResponseECsw) - 100.0) * Wrel;
        if (ECswRel > static_cast<dp>(ECeMin) && ECswRel < static_cast<dp>(ECeMax))
        {
            LocalKsShapeFactorSalt = 3.0;
            KsSalti = KsSalinity(true, ECeMin, ECeMax, ECswRel, LocalKsShapeFactorSalt);
            SaltStressi = (1.0 - KsSalti) * 100.0;
            StoClosure = Coeffb0Salt + Coeffb1Salt * SaltStressi + Coeffb2Salt * SaltStressi * SaltStressi;
            KsStoOut = (1.0 - StoClosure / 100.0);
            if (KsStoOut < 0.0) KsStoOut = 0.0;
            if (KsStoOut > KsStoIN) KsStoOut = KsStoIN;
        }
        else
        {
            if (ECswRel >= static_cast<dp>(ECeMax))
            {
                KsStoOut = 0.0;
            }
            else
            {
                KsStoOut = KsStoIN;
            }
        }
    }
    else
    {
        KsStoOut = KsStoIN;
    }
    return KsStoOut;
}

dp CCatTime(int32_t Dayi, dp CCoIN, dp CGCIN, dp CCxIN)
{
    dp CCi;
    CCi = CCoIN * std::exp(CGCIN * static_cast<dp>(Dayi));
    if (CCi > CCxIN / 2.0)
    {
        CCi = CCxIN - 0.25 * (CCxIN / CCoIN) * CCxIN * std::exp(-CGCIN * static_cast<dp>(Dayi));
    }
    return CCi;
}

void DetermineDayNr(int32_t Dayi, int32_t Monthi, int32_t Yeari, int32_t& DayNr)
{
    DayNr = static_cast<int32_t>(static_cast<dp>(Yeari - 1901) * 365.25 + ElapsedDays[Monthi-1] + static_cast<dp>(Dayi) + 0.05);
}

void DetermineDate(int32_t DayNr, int32_t& Dayi, int32_t& Monthi, int32_t& Yeari)
{
    dp SumDayMonth;

    Yeari = static_cast<int32_t>((static_cast<dp>(DayNr) - 0.05) / 365.25);
    SumDayMonth = (static_cast<dp>(DayNr) - static_cast<dp>(Yeari) * 365.25);
    Yeari = 1901 + Yeari;
    Monthi = 1;

    while (Monthi < 12)
    {
        if (SumDayMonth <= ElapsedDays[Monthi]) break; // ElapsedDays[Monthi] corresponds to ElapsedDays(Monthi+1) in Fortran
        Monthi++;
    }
    Dayi = roundc(SumDayMonth - ElapsedDays[Monthi-1] + 0.25 + 0.06, 1);
}

dp DegreesDay(dp Tbase, dp Tupper, dp TDayMin, dp TDayMax, int8_t GDDSelectedMethod)
{
    dp TstarMax, TstarMin, Tavg;

    switch (GDDSelectedMethod)
    {
    case 1:
        Tavg = (TDayMax + TDayMin) / 2.0;
        if (Tavg > Tupper) Tavg = Tupper;
        if (Tavg < Tbase) Tavg = Tbase;
        break;
    case 2:
        TstarMax = TDayMax;
        if (TDayMax < Tbase) TstarMax = Tbase;
        if (TDayMax > Tupper) TstarMax = Tupper;
        TstarMin = TDayMin;
        if (TDayMin < Tbase) TstarMin = Tbase;
        if (TDayMin > Tupper) TstarMin = Tupper;
        Tavg = (TstarMax + TstarMin) / 2.0;
        break;
    default: // Method 3
        TstarMax = TDayMax;
        if (TDayMax < Tbase) TstarMax = Tbase;
        if (TDayMax > Tupper) TstarMax = Tupper;
        TstarMin = TDayMin;
        if (TDayMin > Tupper) TstarMin = Tupper;
        Tavg = (TstarMax + TstarMin) / 2.0;
        if (Tavg < Tbase) Tavg = Tbase;
        break;
    }
    return Tavg - Tbase;
}

void DetermineCNIandIII(int8_t CN2, int8_t& CN1, int8_t& CN3)
{
    CN1 = static_cast<int8_t>(roundc(1.4 * (std::exp(-14.0 * std::log(10.0))) + 0.507 * static_cast<dp>(CN2) - 0.00374 * static_cast<dp>(CN2) * static_cast<dp>(CN2) + 0.0000867 * static_cast<dp>(CN2) * static_cast<dp>(CN2) * static_cast<dp>(CN2), 1));
    CN3 = static_cast<int8_t>(roundc(5.6 * (std::exp(-14.0 * std::log(10.0))) + 2.33 * static_cast<dp>(CN2) - 0.0209 * static_cast<dp>(CN2) * static_cast<dp>(CN2) + 0.000076 * static_cast<dp>(CN2) * static_cast<dp>(CN2) * static_cast<dp>(CN2), 1));

    if (static_cast<int32_t>(CN1) <= 0) CN1 = 1;
    else if (static_cast<int32_t>(CN1) > 100) CN1 = 100;

    if (static_cast<int32_t>(CN3) <= 0) CN3 = 1;
    else if (static_cast<int32_t>(CN3) > 100) CN3 = 100;

    if (static_cast<int32_t>(CN3) < static_cast<int32_t>(CN2)) CN3 = CN2;
}

void DetermineCN_default(dp Infiltr, int8_t& CN2)
{
    if (Infiltr > 864.0)
    {
        CN2 = 46;
    }
    else if (Infiltr >= 347.0)
    {
        CN2 = 61;
    }
    else if (Infiltr >= 36.0)
    {
        CN2 = 72;
    }
    else
    {
        CN2 = 77;
    }
}

dp ECeComp(const CompartmentIndividual& Comp)
{
    dp volSAT, TotSalt, denominator;
    int32_t i;

    volSAT = soillayer[Comp.Layer - 1].SAT;
    TotSalt = 0.0;
    for (i = 0; i < static_cast<int32_t>(soillayer[Comp.Layer - 1].SCP1); ++i)
    {
        TotSalt += Comp.Salt[i] + Comp.Depo[i];
    }

    denominator = volSAT * 10.0 * Comp.Thickness * (1.0 - soillayer[Comp.Layer - 1].GravelVol / 100.0);
    TotSalt = TotSalt / denominator;

    if (TotSalt > static_cast<dp>(simulparam.SaltSolub))
    {
        TotSalt = static_cast<dp>(simulparam.SaltSolub);
    }

    return TotSalt / equiv;
}

dp ECswComp(const CompartmentIndividual& Comp, bool atFC)
{
    dp TotSalt;
    int32_t i;

    TotSalt = 0.0;
    for (i = 0; i < static_cast<int32_t>(soillayer[Comp.Layer - 1].SCP1); ++i)
    {
        TotSalt += Comp.Salt[i] + Comp.Depo[i];
    }

    if (atFC)
    {
        TotSalt = TotSalt / (soillayer[Comp.Layer - 1].FC * 10.0 * Comp.Thickness * (1.0 - soillayer[Comp.Layer - 1].GravelVol / 100.0));
    }
    else
    {
        TotSalt = TotSalt / (Comp.theta * 1000.0 * Comp.Thickness * (1.0 - soillayer[Comp.Layer - 1].GravelVol / 100.0));
    }

    if (TotSalt > static_cast<dp>(simulparam.SaltSolub))
    {
        TotSalt = static_cast<dp>(simulparam.SaltSolub);
    }

    return TotSalt / equiv;
}

void SaltSolutionDeposit(dp mm, dp& SaltSolution, dp& SaltDeposit)
{
    SaltSolution += SaltDeposit;
    if (SaltSolution > static_cast<dp>(simulparam.SaltSolub) * mm)
    {
        SaltDeposit = SaltSolution - static_cast<dp>(simulparam.SaltSolub) * mm;
        SaltSolution = static_cast<dp>(simulparam.SaltSolub) * mm;
    }
    else
    {
        SaltDeposit = 0.0;
    }
}

dp MultiplierCCoSelfThinning(int32_t Yeari, int32_t Yearx, dp ShapeFactor)
{
    dp fCCo = 1.0;
    dp Year0;

    if (Yeari >= 1 && Yearx >= 2 && roundc(100.0 * ShapeFactor, 1) != 0)
    {
        Year0 = 1.0 + static_cast<dp>(Yearx - 1) * std::exp(ShapeFactor * std::log(10.0));
        if (static_cast<dp>(Yeari) >= Year0 || Year0 <= 1.0)
        {
            fCCo = 0.0;
        }
        else
        {
            fCCo = 1.0 - static_cast<dp>(Yeari - 1) / (Year0 - 1.0);
        }
        if (fCCo < 0.0) fCCo = 0.0;
    }
    return fCCo;
}

dp KsAny(dp Wrel, dp pULActual, dp pLLActual, dp ShapeFactor)
{
    dp pRelativeLLUL, KsVal;
    dp pULActual_local = pULActual;

    if ((pLLActual - pULActual_local) < 0.0001)
    {
        pULActual_local = pLLActual - 0.0001;
    }

    pRelativeLLUL = (Wrel - pULActual_local) / (pLLActual - pULActual_local);

    if (pRelativeLLUL <= 1e-9)
    {
        KsVal = 1.0;
    }
    else if (pRelativeLLUL >= 1.0)
    {
        KsVal = 0.0;
    }
    else
    {
        if (roundc(10.0 * ShapeFactor, 1) == 0)
        {
            KsVal = 1.0 - (std::exp(pRelativeLLUL * 0.01) - 1.0) / (std::exp(0.01) - 1.0);
        }
        else
        {
            KsVal = 1.0 - (std::exp(pRelativeLLUL * ShapeFactor) - 1.0) / (std::exp(ShapeFactor) - 1.0);
        }
        if (KsVal > 1.0) KsVal = 1.0;
        if (KsVal < 0.0) KsVal = 0.0;
    }
    return KsVal;
}

dp CCatGDD(dp GDDi, dp CCoIN, dp GDDCGCIN, dp CCxIN)
{
    dp CCi;
    CCi = CCoIN * std::exp(GDDCGCIN * GDDi);
    if (CCi > CCxIN / 2.0)
    {
        CCi = CCxIN - 0.25 * (CCxIN / CCoIN) * CCxIN * std::exp(-GDDCGCIN * GDDi);
    }
    return CCi;
}

dp CanopyCoverNoStressGDDaysSF(int32_t GDDL0, int32_t GDDL123, int32_t GDDLMaturity, dp SumGDD, dp CCo, dp CCx, dp CGC, dp CDC, dp GDDCGC, dp GDDCDC, int8_t SFRedCGC, int8_t SFRedCCx)
{
    dp CC = 0.0;
    dp CCxAdj, GDDCDCadj;

    if (SumGDD > 0.0 && roundc(SumGDD, 1) <= GDDLMaturity && CCo > 0.0)
    {
        if (SumGDD <= static_cast<dp>(GDDL0))
        {
            CC = 0.0;
        }
        else
        {
            if (SumGDD < static_cast<dp>(GDDL123))
            {
                CC = CCatGDD(SumGDD - static_cast<dp>(GDDL0), CCo, ((1.0 - static_cast<dp>(SFRedCGC) / 100.0) * GDDCGC), ((1.0 - static_cast<dp>(SFRedCCx) / 100.0) * CCx));
            }
            else
            {
                if (CCx < 0.001)
                {
                    CC = 0.0;
                }
                else
                {
                    CCxAdj = CCatGDD(static_cast<dp>(GDDL123 - GDDL0), CCo, ((1.0 - static_cast<dp>(SFRedCGC) / 100.0) * GDDCGC), ((1.0 - static_cast<dp>(SFRedCCx) / 100.0) * CCx));
                    GDDCDCadj = GDDCDC * (CCxAdj + 2.29) / (CCx + 2.29);
                    if (CCxAdj < 0.001)
                    {
                        CC = 0.0;
                    }
                    else
                    {
                        CC = CCxAdj * (1.0 - 0.05 * (std::exp((SumGDD - static_cast<dp>(GDDL123)) * 3.33 * GDDCDCadj / (CCxAdj + 2.29)) - 1.0));
                    }
                }
            }
        }
    }
    if (CC > 1.0) CC = 1.0;
    if (CC < 0.0) CC = 0.0;
    return CC;
}

dp HIadjWStressAtFlowering(dp KsVeg, dp KsSto, int8_t a, dp b)
{
    dp result;
    if (static_cast<int32_t>(a) == undef_int)
    {
        if (roundc(b, 1) == undef_int)
        {
            result = 1.0;
        }
        else if (KsSto > 0.001)
        {
            result = (std::exp(0.10 * std::log(KsSto))) * (1.0 - (1.0 - KsSto) / b);
        }
        else
        {
            result = 0.0;
        }
    }
    else
    {
        if (roundc(b, 1) == undef_int)
        {
            result = (1.0 + (1.0 - KsVeg) / static_cast<dp>(a));
        }
        else if (KsSto > 0.001)
        {
            result = (1.0 + (1.0 - KsVeg) / static_cast<dp>(a)) * (std::exp(0.10 * std::log(KsSto))) * (1.0 - (1.0 - KsSto) / b);
        }
        else
        {
            result = 0.0;
        }
    }
    return result;
}

dp fAdjustedForCO2(dp CO2i, dp WPi, int8_t PercentA)
{
    dp fW, fCO2Old, fType, fSink, fShape, CO2rel, fCO2adj, fCO2;

    // 1. Correction for crop type: fType
    if (WPi >= 40.0)
    {
        fType = 0.0; // no correction for C4 crops
    }
    else
    {
        if (WPi <= 20.0)
        {
            fType = 1.0; // full correction for C3 crops
        }
        else
        {
            fType = (40.0 - WPi) / (40.0 - 20.0);
        }
    }

    // 2. crop sink strength coefficient: fSink
    fSink = static_cast<dp>(PercentA) / 100.0;
    if (fSink < 0.0) fSink = 0.0;
    if (fSink > 1.0) fSink = 1.0;

    // 3. Correction coefficient for CO2: fCO2Old
    fCO2Old = static_cast<dp>(undef_int);
    if (CO2i <= 550.0)
    {
        if (CO2i <= CO2Ref)
        {
            fW = 0.0;
        }
        else
        {
            if (CO2i >= 550.0)
            {
                fW = 1.0;
            }
            else
            {
                fW = 1.0 - (550.0 - CO2i) / (550.0 - CO2Ref);
            }
        }

        fCO2Old = (CO2i / CO2Ref) / (1.0 + (CO2i - CO2Ref) * ((1.0 - fW) * 0.000138 + fW * (0.000138 * fSink + 0.001165 * (1.0 - fSink))));
    }

    // 4. Adjusted correction coefficient for CO2: fCO2adj
    fCO2adj = static_cast<dp>(undef_int);
    if (CO2i > CO2Ref)
    {
        fShape = -4.61824 - 3.43831 * fSink - 5.32587 * fSink * fSink;

        if (CO2i >= 2000.0)
        {
            fCO2adj = 1.58;
        }
        else
        {
            CO2rel = (CO2i - CO2Ref) / (2000.0 - CO2Ref);
            fCO2adj = 1.0 + 0.58 * ((std::exp(CO2rel * fShape) - 1.0) / (std::exp(fShape) - 1.0));
        }
    }

    // 5. Selected adjusted coefficient for CO2: fCO2
    if (CO2i <= CO2Ref)
    {
        fCO2 = fCO2Old;
    }
    else
    {
        fCO2 = fCO2adj;
        if (CO2i <= 550.0 && fCO2Old < fCO2adj)
        {
            fCO2 = fCO2Old;
        }
    }

    // 6. final adjustment
    return 1.0 + fType * (fCO2 - 1.0);
}

bool FullUndefinedRecord(int32_t FromY, int32_t FromD, int32_t FromM, int32_t ToD, int32_t ToM)
{
    return (FromY == 1901 && FromD == 1 && FromM == 1 && ToD == 31 && ToM == 12);
}

void NoIrrigation()
{
    IrriMode_Val = IrriMode::NoIrri;
    IrriDescription = "Rainfed cropping";
    IrriMethod_Val = IrriMethod::MSprinkler;
    Simulation.IrriECw = 0.0;
    GenerateTimeMode_Val = GenerateTimeMode::AllRAW;
    GenerateDepthMode_Val = GenerateDepthMode::ToFC;
    IrriFirstDayNr = undef_int;
    for (int Nri = 0; Nri < 5; ++Nri)
    {
        IrriBeforeSeason[Nri].DayNr = 0;
        IrriBeforeSeason[Nri].param = 0;
        IrriAfterSeason[Nri].DayNr = 0;
        IrriAfterSeason[Nri].param = 0;
    }
    IrriECw.PreSeason = 0.0;
    IrriECw.PostSeason = 0.0;
}

void LoadIrriScheduleInfo(const std::string& FullName)
{
    std::ifstream file(FullName);
    if (!file.is_open()) return;

    int32_t i;
    dp VersionNr;

    std::getline(file, IrriDescription);
    file >> VersionNr;

    file >> i;
    switch (i)
    {
    case 1: IrriMethod_Val = IrriMethod::MSprinkler; break;
    case 2: IrriMethod_Val = IrriMethod::MBasin; break;
    case 3: IrriMethod_Val = IrriMethod::MBorder; break;
    case 4: IrriMethod_Val = IrriMethod::MFurrow; break;
    default: IrriMethod_Val = IrriMethod::MDrip; break;
    }

    int temp_int;
    file >> temp_int; simulparam.IrriFwInSeason = static_cast<int8_t>(temp_int);

    file >> i;
    switch (i)
    {
    case 0: IrriMode_Val = IrriMode::NoIrri; break;
    case 1: IrriMode_Val = IrriMode::Manual; break;
    case 2: IrriMode_Val = IrriMode::Generate; break;
    default: IrriMode_Val = IrriMode::Inet; break;
    }

    if (i == 1 && roundc(VersionNr * 10.0, 1) >= 70)
    {
        file >> IrriFirstDayNr;
    }
    else
    {
        IrriFirstDayNr = undef_int;
    }

    if (IrriMode_Val == IrriMode::Generate)
    {
        file >> i;
        switch (i)
        {
        case 1: GenerateTimeMode_Val = GenerateTimeMode::FixInt; break;
        case 2: GenerateTimeMode_Val = GenerateTimeMode::AllDepl; break;
        case 3: GenerateTimeMode_Val = GenerateTimeMode::AllRAW; break;
        case 4: GenerateTimeMode_Val = GenerateTimeMode::WaterBetweenBunds; break;
        default: GenerateTimeMode_Val = GenerateTimeMode::AllRAW; break;
        }
        file >> i;
        switch (i)
        {
        case 1: GenerateDepthMode_Val = GenerateDepthMode::ToFC; break;
        default: GenerateDepthMode_Val = GenerateDepthMode::FixDepth; break;
        }
        IrriFirstDayNr = undef_int;
    }

    if (IrriMode_Val == IrriMode::Inet)
    {
        file >> simulparam.PercRAW;
        IrriFirstDayNr = undef_int;
    }
    file.close();
}

void GenerateCO2Description(const std::string& CO2FileFull, std::string& CO2Description)
{
    std::ifstream file(CO2FileFull);
    if (file.is_open())
    {
        std::getline(file, CO2Description);
    }
    if (CO2File == "MaunaLoa.CO2")
    {
        CO2Description = "Default atmospheric CO2 concentration from 1902 to 2099";
    }
}

void GetIrriDescription(const std::string& IrriFileFull, std::string& IrriDescription)
{
    std::ifstream file(IrriFileFull);
    if (file.is_open())
    {
        std::getline(file, IrriDescription);
    }
}

void SetIrriDescription(const std::string& str)
{
    IrriDescription = str;
}

void GetDaySwitchToLinear(int32_t HImax, dp dHIdt, dp HIGC, int32_t& tSwitch, dp& HIGClinear)
{
    dp HIi, HiM1, HIfinal;
    int32_t tmax, ti;
    const int32_t HIo = 1;

    tmax = static_cast<int32_t>(roundc(static_cast<dp>(HImax) / dHIdt, 1));
    ti = 0;
    HiM1 = static_cast<dp>(HIo);
    if (tmax > 0)
    {
        do
        {
            ti++;
            HIi = (static_cast<dp>(HIo) * static_cast<dp>(HImax)) / (static_cast<dp>(HIo) + (static_cast<dp>(HImax) - static_cast<dp>(HIo)) * std::exp(-HIGC * static_cast<dp>(ti)));
            HIfinal = HIi + static_cast<dp>(tmax - ti) * (HIi - HiM1);
            HiM1 = HIi;
        } while (!((HIfinal > static_cast<dp>(HImax)) || (ti >= tmax)));
        tSwitch = ti - 1;
    }
    else
    {
        tSwitch = 0;
    }

    if (tSwitch > 0)
    {
        HIi = (static_cast<dp>(HIo) * static_cast<dp>(HImax)) / (static_cast<dp>(HIo) + (static_cast<dp>(HImax) - static_cast<dp>(HIo)) * std::exp(-HIGC * static_cast<dp>(tSwitch)));
    }
    else
    {
        HIi = 0.0;
    }
    HIGClinear = (static_cast<dp>(HImax) - HIi) / static_cast<dp>(tmax - tSwitch);
}

void SplitStringInTwoParams(const std::string& StringIN, dp& Par1, dp& Par2)
{
    std::stringstream ss(StringIN);
    ss >> Par1 >> Par2;
}

void SplitStringInThreeParams(const std::string& StringIN, dp& Par1, dp& Par2, dp& Par3)
{
    std::stringstream ss(StringIN);
    ss >> Par1 >> Par2 >> Par3;
}

dp CO2ForSimulationPeriod(int32_t FromDayNr, int32_t ToDayNr)
{
    int32_t Dayi, Monthi, FromYi, ToYi;
    dp CO2From, CO2To, CO2a, CO2b, YearA, YearB;
    std::string TempString;
    std::ifstream fhandle;

    DetermineDate(FromDayNr, Dayi, Monthi, FromYi);
    DetermineDate(ToDayNr, Dayi, Monthi, ToYi);

    if (FromYi == 1901 || ToYi == 1901)
    {
        return CO2Ref;
    }
    else
    {
        fhandle.open(CO2FileFull);
        if (fhandle.is_open())
        {
            std::string line;
            for(int k=0; k<3; ++k) std::getline(fhandle, line); // Skip 3 lines

            std::getline(fhandle, TempString);
            SplitStringInTwoParams(TempString, YearB, CO2b);

            if (roundc(YearB, 1) >= FromYi)
            {
                CO2From = CO2b;
                YearA = YearB;
                CO2a = CO2b;
            }
            else
            {
                do
                {
                    YearA = YearB;
                    CO2a = CO2b;
                    if (!std::getline(fhandle, TempString)) break;
                    SplitStringInTwoParams(TempString, YearB, CO2b);
                } while (!(roundc(YearB, 1) >= FromYi));

                if (FromYi > roundc(YearB, 1))
                {
                    CO2From = CO2b;
                }
                else
                {
                    CO2From = CO2a + (CO2b - CO2a) * static_cast<dp>(FromYi - static_cast<int32_t>(roundc(YearA, 1))) / static_cast<dp>(static_cast<int32_t>(roundc(YearB, 1)) - static_cast<int32_t>(roundc(YearA, 1)));
                }
            }

            CO2To = CO2From;
            if (ToYi > FromYi && static_cast<dp>(ToYi) > roundc(YearA, 1))
            {
                if (roundc(YearB, 1) >= ToYi)
                {
                    CO2To = CO2a + (CO2b - CO2a) * static_cast<dp>(ToYi - static_cast<int32_t>(roundc(YearA, 1))) / static_cast<dp>(static_cast<int32_t>(roundc(YearB, 1)) - static_cast<int32_t>(roundc(YearA, 1)));
                }
                else if (fhandle.good())
                {
                    do
                    {
                        YearA = YearB;
                        CO2a = CO2b;
                        if (!std::getline(fhandle, TempString)) break;
                        SplitStringInTwoParams(TempString, YearB, CO2b);
                        if (roundc(YearB, 1) >= ToYi) break;
                    } while (fhandle.good());

                    if (ToYi > roundc(YearB, 1))
                    {
                        CO2To = CO2b;
                    }
                    else
                    {
                        CO2To = CO2a + (CO2b - CO2a) * static_cast<dp>(ToYi - static_cast<int32_t>(roundc(YearA, 1))) / static_cast<dp>(static_cast<int32_t>(roundc(YearB, 1)) - static_cast<int32_t>(roundc(YearA, 1)));
                    }
                }
            }
            fhandle.close();
            return (CO2From + CO2To) / 2.0;
        }
        return CO2Ref; // Fallback
    }
}

void ReadRainfallSettings()
{
    std::ifstream fhandle;
    std::string fullName = PathNameSimul + "Rainfall.PAR";
    int NrM;
    int effrainperc, effrainshow, effrainrootE;

    fhandle.open(fullName);
    if (fhandle.is_open())
    {
        std::string line;
        std::getline(fhandle, line); // Settings for processing 10-day or monthly rainfall data
        fhandle >> NrM;
        switch (NrM)
        {
        case 0:
            simulparam.EffectiveRain.EffMethod = EffectiveRainMethod::full;
            break;
        case 1:
            simulparam.EffectiveRain.EffMethod = EffectiveRainMethod::usda;
            break;
        case 2:
            simulparam.EffectiveRain.EffMethod = EffectiveRainMethod::percentage;
            break;
        }
        fhandle >> effrainperc;
        simulparam.EffectiveRain.PercentEffRain = static_cast<int8_t>(effrainperc);
        fhandle >> effrainshow;
        simulparam.EffectiveRain.ShowersInDecade = static_cast<int8_t>(effrainshow);
        fhandle >> effrainrootE;
        simulparam.EffectiveRain.RootNrEvap = static_cast<int8_t>(effrainrootE);
        fhandle.close();
    }
}

void ReadSoilSettings()
{
    std::ifstream fhandle;
    std::string fullName = PathNameSimul + "Soil.PAR";
    int i;
    int simul_saltdiff, simul_saltsolub, simul_root, simul_iniab;
    dp simul_rod;

    fhandle.open(fullName);
    if (fhandle.is_open())
    {
        fhandle >> simul_rod;
        simulparam.RunoffDepth = simul_rod;
        fhandle >> i;
        if (i == 1)
        {
            simulparam.CNcorrection = true;
        }
        else
        {
            simulparam.CNcorrection = false;
        }
        fhandle >> simul_saltdiff;
        fhandle >> simul_saltsolub;
        fhandle >> simul_root;
        simulparam.SaltDiff = static_cast<int8_t>(simul_saltdiff);
        simulparam.SaltSolub = static_cast<int8_t>(simul_saltsolub);
        simulparam.RootNrDF = static_cast<int8_t>(simul_root);
        fhandle >> simul_iniab;
        simulparam.IniAbstract = static_cast<int8_t>(simul_iniab);
        fhandle.close();
    }
}

void LoadClimate(const std::string& FullName, std::string& ClimateDescription, std::string& TempFile, std::string& EToFile, std::string& RainFile, std::string& CO2File)
{
    std::ifstream fhandle(FullName);
    if (fhandle.is_open())
    {
        std::string line;
        std::getline(fhandle, ClimateDescription);
        fhandle >> TempFile;
        fhandle >> EToFile;
        fhandle >> RainFile;
        fhandle >> CO2File;
        fhandle.close();
    }
}

void LoadCropCalendar(const std::string& FullName, bool& GetOnset, bool& GetOnsetTemp, int32_t& DayNrStart, int32_t YearStart)
{
    std::ifstream fhandle(FullName);
    if (fhandle.is_open())
    {
        std::string line;
        std::getline(fhandle, CalendarDescription);
        int i;
        fhandle >> i; GetOnset = (i == 1);
        fhandle >> i; GetOnsetTemp = (i == 1);
        // ... (incomplete placeholder logic)
        fhandle.close();
    }
}

void NoManagement()
{
    Management.Mulch = 0;
    Management.SoilCoverBefore = 0;
    Management.SoilCoverAfter = 0;
    Management.EffectMulchOffS = 0;
    Management.EffectMulchInS = 0;
    Management.FertilityStress = 0;
    Management.BundHeight = 0.0;
    Management.RunoffOn = true;
    Management.CNcorrection = 0;
    Management.WeedRC = 0;
    Management.WeedDeltaRC = 0;
    Management.WeedShape = 0.0;
    Management.WeedAdj = 0;
}

void LoadManagement(const std::string& FullName)
{
    std::ifstream fhandle(FullName);
    if (fhandle.is_open())
    {
        std::string line;
        std::getline(fhandle, ManDescription);
        // ... (incomplete placeholder logic)
        fhandle.close();
    }
}

void SaveCrop(const std::string& totalname) {}
void SaveProfile(const std::string& totalname) {}

void DetermineParametersCR(int8_t SoilClass, dp KsatMM, dp& aParam, dp& bParam)
{
    switch (static_cast<int32_t>(SoilClass))
    {
    case 1: // Sandy soils
        aParam = -0.3123 + 0.00060 * KsatMM;
        bParam = -4.493 + 0.00066 * KsatMM;
        break;
    case 2: // Loamy soils
        aParam = -0.2939 + 0.00042 * KsatMM;
        bParam = -4.466 + 0.00082 * KsatMM;
        break;
    case 3: // Sandy clayey soils
        aParam = -0.1924 + 0.00015 * KsatMM;
        bParam = -4.164 + 0.00159 * KsatMM;
        break;
    case 4: // Silty clayey soils
        aParam = -0.1521 + 0.00011 * KsatMM;
        bParam = -3.978 + 0.00138 * KsatMM;
        break;
    default:
        aParam = 0.0;
        bParam = 0.0;
        break;
    }
}

void DetermineNrandThicknessCompartments()
{
    NrCompartments = 12; // Default for many cases
    for (int i = 0; i < static_cast<int32_t>(NrCompartments); ++i)
    {
        Compartment[i].Thickness = 0.1; // Default 10 cm
    }
}

void DetermineRootZoneSaltContent(dp RootingDepth, dp& ZrECe, dp& ZrECsw, dp& ZrECswFC, dp& ZrKsSalt)
{
    // ... (incomplete placeholder logic)
    ZrECe = 0.0;
    ZrECsw = 0.0;
    ZrECswFC = 0.0;
    ZrKsSalt = 1.0;
}

void CalculateAdjustedFC(dp DepthAquifer, std::vector<CompartmentIndividual>& CompartAdj)
{
    // ... (incomplete placeholder logic)
}

void AdjustOnsetSearchPeriod() {}

int32_t ActiveCells(const CompartmentIndividual& Comp)
{
    // Simplified logic
    return 1;
}

void DetermineSaltContent(dp ECe, CompartmentIndividual& Comp)
{
    // ... (incomplete placeholder logic)
}

void SetClimData() {}

std::string DayString(int32_t DNr)
{
    int32_t d, m, y;
    DetermineDate(DNr, d, m, y);
    return std::to_string(d) + " " + NameMonth[m-1] + " " + std::to_string(y);
}

void AdjustYearPerennials(int8_t TheYearSeason, bool Sown1stYear, modeCycle TheCycleMode, dp Zmax, dp ZminYear1, dp TheCCo, dp TheSizeSeedling, dp TheCGC, dp TheCCx, dp TheGDDCGC, int32_t ThePlantingDens, plant& TypeOfPlanting, dp& Zmin, dp& TheSizePlant, dp& TheCCini, int32_t& TheDaysToCCini, int32_t& TheGDDaysToCCini)
{
    // ... (incomplete placeholder logic)
}

void NoCropCalendar()
{
    onset.GenerateOn = false;
    onset.GenerateTempOn = false;
}

void DetermineLinkedSimDay1(int32_t CropDay1, int32_t& SimDay1)
{
    SimDay1 = CropDay1;
}

void AdjustSimPeriod() {}

void ResetSWCToFC()
{
    for (int i = 0; i < static_cast<int32_t>(NrCompartments); ++i)
    {
        Compartment[i].theta = soillayer[Compartment[i].Layer-1].FC / 100.0;
    }
}

void LoadCrop(const std::string& FullName)
{
    std::ifstream fhandle(FullName);
    int32_t XX, YY;
    dp VersionNr;
    int TempShortInt;

    if (fhandle.is_open())
    {
        std::getline(fhandle, CropDescription);
        fhandle >> VersionNr;
        fhandle >> TempShortInt;

        fhandle >> XX;
        switch (XX)
        {
        case 1: crop.CropSubkind = subkind::Vegetative; break;
        case 2: crop.CropSubkind = subkind::Grain; break;
        case 3: crop.CropSubkind = subkind::Tuber; break;
        case 4: crop.CropSubkind = subkind::Forage; break;
        }

        fhandle >> XX;
        switch (XX)
        {
        case 1: crop.Planting = plant::seed; break;
        case 0: crop.Planting = plant::transplant; break;
        case -9: crop.Planting = plant::regrowth; break;
        default: crop.Planting = plant::seed; break;
        }

        fhandle >> XX;
        if (XX == 0) crop.ModeCycle = modeCycle::GDDays; else crop.ModeCycle = modeCycle::CalendarDays;

        fhandle >> YY;
        if (YY == 0) crop.CropPMethod = pMethod::NoCorrection; else if (YY == 1) crop.CropPMethod = pMethod::FAOCorrection;

        fhandle >> crop.Tbase;
        fhandle >> crop.Tupper;
        fhandle >> crop.GDDaysToHarvest;
        fhandle >> crop.pLeafDefUL;
        fhandle >> crop.pLeafDefLL;
        fhandle >> crop.KsShapeFactorLeaf;
        fhandle >> crop.pdef;
        fhandle >> crop.KsShapeFactorStomata;
        fhandle >> crop.pSenescence;
        // ... more reading ...
        fhandle.close();
    }
}

dp SeasonalSumOfKcPot(int32_t TheDaysToCCini, int32_t TheGDDaysToCCini, int32_t L0, int32_t L12, int32_t L123, int32_t L1234, int32_t GDDL0, int32_t GDDL12, int32_t GDDL123, int32_t GDDL1234, dp CCo, dp CCx, dp CGC, dp GDDCGC, dp CDC, dp GDDCDC, dp KcTop, dp KcDeclAgeing, dp CCeffectProcent, dp Tbase, dp Tupper, dp TDayMin, dp TDayMax, dp GDtranspLow, dp CO2i, modeCycle TheModeCycle, bool ReferenceClimate)
{
    return 0.0;
}

dp HarvestIndexDay(int32_t DAP, int32_t DaysToFlower, int32_t HImax, dp dHIdt, dp CCi, dp CCxadjusted, dp TheCCxWithered, int8_t PercCCxHIfinal, plant plant_temp, int8_t& PercentLagPhase, int32_t& HIfinal)
{
    return 0.0;
}

void CompleteCropDescription() {}

void NoManagementOffSeason()
{
    Management.EffectMulchOffS = 0;
    Management.SoilCoverBefore = 0;
    Management.SoilCoverAfter = 0;
}

void LoadOffSeason(const std::string& FullName)
{
    std::ifstream fhandle(FullName);
    if (fhandle.is_open())
    {
        std::string line;
        std::getline(fhandle, OffSeasonDescription);
        // ...
        fhandle.close();
    }
}

void AdjustThetaInitial(int8_t PrevNrComp, const std::vector<dp>& PrevThickComp, const std::vector<dp>& PrevVolPrComp, const std::vector<dp>& PrevECdSComp) {}

void LoadClim(const std::string& FullName, std::string& ClimateDescription, rep_clim& ClimateRecord)
{
    std::ifstream fhandle(FullName);
    if (fhandle.is_open())
    {
        std::getline(fhandle, ClimateDescription);
        // ...
        fhandle.close();
    }
}

void LoadGroundWater(const std::string& FullName, int32_t AtDayNr, int32_t& Zcm, dp& ECdSm)
{
    std::ifstream fhandle(FullName);
    int32_t i, dayi, monthi, yeari, Year1Gwt;
    int32_t DayNr1Gwt, DayNr1, DayNr2, AtDayNr_local;
    std::string StringREAD;
    dp DayDouble, Z1, EC1, Z2, EC2;
    bool TheEnd;

    if (!fhandle.is_open())
    {
        std::cerr << "Groundwater file not found" << std::endl;
        return;
    }

    AtDayNr_local = AtDayNr;
    TheEnd = false;
    Year1Gwt = 1901;
    DayNr1 = 1;
    DayNr2 = 1;

    std::getline(fhandle, GroundwaterDescription);
    fhandle.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Version

    fhandle >> i;
    switch (i)
    {
    case 0:
        Zcm = undef_int;
        ECdSm = undef_double;
        simulparam.ConstGwt = true;
        TheEnd = true;
        break;
    case 1:
        simulparam.ConstGwt = true;
        break;
    default:
        simulparam.ConstGwt = false;
        break;
    }

    if (!simulparam.ConstGwt)
    {
        fhandle >> dayi >> monthi >> Year1Gwt;
        DetermineDayNr(dayi, monthi, Year1Gwt, DayNr1Gwt);
    }

    if (i > 0)
    {
        // Skip 3 lines
        for(int k=0; k<3; ++k) fhandle.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::getline(fhandle, StringREAD);
        if (StringREAD.empty()) std::getline(fhandle, StringREAD);
        
        SplitStringInThreeParams(StringREAD, DayDouble, Z2, EC2);
        if (i == 1 || fhandle.eof())
        {
            Zcm = (int32_t)roundc(100.0 * Z2, 1);
            ECdSm = EC2;
            TheEnd = true;
        }
        else
        {
            DayNr2 = DayNr1Gwt + (int32_t)roundc(DayDouble, 1) - 1;
        }
    }

    if (!TheEnd)
    {
        DetermineDate(AtDayNr_local, dayi, monthi, yeari);
        if (yeari == 1901 && Year1Gwt != 1901)
        {
            DetermineDayNr(dayi, monthi, Year1Gwt, AtDayNr_local);
        }
        if (yeari != 1901 && Year1Gwt == 1901)
        {
            DetermineDayNr(dayi, monthi, Year1Gwt, AtDayNr_local);
        }

        if (Year1Gwt != 1901)
        {
            if (AtDayNr_local <= DayNr2)
            {
                Zcm = (int32_t)roundc(100.0 * Z2, 1);
                ECdSm = EC2;
            }
            else
            {
                while (!TheEnd)
                {
                    DayNr1 = DayNr2;
                    Z1 = Z2;
                    EC1 = EC2;
                    if (!std::getline(fhandle, StringREAD)) { TheEnd = true; break; }
                    SplitStringInThreeParams(StringREAD, DayDouble, Z2, EC2);
                    DayNr2 = DayNr1Gwt + (int32_t)roundc(DayDouble, 1) - 1;
                    if (AtDayNr_local <= DayNr2)
                    {
                        Zcm = (int32_t)roundc(100.0 * (Z1 + (Z2 - Z1) * (AtDayNr_local - DayNr1) / (DayNr2 - DayNr1)), 1);
                        ECdSm = EC1 + (EC2 - EC1) * (AtDayNr_local - DayNr1) / (DayNr2 - DayNr1);
                        TheEnd = true;
                    }
                }
                if (AtDayNr_local > DayNr2)
                {
                    Zcm = (int32_t)roundc(100.0 * Z2, 1);
                    ECdSm = EC2;
                }
            }
        }
        else
        {
            // Simplified handle for Year1Gwt == 1901
            Zcm = (int32_t)roundc(100.0 * Z2, 1);
            ECdSm = EC2;
        }
    }
    fhandle.close();
}

void AdjustClimRecordTo(int32_t CDayN) {}

void TranslateIniLayersToSWProfile(int8_t NrLay, const std::vector<dp>& LayThickness, const std::vector<dp>& LayVolPr, const std::vector<dp>& LayECdS, int32_t NrComp, std::vector<CompartmentIndividual>& Comp) {}

void TranslateIniPointsToSWProfile(int8_t NrLoc, const std::vector<dp>& LocDepth, const std::vector<dp>& LocVolPr, const std::vector<dp>& LocECdS, int32_t NrComp, std::vector<CompartmentIndividual>& Comp) {}

dp CCiniTotalFromTimeToCCini(int32_t TempDaysToCCini, int32_t TempGDDaysToCCini, int32_t L0, int32_t L12, int32_t L12SF, int32_t L123, int32_t L1234, int32_t GDDL0, int32_t GDDL12, int32_t GDDL12SF, int32_t GDDL123, int32_t GDDL1234, dp CCo, dp CCx, dp CGC, dp GDDCGC, dp CDC, dp GDDCDC, dp RatDGDD, int8_t SFRedCGC, int8_t SFRedCCx, dp SFCDecline, dp fWeed, modeCycle TheModeCycle)
{
    return 0.0;
}

void AdjustCropYearToClimFile(int32_t& CDay1, int32_t& CDayN) {}

std::string EndGrowingPeriod(int32_t Day1, int32_t& DayN)
{
    return "";
}

void LoadInitialConditions(const std::string& SWCiniFileFull, dp& IniSurfaceStorage)
{
    std::ifstream fhandle(SWCiniFileFull);
    int32_t i;
    std::string StringParam, swcinidescr_temp;
    dp VersionNr;
    dp CCini_temp, Bini_temp, Zrini_temp, ECStorageIni_temp;
    int NrLoc_temp;
    dp Loc_i_temp, VolProc_i_temp, SaltECe_i_temp;

    if (!fhandle.is_open()) return;

    std::getline(fhandle, swcinidescr_temp);
    SWCiniDescription = swcinidescr_temp;
    fhandle >> VersionNr;
    if (roundc(10 * VersionNr, 1) < 41)
    {
        Simulation.CCini = undef_double;
    }
    else
    {
        fhandle >> CCini_temp;
        Simulation.CCini = CCini_temp;
    }
    if (roundc(10 * VersionNr, 1) < 41)
    {
        Simulation.Bini = 0.000;
    }
    else
    {
        fhandle >> Bini_temp;
        Simulation.Bini = Bini_temp;
    }
    if (roundc(10 * VersionNr, 1) < 41)
    {
        Simulation.Zrini = undef_double;
    }
    else
    {
        fhandle >> Zrini_temp;
        Simulation.Zrini = Zrini_temp;
    }
    fhandle >> IniSurfaceStorage;
    if (roundc(10 * VersionNr, 1) < 32)
    {
        Simulation.ECStorageIni = 0.0;
    }
    else
    {
        fhandle >> ECStorageIni_temp;
        Simulation.ECStorageIni = ECStorageIni_temp;
    }
    fhandle >> i;
    if (i == 1)
    {
        Simulation.IniSWC.AtDepths = true;
    }
    else
    {
        Simulation.IniSWC.AtDepths = false;
    }
    fhandle >> NrLoc_temp;
    Simulation.IniSWC.NrLoc = (int8_t)NrLoc_temp;
    
    // Skip 3 lines
    for(int k=0; k<3; ++k) fhandle.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (i = 1; i <= Simulation.IniSWC.NrLoc; ++i)
    {
        std::getline(fhandle, StringParam);
        if (StringParam.empty()) std::getline(fhandle, StringParam); // handle empty line after ignore

        if (roundc(10 * VersionNr, 1) < 32)
        {
            SplitStringInTwoParams(StringParam, Loc_i_temp, VolProc_i_temp);
            Simulation.IniSWC.SaltECe[i-1] = 0.0;
        }
        else
        {
            SplitStringInThreeParams(StringParam, Loc_i_temp, VolProc_i_temp, SaltECe_i_temp);
            Simulation.IniSWC.SaltECe[i-1] = SaltECe_i_temp;
        }
        Simulation.IniSWC.Loc[i-1] = Loc_i_temp;
        Simulation.IniSWC.VolProc[i-1] = VolProc_i_temp;
    }
    fhandle.close();
    Simulation.IniSWC.AtFC = false;
}

void AdjustSizeCompartments(dp CropZx) {}

void CheckForKeepSWC(bool& RunWithKeepSWC, dp& ConstZrxForRun)
{
    RunWithKeepSWC = false;
    ConstZrxForRun = static_cast<dp>(undef_double);
}

void InitializeGlobalStrings()
{
    CalendarDescription = "";
    CalendarFile = "";
    CalendarFileFull = "";
    ClimateDescription = "";
    ClimateFile = "";
    ClimateFileFull = "";
    ClimDescription = "";
    ClimFile = "";
    ClimRecord.FromString = "";
    ClimRecord.ToString = "";
    CO2Description = "";
    CO2File = "";
    CO2FileFull = "";
    CropDescription = "";
    CropFile = "";
    CropFileFull = "";
    EToDescription = "";
    EToFile = "";
    EToFileFull = "";
    FullFileNameProgramParameters = "";
    GroundWaterFile = "";
    GroundWaterFilefull = "";
    GroundwaterDescription = "";
    IrriDescription = "";
    IrriFile = "";
    IrriFileFull = "";
    ManDescription = "";
    ManFile = "";
    ManFilefull = "";
    MultipleProjectDescription = "";
    MultipleProjectFile = "";
    MultipleProjectFileFull = "";
    ObservationsDescription = "";
    ObservationsFile = "";
    ObservationsFilefull = "";
    OffSeasonDescription = "";
    OffSeasonFile = "";
    OffSeasonFilefull = "";
    OutputName = "";
    PathNameProg = "";
    ProfDescription = "";
    ProfFile = "";
    ProfFilefull = "";
    ProjectDescription = "";
    ProjectFile = "";
    ProjectFileFull = "";
    RainDescription = "";
    RainFile = "";
    RainFileFull = "";
    SWCiniDescription = "";
    SWCiniFile = "";
    SWCiniFileFull = "";
    TemperatureDescription = "";
    TemperatureFile = "";
    TemperatureFileFull = "";
    TnxReference365DaysFile = "";
    TnxReference365DaysFileFull = "";
    TnxReferenceFile = "";
    TnxReferenceFileFull = "";
}

void LoadProfile(const std::string& FullName)
{
    std::ifstream fhandle(FullName);
    int32_t i;
    dp VersionNr;
    int TempShortInt;
    std::string ProfDescriptionLocal;
    dp thickness_temp, SAT_temp, FC_temp, WP_temp, infrate_temp;
    dp cra_temp, crb_temp;
    std::string description_temp;
    int penetrability_temp, gravelm_temp;
    std::string line;

    if (!fhandle.is_open()) return;

    std::getline(fhandle, ProfDescriptionLocal);
    ProfDescription = ProfDescriptionLocal;
    fhandle >> VersionNr;
    fhandle >> TempShortInt; Soil.CNvalue = (int8_t)TempShortInt;
    fhandle >> TempShortInt; Soil.REW = (int8_t)TempShortInt;
    fhandle >> TempShortInt; Soil.NrSoilLayers = (int8_t)TempShortInt;
    
    // Skip 3 lines
    for(int k=0; k<3; ++k) fhandle.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (i = 1; i <= Soil.NrSoilLayers; ++i)
    {
        if (roundc(VersionNr * 10, 1) < 40)
        {
            fhandle >> thickness_temp >> SAT_temp >> FC_temp >> WP_temp >> infrate_temp;
            std::getline(fhandle, line); // description
            soillayer[i-1].Thickness = thickness_temp;
            soillayer[i-1].SAT = SAT_temp;
            soillayer[i-1].FC = FC_temp;
            soillayer[i-1].WP = WP_temp;
            soillayer[i-1].InfRate = infrate_temp;
            soillayer[i-1].Penetrability = 100;
            soillayer[i-1].GravelMass = 0;
            soillayer[i-1].GravelVol = 0.0;
        }
        else if (roundc(VersionNr * 10, 1) < 60)
        {
            fhandle >> thickness_temp >> SAT_temp >> FC_temp >> WP_temp >> infrate_temp >> cra_temp >> crb_temp;
            std::getline(fhandle, line); // description
            soillayer[i-1].Thickness = thickness_temp;
            soillayer[i-1].SAT = SAT_temp;
            soillayer[i-1].FC = FC_temp;
            soillayer[i-1].WP = WP_temp;
            soillayer[i-1].InfRate = infrate_temp;
            soillayer[i-1].CRa = cra_temp;
            soillayer[i-1].CRb = crb_temp;
            soillayer[i-1].Penetrability = 100;
            soillayer[i-1].GravelMass = 0;
            soillayer[i-1].GravelVol = 0.0;
        }
        else
        {
            fhandle >> thickness_temp >> SAT_temp >> FC_temp >> WP_temp >> infrate_temp >> penetrability_temp >> gravelm_temp >> cra_temp >> crb_temp >> description_temp;
            soillayer[i-1].Thickness = thickness_temp;
            soillayer[i-1].SAT = SAT_temp;
            soillayer[i-1].FC = FC_temp;
            soillayer[i-1].WP = WP_temp;
            soillayer[i-1].InfRate = infrate_temp;
            soillayer[i-1].Penetrability = (int8_t)penetrability_temp;
            soillayer[i-1].GravelMass = (int8_t)gravelm_temp;
            soillayer[i-1].CRa = cra_temp;
            soillayer[i-1].CRb = crb_temp;
            soillayer[i-1].Description = description_temp;
            soillayer[i-1].GravelVol = FromGravelMassToGravelVolume(soillayer[i-1].SAT, soillayer[i-1].GravelMass);
        }
    }
    fhandle.close();
    LoadProfileProcessing(VersionNr);
}

void LoadProfileProcessing(dp VersionNr)
{
    int32_t i;
    dp dx_temp;

    Simulation.SurfaceStorageIni = 0.0;
    Simulation.ECStorageIni = 0.0;

    for (i = 1; i <= Soil.NrSoilLayers; ++i)
    {
        soillayer[i-1].tau = TauFromKsat(soillayer[i-1].InfRate);

        if (soillayer[i-1].InfRate <= 112.0)
        {
            soillayer[i-1].SCP1 = 11;
        }
        else
        {
            soillayer[i-1].SCP1 = (int8_t)roundc(1.6 + 1000.0 / soillayer[i-1].InfRate, 1);
            if (soillayer[i-1].SCP1 < 2) soillayer[i-1].SCP1 = 2;
        }

        soillayer[i-1].SC = soillayer[i-1].SCP1 - 1;
        soillayer[i-1].Macro = (int8_t)roundc(soillayer[i-1].FC, 1);
        soillayer[i-1].UL = (soillayer[i-1].SAT / 100.0) * (soillayer[i-1].SC / (soillayer[i-1].SC + 2.0));
        dx_temp = soillayer[i-1].UL / soillayer[i-1].SC;
        soillayer[i-1].Dx = dx_temp;

        Calculate_Saltmobility(i, simulparam.SaltDiff, soillayer[i-1].Macro, soillayer[i-1].SaltMobility);

        soillayer[i-1].SoilClass = NumberSoilClass(soillayer[i-1].SAT, soillayer[i-1].FC, soillayer[i-1].WP, soillayer[i-1].InfRate);

        if (roundc(VersionNr * 10, 1) < 40)
        {
            DetermineParametersCR(soillayer[i-1].SoilClass, soillayer[i-1].InfRate, soillayer[i-1].CRa, soillayer[i-1].CRb);
        }
    }

    DetermineNrandThicknessCompartments();
    Soil.RootMax = RootMaxInSoilProfile(crop.RootMax, Soil.NrSoilLayers, soillayer);
}

void CalculateETpot(int32_t DAP, int32_t L0, int32_t L12, int32_t L123, int32_t LHarvest, int32_t DayLastCut, dp CCi, dp EToVal, dp KcVal, dp KcDeclineVal, dp CCx, dp CCxWithered, dp CCEffectProcent, dp CO2i, dp GDDayi, dp TempGDtranspLow, dp& TpotVal, dp& EpotVal) {}

void LoadProgramParametersProject(const std::string& FullFileNameProgramParameters) {}

void ReadCropSettingsParameters() {}

void LoadProgramParametersProjectPlugIn(const std::string& FullFileNameProgramParameters, bool& ProgramParametersAvailable) {}

void ReadFieldSettingsParameters() {}

void ReadTemperatureSettingsParameters() {}

void CompleteClimateDescription(rep_clim& ClimateRecord) {}

int32_t SumCalendarDaysReferenceTnx(int32_t ValGDDays, int32_t RefCropDay1, int32_t StartDayNr, dp Tbase, dp Tupper, dp TDayMin, dp TDayMax)
{
    return 0;
}

void DesignateSoilLayerToCompartments(int32_t NrCompartments, int32_t NrSoilLayers, std::vector<CompartmentIndividual>& Compartment) {}

void specify_soil_layer(int32_t NrCompartments, int32_t NrSoilLayers, std::vector<SoilLayerIndividual>& SoilLayer, std::vector<CompartmentIndividual>& Compartment, rep_Content& TotalWaterContent) {}

void Calculate_Saltmobility(int32_t layer, int8_t SaltDiffusion, int8_t Macro, std::vector<dp>& Mobil)
{
    int32_t i, CelMax;
    dp Mix, a, b, xi, yi, UL;

    Mix = SaltDiffusion / 100.0;
    UL = soillayer[layer - 1].UL * 100.0;

    if (Macro > UL)
    {
        CelMax = soillayer[layer - 1].SCP1;
    }
    else
    {
        CelMax = roundc((Macro / UL) * soillayer[layer - 1].SC, 1);
    }

    if (CelMax <= 0)
    {
        CelMax = 1;
    }

    if (Mix < 0.5)
    {
        a = Mix * 2.0;
        b = exp(10.0 * (0.5 - Mix) * log(10.0));
    }
    else
    {
        a = 2.0 * (1.0 - Mix);
        b = exp(10.0 * (Mix - 0.5) * log(10.0));
    }

    for (i = 1; i <= CelMax - 1; ++i)
    {
        xi = i * 1.0 / (CelMax - 1);

        if (Mix > 0.0)
        {
            if (Mix < 0.5)
            {
                yi = exp(log(a) + xi * log(b));
                Mobil[i-1] = (yi - a) / (a * b - a);
            }
            else if (std::abs(Mix - 0.5) < 1e-12)
            {
                Mobil[i-1] = xi;
            }
            else if (Mix < 1.0)
            {
                yi = exp(log(a) + (1.0 - xi) * log(b));
                Mobil[i-1] = 1.0 - (yi - a) / (a * b - a);
            }
            else
            {
                Mobil[i-1] = 1.0;
            }
        }
        else
        {
            Mobil[i-1] = 0.0;
        }
    }

    for (i = CelMax; i <= (int32_t)soillayer[layer - 1].SCP1; ++i)
    {
        Mobil[i-1] = 1.0;
    }
}

void CompleteProfileDescription() {}

void GlobalZero(rep_sum& SumWaBal)
{
    SumWaBal.Epot = 0.0;
    SumWaBal.Tpot = 0.0;
    SumWaBal.Rain = 0.0;
    SumWaBal.Irrigation = 0.0;
    SumWaBal.Infiltrated = 0.0;
    SumWaBal.Runoff = 0.0;
    SumWaBal.Drain = 0.0;
    SumWaBal.Eact = 0.0;
    SumWaBal.Tact = 0.0;
    SumWaBal.TrW = 0.0;
    SumWaBal.ECropCycle = 0.0;
    SumWaBal.CRwater = 0.0;
    SumWaBal.Biomass = 0.0;
    SumWaBal.YieldPart = 0.0;
    SumWaBal.BiomassPot = 0.0;
    SumWaBal.BiomassUnlim = 0.0;
    SumWaBal.BiomassTot = 0.0;
    SumWaBal.SaltIn = 0.0;
    SumWaBal.SaltOut = 0.0;
    SumWaBal.CRsalt = 0.0;
}

} // namespace AquaCrop
