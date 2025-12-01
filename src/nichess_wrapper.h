#pragma once

#include "shapes.h"
#include "agent_cache.h"
#include "nichess/nichess.hpp"
#include <vector>

using namespace nichess;
using namespace alphazero;

namespace nichess_wrapper {

class ModifiedPlayerAction {
  public: 
    PlayerAction action;
    int score;
    
    ModifiedPlayerAction() {}
    ModifiedPlayerAction(PlayerAction action, int score): action(action), score(score) {}
};

class GameWrapper {
  public:
    Game* game;
    int actionsSinceProgress;

    GameWrapper();
    GameWrapper(const std::string encodedBoard);
    GameWrapper(const GameWrapper& other);
    GameWrapper(const std::unique_ptr<nichess_wrapper::GameWrapper> &other);
    ~GameWrapper();

    PlayerAction createNichessAction(uint32_t move);
    Vector<uint8_t> computeValids() const;
    Vector<float> quiescenceSearch();
    void makeAction(uint32_t move);
    std::string moveToPlayerAction(uint32_t move);

  private:
    float evaluateHeuristic();
    float quiescence(float alpha, float beta);
    std::vector<ModifiedPlayerAction> generateNonQuietActionsWithScore();
};

} // namespace nichess_wrapper
