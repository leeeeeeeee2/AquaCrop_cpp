#pragma once

#include "AquaCrop/Global.h"

namespace AquaCrop {

void InitializeSettings(bool use_default_soil_file, bool use_default_crop_file);
void LoadSimulationRunProject(int32_t NrRun);

} // namespace AquaCrop
