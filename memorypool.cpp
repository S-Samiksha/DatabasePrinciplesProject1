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
    this->sizeUsed = 0;             // Total size initialised (regardless of usage)
    this->actualSizeUsed = 0;       // Total size being occupied
    this->allocated = 0;            // Number of blocks that have been initialised

    this->pool = operator new(maxPoolSize);     // Allocate memory to the MemoryPool object
    std::memset(pool, '\0', maxPoolSize);       // Make all memory space null.
    this->block = nullptr;
    this->blockSizeUsed = 0;        // A variable that keeps track of the current block size used so that we can allocate to the right address
    this->blocksAccessed = 0;       // A variable that keeps track of the total number of times the blocks have been accessed
}

bool MemoryPool::allocateBlock(){
    // Charles
    block = (char *)pool + (allocated * blockSize); // Set current block pointer to new block
    blockSizeUsed = 0;
    ++allocated;
    return true;
}

Address MemoryPool::allocate(std::size_t sizeRequired){
    // PK
    if(numAvailBlks > 0 && sizeRequired <= blockSize){
        bool allocatedSuccessful = allocateBlock();
    }
    unsigned short int offset = blockSizeUsed;
    blockSizeUsed += sizeRequired;
    actualSizeUsed += sizeRequired;
    Address recordAddress = {block, offset};
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
        --allocated;
    }

    return true;
}

// Give a block address, offset and size, returns the data there.
void *MemoryPool::loadFromDisk(Address address, std::size_t size){
    // Charles
}

// A function that saves the records into the disk. It returns the disk address.
Address MemoryPool::saveToDisk(void *itemAddress, std::size_t size){
    // Alp
    // You typecast to char pointer so that it copies the raw byte data. 
    // 3 items here, 
    // 1) 'diskAddress.blockAddress + offset' is to store it in the memorypool location,
    // 2) 'itemAddress' is the source object item that you're storing, 
    // 3) 'size' is to indicate the size of the item you're storing.
    Address diskAddress = allocate(size); // Call on the Allocate function to provide a space for storage
    std::memcpy((char *)diskAddress.blockAddress + diskAddress.offset, itemAddress, size);

    // Update block accessed counter
    ++blocksAccessed;   // Using pre-fix is faster than using postfix.
    return diskAddress;
}

// Same function as above, just to retrieve the cursor pointer.
Address MemoryPool::saveToDisk(void *itemAddress, std::size_t size, Address diskAddress){
    // Alp
    std::memcpy((char *)diskAddress.blockAddress + diskAddress.offset, itemAddress, size);

    // Update block accessed counter
    ++blocksAccessed;   // Using pre-fix is faster than using postfix.
    return diskAddress;
}

MemoryPool::~MemoryPool(){};
