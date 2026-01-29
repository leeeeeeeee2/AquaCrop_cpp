#pragma once

#include "AquaCrop/Kinds.h"
#include <vector>
#include <string>

namespace AquaCrop {

struct ProjectInput_type {
    dp VersionNr;
    std::string Description;
    int8_t Simulation_YearSeason;
    int32_t Simulation_DayNr1;
    int32_t Simulation_DayNrN;
    int32_t Crop_Day1;
    int32_t Crop_DayN;
    std::string Climate_Info;
    std::string Climate_Filename;
    std::string Climate_Directory;
    std::string Temperature_Info;
    std::string Temperature_Filename;
    std::string Temperature_Directory;
    std::string ETo_Info;
    std::string ETo_Filename;
    std::string ETo_Directory;
    std::string Rain_Info;
    std::string Rain_Filename;
    std::string Rain_Directory;
    std::string CO2_Info;
    std::string CO2_Filename;
    std::string CO2_Directory;
    std::string Calendar_Info;
    std::string Calendar_Filename;
    std::string Calendar_Directory;
    std::string Crop_Info;
    std::string Crop_Filename;
    std::string Crop_Directory;
    std::string Irrigation_Info;
    std::string Irrigation_Filename;
    std::string Irrigation_Directory;
    std::string Management_Info;
    std::string Management_Filename;
    std::string Management_Directory;
    std::string GroundWater_Info;
    std::string GroundWater_Filename;
    std::string GroundWater_Directory;
    std::string Soil_Info;
    std::string Soil_Filename;
    std::string Soil_Directory;
    std::string SWCIni_Info;
    std::string SWCIni_Filename;
    std::string SWCIni_Directory;
    std::string OffSeason_Info;
    std::string OffSeason_Filename;
    std::string OffSeason_Directory;
    std::string Observations_Info;
    std::string Observations_Filename;
    std::string Observations_Directory;

    void read_project_file(const std::string& filename, int32_t NrRun);
};

extern std::vector<ProjectInput_type> ProjectInput;

void allocate_project_input(int32_t NrRuns);
void initialize_project_input(const std::string& filename, int32_t NrRuns = -1);
void ReadNumberSimulationRuns(const std::string& TempFileNameFull, int32_t& NrRuns);
int32_t GetNumberSimulationRuns();

} // namespace AquaCrop