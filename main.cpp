#include <cstdio>
#include <iostream>

#include "STLData.h"
#include "window_GL.h"

using namespace std;

const char helpText[] =
    "Usage: STLViewer [OPTION]..."
    "\n -i [FILE] \topen [FILE]"
    "\n -b  \t\tfor binary reading mode"
    "\n -s [FILE] \tsave the loaded file as ASCII STL"
    "\n -S [FILE] \tsave the loaded file as Binray STL"
    "\n -fe [FILE] \tsave the loaded file as evolver fe file"
    "\n\n Exmaple:"
    "\n   STLViewer -i data.stl -b -s data_ascii.stl \t Stores as ASCII"
    "\n   STLViewer -i data.stl -S data_ascii.stl \t Stores as binary"
    "\n";

std::string filename = "";
std::string saveName = "";
bool binary = false;
bool save = false;
bool saveBin = false;
bool save_fe = false;

void display_usage() { printf("%s\n", helpText); }

int main(int argc, char *argv[]) {

  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      std::string s = std::string(argv[i]);

      if (s.compare(0, 2, "-h") == 0) {
        display_usage();
        return EXIT_SUCCESS;
      } else if (s.compare(0, 2, "-i") == 0) {
        filename = std::string(argv[i + 1]);
      } else if (s.compare(0, 2, "-b") == 0) {
        binary = true;
      } else if (s.compare(0, 2, "-s") == 0) {
        save = true;
        saveName = std::string(argv[i + 1]);
      } else if (s.compare(0, 3, "-fe") == 0) {
        save_fe = true;
        saveName = std::string(argv[i + 1]);
      } else if (s.compare(0, 2, "-S") == 0) {
        save = true;
        saveBin = true;
        saveName = std::string(argv[i + 1]);
      }
    }
  }

  if (filename.length() == 0) {
    fprintf(stderr, "Please give a file to open or run with -h for help\n");
    return 1;
  }

  STLData *stlData = new STLData(filename.c_str(), binary);

  window_GL *Window_GL = new window_GL(1280, 720, "STLViewer");

  renderer *r = new renderer(0);

  r->loadTriangles(stlData->getTData());
  // stlData->translate(0.0, 0, 0.0);

  Window_GL->registerRenderer(r);

  Window_GL->loop();

  delete Window_GL;

  if (save) {
    stlData->saveToFile(saveName.c_str(), saveBin);
  } if (save_fe){
    stlData->saveToEvolver(saveName.c_str(), "no_refine fixed");  }

  delete stlData;
}