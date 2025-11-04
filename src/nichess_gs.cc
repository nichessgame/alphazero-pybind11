#include "nichess_gs.h"
#include "agent_cache.h"

namespace alphazero::nichess_gs {

[[nodiscard]] std::unique_ptr<GameState> NichessGS::copy() const noexcept {
  auto up = std::make_unique<NichessGS>();
  up->gameWrapper = std::make_unique<nichess_wrapper::GameWrapper>(gameWrapper);
  return up;
}

[[nodiscard]] bool NichessGS::operator==(
    const GameState& other) const noexcept {
  const auto* other_cs = dynamic_cast<const NichessGS*>(&other);
  if (other_cs == nullptr) {
    return false;
  }
  for(int i = 0; i < WIDTH*HEIGHT; i++) {
    if(*gameWrapper->game->board[i] != *other_cs->gameWrapper->game->board[i]) {
      return false;
    }
  }
  return (other_cs->gameWrapper->game->currentPlayer == gameWrapper->game->currentPlayer) &&
    (other_cs->gameWrapper->game->moveNumber == gameWrapper->game->moveNumber);
}

void NichessGS::hash(absl::HashState h) const {
  // TODO
  return;
}

[[nodiscard]] Vector<uint8_t> NichessGS::valid_moves() const noexcept {
  auto valids = gameWrapper->computeValids();
  return valids;
}

void NichessGS::play_move(uint32_t move) {
  gameWrapper->makeAction(move);
}

[[nodiscard]] std::optional<Vector<float>> NichessGS::scores() const noexcept {
  std::optional<nichess::Player> winner = gameWrapper->game->winner();
  auto scores = SizedVector<float, 3>{};
  scores.setZero();
  if(winner) {
    if(*winner == nichess::PLAYER_1) {
      scores(0) = 1;
    } else {
      scores(1) = 1;
    }
  } else if(gameWrapper->game->repetitionsDraw) {
    scores[2] = 1;
  } else if(gameWrapper->game->moveNumber >= 250) {
    scores[2] = 1;
  } else if(gameWrapper->actionsSinceProgress >= 24 && gameWrapper->game->moveNumber < 50) { // TODO: This is not part of the game, but helps with training. Chess has the "50 move rule", but they count 1 move as p1 action and p2 action, so this would be the "25 move rule" chess equivalent.
    scores[2] = 1;
  } else if(gameWrapper->actionsSinceProgress >= 40 && gameWrapper->game->moveNumber < 80) {
    scores[2] = 1;
  } else if(gameWrapper->actionsSinceProgress >= 60) {
    scores[2] = 1;
  } else {
    return std::nullopt;
  }
  return scores;
}

int pieceTypeToCanonicalIndex(nichess::PieceType pt) {
  switch(pt) {
    case P1_KING:
      return 0;
    case P1_MAGE:
      return 1;
    case P1_PAWN:
      return 2;
    case P1_WARRIOR:
      return 3;
    case P1_ASSASSIN:
      return 4;
    case P1_KNIGHT:
      return 5;
    case P2_KING:
      return 6;
    case P2_MAGE:
      return 7;
    case P2_PAWN:
      return 8;
    case P2_WARRIOR:
      return 9;
    case P2_ASSASSIN:
      return 10;
    case P2_KNIGHT:
      return 11;
    default:
      return 1000;
  }
}

int layerNumToSymmetricalLayerNum(int layerNumber) {
  switch(layerNumber) {
    case 0:
      return 6;
    case 1:
      return 7;
    case 2:
      return 8;
    case 3:
      return 9;
    case 4:
      return 10;
    case 5:
      return 11;
    case 6:
      return 0;
    case 7:
      return 1;
    case 8:
      return 2;
    case 9:
      return 3;
    case 10:
      return 4;
    case 11:
      return 5;
    case 12:
      return 18;
    case 13:
      return 19;
    case 14:
      return 20;
    case 15:
      return 21;
    case 16:
      return 22;
    case 17:
      return 23;
    case 18:
      return 12;
    case 19:
      return 13;
    case 20:
      return 14;
    case 21:
      return 15;
    case 22:
      return 16;
    case 23:
      return 17;
    default:
      return 1000;
  }
}

float pieceTypeToMaxHealthPoints(nichess::PieceType pt) {
  switch(pt) {
    case P1_KING:
      return nichess::KING_STARTING_HEALTH_POINTS;
    case P1_MAGE:
      return nichess::MAGE_STARTING_HEALTH_POINTS;
    case P1_PAWN:
      return nichess::PAWN_STARTING_HEALTH_POINTS;
    case P1_WARRIOR:
      return nichess::WARRIOR_STARTING_HEALTH_POINTS;
    case P1_ASSASSIN:
      return nichess::ASSASSIN_STARTING_HEALTH_POINTS;
    case P1_KNIGHT:
      return nichess::KNIGHT_STARTING_HEALTH_POINTS;
    case P2_KING:
      return nichess::KING_STARTING_HEALTH_POINTS;
    case P2_MAGE:
      return nichess::MAGE_STARTING_HEALTH_POINTS;
    case P2_PAWN:
      return nichess::PAWN_STARTING_HEALTH_POINTS;
    case P2_WARRIOR:
      return nichess::WARRIOR_STARTING_HEALTH_POINTS;
    case P2_ASSASSIN:
      return nichess::ASSASSIN_STARTING_HEALTH_POINTS;
    case P2_KNIGHT:
      return nichess::KNIGHT_STARTING_HEALTH_POINTS;
    default:
      return 0;
  }
}

[[nodiscard]] Tensor<float, 3> NichessGS::canonicalized() const noexcept {
  auto out = CanonicalTensor{};
  out.setZero();
  auto currentPlayerIdx = 24 + gameWrapper->game->currentPlayer;
  out.chip(currentPlayerIdx, 0).setConstant(1);
  for(int i = 0; i < gameWrapper->game->playerToPieces[Player::PLAYER_1].size(); i++) {
    Piece* currentPiece = gameWrapper->game->playerToPieces[Player::PLAYER_1][i];
    if(currentPiece->healthPoints <= 0) continue;
    int x = currentPiece->squareIndex -  (std::floor(currentPiece->squareIndex / NUM_ROWS) * NUM_ROWS);
    int y = std::floor(currentPiece->squareIndex / NUM_ROWS);
    int currentCanonicalIdx = pieceTypeToCanonicalIndex(currentPiece->type);
    out(currentCanonicalIdx, y, x) = 1.0f;
    float maxHP = pieceTypeToMaxHealthPoints(currentPiece->type);
    out(currentCanonicalIdx + 12, y, x) = currentPiece->healthPoints / maxHP;
  }

  for(int i = 0; i < gameWrapper->game->playerToPieces[Player::PLAYER_2].size(); i++) {
    Piece* currentPiece = gameWrapper->game->playerToPieces[Player::PLAYER_2][i];
    if(currentPiece->healthPoints <= 0) continue;
    int x = currentPiece->squareIndex -  (std::floor(currentPiece->squareIndex / NUM_ROWS) * NUM_ROWS);
    int y = std::floor(currentPiece->squareIndex / NUM_ROWS);
    int currentCanonicalIdx = pieceTypeToCanonicalIndex(currentPiece->type);
    out(currentCanonicalIdx, y, x) = 1.0f;
    float maxHP = pieceTypeToMaxHealthPoints(currentPiece->type);
    out(currentCanonicalIdx + 12, y, x) = currentPiece->healthPoints / maxHP;
  }

  return out;
}

[[nodiscard]] std::vector<PlayHistory> NichessGS::symmetries(
    const PlayHistory& base) const noexcept {
  std::vector<PlayHistory> syms{base};
  PlayHistory mirror;
  mirror.v = Vector<float>{3};
  mirror.v(0) = base.v(1);
  mirror.v(1) = base.v(0);
  mirror.v(2) = base.v(2);
  mirror.canonical = CanonicalTensor{};
  // -2 because last 2 layers indicate whose turn it is
  for (auto f = 0; f < CANONICAL_SHAPE[0]-2; ++f) {
    for (auto h = 0; h < HEIGHT; ++h) {
      for (auto w = 0; w < WIDTH; ++w) {
        int symLayer = layerNumToSymmetricalLayerNum(f);
        mirror.canonical(symLayer, (HEIGHT - 1) - h, w) = base.canonical(f, h, w);
      }
    }
  }
  // change turn
  for (auto h = 0; h < HEIGHT; ++h) {
    for (auto w = 0; w < WIDTH; ++w) {
      mirror.canonical(24, h, w) = base.canonical(25, h, w);
      mirror.canonical(25, h, w) = base.canonical(24, h, w);
    }
  }
  mirror.pi = Vector<float>{NUM_MOVES};
  mirror.pi.setZero();
  // -1 because the last move is SKIP
  for (auto i = 0; i < NUM_MOVES - 1; ++i) {
    if(base.pi(i) != 0) {
      std::tuple<int, int> srcAndDst = AgentCache::moveIndexToSrcSquareAndDstSquare[i];
      int srcIdx = std::get<0>(srcAndDst);
      int dstIdx = std::get<1>(srcAndDst);
      std::tuple<int, int> srcXAndY = nichess::boardIndexToCoordinates(srcIdx);
      int srcX = std::get<0>(srcXAndY);
      int srcY = std::get<1>(srcXAndY);
      std::tuple<int, int> dstXAndY = nichess::boardIndexToCoordinates(dstIdx);
      int dstX = std::get<0>(dstXAndY);
      int dstY = std::get<1>(dstXAndY);
      int newSrcX = srcX;
      int newSrcY = HEIGHT - 1 - srcY;
      int newDstX = dstX;
      int newDstY = HEIGHT - 1 - dstY;
      int newSrcIdx = nichess::coordinatesToBoardIndex(newSrcX, newSrcY);
      int newDstIdx = nichess::coordinatesToBoardIndex(newDstX, newDstY);
      int symMove = AgentCache::srcSquareToDstSquareToMoveIndex[newSrcIdx][newDstIdx];
      mirror.pi(symMove) = base.pi(i);
    }
  }
  syms.push_back(mirror);
  return syms;
}

[[nodiscard]] std::string NichessGS::dump() const noexcept {
  return gameWrapper->game->dump();
}

[[nodiscard]] std::string NichessGS::move_to_player_action(uint32_t move) const {
  return gameWrapper->moveToPlayerAction(move);
};

[[nodiscard]] int NichessGS::src_and_dst_to_move(int srcIdx, int dstIdx) const {
  return AgentCache::srcSquareToDstSquareToMoveIndex[srcIdx][dstIdx];
};

}
