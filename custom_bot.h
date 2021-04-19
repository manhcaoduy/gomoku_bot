#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "config.h"

// constants
const int INF = (int)1e9;
const int winScore = (int)1e8;
const int winGurantee = (int)1e6;

// direction
int dx[] = {1, 1, 1, 0, -1, -1, -1, 0};
int dy[] = {-1, 0, 1, 1, 1, -1, 0, -1};

// Candidate Point Struct
struct Candidate
{
    std::pair<int, int> score;
    Point point;
    // comparison operator to sort
    bool operator<(const Candidate &other) const
    {
        if (score.first < other.score.first)
        {
            return score.first < other.score.first;
        }
        return score.second < other.score.second;
    }
};

class Gomoku
{
private:
    std::vector<std::vector<int>> board; // board information
    int color;                           // current color
    int num_occupied;                    // number of occupied to trigger the earlyMove function

    // check if coord (row, col) is in the board or not
    bool inBoard(int row, int col)
    {
        return (row >= 0 && row < HEIGHT && col >= 0 && col < WIDTH);
    }

    // return the score of count consecutive piles with blocked - number of sides that is blocked, isNext is True or False
    int getConsecutiveSetScore(int count, int blocked, bool isNext)
    {
        if (blocked == 2 && count < 5)
            // we can't win in this situation
            return 0;
        switch (count)
        {
        case 5:
            // won already
            return winScore;
        case 4:
            if (isNext)
                // only just need a move to win this game
                return winGurantee;
            else
            {
                // The opponents can't stop you to win
                if (blocked == 0)
                    return winGurantee / 4;
                // The opponents can stop you to win, but it's still good for you
                else
                    return 200;
            }
        case 3:
            if (blocked == 0)
            {
                if (isNext)
                    // You only need a move to win this situation
                    return winGurantee / 10;
                else
                    // The opponents can stop you to win, but it's still good for you
                    return 200;
            }
            else
            {
                // This situation is not really good, so the point for it is low
                if (isNext)
                    return 10;
                else
                    return 5;
            }
        case 2:
            // This situation is not really good, so the point for it is low
            if (blocked == 0)
            {
                if (isNext)
                    return 7;
                else
                    return 5;
            }
            else
            {
                return 3;
            }
        case 1:
            // This situation is not really good, so the point for it is low
            return 1;
        }
        // Have more 5 consecutive piles? so good!
        return winScore * 2;
    }

    Point finishMove()
    {
        for (int row = 0; row < HEIGHT; ++row)
        {
            for (int col = 0; col < WIDTH; ++col)
            {
                int value = board[row][col];
                if (value != color)
                    continue;
                for (int direction = 0; direction < 8; ++direction)
                {
                    int block = 0;
                    int new_x = row - dx[direction], new_y = col - dy[direction];
                    if (inBoard(new_x, new_y) && board[new_x][new_y] == -value)
                        block++;
                    new_x = row + 5 * dx[direction];
                    new_y = col + 5 * dy[direction];
                    if (inBoard(new_x, new_y) && board[new_x][new_y] == -value)
                        block++;
                    if (block == 2)
                        continue;
                    bool ok = true;
                    int num = 0;
                    for (int i = 0; i < 5; ++i)
                    {
                        new_x = row + i * dx[direction];
                        new_y = col + i * dy[direction];
                        if (inBoard(new_x, new_y))
                        {
                            if (board[new_x][new_y] == value)
                                num++;
                            else if (board[new_x][new_y] == -value)
                                ok = false;
                        }
                        else
                            ok = false;
                    }
                    if (ok && num == 4)
                    {
                        for (int i = 0; i < 5; ++i)
                        {
                            new_x = row + i * dx[direction];
                            new_y = col + i * dy[direction];
                            if (!board[new_x][new_y])
                                return Point(new_x, new_y);
                        }
                    }
                }
            }
        }

        for (int row = 0; row < HEIGHT; ++row)
        {
            for (int col = 0; col < WIDTH; ++col)
            {
                int value = board[row][col];
                if (value != -color)
                    continue;
                for (int direction = 0; direction < 8; ++direction)
                {
                    int block = 0;
                    int new_x = row - dx[direction], new_y = col - dy[direction];
                    if (inBoard(new_x, new_y) && board[new_x][new_y] == -value)
                        block++;
                    new_x = row + 5 * dx[direction];
                    new_y = col + 5 * dy[direction];
                    if (inBoard(new_x, new_y) && board[new_x][new_y] == -value)
                        block++;
                    if (block == 2)
                        continue;
                    bool ok = true;
                    int num = 0;
                    for (int i = 0; i < 5; ++i)
                    {
                        new_x = row + i * dx[direction];
                        new_y = col + i * dy[direction];
                        if (inBoard(new_x, new_y))
                        {
                            if (board[new_x][new_y] == value)
                                num++;
                            else if (board[new_x][new_y] == -value)
                                ok = false;
                        }
                        else
                            ok = false;
                    }
                    if (ok && num == 4)
                    {
                        for (int i = 0; i < 5; ++i)
                        {
                            new_x = row + i * dx[direction];
                            new_y = col + i * dy[direction];
                            if (!board[new_x][new_y])
                                return Point(new_x, new_y);
                        }
                    }
                }
            }
        }

        return Point(-1, -1);
    }

