#pragma once
#include "AquaCrop/Global.h"

namespace AquaCrop {

void GetDecadeTemperatureDataSet(int32_t DayNr, std::vector<rep_DayEventDbl>& MinDataSet, std::vector<rep_DayEventDbl>& MaxDataSet);
void GetMonthlyTemperatureDataSet(int32_t DayNr, std::vector<rep_DayEventDbl>& MinDataSet, std::vector<rep_DayEventDbl>& MaxDataSet);
void TemperatureFileCoveringCropPeriod(int32_t Day1, int32_t DayN);
int32_t GrowingDegreeDays(int32_t ValPeriod, int32_t FirstDayPeriod, dp Tbase, dp Tupper, dp TDayMin, dp TDayMax);
int32_t SumCalendarDays(int32_t ValGDDays, int32_t FirstDayCrop, dp Tbase, dp Tupper, dp TDayMin, dp TDayMax);
dp MaxAvailableGDD(int32_t DayNr, dp Tbase, dp Tupper, dp Tmin, dp Tmax);
void AdjustCalendarCrop(int32_t CropDay1);
void AdjustCropFileParameters(const rep_CropFileSet& CropFileSet, int32_t LseasonDays, int32_t CropDay1, modeCycle ModeCycle, dp Tbase, dp Tupper, int32_t& Crop_DaysToSenescence, int32_t& Crop_DaysToHarvest, int32_t& Crop_GDDaysToSenescence, int32_t& Crop_GDDaysToHarvest);
int32_t ResetCropDay1(int32_t CropDay1, bool Update);
void LoadSimulationRunProject(int32_t NrRun);

}
