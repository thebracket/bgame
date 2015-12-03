#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <string>
#include "../../engine/ecs.hpp"

using std::vector;
using std::unique_ptr;
using engine::entity;
using std::string;
using std::pair;

namespace raws
{

enum tag_type { RENDERABLE, NAME, DESCRIPTION, GLYPH, COLOR_PAIR, OBSTRUCTION,
                POWER_GENERATOR, POWER_BATTERY, SETTLER_ACTION, BUILDABLE, SKILL_REQUIRED, COMPONENT, SLEEPABLE, TILE,
                LAYER, ITEM_TYPE, MODIFIER, CLOTHING_SLOT, CLOTHING_COLOR, CLOTHING, WORKSHOP, EMOTE, INPUT, POWER_DRAIN,
		OUTPUT, PARTICLE_EMITTER
              };

struct base_raw {
    tag_type type;
    vector<unique_ptr<base_raw>> children;

    string get_item_type () {
	string result = "";
	for ( const unique_ptr<base_raw> &child : children) {
	    if (child->type == ITEM_TYPE) {
	      result = child->get_name_override();
	    }
	}
	return result;
    }
    
    virtual void build_components ( entity &parent, const int &x, const int &y, const bool placeholder=false ) const {
        for ( const unique_ptr<base_raw> &child : children ) {
            child->build_components ( parent, x, y, placeholder );
        }
    }


    bool get_buildable ( string &name, int &tile ) {
        bool is_buildable = false;

        for ( const unique_ptr<base_raw> &child : children ) {

            if ( child->type == BUILDABLE ) {
                is_buildable = true;
            } else if ( child->type == NAME ) {
                name = child->get_name_override();
            } else if ( child->type == TILE ) {
                tile = child->get_tile_override();
            } else if ( child->type == RENDERABLE ) {
                child->get_buildable ( name, tile );
            }
        }

        return is_buildable;
    }

    vector<string> get_buildable_requirements() {
        vector<string> result;
        get_buildable_requirements_recurse ( result );
        return result;
    }

    virtual void get_buildable_requirements_recurse ( vector<string> &result ) {
        for ( const unique_ptr<base_raw> &child : children ) {
            child->get_buildable_requirements_recurse ( result );
        }
    }

    virtual string get_name_override() {
        return "";
    }

    virtual int get_tile_override() {
        return 0;
    }

    virtual int get_modifier(const string &stat) {
	int modifier = 0;
	for ( const unique_ptr<base_raw> &child : children ) {
            modifier += child->get_modifier ( stat );
        }
        return modifier;
    }
    
    virtual bool has_reactions_for_workshop(const string &workshop_name) {
	bool result = false;
	for ( const unique_ptr<base_raw> &child : children ) {
	    if ( child->has_reactions_for_workshop( workshop_name ) ) result = true;
	}
	return result;
    }
};

}
