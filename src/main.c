#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  pos_parsed.col = char_parsed;
  pos_parsed.row = pos_char.row - 1;
#ifdef DEBUG
  printf("parsing: %c, %i --> %i, %i\n", pos_char.col, pos_char.row,
         pos_parsed.col, pos_parsed.row);
#endif
  return pos_parsed;
}

void print_field(int field[8][8]) {
  printf("\u250C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
         "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
         "\u2500\u2500\u2510\n");
  for (int i = 7; i >= 0; i--) {
    printf("\u2502");
    for (int j = 0; j <= 7; j++) {
      printf("%i", field[i][j]);
      if (field[i][j] < 10) {
        printf(" ");
      }
      printf("\u2502");
    };
    if (i != 0) {
      printf(
          "\n\u2502\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
          "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
          "\u2500\u2500\u2502\n");
    }
  };
  printf("\n\u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
         "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
         "\u2500\u2500\u2518\n");
}

bool in_field(PositionChar pos) {
  if ((pos.col >= 'a' && pos.col <= 'h') || (pos.row >= 1 && pos.row <= 8)) {
    return true;
  } else {
    return false;
  }
}

int get_piece(Position origin, int turn_player, int field[8][8]) {
  int piece = field[origin.row][origin.col];
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
  int player = field[origin.row][origin.col];
  if (player < 10) {
    return 0;
  } else {
    return 1;
  }
}

