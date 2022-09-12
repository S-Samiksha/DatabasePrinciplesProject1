#ifndef _DUMMY_H
#define _DUMMY_H

class dummyRecord {
    public:
    dummyRecord(){
        int test=0;
        this->address = &test;
    }

    int* address;
};

#endif