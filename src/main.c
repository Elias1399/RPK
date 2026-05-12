#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int col;
  int row;
} Position;

typedef struct {
  char col;
  int row;
} PositionChar;

Position pos_parse(PositionChar pos_char) {
  Position pos_parsed;
  int char_parsed = pos_char.col - 'a';
  pos_parsed.row = pos_char.row - 1;
  return pos_parsed;
}

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

int rook_legal(int piece, Position origin, Position destination,
               int field[8][8]) {
  if ((origin.col == destination.col || origin.row == destination.row) &&
      (origin.col != destination.col || origin.row != destination.row)) {
    return 1;
  } else {
    return 0;
  }
}

int king_legal(Position origin, Position destination) {
  if ((abs(origin.col - destination.col) == 1 &&
       abs(origin.row - destination.row) <= 1) ||
      ((abs(origin.row - destination.row) == 1 &&
        abs(origin.col - destination.col) <= 1))) {
    return 1;
  } else {
    return 0;
  }
}

int bishop_legal(Position origin, Position destination) {
  if (abs(origin.col - destination.col) == abs(origin.row - destination.row)) {
    return 1;
  } else {
    return 0;
  }
}

int queen_legal(Position origin, Position destination) {
  if (((origin.col == destination.col || origin.row == destination.row) &&
       (origin.col != destination.col || origin.row != destination.row)) ||
      (abs(origin.col - destination.col) ==
       abs(origin.row - destination.row))) {
    return 1;
  } else {
    return 0;
  }
}

int knight_legal(Position origin, Position destination) {
  if ((abs(origin.col - destination.col) == 1 &&
       abs(origin.row - destination.row) == 2) ||
      (abs(origin.col - destination.col) == 2 &&
       abs(origin.row - destination.row) == 1)) {
    return 1;
  } else {
    return 0;
  }
}

void turn_player(int turn) {
  if (turn % 2 == 0) {
    printf("White player, its your turn!\nMake you'r move: ");
  } else {
    printf("Black player, its your turn!\nMake you'r move: ");
  }
}

Position get_pos(int field[8][8], int *current_piece) {
  PositionChar pos_char;
  scanf(" %c %i", &pos_char.col, &pos_char.row);
  Position pos = pos_parse(pos_char);
  if (detect_player(pos, field) == 1) {
    *current_piece = detect_piece(pos, field);
    return pos;
  } else {
    while (detect_player(pos, field) == 0) {
      printf(
          "This is not a valid piece, please input the piece location again: ");
      scanf(" %c %i", &pos_char.col, &pos_char.row);
    }
    *current_piece = detect_piece(pos, field);
    return pos;
  }
}

Position get_move() {
  PositionChar pos_char;
  printf("To what field do you want to move?: ");
  scanf(" %c %i", &pos_char.col, &pos_char.row);

  Position move = pos_parse(pos_char);
  return move;
}

int main(void) {
  int turn = 0;
  int current_piece;
  PositionChar pos_char;

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

  while (1) {
    print_field(field);
    turn_player(turn);
    Position pos = get_pos(field, &current_piece);
    Position move = get_move();

    turn++;
  }
}
