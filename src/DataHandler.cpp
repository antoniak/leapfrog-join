#include "DataHandler.h"

using namespace utils;

void DataHandler::parseSchema(std::string path) {
    property_tree::ptree object;
    try{
        property_tree::read_json(path + "schema.json", object);
        try {
            // read schema 
            property_tree::ptree schema = object.get_child("schema");
            property_tree::ptree variable_order = object.get_child("variable_order");

            int atom_id = 0;
            int variable_id = 0;
            property_tree::ptree::const_iterator atom_it;        
            for (atom_it = schema.begin(); atom_it != schema.end(); ++atom_it) {
                
                relations.push_back(atom_it->first);
                relationsToVariables[atom_id] = {};
                
                property_tree::ptree::const_iterator variable_it;        
                for (variable_it = atom_it->second.begin(); variable_it != atom_it->second.end(); ++variable_it) {                
                    std::string variable = variable_it->second.get_value<std::string>();
                    
                    auto temp = find(variables.begin(), variables.end(), variable);
                    if (temp == variables.end()) {
                        variables.push_back(variable);
                        relationsToVariables[atom_id].push_back(variable_id);
                        variablesToRelations[variable_id] = {atom_id};
                        variable_id++;
                    } else {
                        relationsToVariables[atom_id].push_back(temp - variables.begin());
                        variablesToRelations[temp - variables.begin()].push_back(atom_id);
                    }
                }
                atom_id++;
            }

            // read variable order
            property_tree::ptree::const_iterator vo_it;
            std::string variable;        
            for (vo_it = variable_order.begin(); vo_it != variable_order.end(); ++vo_it) {    
                variable = vo_it->second.get_value<std::string>();
                variableOrder.push_back(find(variables.begin(), variables.end(), variable) - variables.begin());
            }
        } catch (...) {
            std::cerr << "The input json must have 'schema' and 'variable_order' children at the highest level." << std::endl;
            exit(1);
        }

    } catch(const property_tree::json_parser_error &e) {
        std::cerr << "The input json file must be a valid json object." << std::endl;
        exit(1);
    }

    if(relations.size() == 0) {
        std::cerr << "The input must include at least one atom." << std::endl;
        exit(1);
    } else {
        for (auto a: relationsToVariables) {
            if(a.second.size() == 0) {
                std::cerr << "Each atom must have at least one variable." << std::endl;
                exit(1);
            }
        }
    }

    for (auto v: variableOrder) {
        if(find(variables.begin(), variables.end(), variables[v]) == variables.end()){
            std::cerr << "The variables in the variable order do not match the variables in the schema." << std::endl;
            exit(1);
        }
    }
} 


void DataHandler::readDatabase() {
    for (auto relation: relations) {

    }
}

DataHandler::DataHandler(std::string path) {
    parseSchema(path);
    readDatabase();
}

DataHandler::~DataHandler() {
}