    // Update the consecutive piles list
    void loopProcess(int value, int in_color, int next_color, int &consecutive, int &block, int &result)
    {
        if (value == in_color)
        {
            consecutive++;
        }
        else if (value == 0)
        {
            if (consecutive > 0)
            {
                block--;
                result += getConsecutiveSetScore(consecutive, block, in_color == next_color);
                consecutive = 0;
                block = 1;
            }
            else
            {
                block = 1;
            }
        }
        else
        {
            if (consecutive > 0)
            {
                result += getConsecutiveSetScore(consecutive, block, in_color == next_color);
                consecutive = 0;
                block = 2;
            }
            else
            {
                block = 2;
            }
        }
    }

    // get score for the in_color in the board whose turn is next_color
    int getScore(int in_color, int next_color)
    {
        int rowScore = 0, colScore = 0, diagonalScore = 0;
        // row
        int consecutive = 0, block = 2;
        for (int row = 0; row < HEIGHT; ++row)
        {
            for (int col = 0; col < WIDTH; ++col)
            {
                loopProcess(board[row][col], in_color, next_color, consecutive, block, rowScore);
            }
            loopProcess(-in_color, in_color, next_color, consecutive, block, rowScore);
            consecutive = 0;
            block = 2;
        }

        // col
        for (int col = 0; col < WIDTH; ++col)
        {
            for (int row = 0; row < HEIGHT; ++row)
            {
                loopProcess(board[row][col], in_color, next_color, consecutive, block, colScore);
            }

            loopProcess(-in_color, in_color, next_color, consecutive, block, colScore);
            consecutive = 0;
            block = 2;
        }

        // diagonal
        for (int col = 0; col < WIDTH; ++col)
        {
            int start_x = 0, start_y = col;
            while (inBoard(start_x, start_y))
            {
                loopProcess(board[start_x][start_y], in_color, next_color, consecutive, block, diagonalScore);
                start_x++;
                start_y++;
            }
            loopProcess(-in_color, in_color, next_color, consecutive, block, diagonalScore);
            consecutive = 0;
            block = 2;
        }

        for (int row = 1; row < HEIGHT; ++row)
        {
            int start_x = row, start_y = 0;
            while (inBoard(start_x, start_y))
            {
                loopProcess(board[start_x][start_y], in_color, next_color, consecutive, block, diagonalScore);
                start_x++;
                start_y++;
            }
            loopProcess(-in_color, in_color, next_color, consecutive, block, diagonalScore);
            consecutive = 0;
            block = 2;
        }

        for (int col = 0; col < WIDTH; ++col)
        {
            int start_x = 0, start_y = col;
            while (inBoard(start_x, start_y))
            {
                loopProcess(board[start_x][start_y], in_color, next_color, consecutive, block, diagonalScore);
                start_x++;
                start_y--;
            }
            loopProcess(-in_color, in_color, next_color, consecutive, block, diagonalScore);
            consecutive = 0;
            block = 2;
        }

        for (int row = 1; row < HEIGHT; ++row)
        {
            int start_x = row, start_y = WIDTH - 1;
            while (inBoard(start_x, start_y))
            {
                loopProcess(board[start_x][start_y], in_color, next_color, consecutive, block, diagonalScore);
                start_x++;
                start_y--;
            }
            loopProcess(-in_color, in_color, next_color, consecutive, block, diagonalScore);
            consecutive = 0;
            block = 2;
        }
        return rowScore + colScore + diagonalScore;
    }

    // board evaluation function
    double getBoardEvaluation(int next_color)
    {
        int white = getScore(1, next_color);
        int black = getScore(-1, next_color);
        return 1.0 * white / black;
    }

