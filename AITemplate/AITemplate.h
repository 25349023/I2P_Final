#pragma once

#include <BattleShipGame/Wrapper/AI.h>
#include <BattleShipGame/Ship.h>
#include <algorithm>
#include <random>
#include <ctime>
#include <set>

typedef struct fristhit{
    int x;
    int y;
    //int isfinish;
}Target;
class AI : public AIInterface {
    std::vector<std::pair<int, int>> way;
    std::vector<std::pair<int, int>> enemy;
    std::pair<int, int> mycenter;
    std::pair<int, int> up_bound,down_bound,left_bound,right_bound;
    bool ishit;
    std::set<std::pair<int, int>> explored;
public:
    std::vector<TA::Ship>
    init(int size, std::vector<int> ship_size, bool order, std::chrono::milliseconds runtime) override {
        (void) ship_size;
        (void) runtime;
        ishit = false;
        std::vector<TA::Ship> tmp;
        tmp.push_back({3, 0, 0, TA::Ship::State::Available});
        tmp.push_back({3, 5, 0, TA::Ship::State::Available});
        tmp.push_back({5, 0, 5, TA::Ship::State::Available});
        tmp.push_back({7, 10, 10, TA::Ship::State::Available});

        /*for(int i=0;i<size;++i)
            for(int j=0;j<size;++j)
                way.emplace_back(i,j);

        std::mt19937 mt;
        mt.seed( std::time(nullptr) + 7122 + (order?1:0) );
        std::shuffle(way.begin(), way.end(), mt);*/
        way.emplace_back(16, 19);
        way.emplace_back(19, 16);
        way.emplace_back(19, 4);
        way.emplace_back(4, 19);
        way.emplace_back(16, 1);
        way.emplace_back(1, 16);
        way.emplace_back(4, 1);
        way.emplace_back(1, 4);
        way.emplace_back(19, 13);
        way.emplace_back(19, 10);
        way.emplace_back(19, 7);
        way.emplace_back(13, 19);
        way.emplace_back(10, 19);
        way.emplace_back(7, 19);
        way.emplace_back(1, 13);
        way.emplace_back(1, 10);
        way.emplace_back(1, 7);
        way.emplace_back(13, 1);
        way.emplace_back(10, 1);
        way.emplace_back(7, 1);
        way.emplace_back(16, 13);
        way.emplace_back(16, 10);
        way.emplace_back(16, 7);
        way.emplace_back(13, 16);
        way.emplace_back(10, 16);
        way.emplace_back(7, 16);
        way.emplace_back(4, 13);
        way.emplace_back(4, 10);
        way.emplace_back(4, 7);
        way.emplace_back(13, 4);
        way.emplace_back(10, 4);
        way.emplace_back(7, 10);
        way.emplace_back(10, 7);
        way.emplace_back(13, 10);
        way.emplace_back(10, 13);
        way.emplace_back(7, 4);
        way.emplace_back(1, 19);
        way.emplace_back(4, 16);
        way.emplace_back(7, 13);
        way.emplace_back(13, 7);
        way.emplace_back(16, 4);
        way.emplace_back(19, 1);
        way.emplace_back(19, 19);
        way.emplace_back(16, 16);
        way.emplace_back(13, 13);
        way.emplace_back(10, 10);
        way.emplace_back(7, 7);
        way.emplace_back(4, 4);
        way.emplace_back(1, 1);

        return tmp;
    }

    void callbackReportEnemy(std::vector<std::pair<int, int>>) override {

    }

