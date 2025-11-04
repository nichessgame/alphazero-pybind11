#pragma once

#include <vector>
#include <tuple>

class AgentCache {
  public:
      static const std::vector<std::tuple<int,int>> moveIndexToSrcSquareAndDstSquare;
      static const std::vector<std::vector<int>> srcSquareToDstSquareToMoveIndex;

      AgentCache();
};
