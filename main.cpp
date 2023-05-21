#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cctype>
#include <array>

using namespace std;

/*                    функции 
--------------------TIC-TAC-TOE-----------------------
*/

int player = 1; // текущий игрок
char mark; // знак текущего игрока
char board[3][3] = { {'-', '-', '-'}, {'-', '-', '-'}, {'-', '-', '-'}};  //игровое поле

// функция отрисовки игрового поля
void drawBoard() {
  cout << "  1 2 3" << endl;
  for (int i = 0; i < 3; i++) {
    cout << i+1 << " ";
    for (int j = 0; j < 3; j++) {
      cout << board[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

// функция, проверяющая ход на правильность
bool isValidMove(int row, int col) {
  if (row < 0 || row > 2 || col < 0 || col > 2) {
    return false;
  }
  if (board[row][col] != '-') {
    return false;
  }
  return true;
}

// функция, заполняющая клетку игрового поля и переключающая игрока
void makeMove(int row, int col) {
  board[row][col] = mark;
  if (player == 1) {
    player = 2;
    mark = 'O';
  } else {
      player = 1;
      mark = 'X';
    }
}

// функция, проверяющая, есть ли победитель
int checkWinner() {
  // проверка по горизонтали
  for (int i = 0; i < 3; i++) {
    if (board[i][0] != '-' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
      if (board[i][0] == 'X') {
        return 1;
      } else {
          return 2;
        }
      }
  }
  // проверка по вертикали
  for (int i = 0; i < 3; i++) {
    if (board[0][i] != '-' && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
      if (board[0][i] == 'X') {
        return 1;
      } else {
          return 2;
        }
      }
  }
  // проверка по диагоналям
  if (board[0][0] != '-' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
    if (board[0][0] == 'X') {
      return 1;
    } else {
        return 2;
      }
  }
  if (board[0][2] != '-' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
    if (board[0][2] == 'X') {
      return 1;
    } else {
        return 2;
      }
  }
  // проверка на ничью
  bool isDraw = true;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == '-') {
        isDraw = false;
      }
    }
  }
  if (isDraw) {
    return 0;
  }
  // если никто не победил и нет ничьей, возвращаем 3
  return 3;
}

// функция, возвращающая значение минимального элемента
int min(int a, int b) {
  if (a < b) {
    return a;
  } else {
    return b;
  }
}

// функция, возвращающая значение максимального элемента
int max(int a, int b) {
  if (a > b) {
    return a;
  } else {
      return b;
    }
}

// функция, имитирующая ход ИИ
int minimax(char board[3][3], int depth, bool isMax) {
  int score = checkWinner();
  if (score == 1) {
    return -10;
  }
  if (score == 2) {
    return 10;
  }
  if (score == 0) {
    return 0;
  }

  if (isMax) {
    int best = -1000;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (board[i][j] == '-') {
          board[i][j] = 'O';
          best = max(best, minimax(board, depth+1, !isMax));
          board[i][j] = '-';
        }
      }
    }
    return best;
  } else {
      int best = 1000;
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          if (board[i][j] == '-') {
            board[i][j] = 'X';
            best = min(best, minimax(board, depth+1, !isMax));
            board[i][j] = '-';
          }
        }
      }
      return best;
    }
}

// функция, находящая оптимальный ход для ИИ
void findBestMove(char board[3][3]) {
  int bestval = -1000;
  int row, col;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == '-') {
        board[i][j] = 'O';
        int moveval = minimax(board, 0, false);
        board[i][j] = '-';
        if (moveval > bestval) {
          row = i;
          col = j;
          bestval = moveval;
        }
      }
    }
  }
  makeMove(row, col);
}

/*
--------НАЙДИ-ПАРУ--------
*/
const int ROWS = 10;
const int COLUMNS = 10;
int NUMBER_OF_MINES = 15;

// struct для ячейки поля
struct Cell {
  bool mine = false; // false: клетка без мины, true: клетка с миной
  bool opened = false; // false: клетка не открыта, true: клетка открыта 
  bool flagged = false; // false: клетка не помечена флажком, true: клетка помечена флажком 
  int number_of_adjacent_mines = 0; // число мин в соседней клетке
};

// вывод поля на экран
void display_board(const array<array<Cell,COLUMNS>,ROWS>& board) {
  cout << "   ";
  for (int i = 0; i < COLUMNS; i++) {
    cout << i << " ";
  }
  cout << endl;
  for (int i = 0; i < ROWS; i++) {
    cout << i << "  ";
    for (int j = 0; j < COLUMNS; j++) {
      if (!board[i][j].opened) { // поле не открыто: вывести прочерк
        cout << "- ";
      } else if (board[i][j].mine) { // поле с миной: вывести *
          cout << "* ";
        } else if (board[i][j].number_of_adjacent_mines == 0) { // поле с нулевым числом мин в соседней клетке: вывести пустой пробел
            cout << "  ";
          } else { // поле с числом мин: вывести число
              cout << board[i][j].number_of_adjacent_mines << " ";
            }
    }
    cout << endl;
  }
}

