#pragma once

#include <BattleShipGame/Wrapper/AI.h>
#include <BattleShipGame/Ship.h>
#include <algorithm>
#include <random>
#include <ctime>

class AI : public AIInterface {
    std::vector<std::pair<int, int>> way;
    std::vector<std::pair<int, int>> enemy;
public:
    std::vector<TA::Ship>
    init(int size, std::vector<int> ship_size, bool order, std::chrono::milliseconds runtime) override {
        (void) ship_size;
        (void) runtime;

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
        for (int i = 0; i < 20; i++){
            for (int j = 0; j < 20; j++){
                if (_map[i][j] == TA::Board::State::Hit){
                    if (i != 0){
                        if (_map[i - 1][j] == TA::Board::State::Unknown)
                            return std::pair<int, int>(i - 1, j);
                    }
                    if (j != 0){
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
        while (1){
            auto res = way.back();
            way.pop_back();
            if (_map[res.first][res.second] == TA::Board::State::Unknown)
                return res;
        }


    }

    void callbackReportHit(bool) override {

    }

    std::vector<std::pair<int, int>> queryHowToMoveShip(std::vector<TA::Ship>) override {
        return {};
    }
};