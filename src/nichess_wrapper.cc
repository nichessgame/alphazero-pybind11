#include "nichess_wrapper.h"
#include "nichess/nichess.hpp"
#include "nichess/util.hpp"
#include "nichess_constants.h"
#include <math.h>
#include <algorithm>
#include <iostream>

using namespace nichess;
using namespace alphazero::nichess_gs;

/*
std::vector<std::tuple<int, int>> generateMoveIndexToSrcSquareAndDstSquare() {
  // -1 because the last index is reserved for MOVE_SKIP which doesn't have source and destination
  std::vector<std::tuple<int, int>> moveIndexToSrcSquareAndDstSquare(NUM_MOVES - 1);
  int newX, newY, srcSquareIndex, dstSquareIndex, moveIndex;
  for(int srcY = 0; srcY < NUM_ROWS; srcY++) {
    for(int srcX = 0; srcX < NUM_COLUMNS; srcX++) {
      srcSquareIndex = coordinatesToBoardIndex(srcX, srcY);
      int n = 0;
      for(int dy = -2; dy < 3; dy++) {
        for(int dx = -2; dx < 3; dx++) {
          if(dx == 0 && dy == 0) {
            continue; 
          }
          newX = srcX + dx;
          newY = srcY + dy;
          // newX or newY might be off board, but it doesn't matter.
          // Move legality should be checked elsewhere.
          dstSquareIndex = coordinatesToBoardIndex(newX, newY);
          moveIndexToSrcSquareAndDstSquare[srcSquareIndex * NUM_MAX_POSSIBLE_MOVES_FOR_PIECE + n] = std::tuple<int, int>(srcSquareIndex, dstSquareIndex);
          n++;
        }
      }
      // 4 extra moves:
      newX = srcX + 3;
      newY = srcY + 3;
      if(!isOffBoard(newX, newY)) {
        dstSquareIndex = coordinatesToBoardIndex(newX, newY);
        moveIndexToSrcSquareAndDstSquare[srcSquareIndex * NUM_MAX_POSSIBLE_MOVES_FOR_PIECE + n] = std::tuple<int, int>(srcSquareIndex, dstSquareIndex);
      }
      n++;

      newX = srcX + 3;
      newY = srcY - 3;
      if(!isOffBoard(newX, newY)) {
        dstSquareIndex = coordinatesToBoardIndex(newX, newY);
        moveIndexToSrcSquareAndDstSquare[srcSquareIndex * NUM_MAX_POSSIBLE_MOVES_FOR_PIECE + n] = std::tuple<int, int>(srcSquareIndex, dstSquareIndex);
      }
      n++;

      newX = srcX - 3;
      newY = srcY + 3;
      if(!isOffBoard(newX, newY)) {
        dstSquareIndex = coordinatesToBoardIndex(newX, newY);
        moveIndexToSrcSquareAndDstSquare[srcSquareIndex * NUM_MAX_POSSIBLE_MOVES_FOR_PIECE + n] = std::tuple<int, int>(srcSquareIndex, dstSquareIndex);
      }
      n++;

      newX = srcX - 3;
      newY = srcY - 3;
      if(!isOffBoard(newX, newY)) {
        dstSquareIndex = coordinatesToBoardIndex(newX, newY);
        moveIndexToSrcSquareAndDstSquare[srcSquareIndex * NUM_MAX_POSSIBLE_MOVES_FOR_PIECE + n] = std::tuple<int, int>(srcSquareIndex, dstSquareIndex);
      }
      n++;
    }
  }
  return moveIndexToSrcSquareAndDstSquare;
}

std::vector<std::vector<int>> generateSrcSquareToDstSquareToMoveIndex() {
  std::vector<std::vector<int>> srcSquareToDstSquareToMoveIndex(NUM_SQUARES);
  int newX, newY, srcSquareIndex, dstSquareIndex, moveIndex;
  for(int srcY = 0; srcY < NUM_ROWS; srcY++) {
    for(int srcX = 0; srcX < NUM_COLUMNS; srcX++) {
      srcSquareIndex = coordinatesToBoardIndex(srcX, srcY);
      std::vector<int> dstSquareToMoveIndex(NUM_SQUARES);
      int n = 0;
      for(int dy = -2; dy < 3; dy++) {
        for(int dx = -2; dx < 3; dx++) {
          newX = srcX + dx;
          newY = srcY + dy;
          if(isOffBoard(newX, newY)) {
            n++;
            continue; 
          }
          if(dx == 0 && dy == 0) continue;

          dstSquareIndex = coordinatesToBoardIndex(newX, newY);
          dstSquareToMoveIndex[dstSquareIndex] = srcSquareIndex * NUM_MAX_POSSIBLE_MOVES_FOR_PIECE + n;
          n++;
        }
      }
      // 4 extra moves:
      newX = srcX + 3;
      newY = srcY + 3;
      if(!isOffBoard(newX, newY)) {
        dstSquareIndex = coordinatesToBoardIndex(newX, newY);
        dstSquareToMoveIndex[dstSquareIndex] = srcSquareIndex * NUM_MAX_POSSIBLE_MOVES_FOR_PIECE + n;
      }
      n++;

      newX = srcX + 3;
      newY = srcY - 3;
      if(!isOffBoard(newX, newY)) {
        dstSquareIndex = coordinatesToBoardIndex(newX, newY);
        dstSquareToMoveIndex[dstSquareIndex] = srcSquareIndex * NUM_MAX_POSSIBLE_MOVES_FOR_PIECE + n;
      }
      n++;

      newX = srcX - 3;
      newY = srcY + 3;
      if(!isOffBoard(newX, newY)) {
        dstSquareIndex = coordinatesToBoardIndex(newX, newY);
        dstSquareToMoveIndex[dstSquareIndex] = srcSquareIndex * NUM_MAX_POSSIBLE_MOVES_FOR_PIECE + n;
      }
      n++;

      newX = srcX - 3;
      newY = srcY - 3;
      if(!isOffBoard(newX, newY)) {
        dstSquareIndex = coordinatesToBoardIndex(newX, newY);
        dstSquareToMoveIndex[dstSquareIndex] = srcSquareIndex * NUM_MAX_POSSIBLE_MOVES_FOR_PIECE + n;
      }
      n++;
      
      srcSquareToDstSquareToMoveIndex[srcSquareIndex] = dstSquareToMoveIndex;
    }
  }
  return srcSquareToDstSquareToMoveIndex;
}

nichess_wrapper::AgentCache::AgentCache() {
  moveIndexToSrcSquareAndDstSquare = generateMoveIndexToSrcSquareAndDstSquare();
  srcSquareToDstSquareToMoveIndex = generateSrcSquareToDstSquareToMoveIndex();
}
*/

