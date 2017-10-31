#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include <libconfig.h++>
#include <fstream>
#include <sstream>
#include <exceptions.hpp>

/**
 * @brief Simulation parameters
 *
 * Parameters container for a simulation
 */
class parameters {
public:
    bool IS_CONTINUOUS;
    std::vector<std::vector<int>> GRID_WORLD;
    std::vector<double> INITIAL_STATE;
    unsigned POLICY_SELECTOR;
    unsigned MODEL_SELECTOR;
    std::vector<std::vector<double>> ACTION_SPACE;
    unsigned TREE_SEARCH_BUDGET; ///< Budget for tree-search algorithms

    /**
     * @brief Simulation parameters 'default' constructor
     *
     * The parameters are set to the values defined in this constructor.
     * @deprecated
     */
    parameters(double is_continuous = false) :
        IS_CONTINUOUS(is_continuous)
    {}

    /**
     * @brief Simulation parameters 'from-configuration-file' constructor
     *
     * The parameters are taken into the config file whose path is given as an argument.
     * The used library for parsing is libconfig.
     * @param {const char *} cfg_path; path of the configuration file
     */
    parameters(const char *cfg_path) {
        libconfig::Config cfg;
        cfg.readFile(cfg_path);
        std::string grid_path;
        double sr = 0., sc = 0.;
        unsigned nbac= 0;
        if(cfg.lookupValue("is_continuous",IS_CONTINUOUS)
        && cfg.lookupValue("grid_path",grid_path)
        && cfg.lookupValue("initial_state_row",sr)
        && cfg.lookupValue("initial_state_col",sc)
        && cfg.lookupValue("policy_selector",POLICY_SELECTOR)
        && cfg.lookupValue("model_selector",MODEL_SELECTOR)
        && cfg.lookupValue("nb_actions",nbac)
        && cfg.lookupValue("tree_search_budget",TREE_SEARCH_BUDGET)) {
            GRID_WORLD = parse_grid(grid_path);
            INITIAL_STATE = std::vector<double> {sr,sc};
            for(unsigned i=0; i<nbac; ++i) { // actions parsing
                std::string rname = "a";
                rname += std::to_string(i);
                std::string cname = rname;
                rname += std::to_string(0);
                cname += std::to_string(1);
                double rval = 0., cval = 0.;
                if(cfg.lookupValue(rname,rval)
                && cfg.lookupValue(cname,cval)) {
                    ACTION_SPACE.emplace_back(std::vector<double>{rval,cval});
                } else { // Error in action names syntax
                    throw action_names_configuration_file_exception();
                }
            }
        }
        else { // Error in config file
            throw wrong_syntax_configuration_file_exception();
        }
    }

    /**
     * @brief Parse grid
     *
     * Parse a grid and turn it into a 'std::vector<std::vector<int>>' object.
     * @warning Integer within the config file must be separated with a blanck.
     * @param {const std::string &} grid_path; path of the grid configuration file
     * @return Return a discrete grid world
     */
    std::vector<std::vector<int>> parse_grid(const std::string &grid_path) {
        std::vector<std::vector<int>> gw;
        std::ifstream fi;
        fi.open(grid_path);
        if(!fi.good()) { // throw exception if file not found
            throw wrong_grid_path();
        }
        for(std::string line; std::getline(fi, line); ) {
            gw.emplace_back(std::vector<int>{});
            int read;
            for(std::stringstream iss(line); iss >> read; gw.back().push_back(read));
        }
        return gw;
    }
};

#endif // PARAMETERS_HPP_
