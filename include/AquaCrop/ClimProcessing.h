#pragma once
#include "AquaCrop/Global.h"

namespace AquaCrop {

void GetDecadeEToDataSet(int32_t DayNr, std::vector<rep_DayEventDbl>& DataSet);
void GetMonthlyEToDataSet(int32_t DayNr, std::vector<rep_DayEventDbl>& DataSet);
void GetDecadeRainDataSet(int32_t DayNr, std::vector<rep_DayEventDbl>& DataSet);
void GetMonthlyRainDataSet(int32_t DayNr, std::vector<rep_DayEventDbl>& DataSet);

}