nichess_wrapper::GameWrapper::GameWrapper() {
  actionsSinceProgress = 0;
  game = new Game();
}

nichess_wrapper::GameWrapper::GameWrapper(const std::string encodedBoard) {
  actionsSinceProgress = 0; // not encoded, so assume 0
  game = new Game(encodedBoard);
}

nichess_wrapper::GameWrapper::GameWrapper(const nichess_wrapper::GameWrapper& other) {
  actionsSinceProgress = other.actionsSinceProgress;
  game = new Game(*other.game);
}

nichess_wrapper::GameWrapper::GameWrapper(const std::unique_ptr<nichess_wrapper::GameWrapper> &other) {
  actionsSinceProgress = other->actionsSinceProgress;
  game = new Game(*other->game);
}

nichess_wrapper::GameWrapper::~GameWrapper() {
  delete game;
}


Vector<uint8_t> nichess_wrapper::GameWrapper::computeValids() const {
  auto valids = Vector<uint8_t>{NUM_MOVES};
  valids.setZero();
  bool foundLegalMove = false;
  std::vector<Piece*> currentPlayerPieces = game->playerToPieces[game->currentPlayer];
  for(int i = 0; i < currentPlayerPieces.size(); i++) {
    Piece* currentPiece = currentPlayerPieces[i];
    if(currentPiece->healthPoints <= 0) continue; // dead pieces don't move
    std::vector<PlayerAction> legalActions = game->legalActionsByPiece(currentPiece);
    for(int j = 0; j < legalActions.size(); j++) {
      PlayerAction action = legalActions[j];
      int currentIndex = AgentCache::srcSquareToDstSquareToMoveIndex[action.srcIdx][action.dstIdx];
      valids[currentIndex] = 1;
      foundLegalMove = true;
    }
  }
  /* TODO: Always false in the current version.
  if(!foundLegalMove) {
    valids[MOVE_SKIP_IDX] = 1;
  }
  */

  return valids;
}

