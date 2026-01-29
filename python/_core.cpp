// Python bindings for AquaCrop using pybind11
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "AquaCrop/StartUnit.h"
#include "AquaCrop/Global.h"
#include "AquaCrop/Simul.h"
#include "AquaCrop/ProjectInput.h"

namespace py = pybind11;

// Helper class for results
class SimulationResults {
public:
    std::vector<double> time;
    std::vector<double> cc;           // Canopy cover
    std::vector<double> biomass;      // Biomass
    std::vector<double> yield;        // Yield
    std::vector<double> et;           // Evapotranspiration
    std::vector<double> soil_water;   // Soil water content
    
    // Seasonal summary
    double total_rainfall = 0.0;
    double total_irrigation = 0.0;
    double total_et = 0.0;
    double total_biomass = 0.0;
    double grain_yield = 0.0;
    double harvest_index = 0.0;
    double water_productivity = 0.0;
    
    // Get daily results as numpy arrays
    py::array_t<double> get_time() const { return py::cast(time); }
    py::array_t<double> get_cc() const { return py::cast(cc); }
    py::array_t<double> get_biomass() const { return py::cast(biomass); }
    py::array_t<double> get_yield() const { return py::cast(yield); }
    py::array_t<double> get_et() const { return py::cast(et); }
    
    // Get seasonal summary as dict
    py::dict get_seasonal_summary() const {
        py::dict summary;
        summary["total_rainfall_mm"] = total_rainfall;
        summary["total_irrigation_mm"] = total_irrigation;
        summary["total_et_mm"] = total_et;
        summary["total_biomass_kg_ha"] = total_biomass;
        summary["yield_kg_ha"] = grain_yield;
        summary["harvest_index"] = harvest_index;
        summary["water_productivity_kg_m3"] = water_productivity;
        return summary;
    }
    
    std::string summary() const {
        std::ostringstream oss;
        oss << "Seasonal Summary:\n";
        oss << "  Total rainfall: " << total_rainfall << " mm\n";
        oss << "  Total irrigation: " << total_irrigation << " mm\n";
        oss << "  Total ET: " << total_et << " mm\n";
        oss << "  Total biomass: " << total_biomass << " kg/ha\n";
        oss << "  Grain yield: " << grain_yield << " kg/ha\n";
        oss << "  Harvest index: " << harvest_index << "\n";
        oss << "  Water productivity: " << water_productivity << " kg/m3";
        return oss.str();
    }
};

// Global results object
static SimulationResults g_results;

