#include <iostream>
#include <cstdio>

#include "STLData.h"
#include "window_GL.h"


using namespace std;

std::string filename = "input.stl";
bool binary = false;

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
        }
    }

    STLData * stlData = new STLData(filename.c_str(), binary);
//    stlData->saveToFile("test.stl", false);
//   stlData->~STLData();

    window_GL *Window_GL = new window_GL(1280, 720, "STLViewer");

    renderer * r = new renderer(0);

    r->loadTriangles(stlData->getTData());

    Window_GL->registerRenderer(r);

    Window_GL->loop();

    delete Window_GL;



}