Vector<float> nichess_wrapper::GameWrapper::quiescenceSearch() {
  auto retval = Vector<float>{3};
  retval.setZero();

  float val = this->quiescence(-9999, 9999);

  // normalize the value to (0, 1)
  // 0 - current player is losing, 1 - current player is winning
  float tv = 1 / (1 + std::exp(-0.2 * val));
  retval[0] = tv;
  retval[1] = 1 - tv;

  return retval;
}

// This could be a 2D vector in agent_cache but it's easier to edit here.
// Doesn't include special cases: MOVE_REGULAR, MOVE_CASTLE, MAGE_THROW_ASSASSIN, WARRIOR_THROW_WARRIOR, 
// MOVE_PROMOTE_P1_PAWN, MOVE_PROMOTE_P2_PAWN and SKIP. 
float srcAndDstPieceToPolicyScore(Piece *srcPiece, Piece *dstPiece) {
  const float a = 2;
  const float b = 4;
  const float c = 8;
  const float d = 10000;
  switch (srcPiece->type) {
    case P1_KING:
      switch(dstPiece->type) {
        case P2_KING:
          return d;
        case P2_MAGE:
          return b;
        case P2_WARRIOR:
          return a;
        case P2_ASSASSIN:
          return a;
        case P2_KNIGHT:
          return a;
        case P2_PAWN:
          return a;
      }
      break;
    case P1_MAGE:
      switch(dstPiece->type) {
        case P2_KING:
          return d;
        case P2_MAGE:
          return a;
        case P2_WARRIOR:
          return a;
        case P2_ASSASSIN:
          return a;
        case P2_KNIGHT:
          return a;
        case P2_PAWN:
          return a;
      }
      break;
    case P1_WARRIOR:
      switch(dstPiece->type) {
        case P2_KING:
          return d;
        case P2_MAGE:
          return b;
        case P2_WARRIOR:
          return a;
        case P2_ASSASSIN:
          return a;
        case P2_KNIGHT:
          return a;
        case P2_PAWN:
          return a;
      }
      break;
    case P1_ASSASSIN:
      switch(dstPiece->type) {
        case P2_KING:
          return d;
        case P2_MAGE:
          return b;
        case P2_WARRIOR:
          return a;
        case P2_ASSASSIN:
          return b;
        case P2_KNIGHT:
          return b;
        case P2_PAWN:
          return a;
      }
      break;
    case P1_KNIGHT:
      switch(dstPiece->type) {
        case P2_KING:
          return d;
        case P2_MAGE:
          return b;
        case P2_WARRIOR:
          return b;
        case P2_ASSASSIN:
          return b;
        case P2_KNIGHT:
          return b;
        case P2_PAWN:
          return a;
      }
      break;
    case P1_PAWN:
      switch(dstPiece->type) {
        case P2_KING:
          return d;
        case P2_MAGE:
          return c;
        case P2_WARRIOR:
          return c;
        case P2_ASSASSIN:
          return c;
        case P2_KNIGHT:
          return c;
        case P2_PAWN:
          return a;
      }
      break;
    case P2_KING:
      switch(dstPiece->type) {
        case P1_KING:
          return d;
        case P1_MAGE:
          return b;
        case P1_WARRIOR:
          return a;
        case P1_ASSASSIN:
          return a;
        case P1_KNIGHT:
          return a;
        case P1_PAWN:
          return a;
      }
      break;
    case P2_MAGE:
      switch(dstPiece->type) {
        case P1_KING:
          return d;
        case P1_MAGE:
          return a;
        case P1_WARRIOR:
          return a;
        case P1_ASSASSIN:
          return a;
        case P1_KNIGHT:
          return a;
        case P1_PAWN:
          return a;
      }
      break;
    case P2_WARRIOR:
      switch(dstPiece->type) {
        case P1_KING:
          return d;
        case P1_MAGE:
          return b;
        case P1_WARRIOR:
          return a;
        case P1_ASSASSIN:
          return a;
        case P1_KNIGHT:
          return a;
        case P1_PAWN:
          return a;
      }
      break;
    case P2_ASSASSIN:
      switch(dstPiece->type) {
        case P1_KING:
          return d;
        case P1_MAGE:
          return b;
        case P1_WARRIOR:
          return a;
        case P1_ASSASSIN:
          return b;
        case P1_KNIGHT:
          return b;
        case P1_PAWN:
          return a;
      }
      break;
    case P2_KNIGHT:
      switch(dstPiece->type) {
        case P1_KING:
          return d;
        case P1_MAGE:
          return b;
        case P1_WARRIOR:
          return b;
        case P1_ASSASSIN:
          return b;
        case P1_KNIGHT:
          return b;
        case P1_PAWN:
          return a;
      }
      break;
    case P2_PAWN:
      switch(dstPiece->type) {
        case P1_KING:
          return d;
        case P1_MAGE:
          return c;
        case P1_WARRIOR:
          return c;
        case P1_ASSASSIN:
          return c;
        case P1_KNIGHT:
          return c;
        case P1_PAWN:
          return a;
      }
      break;

  }
  return 10000000; // this should never happen
}

