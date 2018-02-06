#pragma once

#include "../bengine/filesystem.hpp"
#include <string>
#include <memory>
#include <fstream>
#include <set>

struct graphviz_t {
    enum graphviz_shape_t { OVAL, PARALLELOGRAM, HOUSE };

    struct graphviz_node_t {
        std::string name;
        std::string destination;
        graphviz_shape_t shape;
    };

    std::vector<graphviz_node_t> nodes;

    graphviz_t(std::string fn) : filename(get_save_path() + std::string("/") + fn)
    {
        gv_file = std::make_unique<std::ofstream>(filename);
        *gv_file << "digraph G {\n";
    }

    ~graphviz_t() {
        std::set<std::string> styles_done;
        *gv_file << "{\n";
        for (const auto &n : nodes) {
            auto finder = styles_done.find(n.name);
            if (finder == styles_done.end()) {
                *gv_file << "    " << n.name << "[";
                switch (n.shape) {
                    case OVAL : *gv_file << "shape=oval"; break;
                    case PARALLELOGRAM : *gv_file << "shape=parallelogram"; break;
                    case HOUSE : *gv_file << "shape=house"; break;
                }
                *gv_file << "]\n";
                styles_done.insert(n.name);
            }
        }
        *gv_file << "}\n";

        for (const auto &n : nodes) {
            *gv_file << n.name << " -> " << n.destination << "\n";
        }

        *gv_file << "}\n";
        gv_file->close();
    }

    inline void add_trees() {
        *gv_file << "\"cut trees\" -> wood_logs\n";
    }

    inline void add_node(const std::string key, const std::string item, graphviz_shape_t shape = OVAL) {
        if (key != "item_block" && item != "item_block") {
            nodes.emplace_back(graphviz_node_t{key, item, shape});
        }
    }

private:
    const std::string filename;
    std::unique_ptr<std::ofstream> gv_file;
};