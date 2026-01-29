#include "AquaCrop/StartUnit.h"
#include "AquaCrop/Global.h"
#include "AquaCrop/Utils.h"
#include "AquaCrop/InitialSettings.h"
#include "AquaCrop/Run.h"
#include "AquaCrop/ProjectInput.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace AquaCrop {

void StartTheProgram() {
    InitializeGlobalStrings();
    InitializeTheProgram();

    std::string ListProjectsFile = GetListProjectsFile();
    bool ListProjectFileExist = FileExists(ListProjectsFile);
    int32_t nprojects = GetNumberOfProjects();

    if (nprojects > 0) {
        WriteProjectsInfo("");
        WriteProjectsInfo("Projects handled:");
    }

    for (int32_t iproject = 1; iproject <= nprojects; ++iproject) {
        std::string TheProjectFile = GetProjectFileName(iproject);
        typeproject TheProjectType;
        GetProjectType(TheProjectFile, TheProjectType);
        InitializeProject(iproject, TheProjectFile, TheProjectType);
        RunSimulation(TheProjectFile, TheProjectType);
    }

    if (nprojects == 0) {
        WriteProjectsInfo("");
        WriteProjectsInfo("Projects loaded: None");
        if (ListProjectFileExist) {
            WriteProjectsInfo("File \"ListProjects.txt\" does not contain ANY project file");
        } else {
            WriteProjectsInfo("Missing File \"ListProjects.txt\" in LIST directory");
        }
    }

    FinalizeTheProgram();
}

void InitializeTheProgram() {
    PathNameOutp = "OUTP/";
    PathNameSimul = "SIMUL/";
    PathNameList = "PARAM/";
    PathNameParam = "PARAM/";
    PathNameProg = "";

    GetTimeAggregationResults();
    GetRequestDailyResults();
    GetRequestParticularResults();
    PrepareReport();
}

void FinalizeTheProgram() {
    // Placeholder
}

void PrepareReport() {
    // Placeholder
}

void GetRequestDailyResults() {
    // Placeholder
}

void GetRequestParticularResults() {
    // Placeholder
}

void GetTimeAggregationResults() {
    // Placeholder
}

static std::vector<std::string> ProjectFileNames;

int32_t GetNumberOfProjects() {
    ProjectFileNames.clear();
    std::ifstream file(GetListProjectsFile());
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                ProjectFileNames.push_back(line);
            }
        }
        file.close();
    }
    return static_cast<int32_t>(ProjectFileNames.size());
}

std::string GetProjectFileName(int32_t iproject) {
    if (iproject >= 1 && iproject <= static_cast<int32_t>(ProjectFileNames.size())) {
        std::string filename = ProjectFileNames[iproject - 1];
        return filename;
    }
    return "";
}

void GetProjectType(const std::string& TheProjectFile, typeproject& TheProjectType) {
    if (TheProjectFile.find(".ACp") != std::string::npos) {
        TheProjectType = typeproject::typepro;
    } else if (TheProjectFile.find(".PRM") != std::string::npos) {
        TheProjectType = typeproject::typeprm;
    } else {
        TheProjectType = typeproject::typenone;
    }
}

void InitializeProject(int32_t iproject, const std::string& TheProjectFile, typeproject TheProjectType) {
    std::cout << "  " << iproject << ". " << TheProjectFile << std::endl;

    if (TheProjectType == typeproject::typenone) return;

    std::string TestFile = PathNameList + TheProjectFile;
    if (!FileExists(TestFile)) {
        std::cerr << "Project file not found: " << TestFile << std::endl;
        return;
    }

    InitializeSettings(true, true);

    if (TheProjectType == typeproject::typepro) {
        ProjectFile = TheProjectFile;
        ProjectFileFull = PathNameList + ProjectFile;
        initialize_project_input(ProjectFileFull, 1);
        
        // CheckFilesInProject(1, CanSelect, FileOK); // Stub
        
        ProjectDescription = ProjectInput[0].Description;
        FullFileNameProgramParameters = "";
        ComposeFileForProgramParameters(ProjectFile, FullFileNameProgramParameters);
        // LoadProgramParametersProjectPlugIn(FullFileNameProgramParameters, ProgramParametersAvailable); // Stub
        
        Simulation.MultipleRun = false;
        Simulation.NrRuns = 1;
    } else if (TheProjectType == typeproject::typeprm) {
        MultipleProjectFile = TheProjectFile;
        MultipleProjectFileFull = PathNameList + MultipleProjectFile;
        initialize_project_input(MultipleProjectFileFull);
        
        int32_t TotalSimRuns = GetNumberSimulationRuns();
        
        MultipleProjectDescription = "undefined";
        FullFileNameProgramParameters = "";
        ComposeFileForProgramParameters(MultipleProjectFile, FullFileNameProgramParameters);
        // LoadProgramParametersProjectPlugIn(FullFileNameProgramParameters, ProgramParametersAvailable); // Stub
        
        Simulation.MultipleRun = true;
        Simulation.NrRuns = TotalSimRuns;
        
        bool MultipleRunWithKeepSWC_temp = Simulation.MultipleRunWithKeepSWC;
        dp MultipleRunConstZrx_temp = Simulation.MultipleRunConstZrx;
        CheckForKeepSWC(MultipleRunWithKeepSWC_temp, MultipleRunConstZrx_temp);
        Simulation.MultipleRunWithKeepSWC = MultipleRunWithKeepSWC_temp;
        Simulation.MultipleRunConstZrx = MultipleRunConstZrx_temp;
    }
}

void ComposeFileForProgramParameters(const std::string& TheFileNameProgram, std::string& FullFileNameProgramParameters) {
    // Placeholder
}

std::string GetListProjectsFile() {
    return PathNameList + "ListProjects.txt";
}

void InitializeProjectFileNames() {
    // Placeholder
}

void WriteProjectsInfo(const std::string& line) {
    if (!line.empty()) {
        std::cout << line << std::endl;
    }
}


} // namespace AquaCrop

