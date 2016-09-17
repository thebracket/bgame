#include "octree.hpp"

octree_t::octree_t(const int &width, const int &height, const int &depth, const int layer) : w(width), h(height), d(depth), n(layer) 
{
    x = 0;
    y = 0;
    z = 0;
}

octree_t::octree_t(const octree_t * parent, int section) {
    w = parent->w/2;
    h = parent->h/2;
    d = parent->d/2;
    n = parent->n+1;

    if (section == 0) { x=parent->x; y=parent->y; z=parent->z; }
    if (section == 1) { x=parent->x + w; y=parent->y; z=parent->z; }
    if (section == 2) { x=parent->x; y=parent->y + h; z=parent->z; }
    if (section == 3) { x=parent->x + w; y=parent->y + h; z=parent->z; }
    if (section == 4) { x=parent->x; y=parent->y; z=parent->z + d; }
    if (section == 5) { x=parent->x + w; y=parent->y; z=parent->z + d; }
    if (section == 6) { x=parent->x; y=parent->y + h; z=parent->z + d; }
    if (section == 7) { x=parent->x + w; y=parent->y + h; z=parent->z + d; }
}

void octree_t::add_node(const octree_location_t loc) {
    contents.push_back(loc);

    if (contents.size()>0 && w!=1 && h!=1 && d!=1) {
        for (const octree_location_t &l : contents) {
            const int subsection = get_child_index(l);
            if (!children[subsection]) {
                children[subsection] = std::make_unique<octree_t>(this, subsection);
            }
            children[subsection]->add_node(l);
        }
        contents.clear();
    }

    ++total_nodes;
}

int octree_t::find_nearest(const octree_location_t &loc) {
    if (total_nodes == 0) {
        return -1;
    }
    if (!contents.empty()) {
        return contents[0].id;
    } else {
        const int subsection = get_child_index(loc);
        if (children[subsection]) return children[subsection]->find_nearest(loc);
        return -1;
    }
}

void octree_t::remove_node(const octree_location_t &loc) {
    // Does this node contain the desired entity?
    auto finder = std::find_if(contents.begin(), contents.end(), [&loc] (octree_location_t &n) { return n.id==loc.id;});
    if (finder != contents.end()) {
        // If so, remove it and we're done
        contents.erase(std::remove_if(contents.begin(), contents.end(), [&loc] (octree_location_t &n) { return n.id==loc.id;}));
        return;
    }

    // If not, determine where it is and traverse down
    const int subsection = get_child_index(loc);
    if (children[subsection]) children[subsection]->remove_node(loc);
}

std::vector<std::size_t> octree_t::find_by_loc(const octree_location_t &loc) {
    std::vector<std::size_t> result;
    find_by_loc_impl(loc, result);
    return result;
}

void octree_t::find_by_loc_impl(const octree_location_t &loc, std::vector<std::size_t> &result) {
    for (const auto &l : contents) {
        if (l.x == loc.x && l.y == loc.y && l.z == loc.z) result.push_back(l.id);
    }

    const int subsection = get_child_index(loc);
    if (children[subsection]) children[subsection]->find_by_loc_impl(loc, result);
}