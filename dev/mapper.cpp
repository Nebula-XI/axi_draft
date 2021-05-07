
#include "mapper.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>

using namespace std;

//-----------------------------------------------------------------------------

Mapper::Mapper()
{
    mappedList.clear();
    _valid = false;
    _fd = -1;
}

//-----------------------------------------------------------------------------

void Mapper::init(int handle)
{
    mappedList.clear();
    if(handle > 0) {
        _valid = true;
        _fd = handle;
    } else {
        _valid = false;
    }
}

//-----------------------------------------------------------------------------

Mapper::Mapper(int handle)
{
    init(handle);
}

//-----------------------------------------------------------------------------

Mapper::~Mapper()
{
    unmap();
}

//-----------------------------------------------------------------------------

void* Mapper::map(size_t physicalAddress, uint32_t areaSize)
{
    if(!_valid) {
        throw std::string("Invalid handle.\n");
    }

    struct map_addr_t map = {0, physicalAddress, areaSize};

    map.virtualAddress = mmap(0, map.areaSize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, (off_t)map.physicalAddress);
    if(map.virtualAddress == MAP_FAILED) {
        throw std::string("Error in mmap().\n");
    }

    mappedList.push_back(map);

    return map.virtualAddress;
}

//-----------------------------------------------------------------------------

void* Mapper::map(void* physicalAddress, uint32_t areaSize)
{
    if(!_valid) {
        throw std::string("Invalid handle.\n");
    }

    struct map_addr_t map = {0, reinterpret_cast<size_t>(physicalAddress), areaSize};

    map.virtualAddress = mmap(0, map.areaSize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, (off_t)map.physicalAddress);
    if(map.virtualAddress == MAP_FAILED) {
        throw std::string("Error in mmap().\n");
    }

    mappedList.push_back(map);

    return map.virtualAddress;
}

//-----------------------------------------------------------------------------

void Mapper::unmap(void* va)
{
    for(unsigned i=0; i<mappedList.size(); i++) {
        struct map_addr_t map = mappedList.at(i);
        if(map.virtualAddress == va) {
            munmap(map.virtualAddress, map.areaSize);
            map.virtualAddress = 0;
            map.physicalAddress = 0;
            map.areaSize = 0;
            mappedList.erase(mappedList.begin()+i);
        }
    }
}

//-----------------------------------------------------------------------------

void Mapper::unmap()
{
    for(unsigned i=0; i<mappedList.size(); i++) {
        struct map_addr_t map = mappedList.at(i);
        if(map.virtualAddress) {
            munmap(map.virtualAddress, map.areaSize);
            map.virtualAddress = 0;
            map.physicalAddress = 0;
            map.areaSize = 0;
        }
    }
    mappedList.clear();
}

//-----------------------------------------------------------------------------