bool own_piece(int player, Position destination, int field[8][8]) {
  if (field[destination.row][destination.col] == 0) {
    return false;
  }
  if (player == 0) {
    if (field[destination.row][destination.col] < 10) {
      return true;
    } else {
      return false;
    }
  } else {
    if (field[destination.row][destination.col] > 10) {
      return true;
    } else {
      return false;
    }
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
    if (origin.col == destination.col && origin.row == destination.row - 1 &&
        detect_destination(destination, field) == 0) {
      return 1;
    } else if (origin.col == destination.col &&
               origin.row == destination.row - 2 && origin.row == 1 &&
               detect_destination(destination, field) == 0) {
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
    if (origin.col == destination.col && origin.row == destination.row + 1 &&
        detect_destination(destination, field) == 0) {
      return 1;
    } else if (origin.col == destination.col &&
               origin.row == destination.row - 2 && origin.row == 6 &&
               detect_destination(destination, field) == 0) {
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
  Position counter = origin;
  if ((origin.col == destination.col || origin.row == destination.row) &&
      (origin.col != destination.col || origin.row != destination.row)) {
    if (origin.col != destination.col) {
      for (int i = 1; i < abs(origin.col - destination.col); i++) {
        if (origin.col - destination.col > 0) {
          counter.col++;
          if (detect_destination(counter, field) != 0) {
            return 0;
          }
        }
        if (origin.col - destination.col > 0) {
          counter.col--;
          if (detect_destination(counter, field) != 0) {
            return 0;
          }
        }
      }
    }
    if (origin.row != destination.row) {
      for (int i = 1; i < abs(origin.row - destination.row); i++) {
        if (origin.row - destination.row > 0) {
          counter.row++;
          if (detect_destination(counter, field) != 0) {
            return 0;
          }
        }
        if (origin.row - destination.row > 0) {
          counter.row--;
          if (detect_destination(counter, field) != 0) {
            return 0;
          }
        }
      }
    }
    return 1;
  } else {
    return 0;
  }
}

void castle(int player, Position origin, Position destination,
            int castle_poss[2], int field[8][8]) {
  if (player == 0) {
    if (origin.row == 0 && (origin.col == 0 || origin.col == 7) &&
        castle_poss[0] == 1) {
      if (origin.col == 0 && destination.col == 3 && field[0][3] == 0) {
        field[0][2] = field[0][4];
      } else if (origin.col == 7 && destination.col == 5 && field[0][5] == 0) {
        field[0][6] = field[0][4];
      }
      field[0][4] = 0;
      castle_poss[0] = 0;
    }
  }
  if (player == 1) {
    if (origin.row == 1 && (origin.col == 0 || origin.col == 7) &&
        castle_poss[1] == 1) {
      if (origin.col == 0 && destination.col == 3 && field[7][3] == 0) {
        field[7][2] = field[7][4];
      } else if (origin.col == 7 && destination.col == 5 && field[7][5] == 0) {
        field[7][6] = field[7][5];
      }
      field[7][5] = 0;
      castle_poss[1] = 0;
    }
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

int bishop_legal(Position origin, Position destination, int field[8][8]) {
  Position counter = origin;
  if (abs(origin.col - destination.col) != abs(origin.row - destination.row)) {
    return 0;
  }
  if (origin.col < destination.col && origin.row < destination.row) {
    for (int i = 1; i < abs(origin.col - destination.col); i++) {
      counter.col++;
      counter.row++;
      if (detect_destination(counter, field) != 0) {
        return 0;
      }
    }
  } else if (origin.col < destination.col && origin.row > destination.row) {
    for (int i = 1; i < abs(origin.col - destination.col); i++) {
      counter.col++;
      counter.row--;
      if (detect_destination(counter, field) != 0) {
        return 0;
      }
    }
  } else if (origin.col > destination.col && origin.row < destination.row) {
    for (int i = 1; i < abs(origin.col - destination.col); i++) {
      counter.col--;
      counter.row++;
      if (detect_destination(counter, field) != 0) {
        return 0;
      }
    }
  } else if (origin.col > destination.col && origin.row > destination.row) {
    for (int i = 1; i < abs(origin.col - destination.col); i++) {
      counter.col--;
      counter.row--;
      if (detect_destination(counter, field) != 0) {
        return 0;
      }
    }
  }
  return 1;
}

int queen_legal(Position origin, Position destination, int field[8][8]) {
  Position counter = origin;
  if (((origin.col == destination.col || origin.row == destination.row) &&
       (origin.col != destination.col || origin.row != destination.row)) ||
      (abs(origin.col - destination.col) ==
       abs(origin.row - destination.row))) {
    if ((origin.col != destination.col) && (origin.row == destination.row)) {
      for (int i = 1; i < abs(origin.col - destination.col); i++) {
        if (origin.col - destination.col > 0) {
          counter.col++;
          if (detect_destination(counter, field) != 0) {
            return 0;
          }
        }
        if (origin.col - destination.col > 0) {
          counter.col--;
          if (detect_destination(counter, field) != 0) {
            return 0;
          }
        }
      }
    }
    if ((origin.row != destination.row) && (origin.col == destination.col)) {
      for (int i = 1; i < abs(origin.row - destination.row); i++) {
        if (origin.row - destination.row > 0) {
          counter.row++;
          if (detect_destination(counter, field) != 0) {
            return 0;
          }
        }
        if (origin.row - destination.row > 0) {
          counter.row--;
          if (detect_destination(counter, field) != 0) {
            return 0;
          }
        }
      }
    }
    if (origin.col < destination.col && origin.row < destination.row) {
      for (int i = 1; i < abs(origin.col - destination.col); i++) {
        counter.col++;
        counter.row++;
        if (detect_destination(counter, field) != 0) {
          return 0;
        }
      }
    } else if (origin.col < destination.col && origin.row > destination.row) {
      for (int i = 1; i < abs(origin.col - destination.col); i++) {
        counter.col++;
        counter.row--;
        if (detect_destination(counter, field) != 0) {
          return 0;
        }
      }
    } else if (origin.col > destination.col && origin.row < destination.row) {
      for (int i = 1; i < abs(origin.col - destination.col); i++) {
        counter.col--;
        counter.row++;
        if (detect_destination(counter, field) != 0) {
          return 0;
        }
      }
    } else if (origin.col > destination.col && origin.row > destination.row) {
      for (int i = 1; i < abs(origin.col - destination.col); i++) {
        counter.col--;
        counter.row--;
        if (detect_destination(counter, field) != 0) {
          return 0;
        }
      }
    }
    return 1;
  } else {
    return 0;
  }
}

int knight_legal(Position origin, Position destination, int field[8][8]) {
  if ((abs(origin.col - destination.col) == 1 &&
       abs(origin.row - destination.row) == 2) ||
      (abs(origin.col - destination.col) == 2 &&
       abs(origin.row - destination.row) == 1)) {
    return 1;
  } else {
    return 0;
  }
}

bool passant(int player, Position origin, Position destination,
             int en_p[2][8]) {
  if (player == 0) {
    if (origin.row == 1 && destination.row == 3) {
      en_p[0][origin.col] = 1;
    } else if (origin.row == 3 && en_p[0][origin.col] == 1) {
      en_p[0][origin.col] = 0;
    }
    if (origin.row == 4 && destination.col == abs(origin.col - 1)) {
      if (en_p[1][destination.col] == 2) {
        return true;
      } else {
        return false;
      }
    }
  }
  if (player == 1) {
    if (origin.row == 6 && destination.row == 4) {
      en_p[1][origin.col] = 1;
    } else if (origin.row == 4 && en_p[1][origin.col] == 1) {
      en_p[0][origin.col] = 0;
    }
    if (origin.row == 5 && destination.col == abs(origin.col - 1)) {
      if (en_p[1][destination.col] == 2) {
        return true;
      } else {
        return false;
      }
    }
  }
  return true;
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
    printf("White player, this is your turn: ");
  } else {
    printf("Black player, this is your turn: ");
  }
  do {
    scanf(" %c %i", &pos_char.col, &pos_char.row);
    if (in_field(pos_char) == false) {
      printf("This is not a valid field, please enter the field you want to "
             "move from: ");
    } else {
      pos = pos_parse(pos_char);
      if (get_piece(pos, player, field) == 0) {
        printf("This is not one of your pieces! Enter a different one:  \n");
      } else {
        varyfied = 1;
      }
    }
  } while (varyfied == 0);
  return pos;
}

Position get_move(int field[8][8], int turn) {
  PositionChar pos_char;
  Position move;
  do {
    printf("To what field do you want to move?: ");
    scanf(" %c %i", &pos_char.col, &pos_char.row);
  } while (in_field(pos_char) == false);
  move = pos_parse(pos_char);
  return move;
}

void do_move(Position origin, Position destination, int turn_player,
             int field[8][8]) {
  int piece = field[origin.row][origin.col];
  field[origin.row][origin.col] = 0;
  if (turn_player == 0) {
    if (field[destination.row][destination.col] == 16) {
      printf("Congratulation white player, you have won the Game!\n");
      exit(0);
    } else if (field[destination.row][destination.col] > 10) {
      printf("You have beaten the an opponents piece, keep going!\n");
    } else {
      printf("You moved your piece to a free field\n");
    }
    field[destination.row][destination.col] = piece;
  }
  if (turn_player == 1) {
    if (field[destination.row][destination.col] == 6) {
      printf("Congratulation black player, you have won the Game!\n");
      exit(0);
    } else if (field[destination.row][destination.col] < 10) {
      printf("You have beaten the an opponents piece, keep going!\n");
    } else {
      printf("You moved your piece to a free field\n");
    }
    field[destination.row][destination.col] = piece;
  }
}

int main() {
  int turn = 0;
  int current_piece;
  PositionChar pos_char;

  // clang-format off
  int field[8][8] = {
      [7] = {12, 13, 14, 15, 16, 14, 13, 12}, 
      [6] = {11, 11, 11, 11, 11, 11, 11, 11},
      [5] = {0, 0, 0, 0, 0, 0, 0, 0},         
      [4] = {0, 0, 0, 0, 0, 0, 0, 0},
      [3] = {0, 0, 0, 0, 0, 0, 0, 0},         
      [2] = {0, 0, 0, 0, 0, 0, 0, 0},
      [1] = {1, 1, 1, 1, 1, 1, 1, 1},         
      [0] = {2, 3, 4, 5, 6, 4, 3, 2},
  };
  // clang-format on

  int en_passant[2][8];
  memset(en_passant, 0, sizeof(en_passant));

  int castle_poss[2];
  memset(castle_poss, 1, sizeof(castle_poss));

  while (1) {
    int legal = 0;
    Position move;
    Position pos;
    Position *p = &pos;
    print_field(field);
    int turn_p = turn_player(turn);
    do {
      *p = get_pos(field, turn);
      Position move = get_move(field, turn_p);
      switch (get_piece(pos, turn_p, field)) {
      case 1:
        legal = pawn_legal(turn_p, pos, move, field);
        if (passant(turn_p, pos, move, en_passant)) {
          legal = 0;
        }
        break;
      case 2:
        legal = rook_legal(pos, move, field);
        if (legal == 1) {
          castle(turn_p, pos, move, castle_poss, field);
          if (turn_p == 0) {
            castle_poss[0] = 0;
          } else if (turn_p == 1) {
            castle_poss[1] = 0;
          }
        }
        break;
      case 3:
        legal = knight_legal(pos, move, field);
        break;
      case 4:
        legal = bishop_legal(pos, move, field);
        break;
      case 5:
        legal = queen_legal(pos, move, field);
        break;
      case 6:
        legal = king_legal(pos, move);
        if (legal == 1) {
          if (turn_p == 0) {
            castle_poss[0] = 0;
          } else if (turn_p == 1) {
            castle_poss[1] = 0;
          }
        }
        break;
      }
      if (own_piece(turn_p, move, field) == true) {
        legal = 0;
      }
      if (legal == 0) {
        printf("This is not a legal move, please input a different move: \n");
      }
    } while (legal == 0);

    do_move(pos, move, turn_p, field);
    turn++;
  }
}
