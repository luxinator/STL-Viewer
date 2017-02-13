//
// Created by lucas on 4/11/16.
//

#include "STLData.h"

#include <climits>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string.h>

/*
 * See:
 * http://stackoverflow.com/questions/2437283/c-c-packing-signed-char-into-int
 */
#define PACK(c0, c1, c2, c3)                                                   \
  (((uint32_t)(uint8_t)(c0) << 24) | ((uint32_t)(uint8_t)(c1) << 16) |         \
   ((uint32_t)(uint8_t)(c2) << 8) | ((uint32_t)(uint8_t)(c3)))

inline float bytesToFloat(const char *bytes, size_t start) {

  float val;
  memcpy(&val, bytes + start, sizeof val);
  return val;
}

void binaryToTriangle(const char *buffer, triangle *t) {

  t->ni = bytesToFloat(buffer, 0);
  t->nj = bytesToFloat(buffer, 4);
  t->nk = bytesToFloat(buffer, 8);

  t->v1[0] = bytesToFloat(buffer, 12);
  t->v1[1] = bytesToFloat(buffer, 16);
  t->v1[2] = bytesToFloat(buffer, 20);

  t->v2[0] = bytesToFloat(buffer, 12 + 12);
  t->v2[1] = bytesToFloat(buffer, 16 + 12);
  t->v2[2] = bytesToFloat(buffer, 20 + 12);

  t->v3[0] = bytesToFloat(buffer, 12 + 24);
  t->v3[1] = bytesToFloat(buffer, 16 + 24);
  t->v3[2] = bytesToFloat(buffer, 20 + 24);
}

STLData::STLData(size_t n) {
  if (n)
    tData.reserve(n);
  this->name = "stlData";
}

STLData::STLData(const char *filename, bool binary) {

  std::ifstream stlFile;

  if (!binary) {
    printf("STLData: Opening file: %s\n", filename);
    stlFile.open(filename, std::ios::in);
    if (!stlFile) {
      fprintf(stderr, "STLData: Error opening file: [%s]\n", filename);
      fprintf(stderr, "%s\n", strerror(errno));
      std::abort();
      return;
    }

    std::string line;

    std::getline(stlFile, line);

    size_t i = line.find(' ');
    if (line.compare(0, i, "solid") != 0) {
      fprintf(stderr, "STLData: STL File does not start with solid [name]\n");
      return;
    }
    this->name = line.substr(i + 1);

    bool facet = false;
    bool outerloop = false;
    int lineCounter = 1;
    size_t vertex = 0;
    triangle *t = nullptr;
    while (std::getline(stlFile, line)) {
      lineCounter++;

      i = line.find("facet normal");
      if (!facet && i < std::string::npos) {
        facet = true;
        t = new triangle;
        std::stringstream subline = std::stringstream(line.substr(i + 12));
        subline >> t->ni >> t->nj >> t->nk;
      }

      if (t != nullptr) {
        if (line.find("endfacet") < std::string::npos && facet) {
          facet = false;
          if (outerloop) {
            fprintf(stderr, "STLData: LINE: %i Found and [endfacet] keyword, "
                            "while still in [outer loop] mode!\n",
                    lineCounter);
            std::abort();
          }
          this->addTriangle(t);
          t = nullptr;
        } else if (line.find("outer loop") < std::string::npos && !outerloop) {
          outerloop = true;
        } else if (line.find("endloop") < std::string::npos && outerloop) {
          outerloop = false;
          vertex = 0;
        } else if (facet && outerloop && vertex < 3) {
          size_t j = line.find("vertex");
          if (j < std::string::npos) {
            std::stringstream subline = std::stringstream(line.substr(j + 6));
            switch (vertex) {
            case 0:
              subline >> t->v1[0] >> t->v1[1] >> t->v1[2];
              break;
            case 1:
              subline >> t->v2[0] >> t->v2[1] >> t->v2[2];
              break;
            case 2:
              subline >> t->v3[0] >> t->v3[1] >> t->v3[2];
              break;
            default:
              fprintf(stderr, "Found more then 3 vertices in a triangle?!\n");
            }

            vertex++;
          } else {
            fprintf(stderr, "STLData: Found Garbage in file %s at line: %i",
                    filename, lineCounter);
          }
        } else if (vertex >= 3) {
          fprintf(stderr, "STLData: LINE %i, Found a Facet with more then 3 "
                          "vertices, skipping!\n",
                  lineCounter);
        }
        if (line.find("endsolid") < std::string::npos) {
          break;
        }
      }
      lineCounter++;
    }
  }
  // Binary Loader
  else {

    printf("STLData: Opening file: %s\n", filename);
    stlFile.open(filename, std::ios::in | std::ios::binary);
    if (!stlFile) {
      fprintf(stderr, "STLData: Error opening file: [%s]\n", filename);
      fprintf(stderr, "%s\n", strerror(errno));
      std::abort();
      return;
    }

    if (CHAR_BIT != 8) {
      fprintf(stderr, "!!Binary File reading only works on 8-bit-wordsize "
                      "systems at the moment!!\n");
    }

    char *header = new char[80];
    // Read in Header:
    stlFile.read(header, 80);

    printf("--- HEADER ---:\n %s\n", header);
    this->name = std::string(header);

    char *byteHolder_32 = new char[4];
    stlFile.read(byteHolder_32, 4);
    uint32_t nrOfFacets_32 = PACK(byteHolder_32[3], byteHolder_32[2],
                                  byteHolder_32[1], byteHolder_32[0]);
    //        printf("nrOfFacets_32: %i\n", nrOfFacets_32);

    tData.reserve(nrOfFacets_32);

    char *buffer = new char[50];
    for (size_t i = 0; i < nrOfFacets_32; i++) {
      triangle *t = new triangle;
      stlFile.read(buffer, 50);
      binaryToTriangle(buffer, t);
      tData.push_back(t);
    }
    delete[] header;
    delete[] byteHolder_32;
    delete[] buffer;
  }
  printf("STLData: Loaded STL:\n");
  this->printInfo();
  printf("STLData: Closing File\n");

  stlFile.close();
}

