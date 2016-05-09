//
// Created by lucas on 4/11/16.
//

#ifndef STLVIEWER_STLDATA_H
#define STLVIEWER_STLDATA_H

#include <string>
#include <vector>


struct triangle {
    float ni, nj, nk;
    float v1[3], v2[3], v3[3];
    uint16_t attributeByte = 0;
};

class STLData {

    std::string name;
    std::vector<triangle *> tData;

public:
    STLData(size_t n = 0);

    STLData(const char *filename, bool binary = true);

    ~STLData();

    size_t addTriangle(triangle *t);

    void addTriangles(std::vector<triangle *>);

    int saveToFile(const char *filename, bool binary);

    void printInfo();


    void setName(const std::string name) {
        STLData::name = name;
    }

    const std::string &getName() const {
        return name;
    }

    const std::vector<triangle *> &getTData() const {
        return tData;
    }
};


#endif //STLVIEWER_STLDATA_H