    // check isGameOver
    bool isGameOver()
    {
        // check if the board is fulfilled or not
        bool isFull = true;
        for (int row = 0; row < HEIGHT; ++row)
        {
            for (int col = 0; col < WIDTH; ++col)
            {
                if (board[row][col] == 0)
                    isFull = false;
            }
        }
        if (isFull)
            return true;
        // check if a 5 consecutive same color exists or not
        for (int x = 0; x < HEIGHT; ++x)
        {
            for (int y = 0; y < WIDTH; ++y)
            {
                int color = board[x][y];
                if (color == 0)
                    continue;
                for (int i = 0; i < 8; ++i)
                {
                    bool ok = true;
                    for (int j = 0; j <= 4; ++j)
                    {
                        int next_x = x + dx[i] * j, next_y = y + dy[i] * j;
                        if (board[next_x][next_y] == color)
                        {
                            ok = false;
                            break;
                        }
                    }
                    if (ok)
                        return true;
                }
            }
        }
        return false;
    }

    // calculate candidate score for a Point in the board
    std::pair<int, int> getCandidateScore(int row, int col)
    {
        std::pair<int, int> ans = std::make_pair(0, 0);
        for (int i = 0; i < 8; ++i)
        {
            int start_x = row + dx[i], start_y = col + dy[i];
            if (!inBoard(start_x, start_y) || board[start_x][start_y] == 0)
                continue;
            int color = board[start_x][start_y];
            int num = 0;
            while (true)
            {
                int next_x = start_x + num * dx[i];
                int next_y = start_y + num * dy[i];
                if (inBoard(next_x, next_y) && board[next_x][next_y] == color)
                {
                    num++;
                }
                else
                {
                    break;
                }
            }
            ans = std::max(ans, std::make_pair(num, (int)(board[row][col] == color)));
        }
        return ans;
    }

    // calculate the candidate list
    std::vector<Point> getCandidate()
    {
        std::vector<Candidate> listCandidate;
        listCandidate.clear();

        for (int new_x = 0; new_x < HEIGHT; ++new_x)
        {
            for (int new_y = 0; new_y < WIDTH; ++new_y)
            {
                if (board[new_x][new_y] != 0)
                    continue;
                bool isCandidate = false;
                for (int i = 0; i < 8; ++i)
                {
                    int x = new_x + dx[i], y = new_y + dy[i];
                    if (inBoard(x, y) && board[x][y] != 0)
                    {
                        isCandidate = true;
                    }
                }
                if (!isCandidate)
                    continue;
                auto score = getCandidateScore(new_x, new_y);
                auto point = Point(new_x, new_y);
                Candidate candidate;
                candidate.score = score;
                candidate.point = point;
                listCandidate.push_back(candidate);
            }
        }
        std::sort(listCandidate.begin(), listCandidate.end());
        std::vector<Point> ans;
        ans.clear();
        for (auto item : listCandidate)
        {
            ans.push_back(Point(item.point.x, item.point.y));
        }
        return ans;
    }

    // alpha-beta pruning recursive function
    double alphaBetaPruning(int depth, double alpha, double beta, bool isMax, int in_color)
    {
        if (depth == 0 || isGameOver())
        {
            return getBoardEvaluation(in_color);
        }
        std::vector<Point> listChild = getCandidate();
        // isMax
        if (isMax)
        {
            double maxEval = -INF;
            for (auto child : listChild)
            {
                if (board[child.x][child.y] != 0)
                    continue;
                board[child.x][child.y] = in_color;
                double eval = alphaBetaPruning(depth - 1, alpha, beta, false, -in_color);
                board[child.x][child.y] = 0;
                maxEval = std::max(maxEval, eval);
                if (eval >= beta)
                    return eval;
                alpha = std::max(alpha, eval);
            }
            return maxEval;
        }
        // isMin
        double minEval = INF;
        for (auto child : listChild)
        {
            if (board[child.x][child.y] != 0)
                continue;
            board[child.x][child.y] = in_color;
            double eval = alphaBetaPruning(depth - 1, alpha, beta, true, -in_color);
            board[child.x][child.y] = 0;
            minEval = std::min(minEval, eval);
            if (eval <= alpha)
                return eval;
            beta = std::min(beta, eval);
        }
        return minEval;
    }

