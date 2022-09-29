#include "memorypool.h"
#include "types.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>


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
    if(sizeUsed + blockSize <= maxPoolSize){
        block = (char *)pool + (allocated * blockSize);
        sizeUsed += blockSize;
        blockSizeUsed = 0;
        ++allocated;
        return true;
    }
    else{
        std::cout << "Error: Block size exceed current available memory" << '\n';
        return false;
    }
}

Address MemoryPool::allocate(std::size_t sizeRequired){
    // PK
    unsigned short int extraSizeUsed;
    unsigned short int offset;
    // If size require is more than blocksize, reject request
    if(actualSizeUsed + sizeRequired > maxPoolSize || sizeRequired > blockSize){
        std::cout << "Required size too large!" << '\n';
        exit(0);
    }
    // If blocksize does not have enough capacity to support allocation, call allocate block for more memory
    if(allocated==0||(blockSizeUsed+sizeRequired>blockSize)){
        // Find out how much overflow and store in extraSizeUsed
        extraSizeUsed = blockSizeUsed + sizeRequired - blockSize;
        bool allocatedSuccessful = allocateBlock();
        if (!allocatedSuccessful){
            throw std::logic_error("Failed to allocate new block!");
        }
        else{
            // set offset before addition of the extraSizeUsed
            offset = blockSizeUsed;
            blockSizeUsed += extraSizeUsed;
        }
    }
    else{
        // set offset before addition of the sizeRequired
        offset = blockSizeUsed;
        blockSizeUsed += sizeRequired;
    }
    // Update total size used
    actualSizeUsed += sizeRequired;
    Address recordAddress = {block, offset};
    return recordAddress;
}

bool MemoryPool::deallocate(Address addressToBeDeleted, std::size_t sizeToDelete){
    // PK
    actualSizeUsed -= sizeToDelete;
    unsigned int blocksToMinus;
    std::memset((char *)addressToBeDeleted.blockAddress + addressToBeDeleted.offset, '\0', sizeToDelete);
    if (sizeToDelete < blockSize) {
        unsigned char testBlock[blockSize];
        memset(testBlock, '\0', blockSize);
        if (memcmp(testBlock, addressToBeDeleted.blockAddress, blockSize) == 0) {
            sizeUsed -= blockSize;
            --allocated;
        }
    }
    else {
        if (addressToBeDeleted.offset == 0){
            blocksToMinus = (floor(addressToBeDeleted.offset + sizeToDelete)/blockSize);
        }
        else{
            blocksToMinus = (floor(addressToBeDeleted.offset + sizeToDelete)/blockSize)-1;
        }
        allocated -= blocksToMinus;
        sizeUsed -= blocksToMinus * blockSize;
    }
    return true;
}

// Give a block address, offset and size, returns the data there.
void *MemoryPool::loadFromDisk(Address address, std::size_t size){
    // Charles
    void *dataAddress = operator new(size);
    std::memcpy(dataAddress, (char *)address.blockAddress + address.offset, size);
    ++blocksAccessed;
    return dataAddress;
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