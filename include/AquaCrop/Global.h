#pragma once

#include "AquaCrop/Kinds.h"

#include <string>
#include <vector>

namespace AquaCrop {

// Constants
constexpr dp equiv = 0.64;
constexpr int32_t max_SoilLayers = 5;
constexpr int32_t max_No_compartments = 12;
constexpr dp undef_double = -9.9;
constexpr int32_t undef_int = -9;
constexpr dp PI = 3.1415926535;
constexpr dp CO2Ref = 369.41;
constexpr dp EvapZmin = 15.0;
constexpr dp eps = 10E-08;
constexpr dp ac_zero_threshold = 0.000001;

const std::vector<dp> ElapsedDays = {0.0, 31.0, 59.25, 90.25, 120.25, 151.25, 181.25, 212.25, 243.25, 273.25, 304.25, 334.25};
const std::vector<int32_t> DaysInMonth = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const std::vector<std::string> NameMonth = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

// Enumerations
enum class modeCycle : intEnum {
    GDDays = 0,
    CalendarDays = 1,
};

enum class pMethod : intEnum {
    NoCorrection = 0,
    FAOCorrection = 1,
};

enum class subkind : intEnum {
    Vegetative = 0,
    Grain = 1,
    Tuber = 2,
    Forage = 3,
};

enum class plant : intEnum {
    seed = 0,
    transplant = 1,
    regrowth = 2,
};

enum class Method : intEnum {
    full = 0,
    usda = 1,
    percentage = 2,
};

enum class EffectiveRainMethod : intEnum {
    full = 0,
    usda = 1,
    percentage = 2,
};

enum class TimeCuttings : intEnum {
    NA = 0,
    IntDay = 1,
    IntGDD = 2,
    DryB = 3,
    DryY = 4,
    FreshY = 5,
};

enum class Criterion : intEnum {
    CumulRain = 0,
    RainPeriod = 1,
    RainDecade = 2,
    RainVsETo = 3,
};

enum class AirTCriterion : intEnum {
    TminPeriod = 0,
    TmeanPeriod = 1,
    GDDPeriod = 2,
    CumulGDD = 3,
};

enum class GenerateTimeMode : intEnum {
    FixInt = 0,
    AllDepl = 1,
    AllRAW = 2,
    WaterBetweenBunds = 3,
};

enum class GenerateDepthMode : intEnum {
    ToFC = 0,
    FixDepth = 1,
};

enum class IrriMode : intEnum {
    NoIrri = 0,
    Manual = 1,
    Generate = 2,
    Inet = 3,
};

enum class IrriMethod : intEnum {
    MBasin = 0,
    MBorder = 1,
    MDrip = 2,
    MFurrow = 3,
    MSprinkler = 4,
};

enum class datatype : intEnum {
    daily = 0,
    decadely = 1,
    monthly = 2,
};

enum class typeproject : intEnum {
    typepro = 0,
    typeprm = 1,
    typenone = 2,
};

enum class typeObsSim : intEnum {
    ObsSimCC = 0,
    ObsSimB = 1,
    ObsSimSWC = 2,
};

// Structures
struct rep_DayEventInt {
    int32_t DayNr;
    int32_t param;
};

struct CompartmentIndividual {
    dp Thickness;
    dp theta;
    dp fluxout;
    int32_t Layer;
    dp Smax;
    dp FCadj;
    int32_t DayAnaero;
    dp WFactor;
    std::vector<dp> Salt = std::vector<dp>(11);
    std::vector<dp> Depo = std::vector<dp>(11);
};

struct SoilLayerIndividual {
    std::string Description;
    dp Thickness;
    dp SAT;
    dp FC;
    dp WP;
    dp tau;
    dp InfRate;
    int8_t Penetrability;
    int8_t GravelMass;
    dp GravelVol;
    dp WaterContent;
    int8_t Macro;
    std::vector<dp> SaltMobility = std::vector<dp>(11);
    int8_t SC;
    int8_t SCP1;
    dp UL;
    dp Dx;
    int8_t SoilClass;
    dp CRa, CRb;
};

struct rep_Shapes {
    int8_t Stress;
    dp ShapeCGC;
    dp ShapeCCX;
    dp ShapeWP;
    dp ShapeCDecline;
    bool Calibrated;
};

struct rep_soil {
    int8_t REW;
    int8_t NrSoilLayers;
    int8_t CNvalue;
    sp RootMax;
};

struct rep_Assimilates {
    bool On;
    int32_t Period;
    int8_t Stored;
    int8_t Mobilized;
};

struct rep_Onset {
    bool GenerateOn;
    bool GenerateTempOn;
    Criterion TimeCriterion;
    AirTCriterion TempCriterion;
    int32_t StartSearchDayNr;
    int32_t StopSearchDayNr;
    int32_t LengthSearchPeriod;
};

struct rep_EndSeason {
    int32_t ExtraYears;
    bool GenerateTempOn;
    AirTCriterion TempCriterion;
    int32_t StartSearchDayNr;
    int32_t StopSearchDayNr;
    int32_t LengthSearchPeriod;
};

struct rep_Content {
    dp BeginDay;
    dp EndDay;
    dp ErrorDay;
};

struct rep_EffectStress {
    int8_t RedCGC;
    int8_t RedCCX;
    int8_t RedWP;
    dp CDecline;
    int8_t RedKsSto;
};

struct rep_EffectiveRain {
    EffectiveRainMethod EffMethod;
    int8_t PercentEffRain;
    int8_t ShowersInDecade;
    int8_t RootNrEvap;
};


struct rep_RootZoneWC {
    dp Actual;
    dp FC;
    dp WP;
    dp SAT;
    dp Leaf;
    dp Thresh;
    dp Sen;
    dp ZtopAct;
    dp ZtopFC;
    dp ZtopWP;
    dp ZtopThresh;
};

struct rep_IrriECw {
    dp PreSeason;
    dp PostSeason;
};

struct rep_clim {
    datatype DataType;
    int32_t FromD, FromM, FromY;
    int32_t ToD, ToM, ToY;
    int32_t FromDayNr, ToDayNr;
    std::string FromString, ToString;
    int32_t NrObs;
};

struct rep_CropFileSet {
    int32_t DaysFromSenescenceToEnd;
    int32_t DaysToHarvest;
    int32_t GDDaysFromSenescenceToEnd;
    int32_t GDDaysToHarvest;
};

struct rep_Cuttings {
    bool Considered;
    int32_t CCcut;
    int32_t Day1;
    int32_t NrDays;
    bool Generate;
    TimeCuttings Criterion;
    bool HarvestEnd;
    int32_t FirstDayNr;
};

struct rep_Manag {
    int8_t Mulch;
    int8_t SoilCoverBefore;
    int8_t SoilCoverAfter;
    int8_t EffectMulchOffS;
    int8_t EffectMulchInS;
    int32_t FertilityStress;
    dp BundHeight;
    bool RunoffOn;
    int32_t CNcorrection;
    int8_t WeedRC;
    int32_t WeedDeltaRC;
    dp WeedShape;
    int8_t WeedAdj;
    rep_Cuttings Cuttings;
};

struct rep_param {
    int8_t EvapDeclineFactor;
    dp KcWetBare;
    int8_t PercCCxHIfinal;
    int32_t RootPercentZmin;
    dp MaxRootZoneExpansion;
    int8_t KsShapeFactorRoot;
    int8_t TAWGermination;
    dp pAdjFAO;
    int32_t DelayLowOxygen;
    dp ExpFsen;
    int8_t Beta;
    int8_t ThicknessTopSWC;
    int8_t EvapZmax;
    dp RunoffDepth;
    bool CNcorrection;
    dp Tmin;
    dp Tmax;
    int8_t GDDMethod;
    int32_t PercRAW;
    dp CompDefThick;
    int32_t CropDay1;
    dp Tbase;
    dp Tupper;
    int8_t IrriFwInSeason;
    int8_t IrriFwOffSeason;
    std::vector<int32_t> ShowersInDecade = std::vector<int32_t>(12);
    rep_EffectiveRain EffectiveRain;
    int8_t SaltDiff;
    int8_t SaltSolub;
    bool ConstGwt;
    int8_t RootNrDF;
    int8_t IniAbstract;
};

struct rep_sum {
    dp Epot, Tpot, Rain, Irrigation, Infiltrated;
    dp Runoff, Drain, Eact, Tact, TrW, ECropCycle, CRwater;
    dp Biomass, YieldPart, BiomassPot, BiomassUnlim, BiomassTot;
    dp SaltIn, SaltOut, CRsalt;
};

struct rep_RootZoneSalt {
    dp ECe;
    dp ECsw;
    dp ECswFC;
    dp KsSalt;
};

struct rep_IniSWC {
    bool AtDepths;
    int8_t NrLoc;
    std::vector<dp> Loc = std::vector<dp>(max_No_compartments);
    std::vector<dp> VolProc = std::vector<dp>(max_No_compartments);
    std::vector<dp> SaltECe = std::vector<dp>(max_No_compartments);
    bool AtFC;
};

struct rep_storage {
    dp Btotal;
    std::string CropString;
    int8_t Season;
};

struct rep_sim {
    int32_t FromDayNr;
    int32_t ToDayNr;
    rep_IniSWC IniSWC;
    std::vector<dp> ThetaIni = std::vector<dp>(max_No_compartments);
    std::vector<dp> ECeIni = std::vector<dp>(max_No_compartments);
    dp SurfaceStorageIni;
    dp ECStorageIni;
    dp CCini;
    dp Bini;
    dp Zrini;
    bool LinkCropToSimPeriod;
    bool ResetIniSWC;
    int32_t InitialStep;
    bool EvapLimitON;
    dp EvapWCsurf;
    int8_t EvapStartStg2;
    dp EvapZ;
    int32_t HIfinal;
    int32_t DelayedDays;
    bool Germinate;
    dp SumEToStress;
    dp SumGDD;
    dp SumGDDfromDay1;
    sp SCor;
    bool MultipleRun;
    int32_t NrRuns;
    bool MultipleRunWithKeepSWC;
    dp MultipleRunConstZrx;
    dp IrriECw;
    int8_t DayAnaero;
    rep_EffectStress EffectStress;
    bool SalinityConsidered;
    bool ProtectedSeedling;
    bool SWCtopSoilConsidered;
    int32_t LengthCuttingInterval;
    int8_t YearSeason;
    int8_t RCadj;
    rep_storage Storage;
    int32_t YearStartCropCycle;
    int32_t CropDay1Previous;
};

struct rep_DayEventDbl {
    int32_t DayNr;
    dp Param;
};

struct rep_Crop {
    subkind CropSubkind;
    modeCycle ModeCycle;
    plant Planting;
    pMethod CropPMethod;
    dp pdef;
    dp pActStom;
    dp KsShapeFactorLeaf;
    dp KsShapeFactorStomata;
    dp KsShapeFactorSenescence;
    dp pLeafDefUL;
    dp pLeafDefLL;
    dp pLeafAct;
    dp pSenescence;
    dp pSenAct;
    dp pPollination;
    int32_t SumEToDelaySenescence;
    int32_t AnaeroPoint;
    rep_Shapes StressResponse;
    int8_t ECemin;
    int8_t ECemax;
    int8_t CCsaltDistortion;
    int32_t ResponseECsw;
    dp SmaxTopQuarter;
    dp SmaxBotQuarter;
    dp SmaxTop;
    dp SmaxBot;
    dp KcTop;
    dp KcDecline;
    int32_t CCEffectEvapLate;
    int32_t Day1;
    int32_t DayN;
    std::vector<int32_t> Length = std::vector<int32_t>(4);
    dp RootMin;
    dp RootMax;
    int8_t RootShape;
    dp Tbase;
    dp Tupper;
    int8_t Tcold;
    int8_t Theat;
    dp GDtranspLow;
    dp SizeSeedling;
    dp SizePlant;
    int32_t PlantingDens;
    dp CCo;
    dp CCini;
    dp CGC;
    dp GDDCGC;
    dp CCx;
    dp CDC;
    dp GDDCDC;
    dp CCxAdjusted;
    dp CCxWithered;
    dp CCoAdjusted;
    int32_t DaysToCCini;
    int32_t DaysToGermination;
    int32_t DaysToFullCanopy;
    int32_t DaysToFullCanopySF;
    int32_t DaysToFlowering;
    int32_t LengthFlowering;
    int32_t DaysToSenescence;
    int32_t DaysToHarvest;
    int32_t DaysToMaxRooting;
    int32_t DaysToHIo;
    int32_t GDDaysToCCini;
    int32_t GDDaysToGermination;
    int32_t GDDaysToFullCanopy;
    int32_t GDDaysToFullCanopySF;
    int32_t GDDaysToFlowering;
    int32_t GDDLengthFlowering;
    int32_t GDDaysToSenescence;
    int32_t GDDaysToHarvest;
    int32_t GDDaysToMaxRooting;
    int32_t GDDaysToHIo;
    dp WP;
    int32_t WPy;
    int8_t AdaptedToCO2;
    int32_t HI;
    dp dHIdt;
    int8_t HIincrease;
    dp aCoeff;
    dp bCoeff;
    int8_t DHImax;
    bool DeterminancyLinked;
    int16_t fExcess;
    int8_t DryMatter;
    dp RootMinYear1;
    bool SownYear1;
    int8_t YearCCx;
    dp CCxRoot;
    rep_Assimilates Assimilates;
};

struct rep_PerennialPeriod {
    bool GenerateOnset;
    AirTCriterion OnsetCriterion;
    int32_t OnsetFirstDay;
    int32_t OnsetFirstMonth;
    int32_t OnsetStartSearchDayNr;
    int32_t OnsetStopSearchDayNr;
    int32_t OnsetLengthSearchPeriod;
    dp OnsetThresholdValue;
    int32_t OnsetPeriodValue;
    int8_t OnsetOccurrence;
    bool GenerateEnd;
    AirTCriterion EndCriterion;
    int32_t EndLastDay;
    int32_t EndLastMonth;
    int32_t ExtraYears;
    int32_t EndStartSearchDayNr;
    int32_t EndStopSearchDayNr;
    int32_t EndLengthSearchPeriod;
    dp EndThresholdValue;
    int32_t EndPeriodValue;
    int8_t EndOccurrence;
    int32_t GeneratedDayNrOnset;
    int32_t GeneratedDayNrEnd;
};

struct rep_FileOK {
    bool Climate_Filename;
    bool Temperature_Filename;
    bool ETo_Filename;
    bool Rain_Filename;
    bool CO2_Filename;
    bool Calendar_Filename;
    bool Crop_Filename;
    bool Irrigation_Filename;
    bool Management_Filename;
    bool GroundWater_Filename;
    bool Soil_Filename;
    bool SWCIni_Filename;
    bool OffSeason_Filename;
    bool Observations_Filename;
};

void GlobalZero(rep_sum& SumWaBal);

// Global variables (extern declarations)
extern std::string RainFile;
extern std::string RainFileFull;
extern std::string RainDescription;
extern std::string EToFile;
extern std::string EToFileFull;
extern std::string EToDescription;
extern std::string CalendarFile;
extern std::string CalendarFileFull;
extern std::string CalendarDescription;
extern std::string CO2File;
extern std::string CO2FileFull;
extern std::string CO2Description;
extern std::string IrriFile;
extern std::string IrriFileFull;
extern std::string CropFile;
extern std::string CropFileFull;
extern std::string CropDescription;
extern std::string PathNameProg;
extern std::string PathNameOutp;
extern std::string PathNameSimul;
extern std::string ProfFile;
extern std::string ProfFilefull;
extern std::string ProfDescription;
extern std::string ManFile;
extern std::string ManFilefull;
extern std::string ObservationsFile;
extern std::string ObservationsFilefull;
extern std::string ObservationsDescription;
extern std::string OffSeasonFile;
extern std::string OffSeasonFilefull;
extern std::string OutputName;
extern std::string GroundWaterFile;
extern std::string GroundWaterFilefull;
extern std::string ClimateFile;
extern std::string ClimateFileFull;
extern std::string ClimateDescription;
extern std::string IrriDescription;
extern std::string ClimFile;
extern std::string SWCiniFile;
extern std::string SWCiniFileFull;
extern std::string SWCiniDescription;
extern std::string ProjectDescription;
extern std::string ProjectFile;
extern std::string ProjectFileFull;
extern std::string MultipleProjectDescription;
extern std::string MultipleProjectFile;
extern std::string TemperatureFile;
extern std::string TemperatureFileFull;
extern std::string TemperatureDescription;
extern std::string MultipleProjectFileFull;
extern std::string FullFileNameProgramParameters;
extern std::string ManDescription;
extern std::string ClimDescription;
extern std::string OffSeasonDescription;
extern std::string GroundwaterDescription;
extern std::string TnxReferenceFile;
extern std::string TnxReferenceFileFull;
extern std::string TnxReference365DaysFile;
extern std::string TnxReference365DaysFileFull;

extern rep_IrriECw IrriECw;
extern rep_Manag Management;
extern rep_PerennialPeriod perennialperiod;
extern rep_param simulparam;
extern rep_Cuttings Cuttings;
extern rep_Onset onset;
extern rep_EndSeason endseason;
extern rep_Crop crop;
extern rep_Content TotalSaltContent;
extern rep_Content TotalWaterContent;
extern rep_EffectiveRain effectiverain;
extern rep_soil Soil;
extern rep_RootZoneWC RootZoneWC;
extern rep_CropFileSet CropFileSet;
extern rep_sum SumWaBal;
extern rep_RootZoneSalt RootZoneSalt;
extern rep_clim TemperatureRecord, ClimRecord, RainRecord, EToRecord;
extern rep_sim Simulation;

extern GenerateTimeMode GenerateTimeMode_Val;
extern GenerateDepthMode GenerateDepthMode_Val;
extern IrriMode IrriMode_Val;
extern IrriMethod IrriMethod_Val;

extern int32_t TnxReferenceYear;
extern int32_t DaySubmerged;
extern int32_t MaxPlotNew;
extern int32_t NrCompartments;
extern int32_t IrriFirstDayNr;
extern dp ZiAqua;

extern int8_t IniPercTAW;
extern int8_t MaxPlotTr;
extern int8_t OutputAggregate;

extern int fTnxReference;
extern int fTnxReference_iostat;
extern int fTnxReference365Days;
extern int fTnxReference365Days_iostat;

extern dp CCiActual;
extern dp CCiprev;
extern dp CCiTopEarlySen;
extern dp CRsalt;
extern dp CRwater;
extern dp ECdrain;
extern dp ECiAqua;
extern dp ECstorage;
extern dp Eact;
extern dp Epot;
extern dp ETo;
extern dp Drain;
extern dp Infiltrated;
extern dp Irrigation;
extern dp Rain;
extern dp RootingDepth;
extern dp Runoff;
extern dp SaltInfiltr;
extern dp Surf0;
extern dp SurfaceStorage;
extern dp Tact;
extern dp Tpot;
extern dp TactWeedInfested;
extern dp Tmax;
extern dp Tmin;
extern dp TmaxCropReference;
extern dp TminCropReference;
extern dp TmaxTnxReference365Days;
extern dp TminTnxReference365Days;
extern std::vector<sp> TmaxRun;
extern std::vector<sp> TminRun;
extern std::vector<sp> TmaxTnxReference12MonthsRun;
extern std::vector<sp> TminTnxReference12MonthsRun;
extern std::vector<sp> TmaxCropReferenceRun;
extern std::vector<sp> TminCropReferenceRun;
extern std::vector<sp> TmaxTnxReference365DaysRun;
extern std::vector<sp> TminTnxReference365DaysRun;

extern bool EvapoEntireSoilSurface;
extern bool PreDay, OutDaily, Out8Irri;
extern bool Out1Wabal;
extern bool Out2Crop;
extern bool Out3Prof;
extern bool Out4Salt;
extern bool Out5CompWC;
extern bool Out6CompEC;
extern bool Out7Clim;
extern bool Part1Mult, Part2Eval;

extern std::string PathNameList, PathNameParam;

extern std::vector<CompartmentIndividual> Compartment;
extern std::vector<SoilLayerIndividual> soillayer;

extern std::vector<rep_DayEventInt> IrriBeforeSeason;
extern std::vector<rep_DayEventInt> IrriAfterSeason;

// Function declarations
dp DeduceAquaCropVersion(const std::string& FullNameXXFile);
sp RootMaxInSoilProfile(dp ZmaxCrop, int8_t TheNrSoilLayers, const std::vector<SoilLayerIndividual>& TheSoilLayer);
void ZrAdjustedToRestrictiveLayers(dp ZrIN, int8_t TheNrSoilLayers, const std::vector<SoilLayerIndividual>& TheLayer, dp& ZrOUT);
void DeclareInitialCondAtFCandNoSalt();
void set_layer_undef(SoilLayerIndividual& LayerData);
void CropStressParametersSoilFertility(const rep_Shapes& CropSResp, int32_t StressLevel, rep_EffectStress& StressOUT);
dp TimeRootFunction(dp t, int8_t ShapeFactor, dp tmax, dp t0);
dp TimeToReachZroot(dp Zi, dp Zo, dp Zx, int8_t ShapeRootDeepening, int32_t Lo, int32_t LZxAdj);
dp CanopyCoverNoStressSF(int32_t DAP, int32_t L0, int32_t L123, int32_t LMaturity, int32_t GDDL0, int32_t GDDL123, int32_t GDDLMaturity, dp CCo, dp CCx, dp CGC, dp CDC, dp GDDCGC, dp GDDCDC, dp SumGDD, modeCycle TypeDays, int8_t SFRedCGC, int8_t SFRedCCx);
dp CCiNoWaterStressSF(int32_t Dayi, int32_t L0, int32_t L12SF, int32_t L123, int32_t L1234, int32_t GDDL0, int32_t GDDL12SF, int32_t GDDL123, int32_t GDDL1234, dp CCo, dp CCx, dp CGC, dp GDDCGC, dp CDC, dp GDDCDC, dp SumGDD, dp RatDGDD, int8_t SFRedCGC, int8_t SFRedCCx, dp SFCDecline, modeCycle TheModeCycle);
dp FromGravelMassToGravelVolume(dp PorosityPercent, int8_t GravelMassPercent);
void CheckForWaterTableInProfile(dp DepthGWTmeter, const std::vector<CompartmentIndividual>& ProfileComp, bool& WaterTableInProfile);
dp GetWeedRC(int32_t TheDay, dp GDDayi, dp fCCx, int8_t TempWeedRCinput, int8_t TempWeedAdj, int32_t& TempWeedDeltaRC, int32_t L12SF, int32_t TempL123, int32_t GDDL12SF, int32_t TempGDDL123, modeCycle TheModeCycle);
void DetermineLengthGrowthStages(dp CCoVal, dp CCxVal, dp CDCVal, int32_t L0, int32_t TotalLength, bool CGCgiven, int32_t TheDaysToCCini, plant ThePlanting, int32_t& Length123, std::vector<int32_t>& StLength, int32_t& Length12, dp& CGCVal);
int32_t TimeToCCini(plant ThePlantingType, int32_t TheCropPlantingDens, dp TheSizeSeedling, dp TheSizePlant, dp TheCropCCx, dp TheCropCGC);
dp MultiplierCCxSelfThinning(int32_t Yeari, int32_t Yearx, dp ShapeFactor);
int32_t DaysToReachCCwithGivenCGC(dp CCToReach, dp CCoVal, dp CCxVal, dp CGCVal, int32_t L0);
int32_t LengthCanopyDecline(dp CCx, dp CDC);
dp HarvestIndexGrowthCoefficient(dp HImax, dp dHIdt);
dp TauFromKsat(dp Ksat);
int8_t NumberSoilClass(dp SatvolPro, dp FCvolPro, dp PWPvolPro, dp Ksatmm);
void DeriveSmaxTopBottom(dp SxTopQ, dp SxBotQ, dp& SxTop, dp& SxBot);
dp KsTemperature(dp T0, dp T1, dp Tin);
dp KsSalinity(bool SalinityResponsConsidered, int8_t ECeN, int8_t ECeX, dp ECeVAR, dp KsShapeSalinity);
void TimeToMaxCanopySF(dp CCo, dp CGC, dp CCx, int32_t L0, int32_t L12, int32_t L123, int32_t LToFlor, int32_t LFlor, bool DeterminantCrop, int32_t& L12SF, int8_t& RedCGC, int8_t& RedCCx, int32_t& ClassSF);
dp SoilEvaporationReductionCoefficient(dp Wrel, dp Edecline);
dp MaxCRatDepth(dp ParamCRa, dp ParamCRb, dp Ksat, dp Zi, dp DepthGWT);
dp CCmultiplierWeed(int8_t ProcentWeedCover, dp CCxCrop, dp FshapeWeed);
dp CCmultiplierWeedAdjusted(int8_t ProcentWeedCover, dp CCxCrop, dp& FshapeWeed, dp fCCx, int8_t Yeari, int8_t MWeedAdj, int8_t& RCadj);
dp BMRange(int32_t HIadj);
dp HImultiplier(dp RatioBM, dp RangeBM, int8_t HIadj);
dp AdjustedKsStoToECsw(int8_t ECeMin, int8_t ECeMax, int32_t ResponseECsw, dp ECei, dp ECswi, dp ECswFCi, dp Wrel, dp Coeffb0Salt, dp Coeffb1Salt, dp Coeffb2Salt, dp KsStoIN);
dp CCatTime(int32_t Dayi, dp CCoIN, dp CGCIN, dp CCxIN);
void DetermineDayNr(int32_t Dayi, int32_t Monthi, int32_t Yeari, int32_t& DayNr);
void DetermineDate(int32_t DayNr, int32_t& Dayi, int32_t& Monthi, int32_t& Yeari);
dp DegreesDay(dp Tbase, dp Tupper, dp TDayMin, dp TDayMax, int8_t GDDSelectedMethod);
void DetermineCNIandIII(int8_t CN2, int8_t& CN1, int8_t& CN3);
void DetermineCN_default(dp Infiltr, int8_t& CN2);
dp ECeComp(const CompartmentIndividual& Comp);
dp ECswComp(const CompartmentIndividual& Comp, bool atFC);
void SaltSolutionDeposit(dp mm, dp& SaltSolution, dp& SaltDeposit);
dp MultiplierCCoSelfThinning(int32_t Yeari, int32_t Yearx, dp ShapeFactor);
dp KsAny(dp Wrel, dp pULActual, dp pLLActual, dp ShapeFactor);
dp CCatGDD(dp GDDi, dp CCoIN, dp GDDCGCIN, dp CCxIN);
dp CanopyCoverNoStressGDDaysSF(int32_t GDDL0, int32_t GDDL123, int32_t GDDLMaturity, dp SumGDD, dp CCo, dp CCx, dp CGC, dp CDC, dp GDDCGC, dp GDDCDC, int8_t SFRedCGC, int8_t SFRedCCx);
dp HIadjWStressAtFlowering(dp KsVeg, dp KsSto, int8_t a, dp b);
dp fAdjustedForCO2(dp CO2i, dp WPi, int8_t PercentA);
bool FullUndefinedRecord(int32_t FromY, int32_t FromD, int32_t FromM, int32_t ToD, int32_t ToM);
void NoIrrigation();
void LoadIrriScheduleInfo(const std::string& FullName);
void GenerateCO2Description(const std::string& CO2FileFull, std::string& CO2Description);
void GetIrriDescription(const std::string& IrriFileFull, std::string& IrriDescription);
void SetIrriDescription(const std::string& str);
void GetDaySwitchToLinear(int32_t HImax, dp dHIdt, dp HIGC, int32_t& tSwitch, dp& HIGClinear);
bool FileExists(const std::string& full_name);
void SplitStringInTwoParams(const std::string& StringIN, dp& Par1, dp& Par2);
void SplitStringInThreeParams(const std::string& StringIN, dp& Par1, dp& Par2, dp& Par3);
dp CO2ForSimulationPeriod(int32_t FromDayNr, int32_t ToDayNr);
void ReadRainfallSettings();
void ReadSoilSettings();
void LoadClimate(const std::string& FullName, std::string& ClimateDescription, std::string& TempFile, std::string& EToFile, std::string& RainFile, std::string& CO2File);
void LoadCropCalendar(const std::string& FullName, bool& GetOnset, bool& GetOnsetTemp, int32_t& DayNrStart, int32_t YearStart);
void NoManagement();
void LoadManagement(const std::string& FullName);
void SaveCrop(const std::string& totalname);
void SaveProfile(const std::string& totalname);
void DetermineParametersCR(int8_t SoilClass, dp KsatMM, dp& aParam, dp& bParam);
void DetermineNrandThicknessCompartments();
void DetermineRootZoneSaltContent(dp RootingDepth, dp& ZrECe, dp& ZrECsw, dp& ZrECswFC, dp& ZrKsSalt);
void CalculateAdjustedFC(dp DepthAquifer, std::vector<CompartmentIndividual>& CompartAdj);
void AdjustOnsetSearchPeriod();
int32_t ActiveCells(const CompartmentIndividual& Comp);
void DetermineSaltContent(dp ECe, CompartmentIndividual& Comp);
void SetClimData();
std::string DayString(int32_t DNr);
void AdjustYearPerennials(int8_t TheYearSeason, bool Sown1stYear, modeCycle TheCycleMode, dp Zmax, dp ZminYear1, dp TheCCo, dp TheSizeSeedling, dp TheCGC, dp TheCCx, dp TheGDDCGC, int32_t ThePlantingDens, plant& TypeOfPlanting, dp& Zmin, dp& TheSizePlant, dp& TheCCini, int32_t& TheDaysToCCini, int32_t& TheGDDaysToCCini);
void NoCropCalendar();
void DetermineLinkedSimDay1(int32_t CropDay1, int32_t& SimDay1);
void AdjustSimPeriod();
void ResetSWCToFC();
void LoadCrop(const std::string& FullName);
dp SeasonalSumOfKcPot(int32_t TheDaysToCCini, int32_t TheGDDaysToCCini, int32_t L0, int32_t L12, int32_t L123, int32_t L1234, int32_t GDDL0, int32_t GDDL12, int32_t GDDL123, int32_t GDDL1234, dp CCo, dp CCx, dp CGC, dp GDDCGC, dp CDC, dp GDDCDC, dp KcTop, dp KcDeclAgeing, dp CCeffectProcent, dp Tbase, dp Tupper, dp TDayMin, dp TDayMax, dp GDtranspLow, dp CO2i, modeCycle TheModeCycle, bool ReferenceClimate);
dp HarvestIndexDay(int32_t DAP, int32_t DaysToFlower, int32_t HImax, dp dHIdt, dp CCi, dp CCxadjusted, dp TheCCxWithered, int8_t PercCCxHIfinal, plant plant_temp, int8_t& PercentLagPhase, int32_t& HIfinal);
void CompleteCropDescription();
void NoManagementOffSeason();
void LoadOffSeason(const std::string& FullName);
void AdjustThetaInitial(int8_t PrevNrComp, const std::vector<dp>& PrevThickComp, const std::vector<dp>& PrevVolPrComp, const std::vector<dp>& PrevECdSComp);
void LoadClim(const std::string& FullName, std::string& ClimateDescription, rep_clim& ClimateRecord);
void LoadGroundWater(const std::string& FullName, int32_t AtDayNr, int32_t& Zcm, dp& ECdSm);
void AdjustClimRecordTo(int32_t CDayN);
void TranslateIniLayersToSWProfile(int8_t NrLay, const std::vector<dp>& LayThickness, const std::vector<dp>& LayVolPr, const std::vector<dp>& LayECdS, int32_t NrComp, std::vector<CompartmentIndividual>& Comp);
void TranslateIniPointsToSWProfile(int8_t NrLoc, const std::vector<dp>& LocDepth, const std::vector<dp>& LocVolPr, const std::vector<dp>& LocECdS, int32_t NrComp, std::vector<CompartmentIndividual>& Comp);
dp CCiniTotalFromTimeToCCini(int32_t TempDaysToCCini, int32_t TempGDDaysToCCini, int32_t L0, int32_t L12, int32_t L12SF, int32_t L123, int32_t L1234, int32_t GDDL0, int32_t GDDL12, int32_t GDDL12SF, int32_t GDDL123, int32_t GDDL1234, dp CCo, dp CCx, dp CGC, dp GDDCGC, dp CDC, dp GDDCDC, dp RatDGDD, int8_t SFRedCGC, int8_t SFRedCCx, dp SFCDecline, dp fWeed, modeCycle TheModeCycle);
void AdjustCropYearToClimFile(int32_t& CDay1, int32_t& CDayN);
std::string EndGrowingPeriod(int32_t Day1, int32_t& DayN);
void LoadInitialConditions(const std::string& SWCiniFileFull, dp& IniSurfaceStorage);
void AdjustSizeCompartments(dp CropZx);
void CheckForKeepSWC(bool& RunWithKeepSWC, dp& ConstZrxForRun);
void InitializeGlobalStrings();
void LoadProfile(const std::string& FullName);
void LoadProfileProcessing(dp VersionNr);
void DetermineRootZoneWC(dp RootingDepth, bool& ZtopSWCconsidered);
void CalculateETpot(int32_t DAP, int32_t L0, int32_t L12, int32_t L123, int32_t LHarvest, int32_t DayLastCut, dp CCi, dp EToVal, dp KcVal, dp KcDeclineVal, dp CCx, dp CCxWithered, dp CCEffectProcent, dp CO2i, dp GDDayi, dp TempGDtranspLow, dp& TpotVal, dp& EpotVal);
void LoadSimulationRunProject(const std::string& projectFileName);
void ReadCropSettingsParameters();
void ReadFieldSettingsParameters();
void ReadTemperatureSettingsParameters();
void CompleteClimateDescription(rep_clim& ClimateRecord);
int32_t SumCalendarDaysReferenceTnx(int32_t ValGDDays, int32_t RefCropDay1, int32_t StartDayNr, dp Tbase, dp Tupper, dp TDayMin, dp TDayMax);
void DesignateSoilLayerToCompartments(int32_t NrCompartments, int32_t NrSoilLayers, std::vector<CompartmentIndividual>& Compartment);
void specify_soil_layer(int32_t NrCompartments, int32_t NrSoilLayers, std::vector<SoilLayerIndividual>& SoilLayer, std::vector<CompartmentIndividual>& Compartment, rep_Content& TotalWaterContent);
void Calculate_Saltmobility(int32_t layer, int8_t SaltDiffusion, int8_t Macro, std::vector<dp>& Mobil);
void CompleteProfileDescription();
extern std::string GetProjectFileName(int32_t iproject);

} // namespace AquaCrop
