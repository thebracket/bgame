#include "voxreader.hpp"
#include "voxel_model.hpp"
#include <boost/container/flat_map.hpp>

// Credit to vox2png for some of this code. https://github.com/StijnBrouwer/vox2png/blob/master/vox2png.c
// Modified to be less C-like, and more C++ like!

namespace vox {
    // Storage for VOX models
    std::vector<voxel_model> voxel_models;
    boost::container::flat_map<std::string, std::size_t> voxel_index;

    // Reader code
    constexpr uint32_t file_magic   = 542658390;   /* V O X [space] */
    constexpr uint32_t file_version = 150;         /* MagicaVoxel 0.98 */

    constexpr uint32_t pack_id      = 1262698832;  /* P A C K */
    constexpr uint32_t size_id      = 1163544915;  /* S I Z E */
    constexpr uint32_t voxel_id     = 1230657880;  /* X Y Z I */
    constexpr uint32_t palette_id   = 1094862674;  /* R G B A */

    /* MagicaVoxel's default color palette */
    constexpr unsigned int default_palette[256] = {
            0x00000000, 0xffffffff, 0xffccffff, 0xff99ffff, 0xff66ffff, 0xff33ffff, 0xff00ffff, 0xffffccff, 0xffccccff, 0xff99ccff, 0xff66ccff, 0xff33ccff, 0xff00ccff, 0xffff99ff, 0xffcc99ff, 0xff9999ff,
            0xff6699ff, 0xff3399ff, 0xff0099ff, 0xffff66ff, 0xffcc66ff, 0xff9966ff, 0xff6666ff, 0xff3366ff, 0xff0066ff, 0xffff33ff, 0xffcc33ff, 0xff9933ff, 0xff6633ff, 0xff3333ff, 0xff0033ff, 0xffff00ff,
            0xffcc00ff, 0xff9900ff, 0xff6600ff, 0xff3300ff, 0xff0000ff, 0xffffffcc, 0xffccffcc, 0xff99ffcc, 0xff66ffcc, 0xff33ffcc, 0xff00ffcc, 0xffffcccc, 0xffcccccc, 0xff99cccc, 0xff66cccc, 0xff33cccc,
            0xff00cccc, 0xffff99cc, 0xffcc99cc, 0xff9999cc, 0xff6699cc, 0xff3399cc, 0xff0099cc, 0xffff66cc, 0xffcc66cc, 0xff9966cc, 0xff6666cc, 0xff3366cc, 0xff0066cc, 0xffff33cc, 0xffcc33cc, 0xff9933cc,
            0xff6633cc, 0xff3333cc, 0xff0033cc, 0xffff00cc, 0xffcc00cc, 0xff9900cc, 0xff6600cc, 0xff3300cc, 0xff0000cc, 0xffffff99, 0xffccff99, 0xff99ff99, 0xff66ff99, 0xff33ff99, 0xff00ff99, 0xffffcc99,
            0xffcccc99, 0xff99cc99, 0xff66cc99, 0xff33cc99, 0xff00cc99, 0xffff9999, 0xffcc9999, 0xff999999, 0xff669999, 0xff339999, 0xff009999, 0xffff6699, 0xffcc6699, 0xff996699, 0xff666699, 0xff336699,
            0xff006699, 0xffff3399, 0xffcc3399, 0xff993399, 0xff663399, 0xff333399, 0xff003399, 0xffff0099, 0xffcc0099, 0xff990099, 0xff660099, 0xff330099, 0xff000099, 0xffffff66, 0xffccff66, 0xff99ff66,
            0xff66ff66, 0xff33ff66, 0xff00ff66, 0xffffcc66, 0xffcccc66, 0xff99cc66, 0xff66cc66, 0xff33cc66, 0xff00cc66, 0xffff9966, 0xffcc9966, 0xff999966, 0xff669966, 0xff339966, 0xff009966, 0xffff6666,
            0xffcc6666, 0xff996666, 0xff666666, 0xff336666, 0xff006666, 0xffff3366, 0xffcc3366, 0xff993366, 0xff663366, 0xff333366, 0xff003366, 0xffff0066, 0xffcc0066, 0xff990066, 0xff660066, 0xff330066,
            0xff000066, 0xffffff33, 0xffccff33, 0xff99ff33, 0xff66ff33, 0xff33ff33, 0xff00ff33, 0xffffcc33, 0xffcccc33, 0xff99cc33, 0xff66cc33, 0xff33cc33, 0xff00cc33, 0xffff9933, 0xffcc9933, 0xff999933,
            0xff669933, 0xff339933, 0xff009933, 0xffff6633, 0xffcc6633, 0xff996633, 0xff666633, 0xff336633, 0xff006633, 0xffff3333, 0xffcc3333, 0xff993333, 0xff663333, 0xff333333, 0xff003333, 0xffff0033,
            0xffcc0033, 0xff990033, 0xff660033, 0xff330033, 0xff000033, 0xffffff00, 0xffccff00, 0xff99ff00, 0xff66ff00, 0xff33ff00, 0xff00ff00, 0xffffcc00, 0xffcccc00, 0xff99cc00, 0xff66cc00, 0xff33cc00,
            0xff00cc00, 0xffff9900, 0xffcc9900, 0xff999900, 0xff669900, 0xff339900, 0xff009900, 0xffff6600, 0xffcc6600, 0xff996600, 0xff666600, 0xff336600, 0xff006600, 0xffff3300, 0xffcc3300, 0xff993300,
            0xff663300, 0xff333300, 0xff003300, 0xffff0000, 0xffcc0000, 0xff990000, 0xff660000, 0xff330000, 0xff0000ee, 0xff0000dd, 0xff0000bb, 0xff0000aa, 0xff000088, 0xff000077, 0xff000055, 0xff000044,
            0xff000022, 0xff000011, 0xff00ee00, 0xff00dd00, 0xff00bb00, 0xff00aa00, 0xff008800, 0xff007700, 0xff005500, 0xff004400, 0xff002200, 0xff001100, 0xffee0000, 0xffdd0000, 0xffbb0000, 0xffaa0000,
            0xff880000, 0xff770000, 0xff550000, 0xff440000, 0xff220000, 0xff110000, 0xffeeeeee, 0xffdddddd, 0xffbbbbbb, 0xffaaaaaa, 0xff888888, 0xff777777, 0xff555555, 0xff444444, 0xff222222, 0xff111111
    };

