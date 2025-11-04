#include "nichess_wrapper.h"
#include "nichess/nichess.hpp"
#include "nichess/util.hpp"
#include "nichess_constants.h"
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
  int currentIndex;
  bool foundLegalMove = false;
  std::vector<Piece*> currentPlayerPieces = game->playerToPieces[game->currentPlayer];
  for(int i = 0; i < currentPlayerPieces.size(); i++) {
    Piece* currentPiece = currentPlayerPieces[i];
    if(currentPiece->healthPoints <= 0) continue; // dead pieces don't move
    std::vector<PlayerAction> legalActions = game->legalActionsByPiece(currentPiece);
    for(int j = 0; j < legalActions.size(); j++) {
      PlayerAction action = legalActions[j];
      currentIndex = AgentCache::srcSquareToDstSquareToMoveIndex[action.srcIdx][action.dstIdx];
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
