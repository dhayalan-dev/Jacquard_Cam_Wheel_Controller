#include "pattern_registry.h"

static const pattern_entry_t patternRegistry[] =
{
    {
        "Diamond",
        "/patterns/diamond.bin"
    },
    {
        "Heart",
        "/patterns/heart.bin"
    },
    {
        "Hexagon",
        "/patterns/hexagon.bin"
    },
    {
        "Square",
        "/patterns/square.bin"
    },
    {
        "Star",
        "/patterns/star.bin"
    },
    {
        "Triangle",
        "/patterns/triangle.bin"
    }
};

uint8_t patternRegistryGetCount()
{
    return ( sizeof(patternRegistry) / sizeof(patternRegistry[0]) );
}

const pattern_entry_t* patternRegistryGet(uint8_t index)
{
    if (index >= patternRegistryGetCount())
    {
        return nullptr;
    }

    return &patternRegistry[index];
}

const char* patternRegistryGetName(uint8_t index)
{
    const pattern_entry_t *pattern = patternRegistryGet(index);

    if(pattern == nullptr)
    {
        return "";
    }

    return pattern->name;
}

const char* patternRegistryGetFileName(uint8_t index)
{
    const pattern_entry_t *pattern = patternRegistryGet(index);

    if(pattern == nullptr)
    {
        return "";
    }

    return pattern->filename;
}