size_t STLData::addTriangle(triangle *t) {

  tData.push_back(t);
  return tData.size();
}

STLData::~STLData() {

  for (size_t i = 0; i < tData.size(); i++) {
    delete tData[i];
    tData[i] = nullptr;
  }
}

void STLData::printInfo() {

  printf("STLData: %p with name %s \n Contains: %lu triangles\n Has capacity "
         "%lu\n",
         this, name.c_str(), tData.size(), tData.capacity());
}

/*
 * Save to Surface Evolver fe format.
 * Has following structure:
 *  vertices
 *  ID  x   y   z   (options)
 *
 *  edges
 *  ID  vert_id vert_id (options)
 *
 *  faces //For face of N edges, in our case alway three
 *  ID  (edge_id edge_id)^N
 *
 *  bodies //For bodie of M faces, which can be HUGE
 *  ID (face_id face_id ...)^M
 */
int STLData::saveToEvolver(const char *filename, const char *options,
                           const size_t id, const size_t start_id) {

  printf("Opening File %s\n", filename);
  std::FILE *feFile = fopen(filename, "wb");

  if (!feFile) {
    fprintf(stderr, "STLData: Error opening file: [%s]\n", filename);
    fprintf(stderr, "%s\n", strerror(errno));
    return -1;
  }

  printf("Writing...\n");

  // Dump vertices:
  fprintf(feFile, "vertices\n");
  printf("Writing vertices\n");
  for (size_t i = 0; i < tData.size(); i++) {
    triangle *t = tData[i];
    size_t id = start_id + i * 3;
    fprintf(feFile, "%zu\t%f\t%f\t%f\t\n", id + 1, t->v1[0], t->v1[1], t->v1[2]);
    fprintf(feFile, "%zu\t%f\t%f\t%f\t\n", id + 2, t->v2[0], t->v2[1],
            t->v2[2]);
    fprintf(feFile, "%zu\t%f\t%f\t%f\t\n", id + 3, t->v3[0], t->v3[1],
            t->v3[2]);
  }

  fprintf(feFile, "\nedges\n");
  printf("Writing edges\n");
  for (size_t i = 0; i < tData.size(); i++) {
    size_t id = start_id + i * 3;
    fprintf(feFile, "%zu\t%zu\t%zu\t%s\n", id + 1, id + 1, id + 2,
            options);
    fprintf(feFile, "%zu\t%zu\t%zu\t%s\n", id + 2, id + 2, id + 3,
            options);
    fprintf(feFile, "%zu\t%zu\t%zu\t%s\n", id + 3, id + 3, id + 1,
            options);
  }

  fprintf(feFile, "\nfaces\n");
  printf("Writing faces\n");
  for (size_t i = 0; i < tData.size(); i++) {
    size_t id = start_id + i;
    fprintf(feFile, "%zu\t%zu\t%zu\t%zu\tno_refine %s\n", id + 1, start_id + i * 3 + 1, start_id + i * 3 + 2,
           start_id + i * 3 + 3, options);
  }

  // We Assum our STL is one body... if possible...
  fprintf(feFile, "\nbodies\n");
  printf("Writing bodies\n");
  fprintf(feFile, "%zu\t", start_id);
  for (size_t i = 0; i < tData.size(); i++) {
    fprintf(feFile, "%zu ", start_id + i + 1);
  }
  //fprintf(feFile, "%s\n", options);

  printf("Closing file!\n");
  fclose(feFile);
  return 0;
}

