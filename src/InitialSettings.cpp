#include "AquaCrop/InitialSettings.h"
#include "AquaCrop/Global.h"
#include "AquaCrop/Utils.h"
#include "AquaCrop/ProjectInput.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

namespace AquaCrop {

void ResetDefaultSoil(bool use_default_soil_file)
{
    int32_t i;

    ProfDescription = "default soil";
    Soil.CNvalue = 61;
    Soil.REW = 9;
    Soil.NrSoilLayers = 1;
    if (use_default_soil_file)
    {
        Soil.NrSoilLayers = 3;
    }

    for (i = 0; i < 1; ++i)
    {
        soillayer[i].Thickness = 4.00;
        soillayer[i].SAT = 46.0;
        soillayer[i].FC = 31.0;
        soillayer[i].WP = 15.0;
        soillayer[i].InfRate = 500.0;
        soillayer[i].Penetrability = 100;
        soillayer[i].GravelMass = 0;
        soillayer[i].GravelVol = 0;
        soillayer[i].Description = "Loamy Sand";
        soillayer[i].SoilClass = 2;
        DetermineParametersCR(2, 500.0, soillayer[i].CRa, soillayer[i].CRb);
    }
    if (use_default_soil_file)
    {
        soillayer[0].Thickness = 0.30;
        soillayer[1] = soillayer[0]; // Copy properties
        soillayer[1].Thickness = 0.90;
        soillayer[2] = soillayer[0]; // Copy properties
        soillayer[2].Thickness = 2.80;
    }
    
    // Set other layers to undefined
    for (i = (use_default_soil_file ? 3 : 1); i < 5; ++i)
    {
        set_layer_undef(soillayer[i]);
    }
}

void ResetDefaultCrop(bool use_default_crop_file)
{
    CropDescription = "a generic crop";
    crop.CropSubkind = subkind::Grain;
    crop.Planting = plant::seed;
    crop.SownYear1 = true;
    crop.ModeCycle = modeCycle::CalendarDays;
    crop.CropPMethod = pMethod::FAOCorrection;
    crop.Tbase = 5.5;
    crop.Tupper = 30.0;
    crop.pLeafDefUL = 0.25;
    crop.pLeafDefLL = 0.60;
    crop.KsShapeFactorLeaf = 3.0;
    crop.pdef = 0.50;
    crop.KsShapeFactorStomata = 3.0;
    crop.pSenescence = 0.85;
    crop.KsShapeFactorSenescence = 3.0;
    crop.SumEToDelaySenescence = 50;
    crop.pPollination = 0.90;
    crop.AnaeroPoint = 5;

    crop.StressResponse.Stress = 50;
    crop.StressResponse.ShapeCGC = 2.16;
    crop.StressResponse.ShapeCCX = 0.79;
    crop.StressResponse.ShapeWP = 1.67;
    crop.StressResponse.ShapeCDecline = 1.67;
    crop.StressResponse.Calibrated = true;

    crop.ECemin = 2;
    crop.ECemax = 12;
    crop.CCsaltDistortion = 25;
    crop.ResponseECsw = 100;
    crop.Tcold = 8;
    crop.Theat = 40;
    crop.GDtranspLow = 11.1;
    crop.KcTop = 1.10;
    crop.KcDecline = 0.150;
    crop.RootMin = 0.30;
    crop.RootMax = 1.00;
    crop.RootMinYear1 = crop.RootMin;
    crop.RootShape = 15;
    crop.SmaxTopQuarter = 0.048;
    crop.SmaxBotQuarter = 0.012;
    crop.CCEffectEvapLate = 50;
    crop.SizeSeedling = 6.50;
    crop.SizePlant = crop.SizeSeedling;
    crop.PlantingDens = 185000;
    crop.CCo = (crop.SizeSeedling / 10000.0) * (crop.PlantingDens / 10000.0);
    crop.CCini = crop.CCo;
    crop.CGC = 0.15;
    crop.YearCCx = undef_int;
    crop.CCxRoot = undef_double;
    crop.CCx = 0.80;
    crop.CDC = 0.1275;
    crop.DaysToCCini = 0;
    crop.DaysToGermination = 5;
    crop.DaysToMaxRooting = 100;
    crop.DaysToSenescence = 110;
    crop.DaysToHarvest = 125;
    crop.DaysToFlowering = 70;
    crop.LengthFlowering = 10;
    crop.DaysToHIo = 50;
    crop.DeterminancyLinked = true;
    crop.fExcess = 50;
    crop.WP = 17.0;
    crop.WPy = 100;
    crop.AdaptedToCO2 = 100;
    crop.HI = 50;
    crop.DryMatter = 25;
    crop.HIincrease = 5;
    crop.aCoeff = 10.0;
    crop.bCoeff = 8.0;
    crop.DHImax = 15;
    crop.dHIdt = -9.0;
    crop.GDDaysToCCini = -9;
    crop.GDDaysToGermination = -9;
    crop.GDDaysToMaxRooting = -9;
    crop.GDDaysToSenescence = -9;
    crop.GDDaysToHarvest = -9;
    crop.GDDaysToFlowering = -9;
    crop.GDDLengthFlowering = -9;
    crop.GDDaysToHIo = -9;
    crop.GDDCGC = -9.0;
    crop.GDDCDC = -9.0;
    crop.Assimilates.On = false;
    crop.Assimilates.Period = -9;
    crop.Assimilates.Stored = -9;
    crop.Assimilates.Mobilized = -9;
}

void InitializeSettings(bool use_default_soil_file, bool use_default_crop_file)
{
    ResetDefaultSoil(use_default_soil_file);
    ResetDefaultCrop(use_default_crop_file);
}

void LoadSimulationRunProject(int32_t NrRun)
{
    if (NrRun < 1 || NrRun > static_cast<int32_t>(ProjectInput.size())) return;

    const auto& input = ProjectInput[NrRun - 1];

    // 0. Year of cultivation and Simulation and Cropping period
    Simulation.YearSeason = input.Simulation_YearSeason;
    crop.Day1 = input.Crop_Day1;
    crop.DayN = input.Crop_DayN;
    Simulation.FromDayNr = input.Simulation_DayNr1;
    Simulation.ToDayNr = input.Simulation_DayNrN;

    // 1. Climate
    ClimateFile = input.Climate_Filename;
    if (ClimateFile != "(None)" && ClimateFile != "(External)") {
        ClimateFileFull = input.Climate_Directory + ClimateFile;
        // Simplified description loading
        ClimateDescription = input.Climate_Info;
    }

    // 1.1 Temperature
    TemperatureFile = input.Temperature_Filename;
    if (TemperatureFile != "(None)" && TemperatureFile != "(External)") {
        TemperatureFileFull = input.Temperature_Directory + TemperatureFile;
        LoadClim(TemperatureFileFull, TemperatureDescription, TemperatureRecord);
        CompleteClimateDescription(TemperatureRecord);
    }

    // 1.2 ETo
    EToFile = input.ETo_Filename;
    if (EToFile != "(None)" && EToFile != "(External)") {
        EToFileFull = input.ETo_Directory + EToFile;
        LoadClim(EToFileFull, EToDescription, EToRecord);
        CompleteClimateDescription(EToRecord);
    }

    // 1.3 Rain
    RainFile = input.Rain_Filename;
    if (RainFile != "(None)" && RainFile != "(External)") {
        RainFileFull = input.Rain_Directory + RainFile;
        LoadClim(RainFileFull, RainDescription, RainRecord);
        CompleteClimateDescription(RainRecord);
    }

    // 1.4 CO2
    CO2File = input.CO2_Filename;
    if (CO2File != "(None)" && CO2File != "(External)") {
        CO2FileFull = input.CO2_Directory + CO2File;
        GenerateCO2Description(CO2FileFull, CO2Description);
    }

    // 2. Calendar
    CalendarFile = input.Calendar_Filename;
    if (CalendarFile != "(None)") {
        CalendarFileFull = input.Calendar_Directory + CalendarFile;
        bool dummy1, dummy2;
        int32_t dummy3;
        LoadCropCalendar(CalendarFileFull, dummy1, dummy2, dummy3, 2000);
    }

    // 3. Crop
    CropFile = input.Crop_Filename;
    if (CropFile != "(None)") {
        CropFileFull = input.Crop_Directory + CropFile;
        LoadCrop(CropFileFull);
    }

    // 4. Irrigation
    IrriFile = input.Irrigation_Filename;
    if (IrriFile != "(None)") {
        IrriFileFull = input.Irrigation_Directory + IrriFile;
        LoadIrriScheduleInfo(IrriFileFull);
    }

    // 5. Management
    ManFile = input.Management_Filename;
    if (ManFile != "(None)") {
        ManFilefull = input.Management_Directory + ManFile;
        LoadManagement(ManFilefull);
    }

    // 6. Soil
    ProfFile = input.Soil_Filename;
    if (ProfFile != "(None)") {
        ProfFilefull = input.Soil_Directory + ProfFile;
        LoadProfile(ProfFilefull);
    }

    // 7. GroundWater
    GroundWaterFile = input.GroundWater_Filename;
    if (GroundWaterFile != "(None)") {
        GroundWaterFilefull = input.GroundWater_Directory + GroundWaterFile;
        int32_t Zcm;
        dp ECdSm;
        LoadGroundWater(GroundWaterFilefull, Simulation.FromDayNr, Zcm, ECdSm);
        ZiAqua = static_cast<dp>(Zcm);
        ECiAqua = ECdSm;
    }

    // 8. Initial SWC
    SWCiniFile = input.SWCIni_Filename;
    if (SWCiniFile == "KeepSWC") {
        Simulation.MultipleRunWithKeepSWC = true;
    } else if (SWCiniFile != "(None)") {
        SWCiniFileFull = input.SWCIni_Directory + SWCiniFile;
        LoadInitialConditions(SWCiniFileFull, Simulation.SurfaceStorageIni);
    }

    // 9. Off-season
    OffSeasonFile = input.OffSeason_Filename;
    if (OffSeasonFile != "(None)") {
        OffSeasonFilefull = input.OffSeason_Directory + OffSeasonFile;
        LoadOffSeason(OffSeasonFilefull);
    }

    // 10. Observations
    ObservationsFile = input.Observations_Filename;
    if (ObservationsFile != "(None)") {
        ObservationsFilefull = input.Observations_Directory + ObservationsFile;
        // LoadObservations(ObservationsFilefull); // Stub
    }
}

} // namespace AquaCrop

