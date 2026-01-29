#include "AquaCrop/ProjectInput.h"
#include "AquaCrop/Utils.h"
#include "AquaCrop/Global.h" // For split/parsing helpers if needed, or I'll reimplement/use existing helpers
#include <fstream>
#include <iostream>
#include <limits>

namespace AquaCrop {

std::vector<ProjectInput_type> ProjectInput;

void allocate_project_input(int32_t NrRuns) {
    ProjectInput.resize(NrRuns);
}

void initialize_project_input(const std::string& filename, int32_t NrRuns) {
    int32_t NrRuns_local;

    if (NrRuns != -1) {
        NrRuns_local = NrRuns;
    } else {
        ReadNumberSimulationRuns(filename, NrRuns_local);
    }
    allocate_project_input(NrRuns_local);

    for (int32_t i = 1; i <= NrRuns_local; ++i) {
        ProjectInput[i - 1].read_project_file(filename, i);
    }
}

void ReadNumberSimulationRuns(const std::string& TempFileNameFull, int32_t& NrRuns) {
    std::ifstream fhandle(TempFileNameFull);
    std::string line;
    int32_t i;
    int32_t NrFileLines = 42;

    NrRuns = 1;

    if (!fhandle.is_open()) {
        std::cerr << "Error opening file: " << TempFileNameFull << std::endl;
        return;
    }

    std::getline(fhandle, line); // Description
    std::getline(fhandle, line); // Version

    for (i = 0; i < 5; ++i) {
        std::getline(fhandle, line);
    }

    for (i = 0; i < NrFileLines; ++i) {
        std::getline(fhandle, line);
    }

    while (fhandle.good()) {
        i = 0;
        bool end_reached = false;
        while (i < (NrFileLines + 5)) {
            if (!std::getline(fhandle, line)) {
                end_reached = true;
                break;
            }
            i++;
        }
        if (!end_reached && i == (NrFileLines + 5)) {
            NrRuns++;
        } else {
            break;
        }
    }
    fhandle.close();
}

int32_t GetNumberSimulationRuns() {
    return ProjectInput.size();
}

void ProjectInput_type::read_project_file(const std::string& filename, int32_t NrRun) {
    std::ifstream fhandle(filename);
    std::string line;
    int32_t i, Runi;

    if (!fhandle.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::getline(fhandle, Description);
    fhandle >> VersionNr;
    std::getline(fhandle, line); // consume newline after version nr

    if (NrRun > 1) {
        for (Runi = 1; Runi < NrRun; ++Runi) {
            for (i = 0; i < 47; ++i) {
                std::getline(fhandle, line);
            }
        }
    }

    int32_t temp_int;
    fhandle >> temp_int; Simulation_YearSeason = (int8_t)temp_int;
    std::getline(fhandle, line); // consume rest of line
    fhandle >> Simulation_DayNr1;
    std::getline(fhandle, line); // consume rest of line
    fhandle >> Simulation_DayNrN;
    std::getline(fhandle, line); // consume rest of line
    fhandle >> Crop_Day1;
    std::getline(fhandle, line); // consume rest of line
    fhandle >> Crop_DayN;
    std::getline(fhandle, line); // consume rest of line

    std::cout << "    DEBUG: read run " << NrRun << ": YearSeason=" << (int)Simulation_YearSeason 
              << " SimDay1=" << Simulation_DayNr1 << " SimDayN=" << Simulation_DayNrN 
              << " CropDay1=" << Crop_Day1 << " CropDayN=" << Crop_DayN << std::endl;

    auto read_section = [&](std::string& info, std::string& fname, std::string& dir) {
        if (!std::getline(fhandle, info)) return;
        if (!std::getline(fhandle, fname)) return;
        if (!std::getline(fhandle, dir)) return;
        // Trim strings to handle trailing comments or spaces
        auto trim = [](std::string& s) {
            s.erase(s.find_last_not_of(" \t\r\n") + 1);
            s.erase(0, s.find_first_not_of(" \t\r\n"));
        };
        trim(info);
        trim(fname);
        trim(dir);
        // Remove single quotes if present in dir
        if (dir.size() >= 2 && dir.front() == '\'' && dir.back() == '\'') {
            dir = dir.substr(1, dir.size() - 2);
        }
    };

    read_section(Climate_Info, Climate_Filename, Climate_Directory);
    read_section(Temperature_Info, Temperature_Filename, Temperature_Directory);
    read_section(ETo_Info, ETo_Filename, ETo_Directory);
    read_section(Rain_Info, Rain_Filename, Rain_Directory);
    read_section(CO2_Info, CO2_Filename, CO2_Directory);
    read_section(Calendar_Info, Calendar_Filename, Calendar_Directory);
    read_section(Crop_Info, Crop_Filename, Crop_Directory);
    read_section(Irrigation_Info, Irrigation_Filename, Irrigation_Directory);
    read_section(Management_Info, Management_Filename, Management_Directory);
    read_section(Soil_Info, Soil_Filename, Soil_Directory);
    read_section(GroundWater_Info, GroundWater_Filename, GroundWater_Directory);
    read_section(SWCIni_Info, SWCIni_Filename, SWCIni_Directory);
    read_section(OffSeason_Info, OffSeason_Filename, OffSeason_Directory);
    read_section(Observations_Info, Observations_Filename, Observations_Directory);

    fhandle.close();
}

} // namespace AquaCrop