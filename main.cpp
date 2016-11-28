#include <iostream>
#include <cstdio>

#include "STLData.h"
#include "window_GL.h"


using namespace std;


std::string filename = "input.stl";
std::string saveName = "";
bool binary = false;
bool save = false;
bool saveBin = false;


void display_usage(){
    printf("PLACEHOLDERTEXT\n");
}

int main(int argc, char * argv[]) {

    if(argc > 1) {
        for (int i = 1; i < argc; i++) {
            std::string s = std::string(argv[i]);

            if (s.compare(0, 2, "-h") == 0) {
                display_usage();
                return EXIT_SUCCESS;
            }
            else if (s.compare(0, 2, "-i") == 0) {
                filename = std::string(argv[i + 1]);
            }
            else if (s.compare(0, 2, "-b") == 0) {
                binary = true;
            }
            else if (s.compare(0, 2, "-s") == 0) {
                save = true;
                saveName = std::string(argv[i + 1]);
            }
            else if (s.compare(0, 2, "-S") == 0) {
                save = true;
                saveBin = true;
                saveName = std::string(argv[i + 1]);
            }
        }
    }

    STLData * stlData = new STLData(filename.c_str(), binary);

    window_GL *Window_GL = new window_GL(1280, 720, "STLViewer");

    renderer * r = new renderer(0);
    renderer * r2 = new renderer(1);

    r->loadTriangles(stlData->getTData());
    stlData->translate(100.0,0,0.0);
    r2->loadTriangles(stlData->getTData());

    Window_GL->registerRenderer(r);
    Window_GL->registerRenderer(r2);

    Window_GL->loop();

    delete Window_GL;

    if(save) {
        stlData->saveToFile(saveName.c_str(), saveBin);
    }

    delete stlData;

}