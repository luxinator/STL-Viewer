//
// Created by lucas on 4/11/16.
//

#ifndef STLVIEWER_STLDATA_H
#define STLVIEWER_STLDATA_H

#include <string>
#include <vector>


struct triangle{
    float ni, nj, nk;
    float v1[3], v2[3], v3[3];
    uint16_t attributeByte = 0;
};

class STLData {

    std::string name;

    unsigned long tDataSize;

    std::vector<triangle*> tData;

public:
    STLData(size_t n);
    STLData(const char * filename, bool binary=true);
    ~STLData();
    size_t addTriangle(triangle *t);
    int saveToFile(const char* filename, bool binary);
    void printInfo();


    const std::vector<triangle *> &getTData() const {
        return tData;
    }
};


#endif //STLVIEWER_STLDATA_H
