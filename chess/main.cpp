#include <bits/stdc++.h>
#include <cinttypes>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define RESET "\033[0m"
#define BOLD "\033[1m"

using namespace std;

// Theme color codes
string BLACK_BG = "\033[48;5;17m";  // Default navy
string WHITE_BG = "\033[48;5;153m"; // Default light cyan

enum Material { PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING, NA };
enum MaterialColor { WHITE, BLACK };

class Block {
public:
  char x, y;
  Material m;
  MaterialColor c;
  Block(char x, char y, MaterialColor c, Material m) {
    this->x = x;
    this->y = y;
    this->m = m;
    this->c = c;
  }
};

class Helper {
public:
  static void makeMove(bool &turn, string &move,
                       vector<vector<Block *>> &board) {
    if (move.size() != 5 || move[2] != '-') {
      cout << "Incorrect input format. Use format like e2-e4." << endl;
      return;
    }

    int fromCol = move[0] - 'a';
    int fromRow = 8 - (move[1] - '0');
    int toCol = move[3] - 'a';
    int toRow = 8 - (move[4] - '0');

    // Validate board boundaries
    if (fromRow < 0 || fromRow >= 8 || fromCol < 0 || fromCol >= 8 ||
        toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8) {
      cout << "Invalid square. Stay within a1 to h8." << endl;
      return;
    }

    Block *from = board[fromRow][fromCol];
    Block *to = board[toRow][toCol];

    MaterialColor currentColor = turn ? WHITE : BLACK;

    // Check if there is a piece to move
    if (from->m == NA) {
      cout << "No piece at source square!" << endl;
      return;
    }

    // Check if the piece belongs to the current player
    if (from->c != currentColor) {
      cout << "You can only move your own pieces." << endl;
      return;
    }

    // Cannot capture your own piece
    if (to->m != NA && to->c == currentColor) {
      cout << "Illegal move: cannot capture your own piece." << endl;
      return;
    }

    // Perform the move
    to->m = from->m;
    to->c = from->c;
    from->m = NA;

    // Switch turn
    turn = !turn;
  }
  static void resetGame(vector<vector<Block *>> &board) {
    int n = 8;
    for (int j = 0; j < n; j++) {
      board[7][j]->m = (j == 0 || j == 7)   ? ROOK
                       : (j == 1 || j == 6) ? KNIGHT
                       : (j == 2 || j == 5) ? BISHOP
                       : (j == 3)           ? QUEEN
                                            : KING;
      board[7][j]->c = WHITE;
      board[6][j]->m = PAWN;
      board[6][j]->c = WHITE;
      board[0][j]->m = (j == 0 || j == 7)   ? ROOK
                       : (j == 1 || j == 6) ? KNIGHT
                       : (j == 2 || j == 5) ? BISHOP
                       : (j == 3)           ? QUEEN
                                            : KING;
      board[0][j]->c = BLACK;
      board[1][j]->m = PAWN;
      board[1][j]->c = BLACK;
    }
  }

  static string getUniCodes(MaterialColor c, Material m) {
    switch (m) {
    case KING:
      return c == WHITE ? "\u2654" : "\u265A";
    case QUEEN:
      return c == WHITE ? "\u2655" : "\u265B";
    case ROOK:
      return c == WHITE ? "\u2656" : "\u265C";
    case BISHOP:
      return c == WHITE ? "\u2657" : "\u265D";
    case KNIGHT:
      return c == WHITE ? "\u2658" : "\u265E";
    case PAWN:
      return c == WHITE ? "\u2659" : "\u265F";
    default:
      return " ";
    }
  }

  static void chooseTheme(int theme) {
    switch (theme) {
    case 1:                        // Classic Green
      BLACK_BG = "\033[48;5;22m";  // Forest green
      WHITE_BG = "\033[48;5;230m"; // Beige
      break;
    case 2:                        // Elegant Blue
      BLACK_BG = "\033[48;5;17m";  // Navy
      WHITE_BG = "\033[48;5;153m"; // Light cyan
      break;
    case 3:                        // Wooden Style
      BLACK_BG = "\033[48;5;94m";  // Brown
      WHITE_BG = "\033[48;5;230m"; // Ivory
      break;
    default:
      cout << "Invalid theme. Using default (Blue)." << endl;
    }
  }
};

void clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

int main() {
  int themeChoice = 3;
  // cout << "Select a board theme:\n";
  // cout << "1. Classic Green\n2. Elegant Blue\n3. Wooden Style\n";
  // cout << "Enter theme number: ";
  // cin >> themeChoice;
  Helper::chooseTheme(themeChoice);

  int n = 8;
  vector<vector<Block *>> board(n, vector<Block *>(8));
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      board[i][j] = new Block(i, j, WHITE, NA);

  Helper::resetGame(board);
  bool gameOver = false;
  bool turn = 1;
  while (!gameOver) {
    cout << "    White's Perspective      Black's Perspective" << endl;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        Block *cell = board[i][j];
        string unicode = Helper::getUniCodes(cell->c, cell->m);
        bool white_square = (i + j) % 2 == 0;
        string bg = white_square ? WHITE_BG : BLACK_BG;
        cout << bg << BOLD << " " << unicode << " " << RESET;
      }
      cout << "  ";
      for (int j = 0; j < n; j++) {
        Block *cell = board[i][j];
        string unicode = Helper::getUniCodes(cell->c, cell->m);
        bool white_square = (i + j) % 2 == 0;
        string bg = white_square ? WHITE_BG : BLACK_BG;
        cout << bg << BOLD << " " << unicode << " " << RESET;
      }
      cout << endl;
    }
    string move;
    if (turn) {
      cout << "White's turn:";
    } else {
      cout << "Black's turn:";
    }
    cin >> move;
    if (move == "resign") {
      if (!turn) {
        cout << "White won by opponent resignation.";
      } else {
        cout << "Black won by opponent resignation.";
      }
      gameOver = true;
      break;
    }
    while (move == "draw") {
      cout << "Draw offered: type draw to accept else any key to reject"
           << endl;
      if (!turn) {
        cout << "White's turn:";
      } else {
        cout << "Black's turn:";
      }
      cin >> move;
      if (move == "draw") {
        cout << "Game Over with a Draw." << endl;
        gameOver = true;
        break;
      } else {
        cout << "Draw offer rejected." << endl;
        if (turn) {
          cout << "White's turn:";
        } else {
          cout << "Black's turn:";
        }
        cin >> move;
      }
    }
    if (!gameOver)
      clearScreen();
    Helper::makeMove(turn, move, board);
  }

  return 0;
}
