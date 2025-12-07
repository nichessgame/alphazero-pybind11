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

class ActionValue {
  public:
    PlayerAction action;
    float value;

    ActionValue() {}
    ActionValue(PlayerAction action, float value): action(action), value(value) {}
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
    Vector<float> heuristicPolicy();
    Vector<float> alphaBetaPolicy();
    void makeAction(uint32_t move);
    std::string moveToPlayerAction(uint32_t move);
    float quiescence(float alpha, float beta);
    std::vector<ModifiedPlayerAction> generateActionsWithScore();
    std::vector<ModifiedPlayerAction> generateNonQuietActionsWithScore();

  private:
    float evaluateHeuristic();
};

} // namespace nichess_wrapper