// проверка, можно ли поставить флажок
bool flag_cell(array<array<Cell,COLUMNS>,ROWS>& board, int row, int column) {
  if (board[row][column].opened) {
    cout << "Нельзя поставить флажок на открытую клетку." << endl;
    return false;
  }
  board[row][column].flagged = !board[row][column].flagged; // поменять значение: false на true, true на false
  return true;
}

// открытие клетки
void open_cell(array<array<Cell,COLUMNS>,ROWS>& board, int row, int column) {
  if (board[row][column].mine) { // если открыли клетку с миной
    board[row][column].opened = true;
    return;
  }
  if (board[row][column].opened) { // если клетка уже открыта
    cout << "Клетка уже открыта." << endl;
    return;
  }
  board[row][column].opened = true;
  if (board[row][column].number_of_adjacent_mines == 0) { // если рядом с клеткой нет мин
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        int new_row = row + i;
        int new_column = column + j;
        if (new_row >= 0 && new_row < ROWS && new_column >= 0 && new_column < COLUMNS && !board[new_row][new_column].opened) {
          open_cell(board, new_row, new_column);
        }
      }
    }
  }
}

// проверка на победу
bool check_win(const array<array<Cell,COLUMNS>,ROWS>& board) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      if (!board[i][j].opened && !board[i][j].mine) { // остались закрытые клетки без мин
        return false;
      }
    }
  }
  return true;
}



