#ifndef _BIGMAP_
#define _BIGMAP_
#pragma once

#include <string>
#include <vector>

#include "IslandBox.h"
#include "TreasureMap.h"

class BigMap
{
public:
  BigMap(std::string file);

  auto isMatch(const TreasureMap& treasure)->IslandBox&;
  void findIslands();
  void drawGrid(int count);
  void loadIslandNames();

private:
  std::vector<std::pair<std::string, std::string>>names;
  int boxSize = 164;
  std::string file_;
  std::string debugFile_;
  std::string diff_;
  cimg_library::CImg<unsigned char> image_;
  std::vector<IslandBox> islandBoxes;

  bool isBlack(int x, int y);
  bool isColor(unsigned char* color, unsigned char* min, unsigned char* max);
  int hasBlack(int startX, int startY, int count, bool alongX, bool reverse = false);
  std::pair<char, int> convertCoords(int x, int y);
};

#endif

