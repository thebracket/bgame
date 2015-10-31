#pragma once

#include <fstream>

using std::fstream;

namespace engine {

class entity {
public:
    int handle;
    void save(fstream &lbfile);
    void load(fstream &lbfile);
};

entity construct_entity_from_file(fstream &lbfile);

}