int main() {
  int a;
  while (true) {
    cout << "Привет! Это сборник из 4 игр в консоли на с++!!\n\nВыбери игру:\n1. Крестики-нолики\n2. Угадай число\n3. Виселица (на английском)\n4. Минёр\n\nВыбор: ";
    int a; cin >> a;

    if(a == 1){   //tic-tac-toe
      while (true) {
        player = 1; //обнуление значений
        mark = ' '; 
    
        for (int i = 0; i < 3; ++i) {
          for (int j = 0; j < 3; ++j) {
            board[i][j] = '-';
          }
        }
    
        system("clear");
        // инициализация генератора случайных чисел
        srand(time(0));
        // рандомно выбираем, кто первый ходит
        player = rand() % 2 + 1;
        if (player == 1) {
          mark = 'X';
        } else {
            mark = 'O';
            findBestMove(board); // первый ход ИИ
          }
        // цикл игры
        while (true) {
          drawBoard();
          //cout << "Ход игрока №" << player << " (" << mark << ")" << endl;
          int row, col;
          if (player == 1) {
            // ходит игрок
            cout << "Введи число по горизонтали (1-3): ";
            cin >> row;
            cout << "Введи число по вертикали (1-3): ";
            cin >> col;
            row--;
            col--;
            if (isValidMove(row, col)) {
              makeMove(row, col);
            } else {
                cout << "Неверный ход. Попробуй ещё раз." << endl;
                continue;
              }
            } else {
                // ходит ИИ
                findBestMove(board);
              }
            system("clear");
            int winner = checkWinner();
            if (winner == 1) {
              drawBoard();
              cout << "Игрок 1 (X) выиграл!" << endl;
              break;
            } else if (winner == 2) {
                drawBoard();
                cout << "Игрок 2 (O) выиграл!" << endl;
                break;
              } else if (winner == 0) {
                  drawBoard();
                  cout << "Ничья!" << endl;
                  break;
                }
        }
        cout << "\n\n\nНачать снова\n 1. Да\n 2. Нет\n\nВыбор: ";
        a = 0; cin >> a;
        if(a == 2){
          system("clear");
          break;
        }
      }
    }   //крестики нолики
    else if (a == 2) {
      int secretNumber = rand() % 100 + 1; // Генерация случайного числа от 1 до 100
      int guess;
      int tries = 0;
      
      while (true) {
        secretNumber = rand() % 100 + 1; // Генерация случайного числа от 1 до 100
        guess = 0;
        tries = 0;
        system("clear");
        srand(time(NULL)); // Инициализация генератора случайных чисел

        while (guess != secretNumber) {
          cout << "Введите число от 1 до 100: ";
          cin >> guess;
          tries++;

          if (guess > secretNumber) {
            cout << "Меньше" << endl;
          } else if (guess < secretNumber) {
            cout << "Больше" << endl;
            }
        } 

        system("clear");
      
        cout << endl << "Вы угадали число за " << tries << " попыток!" << endl;

        cout << "\n\n\nНачать снова\n 1. Да\n 2. Нет\n\nВыбор: ";
        a = 0; cin >> a;
        if(a == 2){
          system("clear");
          break;
        }        
      }
    }   //угадай число
    else if (a == 3) {
      system("clear");
      const int MAX_WRONG = 8;  // Максимальное количество ошибок, которое можно совершить
      vector<string> words;  // Коллекция слов для игры

      ifstream file("words.txt");
      string word;
      while (file >> word) {
        words.push_back(word);
      }
      file.close();
      while (true){
        system("clear");
        srand(static_cast<unsigned int>(time(0)));
        random_shuffle(words.begin(), words.end()); // Случайный выбор слова из коллекции

        string THE_WORD = words[0];
        int wrong = 0; // Количество совершенных ошибок
        string soFar(THE_WORD.size(), '-');

        string used = ""; // Использованные буквы

        cout << "Добро пожаловать в 'ВИСЕЛИЦУ', удачи!\n";

        // Основной цикл игры
        while ((wrong < MAX_WRONG) && (soFar != THE_WORD))
        {
          cout << "\n\nУ тебя есть " << (MAX_WRONG - wrong);
          cout << " попыток.\n";
          cout << "\nВы использовали следующие буквы:\n" << used << endl;
          cout << "\nПока что слово выглядит так:\n" << soFar << endl;

          char guess;
          cout << "\nВведи свое предположение: ";
          cin >> guess;
          guess = toupper(guess); // Преобразование введённой буквы в верхний регистр

          while (used.find(guess) != string::npos) // Проверка, не использовалась ли данная буква ранее
          {
            cout << "\nВы уже догадались " << guess << endl;
            cout << "Введите свое предположение: ";
            cin >> guess;
            guess = toupper(guess);
          }

          used += guess;
          system("clear");
          if (THE_WORD.find(guess) != string::npos) // Проверка, находится ли введённая буква в слове
          {
            cout << "Верно! " << guess << " есть в этом слове.\n";

            // Обновление строки с открытыми буквами
            for (unsigned int i = 0; i < THE_WORD.length(); ++i)
            {
              if (THE_WORD[i] == guess)
              {
                soFar[i] = guess;
              }
            }
          } else
            {
              cout << "Прости, " << guess << " нет в слове.\n";
              ++wrong;
            }
        }

        // Завершение игры
        system("clear");
        if (wrong == MAX_WRONG) cout << "\nТебя повесили!";
        else cout << "\nВы угадали!";

        cout << "\nЗагаданным словом было: " << THE_WORD << endl;

        cout << "\n\n\nНачать снова\n 1. Да\n 2. Нет\n\nВыбор: ";
        a = 0; cin >> a;
        if(a == 2){
          system("clear");
          break;
        } 
      }
    }   //виселица
    else if (a == 4) {
      while (true) {
        int seed = time(0);
        srand(seed);
      
        array<array<Cell,COLUMNS>,ROWS> board{}; // инициализация поля

        // расставляем мины
        for (int i = 0; i < NUMBER_OF_MINES; i++) {
          int row = rand() % ROWS;
          int column = rand() % COLUMNS;
          if (board[row][column].mine) { // если там уже есть мина
            i--; // пробуем еще раз
          } else {
              board[row][column].mine = true;
            }
        }

        // подсчитываем число мин в соседних клетках
        for (int i = 0; i < ROWS; i++) {
          for (int j = 0; j < COLUMNS; j++) {
            if (board[i][j].mine) {
              continue;
            }
            int count = 0;
            for (int k = -1; k <= 1; k++) {
              for (int l = -1; l <= 1; l++) {
                int new_row = i + k;
                int new_column = j + l;
                if (new_row >= 0 && new_row < ROWS && new_column >= 0 && new_column < COLUMNS && board[new_row][new_column].mine) {
                  count++;
                }
              }
            }
            board[i][j].number_of_adjacent_mines = count;
          }
        }

        // основной цикл игры
        while (true) {
          system("clear");
          display_board(board); // выводим поле на экран
          int row, column;
          char action;
          cout << "Введите: строку колонку: ";
          cin >> row >> column;
          action = 'o';
          if (action == 'f' || action == 'F') {
            if (flag_cell(board, row, column)) {
              cout << "Флажок установлен." << endl;
            }
          } else if (action == 'o' || action == 'O') {
              open_cell(board, row, column);
              if (board[row][column].mine) { // поражение
                system("clear");
                cout << "Boom! Бомба взорвалась. Вы проиграли." << endl;
                display_board(board); // выводим поле на экран
                break;
              }
              if (check_win(board)) { // победа
                system("clear");
                cout << "Поздравляем! Вы победили!" << endl;
                display_board(board); // выводим поле на экран
                break;
              }
            }
        }

        cout << "\n\n\nНачать снова\n 1. Да\n 2. Нет\n\nВыбор: ";
        a = 0; cin >> a;
        if(a == 2){
          system("clear");
          break;
        } 
      }     
    }   //минёр
  }
  return 0;
}