int STLData::saveToFile(const char *filename, bool binary) {

  int nrWritten = 0;
  printf("Opening File %s\n", filename);

  if (!binary) {

    std::FILE *stlFile = fopen(filename, "w");

    if (!stlFile) {
      fprintf(stderr, "STLData: Error opening file: [%s]\n", filename);
      fprintf(stderr, "%s\n", strerror(errno));
      return -1;
    }

    printf("Writing...\n");
    fprintf(stlFile, "solid %s\n", this->name.c_str());

    for (size_t i = 0; i < tData.size(); i++) {
      triangle *t = tData[i];
      fprintf(stlFile, "facet normal %f %f %f\n", t->ni, t->nj, t->nk);
      fprintf(stlFile, " outer loop\n");
      fprintf(stlFile, "  vertex %f %f %f\n", t->v1[0], t->v1[1], t->v1[2]);
      fprintf(stlFile, "  vertex %f %f %f\n", t->v2[0], t->v2[1], t->v2[2]);
      fprintf(stlFile, "  vertex %f %f %f\n", t->v3[0], t->v3[1], t->v3[2]);
      fprintf(stlFile, " endloop\n");
      fprintf(stlFile, "endfacet\n");
      nrWritten++;
    }

    fprintf(stlFile, "endsolid %s", name.c_str());

  } else {

    std::FILE *stlFile = fopen(filename, "wb");

    if (!stlFile) {
      fprintf(stderr, "STLData: Error opening file: [%s]\n", filename);
      fprintf(stderr, "%s\n", strerror(errno));
      return -1;
    }

    printf("Writing...\n");
    // Assuming a char is 8-bit wide (one word)
    char ASCIIBuff[80];
    memset(&ASCIIBuff, 0, 80);
    memcpy(&ASCIIBuff, this->name.c_str(), this->name.size());

    uint32_t size = (uint32_t)tData.size();

    fwrite(ASCIIBuff, sizeof(char), 80, stlFile);
    fwrite(&size, sizeof(uint32_t), 1, stlFile);
    // So copying writing the struct in one go fails... so piecewise!
    for (size_t i = 0; i < tData.size(); i++) {
      fwrite(&tData[i]->ni, sizeof(float), 1, stlFile);
      fwrite(&tData[i]->nj, sizeof(float), 1, stlFile);
      fwrite(&tData[i]->nk, sizeof(float), 1, stlFile);

      fwrite(&tData[i]->v1[0], sizeof(float), 1, stlFile);
      fwrite(&tData[i]->v1[1], sizeof(float), 1, stlFile);
      fwrite(&tData[i]->v1[2], sizeof(float), 1, stlFile);

      fwrite(&tData[i]->v2[0], sizeof(float), 1, stlFile);
      fwrite(&tData[i]->v2[1], sizeof(float), 1, stlFile);
      fwrite(&tData[i]->v2[2], sizeof(float), 1, stlFile);

      fwrite(&tData[i]->v3[0], sizeof(float), 1, stlFile);
      fwrite(&tData[i]->v3[1], sizeof(float), 1, stlFile);
      fwrite(&tData[i]->v3[2], sizeof(float), 1, stlFile);
      fwrite(&tData[i]->attributeByte, sizeof(tData[i]->attributeByte), 1,
             stlFile);
    }
  }
  printf("Done! Closing\n");

  return nrWritten;
}

void STLData::addTriangles(std::vector<triangle *> a) {
  this->tData.insert(tData.end(), a.begin(), a.end());
}
