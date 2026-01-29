#pragma once

#include "AquaCrop/Global.h"

namespace AquaCrop {

// Function declarations
void StartTheProgram();
void InitializeTheProgram();
void FinalizeTheProgram();
void PrepareReport();
void GetRequestDailyResults();
void GetRequestParticularResults();
void GetTimeAggregationResults();
void GetProjectType(const std::string& TheProjectFile, typeproject& TheProjectType);
void InitializeProject(int32_t iproject, const std::string& TheProjectFile, typeproject TheProjectType);
void ComposeFileForProgramParameters(const std::string& TheFileNameProgram, std::string& FullFileNameProgramParameters);
void LoadProgramParametersProjectPlugIn(const std::string& FullFileNameProgramParameters, bool& ProgramParametersAvailable);
std::string GetListProjectsFile();
void InitializeProjectFileNames();
int32_t GetNumberOfProjects();
std::string GetProjectFileName(int32_t iproject);
void WriteProjectsInfo(const std::string& line);

} // namespace AquaCrop
