#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>

using namespace std;

class PositionManager {
private:
    vector<int> sequenceN;
    vector<char> sequenceA;
    int currentIndex;
    int N;

public:
    PositionManager(int n) : N(n), currentIndex(-1) {
        srand(time(0));
    }

    void generatePosition() {
        currentIndex++;
        sequenceN.push_back(rand() % 9);
        sequenceA.push_back('A' + rand() % 26);
    }

    int getCurrentPosition() {
        return sequenceN[currentIndex];
    }

    char getCurrentAlphabet() {
        return sequenceA[currentIndex];
    }

    bool isMatch(int stepBack) {
        return (currentIndex >= stepBack && sequenceN[currentIndex] == sequenceN[currentIndex - stepBack]);
    }

    bool isMatch1(int stepBack) {
        return (currentIndex >= stepBack && sequenceA[currentIndex] == sequenceA[currentIndex - stepBack]);
    }

    void clearSequence() {
        sequenceN.clear();
        sequenceA.clear();
        currentIndex = -1;
    }
};

class GameBoard {
private:
    vector<string> board;
    PositionManager* positionManager;

public:
    GameBoard(PositionManager* pm) : positionManager(pm), board(9, " ") {}

    void displayBoard() {
        cout << "     |     |     \n";
        cout << "  " << board[0] << "  |  " << board[1] << "  |  " << board[2] << "\n";
        cout << "_____|_____|_____\n";
        cout << "     |     |     \n";
        cout << "  " << board[3] << "  |  " << board[4] << "  |  " << board[5] << "\n";
        cout << "_____|_____|_____\n";
        cout << "     |     |     \n";
        cout << "  " << board[6] << "  |  " << board[7] << "  |  " << board[8] << "\n";
        cout << "     |     |     \n\n";
    }

    void updateBoard() {
        positionManager->generatePosition();
        int position = positionManager->getCurrentPosition();
        char alpha = positionManager->getCurrentAlphabet();
        board[position] = alpha;
    }

    void clearBoard() {
        fill(board.begin(), board.end(), " ");
    }

    bool isMatch(int N) {
        return positionManager->isMatch(N);
    }

    bool isMatch1(int N) {
        return positionManager->isMatch1(N);
    }
};

class Player {
private:
    int score;
    int N;
    GameBoard* gameBoard;

public:
    Player(int n, GameBoard* gb) : N(n), gameBoard(gb), score(0) {}

    void updateScore(bool correct) {
        if (correct && (gameBoard->isMatch(N) || gameBoard->isMatch1(N))) {
            score++;
        } else if (correct) {
            score--;
        }
    }

    int getScore() {
        return score;
    }

    void playRound() {
        gameBoard->clearBoard();
        gameBoard->updateBoard();
        gameBoard->displayBoard();

        char userResponse;
        cout << "Is this Position or Alphabet the same " << N << " rounds back? (y/n): ";
        cin >> userResponse;

        bool correct = (userResponse == 'y');
        updateScore(correct);

        this_thread::sleep_for(chrono::seconds(3));

        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        cout << "Current Score: " << getScore() << "\n";
    }
};

int main() {
    int N, maxRounds;
    cout << "Enter the value of N: ";
    cin >> N;
    cout << "Enter the number of rounds you want to try: ";
    cin >> maxRounds;

    PositionManager positionManager(N);
    GameBoard gameBoard(&positionManager);
    Player player(N, &gameBoard);

    char userResponse;
    int currentRound = 1;

    while (currentRound <= maxRounds) {
        cout << "\nRound " << currentRound << endl;
        player.playRound();
        currentRound++;

        if (currentRound == maxRounds + 1) {
            cout << "You have finished " << maxRounds << " rounds. Continue? (y/n): ";
            cin >> userResponse;
            if (userResponse == 'n') {
                break;
            } else {
                cout << "Same N and rounds as before? (y/n): ";
                cin >> userResponse;
                if (userResponse == 'y') {
                    currentRound = 1;
                } else {
                    cout << "Enter the value of N: ";
                    cin >> N;
                    cout << "Enter the number of rounds you want to try: ";
                    cin >> maxRounds;
                    currentRound = 1;
                    positionManager.clearSequence();
                }
            }
        }
    }

    cout << "Final Score: " << player.getScore() << "\n";
    return 0;
}