    std::pair<int, int> queryWhereToHit(TA::Board _map) override {
        /* auto res = way.back();
         way.pop_back();*/
         /*
        for (int i = 0; i < 20; i++){
            for (int j = 0; j < 20; j++){
                if (_map[i][j] == TA::Board::State::Hit){
                    if (i != 0){
                        if (_map[i - 1][j] == TA::Board::State::Unknown)
                            return std::pair<int, int>(i - 1, j);
                    }
                    if (j != 0){
        (void) runtime;

        std::vector<TA::Ship> tmp;
        tmp.push_back({3, 0, 0, TA::Ship::State::Available});
        tmp.push_back({3, 5, 0, TA::Ship::State::Available});
        tmp.push_back({5, 0, 5, TA::Ship::State::Available});
        tmp.push_back({7, 10, 10, TA::Ship::State::Available});

                        if (_map[i][j - 1] == TA::Board::State::Unknown)
                            return std::pair<int, int>(i, j - 1);
                    }
                    if (i != 19){
                        if (_map[i + 1][j] == TA::Board::State::Unknown)
                            return std::pair<int, int>(i + 1, j);
                    }
                    if (j != 19){
                        if (_map[i][j + 1] == TA::Board::State::Unknown)
                            return std::pair<int, int>(i, j + 1);
                    }
                }
            }
        }
        */
        if (!ishit){
            for (int i = 0; i < 20; i++){
                for (int j = 0; j < 20; j++){
                    if (_map[i][j] == TA::Board::State::Hit && explored.find(std::make_pair(i,j)) == explored.end()){
                        ishit = true;
                        explored.insert(std::make_pair(i,j));
                        mycenter = std::make_pair(i,j);
                    }
                }
            }
        }

        if (ishit){
            int k = mycenter.second, lenl = 0, lenr = 0, lenu = 0, lend = 0;
                    while (k > 0){
                        if (_map[mycenter.first][k-1] == TA::Board::State::Unknown){
                            explored.insert(std::make_pair(mycenter.first, k - 1));
                            return std::pair<int, int>(mycenter.first, k - 1);
                        }
                            
                        if (_map[mycenter.first][k-1] == TA::Board::State::Empty){
                            //explored
                            lenl--;
                            break;
                        }
                            
                        k--;
                        lenl++;
                    }
                    k = mycenter.second;
                    while (k < 19){
                        if (_map[mycenter.first][k + 1] == TA::Board::State::Unknown){
                            explored.insert(std::make_pair(mycenter.first, k + 1));
                            return std::pair<int, int>(mycenter.first, k + 1);
                        }
                            
                        if (_map[mycenter.first][k + 1] == TA::Board::State::Empty){
                            lenr--;
                            break;
                        }
                            
                        k++;
                        lenr++;
                    }
                    k = mycenter.first;
                    while (k > 0){
                        if (_map[k - 1][mycenter.second] == TA::Board::State::Unknown){
                            explored.insert(std::make_pair(k - 1, mycenter.second));
                            return std::pair<int, int>(k - 1, mycenter.second);
                        }
                            
                        if (_map[k - 1][mycenter.second] == TA::Board::State::Empty){
                            lend--;
                            break;
                        }
                            
                        k--;
                        lend++;
                    }
                    k = mycenter.first;
                    while (k < 19){
                        if (_map[k + 1][mycenter.second] == TA::Board::State::Unknown){
                            explored.insert(std::make_pair(k + 1, mycenter.second));
                            return std::pair<int, int>(k + 1, mycenter.second);
                        }
                            
                        if (_map[k + 1][mycenter.second] == TA::Board::State::Empty){
                            lenu--;
                            break;
                        }
                            
                        k++;
                        lenu++;
                    }
                    ishit = false;
                    if ((lenl + lenr == 2 && lenu + lend == 2) || (lenl + lenr == 4 && lenu + lend == 4) || (lenl + lenr == 6 && lenu + lend == 6)){
                        
                        if (_map[mycenter.first - (lenl - lenr) / 2][mycenter.second - (lenu - lend) / 2] == TA::Board::State::Unknown){
                            explored.insert(std::make_pair(mycenter.first - (lenl - lenr) / 2, mycenter.second - (lenu - lend) / 2));
                            return std::pair<int, int>(mycenter.first - (lenl - lenr) / 2, mycenter.second - (lenu - lend) / 2);
                        }  
                    }
                   
                    
        }

        while (1){
            auto res = way.back();
            way.pop_back();
            if (_map[res.first][res.second] == TA::Board::State::Unknown)
                return res;
        }
        /*for (int i = 0; i < 20; i++){
            for (int j = 0; j < 20; j++){
                if (_map[i][j] == TA::Board::State::Hit){
                    int k = j, lenl = 0, lenr = 0, lenu = 0, lend = 0;
                    while (k > 0){
                        if (_map[i][k-1] == TA::Board::State::Unknown)
                            return std::pair<int, int>(i, k - 1);
                        if (_map[i][k-1] == TA::Board::State::Empty)
                            break;
                        k--;
                        lenl++;
                    }
                    k = j;
                    while (k < 19){
                        if (_map[i][k + 1] == TA::Board::State::Unknown)
                            return std::pair<int, int>(i, k + 1);
                        if (_map[i][k + 1] == TA::Board::State::Empty)
                            break;
                        k++;
                        lenr++;
                    }
                    k = i;
                    while (k > 0){
                        if (_map[k - 1][j] == TA::Board::State::Unknown)
                            return std::pair<int, int>(k - 1, j);
                        if (_map[k - 1][j] == TA::Board::State::Empty)
                            break;
                        k--;
                        lend++;
                    }
                    k = i;
                    while (k < 19){
                        if (_map[k - 1][j] == TA::Board::State::Unknown)
                            return std::pair<int, int>(k - 1, j);
                        if (_map[k - 1][j] == TA::Board::State::Empty)
                            break;
                        k++;
                        lenu++;
                    }
                    if (lenl + lenr == 2 && lenu + lend == 2){
                        if (_map[i - (lenl - lenr) / 2][j - (lenu - lend) / 2] == TA::Board::State::Unknown)
                            return std::pair<int, int>(i - (lenl - lenr) / 2, j - (lenu - lend) / 2);
                    }
                    else if (lenl + lenr == 4 && lenu + lend == 4){
                        if (_map[i - (lenl - lenr) / 2][j - (lenu - lend) / 2] == TA::Board::State::Unknown)
                            return std::pair<int, int>(i - (lenl - lenr) / 2, j - (lenu - lend) / 2);
                    }
                    else if (lenl + lenr == 6 && lenu + lend == 6){
                        if (_map[i - (lenl - lenr) / 2][j - (lenu - lend) / 2] == TA::Board::State::Unknown)
                            return std::pair<int, int>(i - (lenl - lenr) / 2, j - (lenu - lend) / 2);
                    }
                    else if ()

                }
            }
        }*/
       


    }

    void callbackReportHit(bool) override {

    }

    std::vector<std::pair<int, int>> queryHowToMoveShip(std::vector<TA::Ship>) override {
        return {};
    }
};