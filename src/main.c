#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int col;
  int row;
} Position;

int print_field(int field[8][8]) {
  for (int i = 0; i <= 7; i++) {
    printf("\n");
    for (int j = 0; j <= 7; j++) {
      printf("%i", field[i][j]);
    };
  };
  return 0;
}

int detect_piece(Position origin, int field[8][8]) {
  int piece = field[origin.col][origin.row];
  if (piece % 10 == 1) {
    return 1;
  } else if (piece % 10 == 2) {
    return 2;
  } else if (piece % 10 == 3) {
    return 3;
  } else if (piece % 10 == 4) {
    return 4;
  } else if (piece % 10 == 5) {
    return 5;
  } else if (piece % 10 == 6) {
    return 6;
  }
  return 0;
}

int detect_player(Position origin, int field[8][8]) {
  int player = field[origin.col][origin.row];
  if (player < 10) {
    return 0;
  } else {
    return 1;
  }
}

int detect_destination(Position destination, int field[8][8]) {
  if (field[destination.row][destination.col] == 0) {
    // field is empty
    return 0;
  } else if (field[destination.row][destination.col] % 10 == 6) {
    // king on field, game ends
    return 1;
  } else {
    // any other piece gets beaten
    return 2;
  }
}

int pawn_legal(int player, int piece, Position origin, Position destination,
               int field[8][8]) {
  if (player == 0) {
    if (origin.col == destination.col && origin.row == destination.row - 1) {
      return 1;
    } else if (abs(origin.col - destination.col) == 1 &&
               origin.row == destination.row - 1) {
      if (detect_destination(destination, field) == 2) {
        return 1;
      } else {
        return 0;
      }
    }
  } else if (player == 1) {
    if (origin.col == destination.col && origin.row == destination.row + 1) {
      return 1;
    } else if (abs(origin.col - destination.col) == 1 &&
               origin.row == destination.row + 1) {
      if (detect_destination(destination, field) == 2) {
        return 1;
      } else {
        return 0;
      }
    }
  }
  // error
  return 2;
}

int rook_legal(int player, int piece, Position origin, Position destination,
               int field[8][8]) {
  if ((origin.col == destination.col || origin.row == destination.row) &&
      (origin.col != destination.col || origin.row != destination.row)) {
    return 1;
  } else {
    return 0;
  }
}

int player_move(int player, Position origin, Position destination) { return 0; }

int main(void) {
  // clang-format off
  int field[8][8] = {
      {12, 13, 14, 15, 16, 14, 13, 12}, 
      {11, 11, 11, 11, 11, 11, 11, 11},
      {0, 0, 0, 0, 0, 0, 0, 0},         
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},         
      {0, 0, 0, 0, 0, 0, 0, 0},
      {1, 1, 1, 1, 1, 1, 1, 1},         
      {2, 3, 4, 6, 5, 4, 3, 2},
  };
  // clang-format on

  printf("White player, make your first move");
  print_field(field);
}