PYBIND11_MODULE(_core, m) {
    m.doc() = "AquaCrop C++ core module";
    
    // Version
    m.attr("__version__") = "1.0.0";
    
    // SimulationResults class
    py::class_<SimulationResults>(m, "SimulationResults")
        .def("daily", &SimulationResults::get_time,
             "Get daily time steps as numpy array")
        .def("cc", &SimulationResults::get_cc,
             "Get canopy cover time series")
        .def("biomass", &SimulationResults::get_biomass,
             "Get biomass time series")
        .def("yield_", &SimulationResults::get_yield,
             "Get yield time series")
        .def("et", &SimulationResults::get_et,
             "Get evapotranspiration time series")
        .def("seasonal", &SimulationResults::get_seasonal_summary,
             "Get seasonal summary as dictionary")
        .def("summary", &SimulationResults::summary,
             "Get human-readable summary string")
        .def("export_csv", [](const SimulationResults& self, const std::string& filename) {
            // Export to CSV format
            // Implementation would write CSV file
        }, "Export results to CSV file")
        .def("export_json", [](const SimulationResults& self, const std::string& filename) {
            // Export to JSON format
            // Implementation would write JSON file
        }, "Export results to JSON file");
    
    // SoilParameters structure
    py::class_<SoilParams>(m, "SoilParams")
        .def(py::init<>())
        .def_readwrite("field_capacity", &SoilParams::field_capacity)
        .def_readwrite("wilting_point", &SoilParams::wilting_point)
        .def_readwrite("saturation", &SoilParams::saturation)
        .def_readwrite("cn", &SoilParams::cn);
    
    // ClimateParams structure
    py::class_<ClimateParams>(m, "ClimateParams")
        .def(py::init<>())
        .def_readwrite("tmin_file", &ClimateParams::tmin_file)
        .def_readwrite("tmax_file", &ClimateParams::tmax_file)
        .def_readwrite("eto_file", &ClimateParams::eto_file)
        .def_readwrite("rainfall_file", &ClimateParams::rainfall_file);
    
    // CropParams structure
    py::class_<CropParams>(m, "CropParams")
        .def(py::init<>())
        .def_readwrite("crop_name", &CropParams::crop_name)
        .def_readwrite("planting_method", &CropParams::planting_method)
        .def_readwrite("gdd_method", &CropParams::gdd_method)
        .def_readwrite("harvest_index", &CropParams::harvest_index)
        .def_readwrite("cgc", &CropParams::cgc)
        .def_readwrite("cdc", &CropParams::cdc);
    
    // Model class
    py::class_<Model>(m, "Model")
        .def(py::init<>())
        .def(py::init<const std::string&>(),
             py::arg("config") = "")
        
        // Soil methods
        .def("set_soil", &Model::set_soil,
             py::arg("soil_file"),
             "Set soil parameters from file")
        .def("set_soil_parameters", &Model::set_soil_parameters,
             py::arg("params"),
             "Set soil parameters from dictionary")
        
        // Climate methods
        .def("set_climate", &Model::set_climate,
             py::arg("climate_file"),
             "Set climate data from file")
        .def("set_climate_parameters", &Model::set_climate_parameters,
             py::arg("params"),
             "Set climate parameters from dictionary")
        
        // Crop methods
        .def("set_crop", &Model::set_crop,
             py::arg("crop_file"),
             "Set crop parameters from file")
        .def("set_crop_parameters", &Model::set_crop_parameters,
             py::arg("params"),
             "Set crop parameters from dictionary")
        
        // Irrigation methods
        .def("set_irrigation", &Model::set_irrigation,
             py::arg("irrigation"),
             "Set irrigation parameters")
        
        // Simulation control
        .def("set_start_date", &Model::set_start_date,
             py::arg("date"),
             "Set simulation start date (YYYY-MM-DD)")
        .def("set_end_date", &Model::set_end_date,
             py::arg("date"),
             "Set simulation end date (YYYY-MM-DD)")
        .def("set_initial_conditions", &Model::set_initial_conditions,
             py::arg("conditions"),
             "Set initial soil water conditions")
        
        // Run simulation
        .def("run", &Model::run,
             py::arg("project_file") = "",
             "Run the simulation")
        
        // Results
        .def("get_results", &Model::get_results,
             "Get simulation results");
    
    // Free functions
    m.def("version", &version,
          "Get AquaCrop version");
    m.def("get_config", &get_config,
          "Get current configuration as dictionary");
    m.def("set_log_level", &set_log_level,
          py::arg("level"),
          "Set logging level");
    
    // Constants
    m.attr("equiv") = AquaCrop::equiv;
    m.attr("PI") = AquaCrop::PI;
    m.attr("CO2_REF") = AquaCrop::CO2Ref;
    
    // Enumerations
    py::enum_<AquaCrop::modeCycle>(m, "ModeCycle")
        .value("GD_DAYS", AquaCrop::modeCycle::GDDays)
        .value("CALENDAR_DAYS", AquaCrop::modeCycle::CalendarDays)
        .export_values();
    
    py::enum_<AquaCrop::IrriMode>(m, "IrriMode")
        .value("NO_IRRI", AquaCrop::IrriMode::NoIrri)
        .value("MANUAL", AquaCrop::IrriMode::Manual)
        .value("GENERATE", AquaCrop::IrriMode::Generate)
        .export_values();
    
    py::enum_<AquaCrop::typeproject>(m, "ProjectType")
        .value("NONE", AquaCrop::typeproject::typenone)
        .value("PRO", AquaCrop::typeproject::typepro)
        .value("PRM", AquaCrop::typeproject::typeprm)
        .export_values();
}