    /* Header of the .VOX file */
    struct file_header {
        uint32_t magic, version;
    };

    /* Header of a chunk */
    struct chunk_header {
        uint32_t id;
        uint32_t sizeContent;
        uint32_t sizeChildren;
    };

    /* A chunk that contains the number of models/keyframes */
    struct pack_chunk {
        chunk_header header;
        uint32_t numModels;
    };

    /* A chunk that contains the dimensions of a model */
    struct size_chunk {
        chunk_header header;
        uint32_t x, y, z;
    };

    /* A chunk that contains the voxels of a model */
    struct voxel_chunk {
        chunk_header header;
        uint32_t numVoxels;
    };

    /* A chunk that contains the color palette */
    struct palette_chunk {
        chunk_header header;
        uint32_t colors[256];
    };

    /* A voxel in VoxelChunk */
    struct voxel {
        uint8_t x, y, z;
        uint8_t colorIndex;
    };

    /* A struct containing pointers to all the data we need from the .vox file */
    struct parsed_vox {
        /* The number of models the file contains */
        uint32_t numModels;

        /* An array of pointers to the size chunks */
        size_chunk **sizeChunks;

        /* An array of pointers to the voxel chunks */
        voxel_chunk **voxelChunks;

        /* A pointer to the used palette */
        const uint32_t *palette;
    };

    /* Parses a .vox file and puts the relevant data in a ParsedVox struct */
    parsed_vox parse_vox(size_t len, const char *buf) {
        /* Check the file header before we begin parsing */
        file_header *fHeader = (file_header *) buf;
        if (len < 8 || fHeader->magic != file_magic) {
            fputs("Error: Vox file is corrupted\n", stderr);
            exit(-1);
        }
        if (fHeader->version > file_version) {
            fputs("Warning: Vox file is for a newer MagicaVoxel version than this tool supports\n", stderr);
        }

        uint32_t numModels = 1;
        size_chunk **sizeChunks = NULL;
        int currentSize = 0;
        voxel_chunk **voxelChunks = NULL;
        int currentVoxel = 0;
        const uint32_t *palette = (uint32_t *) &default_palette[0];

        /* Iterate the .vox file chunk by chunk */
        const chunk_header *chunk = (chunk_header *) (fHeader + 1);
        for (;;) {
            uint32_t id = chunk->id;

            /* Handle the chunk types we care about */
            if (id == pack_id) {
                /* Store the amount of models */
                const pack_chunk *pack = (pack_chunk *) chunk;
                numModels = pack->numModels;
                //printf("Found %i models\n", numModels);
            }
            else if (id == size_id) {
                /* Create an array of SizeChunks if we don't have one already */
                if (sizeChunks == NULL) {
                    sizeChunks = (size_chunk **)malloc(sizeof(size_chunk *) * numModels);
                }
                /* Store the SizeChunk in the array */
                size_chunk *size = (size_chunk *) chunk;
                sizeChunks[currentSize] = size;
                currentSize++;
            }
            else if (id == voxel_id) {
                /* Create an array of VoxelChunks if we don't have one already */
                if (voxelChunks == NULL) {
                    voxelChunks = (voxel_chunk **)malloc(sizeof(voxel_chunk *) * numModels);
                }
                /* Store the VoxelChunk in the array */
                voxel_chunk *voxels = (voxel_chunk *) chunk;
                voxelChunks[currentVoxel] = voxels;
                currentVoxel++;
            }
            else if (id == palette_id) {
                /* Store the palette */
                palette_chunk *pal = (palette_chunk *) chunk;
                palette = &pal->colors[0];
                //puts("Found a palette");
            }

            /* Step to the next chunk */
            chunk = (chunk_header *) ((char *) chunk + sizeof(chunk_header) + chunk->sizeContent);
            /* Break if we're at the end of the file */
            if ((char *) chunk >= buf + len) break;
        }

        return parsed_vox{ numModels, sizeChunks, voxelChunks, palette };
    }

