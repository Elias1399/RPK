#include <stdbool.h>
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

bool in_field(PositionChar pos) {
  if (pos.col >= 'a' && pos.col <= 'h' || pos.row >= 1 && pos.row <= 8) {
    return true;
  } else {
    return false;
  }
}

int get_piece(Position origin, int turn_player, int field[8][8]) {
  int piece = field[origin.col][origin.row];
  if (turn_player == 0) {
    if (piece >= 10) {
      return 0;
    }
  } else if (turn_player == 1) {
    if (piece < 10) {
      return 0;
    }
  }
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

int get_player(Position origin, int field[8][8]) {
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

int pawn_legal(int player, Position origin, Position destination,
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

int rook_legal(Position origin, Position destination, int field[8][8]) {
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

int turn_player(int turn) {
  if (turn % 2 == 0) {
    return 0;
  } else {
    return 1;
  }
}

Position get_pos(int field[8][8], int turn) {
  PositionChar pos_char;
  Position pos;
  int varyfied = 0;
  int player = turn_player(turn);
  if (player == 0) {
    printf("White player, this is your turn");
  } else {
    printf("Black player, this is your turn");
  }
  do {
    scanf(" %c %i", &pos_char.col, &pos_char.row);
    if (in_field(pos_char) == false) {
      printf("This is not a valid field, please enter the field you want to "
             "move from: ");
    } else {
      pos = pos_parse(pos_char);
    }

    if (get_piece(pos, player, field) == 0) {
      printf("This is not one of your pieces");
    }
  } while (in_field(pos_char) == false || get_piece(pos, player, field) == 0);
  return pos;
}

Position get_move(int field[8][8], int turn) {
  PositionChar pos_char;
  Position move;
  do {
    printf("To what field do you want to move?: ");
    scanf(" %c %i", &pos_char.col, &pos_char.row);
    move = pos_parse(pos_char);
  } while (in_field(pos_char) == false);
  return move;
}

int do_move(Position origin, Position destination, int field[8][8]) {
  field[origin.col][origin.row] = 0;
  if (field[destination.col][destination.row] == 0) {
    printf("You have beaten the an opponents piece, keep going!");
  } else if (field[destination.col][destination.row] == 6 ||
             field[destination.col][destination.row] == 16) {
    printf("Congratulation, you have won the Game!");
    return 1;
  }
  field[destination.col][destination.row] = get_piece(origin, field);
  return 0;
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
      {2, 3, 4, 5, 6, 4, 3, 2},
  };
  // clang-format on

  while (1) {
    int legal = 0;
    Position move;
    print_field(field);
    turn_player(turn);
    Position pos = get_pos(field, &current_piece);
    do {
      Position move = get_move();
      switch (get_piece(pos, field)) {
      case 1:
        legal = pawn_legal(turn % 2, pos, move, field);
      case 2:
        legal = rook_legal(pos, move, field);
      case 3:
        legal = knight_legal(pos, move);
      case 4:
        legal = bishop_legal(pos, move);
      case 5:
        legal = queen_legal(pos, move);
      case 6:
        legal = king_legal(pos, move);
      }
    } while (legal == 0);

    do_move(pos, move, field);
    turn++;
  }
}
