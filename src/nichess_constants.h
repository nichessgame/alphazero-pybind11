#pragma once

namespace alphazero::nichess_gs {

constexpr const int WIDTH = 8;
constexpr const int HEIGHT = 8;
constexpr const int NUM_MAX_POSSIBLE_MOVES_FOR_PIECE = 7*8 + 8; // 7 squares x 8 directions + 8 knight moves
constexpr const int NUM_MOVES = WIDTH * HEIGHT * NUM_MAX_POSSIBLE_MOVES_FOR_PIECE + 1; // +1 for MOVE_SKIP
constexpr const int MOVE_SKIP_IDX = NUM_MOVES - 1;
constexpr const int NUM_PLAYERS = 2;
constexpr const int NUM_SYMMETRIES = 2;
constexpr const std::array<int, 3> BOARD_SHAPE = {2, HEIGHT, WIDTH};
// 12 piece types (exluding NO_PIECE, 6 for P1 and 6 for P2)
// 2 layers per type, first indicates whether a piece is alive and second the % of max health points.
// 2 layers indicating whether it's P1 or P2 turn.
constexpr const std::array<int, 3> CANONICAL_SHAPE = {12 * 2 + 2, HEIGHT, WIDTH};

} // namespace alphazero::nichess_gs
