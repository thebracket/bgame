#pragma once

enum processor_type_t { AND, OR, NOTGATE, NAND, NOR, EOR, ENOR };

struct signal_processor_t {
	bool active = false;
	processor_type_t processor;
};
