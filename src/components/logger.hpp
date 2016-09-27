#pragma once

#include <rltk.hpp>

struct log_line_t {
    int age = 0;
    std::vector<rltk::vchar> chars;
};

struct logger_t {
	logger_t() {}
    std::vector<log_line_t> lines;

	std::size_t serialization_identity = 26;

	void save(std::ostream &lbfile) {
        std::size_t sz = lines.size();
        serialize(lbfile, sz);
        for (const log_line_t &line : lines) {
            std::size_t line_size = line.chars.size();
            serialize(lbfile, line_size);
            for (const rltk::vchar &c : line.chars) {
                serialize(lbfile, c.glyph);
                serialize(lbfile, c.foreground);
                serialize(lbfile, c.background);
            }
        }
	}

	static logger_t load(std::istream &lbfile) {
        logger_t c;

        std::size_t sz;
        deserialize(lbfile, sz);
        for (std::size_t i=0; i<sz; ++i) {
            log_line_t line;
            std::size_t lsz;
            deserialize(lbfile, lsz);
            for (std::size_t j=0; j<lsz; ++j) {
                rltk::vchar ch;
                deserialize(lbfile, ch.glyph);
                deserialize(lbfile, ch.foreground);
                deserialize(lbfile, ch.background);
                line.chars.push_back(ch);
            }
            c.lines.push_back(line);
        }

		return c;
	}
};