Vector<float> nichess_wrapper::GameWrapper::heuristicPolicy() {
  auto retval = Vector<float>{NUM_MOVES};
  retval.setZero();
  std::vector<PlayerAction> actions = game->generateLegalActions();
  float scoreSum = 0;
  for(int i = 0; i < actions.size(); i++) {
    PlayerAction pa = actions[i];
    // special cases:
    if(pa.actionType == ActionType::SKIP) {
      // never true in the current version
      retval[MOVE_SKIP_IDX] = 1;
      scoreSum += 1;
      continue;
    } else if(
        pa.actionType == ActionType::ABILITY_MAGE_THROW_ASSASSIN ||
        pa.actionType == ActionType::ABILITY_WARRIOR_THROW_WARRIOR ||
        pa.actionType == ActionType::MOVE_PROMOTE_P1_PAWN ||
        pa.actionType == ActionType::MOVE_PROMOTE_P2_PAWN
        ) {
      int currentIndex = AgentCache::srcSquareToDstSquareToMoveIndex[pa.srcIdx][pa.dstIdx];
      retval[currentIndex] += 5;
      scoreSum += 5;
      continue;
    } else if(
        pa.actionType == ActionType::MOVE_REGULAR ||
        pa.actionType == ActionType::MOVE_CASTLE
        ) {
      int currentIndex = AgentCache::srcSquareToDstSquareToMoveIndex[pa.srcIdx][pa.dstIdx];
      retval[currentIndex] += 1;
      scoreSum += 1;
      continue;
    } else {
      Piece *srcPiece  = game->board[pa.srcIdx];
      Piece *dstPiece  = game->board[pa.dstIdx];
      int currentIndex = AgentCache::srcSquareToDstSquareToMoveIndex[pa.srcIdx][pa.dstIdx];
      float heuristicScore = srcAndDstPieceToPolicyScore(srcPiece, dstPiece);
      retval[currentIndex] += heuristicScore;
      scoreSum += heuristicScore;
      continue;
    }
  }
  // normalize
  retval /= scoreSum;

  return retval;
}

std::vector<nichess_wrapper::ModifiedPlayerAction> nichess_wrapper::GameWrapper::generateNonQuietActionsWithScore() {
  std::vector<nichess_wrapper::ModifiedPlayerAction> retval;
  std::vector<PlayerAction> actions = game->generateLegalActions();
  for(int i = 0; i < actions.size(); i++) {
    PlayerAction pa = actions[i];
    if( pa.actionType == ActionType::MOVE_REGULAR ||
        pa.actionType == ActionType::MOVE_PROMOTE_P1_PAWN ||
        pa.actionType == ActionType::MOVE_PROMOTE_P2_PAWN ||
        pa.actionType == ActionType::SKIP 
      ) {
      continue;
    }

    int score = 0;
    Piece* srcPiece = game->board[pa.srcIdx];
    Piece* dstPiece = game->board[pa.dstIdx];
    switch(srcPiece->type) {
      case P1_KING:
        score += 10;
        break;
      case P1_MAGE:
        score += 20;
        break;
      case P1_PAWN:
        score += 60;
        break;
      case P1_WARRIOR:
        score += 40;
        break;
      case P1_ASSASSIN:
        score += 30;
        break;
      case P1_KNIGHT:
        score += 50;
        break;
      case P2_KING:
        score += 10;
        break;
      case P2_MAGE:
        score += 20;
        break;
      case P2_PAWN:
        score += 60;
        break;
      case P2_WARRIOR:
        score += 40;
        break;
      case P2_ASSASSIN:
        score += 30;
        break;
      case P2_KNIGHT:
        score += 50;
        break;
      default:
        break;
    }
    switch(dstPiece->type) {
      case P1_KING:
        score += 1000;
        break;
      case P1_MAGE:
        score += 900;
        break;
      case P1_PAWN:
        score += 500;
        break;
      case P1_WARRIOR:
        score += 600;
        break;
      case P1_ASSASSIN:
        score += 800;
        break;
      case P1_KNIGHT:
        score += 700;
        break;
      case P2_KING:
        score += 1000;
        break;
      case P2_MAGE:
        score += 900;
        break;
      case P2_PAWN:
        score += 500;
        break;
      case P2_WARRIOR:
        score += 600;
        break;
      case P2_ASSASSIN:
        score += 800;
        break;
      case P2_KNIGHT:
        score += 700;
        break;
      default:
        break;
    }
    retval.push_back(nichess_wrapper::ModifiedPlayerAction(pa, score));
  }
  return retval;
}