    /* Frees the arrays allocated by parseVox */
    void free_parsed_vox(parsed_vox parsedVox) {
        free(parsedVox.sizeChunks);
        free(parsedVox.voxelChunks);
    }

    /* Allocates a buffer and reads the contents of the file into it */
    void read_file(const char *path, size_t *len, char **buf) {
        FILE *handle;
        handle = fopen(path, "rb");
        if (!handle) {
            fputs("Error: Could not read file, are you sure it exists?\n", stderr);
            exit(-1);
        }
        /* Get the length of the file */
        fseek(handle, 0, SEEK_END);
        *len = ftell(handle);
        fseek(handle, 0, SEEK_SET);
        /* Allocate a big enough buffer to fit the file */
        *buf = (char *)malloc(*len + 1);
        /* Read the file into the buffer and close it */
        fread(*buf, *len, 1, handle);
        fclose(handle);
    }

    /* Returns the a pointer to the first voxel in a VoxelChunk */
    const voxel *get_voxels(const voxel_chunk *voxelChunk) {
        return (const voxel *) ((&voxelChunk->numVoxels) + 1);
    }

/* Returns the color from the palette at the specified index */
    uint32_t get_color(const uint32_t *palette, uint8_t index) {
        return palette[((int) index - 1 + 0xFF) % 0xFF];
    }

    void load_vox(const std::string &filename, const std::string &tag)
    {
        auto finder = voxel_index.find(tag);
        if (finder != voxel_index.end()) return; // Do not load duplicates

        char *voxBuffer;
        std::size_t voxLength;
        read_file(filename.c_str(), &voxLength, &voxBuffer);
        parsed_vox parsed = parse_vox(voxLength, voxBuffer);

        size_chunk * current_size = nullptr;
        voxel_model model;
        for (uint32_t i=0; i<parsed.numModels; ++i) {
            current_size = parsed.sizeChunks[i];
            model.width = current_size->x;
            model.height = current_size->y;
            model.depth = current_size->z;
            auto voxel_count = parsed.voxelChunks[i]->numVoxels;

            const voxel *voxels = get_voxels(parsed.voxelChunks[i]);

            for (auto j=0; j<voxel_count; ++j) {
                voxel current_voxel = voxels[j];
                uint32_t color = get_color(parsed.palette, current_voxel.colorIndex);

                const uint8_t a = (color & 0xff000000) >> 24;
                const uint8_t r = (color & 0x00ff0000) >> 16;
                const uint8_t g = (color & 0x0000ff00) >> 8;
                const uint8_t b = (color & 0x000000ff);
                //std::cout << color << " = " << +r << "/" << +g << "/" << +b << "/" << +a << "\n";

                subvoxel v{
                        current_voxel.x,
                        current_voxel.y,
                        current_voxel.z,
                        std::pow((float)b / 256.0f, 2.2f),
                        std::pow((float)g / 256.0f, 2.2f),
                        std::pow((float)r / 256.0f, 2.2f)
                };
                //std::cout << v.x << "," << v.y << "," << v.z << "\n";

                model.voxels.emplace_back(v);
            }
        }
        auto index = voxel_models.size()+1;
        model.build_model();
        voxel_models.emplace_back(model);
        voxel_index.insert(std::make_pair(tag, index));

        free_parsed_vox(parsed);
        free(voxBuffer);
    }

    std::size_t model_index(const std::string tag) {
        auto finder = voxel_index.find(tag);
        if (finder == voxel_index.end()) {
			glog(log_target::LOADER, log_severity::error, "Model not found: {0}", tag);
            return 0;
        } else {
            return finder->second;
        }
    }

    voxel_model * get_model(const std::size_t &index) {
        return &voxel_models[index-1];
    }

}