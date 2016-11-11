#pragma once

#include <fstream>
#include <rltk.hpp>

void component_loader_xml(rltk::xml_node * node, std::size_t entity_id, std::string xml_identity);