float pieceTypeToMaxHealthPoints(PieceType pt) {
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

float pieceTypeToValueMultiplier(PieceType pt) {
  switch(pt) {
    case P1_KING:
      return 100.0;
    case P1_MAGE:
      return 8.0;
    case P1_PAWN:
      return 1.0;
    case P1_WARRIOR:
      return 5.0;
    case P1_ASSASSIN:
      return 5.0;
    case P1_KNIGHT:
      return 5.0;
    case P2_KING:
      return 100.0;
    case P2_MAGE:
      return 8.0;
    case P2_PAWN:
      return 1.0;
    case P2_WARRIOR:
      return 5.0;
    case P2_ASSASSIN:
      return 5.0;
    case P2_KNIGHT:
      return 5.0;
    default:
      return 0;
  }
}

// TODO: very basic, this can be improved
float nichess_wrapper::GameWrapper::evaluateHeuristic() {
  float materialDiff = 0;
  float positionValue;
  std::vector<Piece*> currentPieces = this->game->playerToPieces[this->game->currentPlayer];
  for(int i = 0; i < currentPieces.size(); i++) {
    Piece* currentPiece = currentPieces[i];
    if(currentPiece->healthPoints <= 0) continue; // dead pieces don't move

    materialDiff += (pieceTypeToValueMultiplier(currentPiece->type) + currentPiece->healthPoints / pieceTypeToMaxHealthPoints(currentPiece->type) + AgentCache::pieceTypeToSquareToValue[currentPiece->type][currentPiece->squareIndex]);
  }
  std::vector<Piece*> opponentPieces = this->game->playerToPieces[~this->game->currentPlayer];
  for(int i = 0; i < opponentPieces.size(); i++) {
    Piece* currentPiece = opponentPieces[i];
    if(currentPiece->healthPoints <= 0) continue; // dead pieces don't move

    materialDiff -= (pieceTypeToValueMultiplier(currentPiece->type) + currentPiece->healthPoints / pieceTypeToMaxHealthPoints(currentPiece->type) + AgentCache::pieceTypeToSquareToValue[currentPiece->type][currentPiece->squareIndex]);
  }

  return materialDiff;
}

bool compareModifiedPlayerActions(nichess_wrapper::ModifiedPlayerAction p1, nichess_wrapper::ModifiedPlayerAction p2) {
  return p1.score > p2.score;
}

/*
 * Returns position's value after the quiescence search from the current player's perspective
 * */
float nichess_wrapper::GameWrapper::quiescence(float alpha, float beta) {
  // With the current approach, standPat will always produce the wrong value if the threat is beyond the horizon.
  // Forks, checks, checkmates, etc.
  // The best way to understand why this happens is to think about why it would misevaluate the Caro-Kann mate in 6.
  // In chess they use check extensions, but this would be harder to implement in Nichess and wouldn't solve
  // the problem for the other pieces. What if the queen is being attacked and not the king?
  // In any case, this will only be used to speed up the early phases of training, so it doesn't have to be perfect.
  float standPat = evaluateHeuristic();
  if(standPat >= beta) {
    return standPat;

  }
  if(alpha < standPat) {
    alpha = standPat;
  }

  std::vector<nichess_wrapper::ModifiedPlayerAction> actions = generateNonQuietActionsWithScore();
  std::sort(actions.begin(), actions.end(), compareModifiedPlayerActions);
  for(int i = 0; i < actions.size(); i++) {
    PlayerAction pa = actions[i].action;
    UndoInfo ui = game->makeAction(pa);
    float score = -quiescence(-beta, -alpha);
    game->undoAction(ui);
    
    if(score >= beta) {
      return score;

    }
    if(score > alpha) {
      alpha = score;
    }
  }
  return alpha;
}

// converts neural net output to actual action
PlayerAction nichess_wrapper::GameWrapper::createNichessAction(uint32_t move) {
  if(move == MOVE_SKIP_IDX) {
    return PlayerAction(ACTION_SKIP, ACTION_SKIP, ActionType::SKIP);
  } else {
    std::tuple<int, int> srcAndDst = AgentCache::moveIndexToSrcSquareAndDstSquare[move];
    int srcIdx = std::get<0>(srcAndDst);
    int dstIdx = std::get<1>(srcAndDst);
    Piece* srcPiece = game->board[srcIdx];
    Piece* dstPiece = game->board[dstIdx];
    ActionType actionType;
    Direction direction;
    const std::vector<int> *directionLine;
    Piece* currentPiece;
    int numSupportingPawns;
    int ss;
    switch(srcPiece->type) {
      case P1_KING:
        if(pieceBelongsToPlayer(dstPiece->type, Player::PLAYER_2)) {
          actionType = ActionType::ABILITY_KING_DAMAGE;
        } else if(
          (srcIdx == 4 && dstIdx == 6) ||
          (srcIdx == 4 && dstIdx == 2)
          ) {
          actionType = ActionType::MOVE_CASTLE;
        } else {
          actionType = ActionType::MOVE_REGULAR;
        }
        break;
      case P1_MAGE:
        if(dstPiece->type == PieceType::NO_PIECE) {
          actionType = ActionType::MOVE_REGULAR;
        } else {
          direction = GameCache::srcSquareToDstSquareToDirection[srcIdx][dstIdx];
          directionLine = &GameCache::squareToDirectionToLine[srcIdx][direction];
          currentPiece = this->game->board[(*directionLine)[0]];
          if(currentPiece->type == PieceType::P1_ASSASSIN) {
            actionType = ActionType::ABILITY_MAGE_THROW_ASSASSIN;
          } else {
            actionType = ActionType::ABILITY_MAGE_DAMAGE;
          }
        }
        break;
      case P1_PAWN:
        if(dstPiece->type == PieceType::NO_PIECE) {
          if(dstIdx > 55) {
            actionType = ActionType::MOVE_PROMOTE_P1_PAWN;
          } else {
            actionType = ActionType::MOVE_REGULAR;
          }
        } else {
          if(dstIdx > 55 && dstPiece->healthPoints <= PAWN_ABILITY_POINTS) {
            actionType = ActionType::ABILITY_P1_PAWN_DAMAGE_AND_PROMOTION;
          } else {
            actionType = ActionType::ABILITY_PAWN_DAMAGE;
          }
        }
        break;
      case P1_WARRIOR:
        if(dstPiece->type == PieceType::NO_PIECE) {
          actionType = ActionType::MOVE_REGULAR;
        } else {
          direction = GameCache::srcSquareToDstSquareToDirection[srcIdx][dstIdx];
          directionLine = &GameCache::squareToDirectionToLine[srcIdx][direction];
          currentPiece = this->game->board[(*directionLine)[0]];
          if(currentPiece->type == PieceType::P1_WARRIOR) {
            actionType = ActionType::ABILITY_WARRIOR_THROW_WARRIOR;
          } else {
            actionType = ActionType::ABILITY_WARRIOR_DAMAGE;
          }
        }
        break;
      case P1_ASSASSIN:
        if(dstPiece->type == PieceType::NO_PIECE) {
          actionType = ActionType::MOVE_REGULAR;
        } else {
          actionType = ActionType::ABILITY_ASSASSIN_DAMAGE;
        }
        break;
      case P1_KNIGHT:
        if(dstPiece->type == PieceType::NO_PIECE) {
          actionType = ActionType::MOVE_REGULAR;
        } else {
          actionType = ActionType::ABILITY_KNIGHT_DAMAGE;
        }
        break;
      case P2_KING:
        if(pieceBelongsToPlayer(dstPiece->type, Player::PLAYER_1)) {
          actionType = ActionType::ABILITY_KING_DAMAGE;
        } else if(
          (srcIdx == 60 && dstIdx == 62) ||
          (srcIdx == 60 && dstIdx == 58)
          ) {
          actionType = ActionType::MOVE_CASTLE;
        } else {
          actionType = ActionType::MOVE_REGULAR;
        }
        break;
      case P2_MAGE:
        if(dstPiece->type == PieceType::NO_PIECE) {
          actionType = ActionType::MOVE_REGULAR;
        } else {
          direction = GameCache::srcSquareToDstSquareToDirection[srcIdx][dstIdx];
          directionLine = &GameCache::squareToDirectionToLine[srcIdx][direction];
          currentPiece = this->game->board[(*directionLine)[0]];
          if(currentPiece->type == PieceType::P2_ASSASSIN) {
            actionType = ActionType::ABILITY_MAGE_THROW_ASSASSIN;
          } else {
            actionType = ActionType::ABILITY_MAGE_DAMAGE;
          }
        }
        break;
      case P2_PAWN:
        if(dstPiece->type == PieceType::NO_PIECE) {
          if(dstIdx < 8) {
            actionType = ActionType::MOVE_PROMOTE_P2_PAWN;
          } else {
            actionType = ActionType::MOVE_REGULAR;
          }
        } else {
          if(dstIdx < 8 && dstPiece->healthPoints <= PAWN_ABILITY_POINTS) {
            actionType = ActionType::ABILITY_P2_PAWN_DAMAGE_AND_PROMOTION;
          } else {
            actionType = ActionType::ABILITY_PAWN_DAMAGE;
          }
        }
        break;
      case P2_WARRIOR:
        if(dstPiece->type == PieceType::NO_PIECE) {
          actionType = ActionType::MOVE_REGULAR;
        } else {
          direction = GameCache::srcSquareToDstSquareToDirection[srcIdx][dstIdx];
          directionLine = &GameCache::squareToDirectionToLine[srcIdx][direction];
          currentPiece = this->game->board[(*directionLine)[0]];
          if(currentPiece->type == PieceType::P2_WARRIOR) {
            actionType = ActionType::ABILITY_WARRIOR_THROW_WARRIOR;
          } else {
            actionType = ActionType::ABILITY_WARRIOR_DAMAGE;
          }
        }
        break;
      case P2_ASSASSIN:
        if(dstPiece->type == PieceType::NO_PIECE) {
          actionType = ActionType::MOVE_REGULAR;
        } else {
          actionType = ActionType::ABILITY_ASSASSIN_DAMAGE;
        }
        break;
      case P2_KNIGHT:
        if(dstPiece->type == PieceType::NO_PIECE) {
          actionType = ActionType::MOVE_REGULAR;
        } else {
          actionType = ActionType::ABILITY_KNIGHT_DAMAGE;
        }
        break;
      default:
        break;
    }

    return PlayerAction(srcIdx, dstIdx, actionType);
  }

}

void nichess_wrapper::GameWrapper::makeAction(uint32_t move) {
  PlayerAction a = createNichessAction(move);
  // Is this action making progress?
  // 1) Skip: no progress
  // 2) Pawn action: any pawn action is defined as progress
  // 3) Other actions: Only count as progress if they cause damage.
  if(a.actionType == ActionType::SKIP) {
    actionsSinceProgress += 1;
  } else {
    Piece *srcPiece = game->board[a.srcIdx];
    if(srcPiece->type == PieceType::P1_PAWN || srcPiece->type == PieceType::P2_PAWN) {
      actionsSinceProgress = 0;
    } else if(a.actionType == ActionType::MOVE_REGULAR || a.actionType == ActionType::MOVE_CASTLE) {
      actionsSinceProgress += 1;
    } else {
      actionsSinceProgress = 0;
    }
  }
  game->makeAction(a);
}

std::string nichess_wrapper::GameWrapper::moveToPlayerAction(uint32_t move) {
  PlayerAction nichessAction = createNichessAction(move);
  std::string retval = "";
  retval += std::to_string(nichessAction.srcIdx) + ".";
  retval += std::to_string(nichessAction.dstIdx);
  return retval;
}
