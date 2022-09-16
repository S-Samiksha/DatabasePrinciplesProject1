#include "memorypool.h"
#include "types.h"

#include <vector>
#include <cstring>

// We need to create a general memory pool that can be used for both the relational data and the index.
// This pool should be able to assign new blocks if necessary.

// The Constructor

MemoryPool::MemoryPool(std::size_t maxPoolSize, std::size_t blockSize) {
    this->maxPoolSize = maxPoolSize;
    this->blockSize = blockSize;
    this->sizeUsed = 0;
    this->actualSizeUsed = 0;
    this->allocated = 0;

    this->pool = operator new(maxPoolSize); // Allocate memory to the MemoryPool object
    std::memset(pool, '\0', maxPoolSize); // Make all memory space null.
    this->block = nullptr;
    this->blockSizeUsed = 0;
    this->blocksAccessed = 0;
    this->numAvailBlks = maxPoolSize/blockSize;
}

bool MemoryPool::allocateBlock(){
    // Charles
    block = (char *)pool + (allocated * blockSize); // Set current block pointer to new block
    blockSizeUsed = 0;
    allocated += 1;
    return true;
}

Address MemoryPool::allocate(std::size_t sizeRequired){
    // PK
    if(numAvailBlks>0&&sizeRequired<=blockSize){
        bool allocatedSuccessful = allocateBlock();
    }
    short int offset = blockSizeUsed;
    blockSizeUsed += sizeRequired;
    actualSizeUsed += sizeRequired;
    Address recordAddress = {block,offset};
    return recordAddress;
}

bool MemoryPool::deallocate(Address address, std::size_t sizeToDelete){
    // PK
    actualSizeUsed -= sizeToDelete;
    std::memset((char *)address.blockAddress + address.offset, '\0', sizeToDelete);
    uchar testBlock[blockSize];
    memset(testBlock, '\0', blockSize);
    if (memcmp(testBlock, address.blockAddress, blockSize) == 0)
    {
      sizeUsed -= blockSize;
      allocated--;
    }

    return true;
}

// Give a block address, offset and size, returns the data there.
void *MemoryPool::loadFromDisk(Address address, std::size_t size){
    // Charles
}

// Saves something into the disk. Returns disk address.
Address MemoryPool::saveToDisk(void *itemAddress, std::size_t size){
    // Alp
}

// Update data in disk if I have already saved it before.
Address MemoryPool::saveToDisk(void *itemAddress, std::size_t size, Address diskAddress){
    // Alp
}

MemoryPool::~MemoryPool(){};