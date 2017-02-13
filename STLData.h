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

    int saveToEvolver(const char *filename, const char *options = "fixed",
                           const size_t id = 1, const size_t start_id = 100);
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

    void translate(float x, float y, float z){
        for(size_t i = 0; i < tData.size(); i++){
            tData[i]->v1[0] += x;
            tData[i]->v1[1] += y;
            tData[i]->v1[2] += z;

            tData[i]->v2[0] += x;
            tData[i]->v2[1] += y;
            tData[i]->v2[2] += z;

            tData[i]->v3[0] += x;
            tData[i]->v3[1] += y;
            tData[i]->v3[2] += z;
        }
    }
};


#endif //STLVIEWER_STLDATA_H