    // early move when num_occupied < 4
    Point earlyMove()
    {
        if (num_occupied == 0)
            return Point(HEIGHT / 2, WIDTH / 2);
        if (num_occupied == 1)
        {
            // find the occupied point
            for (int i = 0; i < HEIGHT; ++i)
            {
                for (int j = 0; j < WIDTH; ++j)
                {
                    if (board[i][j] != 0)
                    // set the des point based on the occupied point
                    {
                        int des_x = i, des_y = j;
                        if (i > HEIGHT / 2)
                            des_x--;
                        if (j > WIDTH / 2)
                            des_y--;
                        if (i < HEIGHT / 2)
                            des_x++;
                        if (j < WIDTH / 2)
                            des_y++;
                        if (des_x == i && des_y == j)
                            des_y++;
                        return Point(des_x, des_y);
                    }
                }
            }
        }
        if (num_occupied == 2)
        {
            int f_x = 0, f_y = 0, s_x = 0, s_y = 0;
            for (int i = 0; i < HEIGHT; ++i)
            {
                for (int j = 0; j < WIDTH; ++j)
                {
                    // find our first point
                    if (board[i][j] == color)
                    {
                        f_x = i;
                        f_y = j;
                    }
                    else if (board[i][j] != 0)
                    {
                        s_x = i;
                        s_y = j;
                    }
                }
            }
            int des_x = f_x, des_y = f_y;
            if (f_x == s_x || f_y == s_y)
            {
                return Point(f_x + 1, f_y + 1);
            }
            else
            {
                if (f_x > s_x)
                    des_x--;
                else
                    des_x++;
                if (f_y > s_y)
                    des_y++;
                else
                    des_y--;
                return Point(des_x, des_y);
            }
        }
        if (num_occupied == 3)
        {
            // find our point
            for (int row = 0; row < HEIGHT; ++row)
            {
                for (int col = 0; col < WIDTH; ++col)
                {
                    if (board[row][col] == color)
                    {
                        for (int k = 0; k < 8; ++k)
                        {
                            int new_x = row + dx[k], new_y = col + dy[k];
                            if (board[new_x][new_y] != 0)
                                continue;
                            return Point(new_x, new_y);
                        }
                    }
                }
            }
        }
    }

public:
    // initialize the size of the board
    Gomoku()
    {
        board.resize(HEIGHT);
        for (int i = 0; i < HEIGHT; ++i)
        {
            board[i].resize(WIDTH);
        }
    }

    // nextMove API
    Point nextMove(int in_board[][WIDTH], int in_color)
    {
        num_occupied = 0;
        for (int i = 0; i < HEIGHT; ++i)
        {
            for (int j = 0; j < WIDTH; ++j)
            {
                board[i][j] = in_board[i][j];
                if (board[i][j] != 0)
                {
                    num_occupied++;
                }
            }
        }
        color = in_color;
        if (num_occupied < 4)
            return earlyMove();
        else
        {
            if (isGameOver())
            {
                return Point(-1, -1);
            }
            Point finish = finishMove();
            if (finish.x != -1 && finish.y != -1)
            {
                return finish;
            }
            else
            {
                std::vector<Point> listChild = getCandidate();
                double result = (num_occupied % 2 == 0 ? -INF : INF);
                double alpha = -INF, beta = INF;
                int best_x = -1, best_y = -1;
                // std::cout << "-----------------------------" << std::endl;
                for (auto child : listChild)
                {
                    if (board[child.x][child.y] != 0)
                        continue;
                    board[child.x][child.y] = color;
                    double eval = 0;
                    if (num_occupied % 2 == 0)
                    {
                        eval = alphaBetaPruning(DEPTH - 1, alpha, beta, false, -color);
                    }
                    else
                    {
                        eval = alphaBetaPruning(DEPTH - 1, alpha, beta, true, -color);
                    }
                    board[child.x][child.y] = 0;
                    if (num_occupied % 2 == 0)
                    {
                        if (result < eval)
                        {
                            result = eval;
                            best_x = child.x;
                            best_y = child.y;
                        }
                    }
                    else
                    {
                        if (result > eval)
                        {
                            result = eval;
                            best_x = child.x;
                            best_y = child.y;
                        }
                    }
                    // std::cout << child.x << " " << child.y << " " << eval << std::endl;
                    if (num_occupied % 2 == 0)
                    {
                        alpha = std::max(alpha, eval);
                    }
                    else
                    {
                        beta = std::min(beta, eval);
                    }
                }
                // std::cout << "--------------------------" << std::endl;
                return Point(best_x, best_y);
            }
            return Point(-1, -1);
        }
    }

    // just for testing (test.cpp file)

    // API getBoardEvaluation
    int boardVal(int in_color)
    {
        return getBoardEvaluation(in_color);
    }

    // set board values
    void initBoard(int in_board[][WIDTH])
    {
        for (int i = 0; i < HEIGHT; ++i)
        {
            for (int j = 0; j < WIDTH; ++j)
            {
                board[i][j] = in_board[i][j];
            }
        }
        color = -1;
    }
};
