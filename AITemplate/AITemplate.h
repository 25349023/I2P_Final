#pragma once

#include <BattleShipGame/Wrapper/AI.h>
#include <BattleShipGame/Ship.h>
#include <algorithm>
#include <random>
#include <ctime>
#include <set>
#include <vector>

struct Shipcenter
{
    int x;
    int y;
    int edge;
};
class AI : public AIInterface
{
    std::vector<std::pair<int, int>> way;
    std::vector<std::pair<int, int>> enemy;
    std::pair<int, int> mycenter;
    std::pair<int, int> up_bound,down_bound,left_bound,right_bound;
    //std::set<std::pair<int, int>> explored;
    std::vector<Shipcenter> deadship;
    bool ishit;
    std::pair<int, int> specialcase;
    bool isspecial;
public:
    std::vector<TA::Ship>
    init(int size, std::vector<int> ship_size, bool order, std::chrono::milliseconds runtime) override
    {
        (void) ship_size;
        (void) runtime;
        ishit = false;
        std::vector<TA::Ship> tmp;
        tmp.push_back({3, 14, 13, TA::Ship::State::Available});
        tmp.push_back({3, 9, 12, TA::Ship::State::Available});
        tmp.push_back({5, 9, 15, TA::Ship::State::Available});
        tmp.push_back({7, 2, 13, TA::Ship::State::Available});

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

    void callbackReportEnemy(std::vector<std::pair<int, int>>) override
    {

    }

    bool isdead(int i, int j)
    {
        //return false;
        if (deadship.size() == 0)
            return true;
        for (auto d : deadship)
        {
            if(i <= d.x + d.edge/2 && i >= d.x - d.edge/2 && j <= d.y + d.edge/2 && j >= d.y - d.edge/2)
            {
                ////explored.insert(std::make_pair(i, j));
                return false;
            }
        }
        return true;
    }
    std::pair<int,int> getlen(int x, int y,TA::Board &_map)
    {
        if (!ishit)
        {
            for (int i = 0; i < 20; i++)
            {
                for (int j = 0; j < 20; j++)
                {
                    if (_map[i][j] == TA::Board::State::Hit  && isdead(i, j))
                    {
                        ishit = true;
                        //explored.insert(std::make_pair(i,j));
                        mycenter = std::make_pair(i,j);
                        break;
                    }

                }
                if (ishit == true)
                    break;
            }
            
        }

        if (ishit)
        {
            int k = y, lenl = 0, lenr = 0, lenu = 0, lend = 0;
            while (k > 0)
            {
                if (_map[x][k-1] == TA::Board::State::Unknown && isdead(x, k - 1))
                {
                    //explored.insert(std::make_pair(x, k - 1));

                    return std::pair<int, int>(x, k - 1);
                }

                if (_map[x][k-1] == TA::Board::State::Empty || !isdead(x, k - 1))
                {
                    left_bound = std::make_pair(x,k);
                    break;
                }
                if (_map[x][k-1] == TA::Board::State::Hit && isdead(x, k - 1))
                {
                    //explored.insert(std::make_pair(x, k - 1));
                    lenl++;
                }
                if(k - 1 == 0 && _map[x][k-1] == TA::Board::State::Hit)
                    left_bound = std::make_pair(x,k - 1); 
                k--;

            }
            k = y;
            while (k < 19)
            {
                if (_map[x][k + 1] == TA::Board::State::Unknown && isdead(x, k + 1))
                {
                    //explored.insert(std::make_pair(x, k + 1));
                    //lenr++;
                    return std::pair<int, int>(x, k + 1);
                }

                if (_map[x][k + 1] == TA::Board::State::Empty || !isdead(x, k + 1))
                {
                    right_bound = std::make_pair(x,k);
                    break;
                }
                if (_map[x][k + 1] == TA::Board::State::Hit && isdead(x, k + 1))
                {
                    //explored.insert(std::make_pair(x, k + 1));
                    lenr++;
                }
                if(k + 1 == 19 && _map[x][k + 1] == TA::Board::State::Hit)
                    right_bound = std::make_pair(x,k + 1);
                k++;

            }
            k = x;
            while (k > 0)
            {
                if (_map[k - 1][y] == TA::Board::State::Unknown && isdead(k - 1, y))
                {
                    //explored.insert(std::make_pair(k - 1, y));

                    return std::pair<int, int>(k - 1, y);
                }

                if (_map[k - 1][y] == TA::Board::State::Empty || !isdead(k - 1, y))
                {
                    up_bound = std::make_pair(k,y);
                    break;
                }
                if (_map[k - 1][y] == TA::Board::State::Hit && isdead(k - 1, y))
                {
                    //explored.insert(std::make_pair(k - 1, y));
                    lenu++;
                }
                if(k - 1 == 0 && _map[k - 1][y] == TA::Board::State::Hit)
                    up_bound = std::make_pair(k - 1, y);
                k--;

            }
            k = x;
            while (k < 19)
            {
                if (_map[k + 1][y] == TA::Board::State::Unknown && isdead(k + 1, y))
                {

                    //explored.insert(std::make_pair(k + 1, y) );
                    return std::pair<int, int>(k + 1, y);
                }

                if (_map[k + 1][y] == TA::Board::State::Empty || !isdead(k + 1, y))
                {
                    down_bound = std::make_pair(k,y);
                    break;
                }
                if (_map[k + 1][y] == TA::Board::State::Hit && isdead(k + 1, y))
                {
                    //explored.insert(std::make_pair(k + 1, y) );
                    lend++;
                }
                if(k + 1 == 19 && _map[k + 1][y] == TA::Board::State::Hit)
                    down_bound = std::make_pair(k + 1, y);
                k++;

            }
            if ((lenl + lenr == 2 && lenu + lend == 2) || (lenl + lenr == 4 && lenu + lend == 4) || (lenl + lenr == 6 && lenu + lend == 6))
            {
                //ishit = false;
                Shipcenter temp;
                temp.x = x- (lenu - lend) / 2;
                temp.y = y - (lenl - lenr) / 2;
                temp.edge = lenl + lenr;

                deadship.push_back(temp);
                if (!isdead(mycenter.first, mycenter.second))
                    ishit = false;
                //explored.insert(std::make_pair(temp.x, temp.y));
                if (_map[temp.x][temp.y] == TA::Board::State::Unknown)
                {

                    return std::pair<int, int>(temp.x, temp.y);
                }
            }
            else
            {
                
                if (lenl == 0)
                {
                    //ishit = false;
                    //exit(0);
                    if (lenu + lend == 2 || lenu + lend == 4 || lenu + lend == 6)
                    {
                        //ishit = false;
                        Shipcenter temp;
                        temp.x = x - (lenu - lend) / 2;
                        temp.y = y + (lenu + lend) / 2;
                        temp.edge = lenu + lend;

                        deadship.push_back(temp);
                        if (!isdead(mycenter.first, mycenter.second))
                            ishit = false;
                        //explored.insert(std::make_pair(temp.x, temp.y));
                        if (_map[temp.x][temp.y] == TA::Board::State::Unknown)
                        {

                            return std::pair<int, int>(temp.x, temp.y);
                        }
                        else
                            return std::make_pair(-1,-1);


                        //return std::make_pair(x - (lenu - lend) / 2, y + (lenu + lend) / 2)
                    }
                    if (lenu + lend == 5){
                        Shipcenter temp;
                        if (lenu % 3 == 0)
                            temp.x = x + 1;
                        else if (lenu % 3 == 1)
                            temp.x = x;
                        else
                            temp.x = x - 1; 
                        temp.y = y + 1;
                        temp.edge = 3;

                        deadship.push_back(temp);
                        if (!isdead(mycenter.first, mycenter.second))
                            ishit = false;
                        //explored.insert(std::make_pair(temp.x, temp.y));
                        if (_map[temp.x][temp.y] == TA::Board::State::Unknown)
                        {

                            return std::pair<int, int>(temp.x, temp.y);
                        }
                        else
                            return std::make_pair(-1,-1);

                    }
                }
                if (lenr == 0)
                {
                    if(lenu + lend == 2 || lenu + lend == 4 || lenu + lend == 6)
                    {
                        Shipcenter temp;
                        temp.x = x - (lenu - lend) / 2;
                        temp.y = y - (lenu + lend) / 2;
                        temp.edge = lenu + lend;
                        deadship.push_back(temp);
                        if (!isdead(mycenter.first, mycenter.second))
                            ishit = false;
                        //explored.insert(std::make_pair(temp.x, temp.y));
                        if (_map[temp.x][temp.y] == TA::Board::State::Unknown)
                        {

                            return std::pair<int, int>(temp.x, temp.y);
                        }
                        else
                            return std::make_pair(-1,-1);
                    }
                    if (lenu + lend == 5){
                        Shipcenter temp;
                        if (lenu % 3 == 0)
                            temp.x = x + 1;
                        else if (lenu % 3 == 1)
                            temp.x = x;
                        else
                            temp.x = x - 1; 
                        temp.y = y - 1;
                        temp.edge = 3;
                        deadship.push_back(temp);
                        if (!isdead(mycenter.first, mycenter.second))
                            ishit = false;
                        //explored.insert(std::make_pair(temp.x, temp.y));
                        if (_map[temp.x][temp.y] == TA::Board::State::Unknown)
                        {

                            return std::pair<int, int>(temp.x, temp.y);
                        }
                        else
                            return std::make_pair(-1,-1);

                    }
                }
                if (lenu == 0)
                {
                    
                    
                    if( lenl + lenr == 2 || lenl + lenr == 4 || lenl + lenr == 6 )
                    {
                        //ishit = false;
                        
                        Shipcenter temp;
                        temp.x = x + (lenl + lenr) / 2;
                        temp.y = y - (lenl - lenr) / 2;
                        temp.edge = lenl + lenr;
                        /*if (lenl + lenr == 6)
                            return std::pair<int, int>(12, 11);*/
                        deadship.push_back(temp);
                        if (!isdead(mycenter.first, mycenter.second))
                            ishit = false;
                        //explored.insert(std::make_pair(temp.x, temp.y));
                        if ( _map[temp.x][temp.y] == TA::Board::State::Unknown)
                        {

                            return std::pair<int, int>(temp.x, temp.y);
                        }
                        else
                            return std::make_pair(-1,-1);


                    }
                    if (lenl + lenr == 5){
                        Shipcenter temp;
                        if (lenr % 3 == 0)
                            temp.y = y - 1;
                        else if (lenr % 3 == 1)
                            temp.y = y;
                        else
                            temp.y = y + 1; 
                        temp.x = x + 1;
                        temp.edge = 3;

                        deadship.push_back(temp);
                        if (!isdead(mycenter.first, mycenter.second))
                            ishit = false;
                        //explored.insert(std::make_pair(temp.x, temp.y));
                        if (_map[temp.x][temp.y] == TA::Board::State::Unknown)
                        {

                            return std::pair<int, int>(temp.x, temp.y);
                        }
                        else
                            return std::make_pair(-1,-1);

                    }
                }
                if (lend == 0)
                {
                    if(lenl + lenr == 2 || lenl + lenr == 4 || lenl + lenr == 6)
                    {
                        ishit = false;
                        Shipcenter temp;
                        temp.x = x - (lenl + lenr) / 2;
                        temp.y = y - (lenl - lenr) / 2;
                        temp.edge = lenl + lenr;

                        deadship.push_back(temp);
                        if (!isdead(mycenter.first, mycenter.second))
                            ishit = false;
                        //explored.insert(std::make_pair(temp.x, temp.y));
                        if (_map[temp.x][temp.y] == TA::Board::State::Unknown)
                        {

                            return std::pair<int, int>(temp.x, temp.y);
                        }
                        else
                            return std::make_pair(-1,-1);
                    }
                    if (lenl + lenr == 5){
                        Shipcenter temp;
                        if (lenr % 3 == 0)
                            temp.y = y - 1;
                        else if (lenr % 3 == 1)
                            temp.y = y;
                        else
                            temp.y = y + 1; 
                        temp.x = x - 1;
                        temp.edge = 3;

                        deadship.push_back(temp);
                        if (!isdead(mycenter.first, mycenter.second))
                            ishit = false;
                        //explored.insert(std::make_pair(temp.x, temp.y));
                        if (_map[temp.x][temp.y] == TA::Board::State::Unknown)
                        {

                            return std::pair<int, int>(temp.x, temp.y);
                        }
                        else
                            return std::make_pair(-1,-1);

                    }
                }
                if (lenl + lenr != 2 && lenl + lenr != 4 && lenl + lenr != 6)
                {
                    if (!(x == left_bound.first && y == left_bound.second )){
                        std::pair<int, int> res = getlen(left_bound.first, left_bound.second, _map);
                        if (res.first != -1 && res.second != -1 && _map[res.first][res.second] == TA::Board::State::Unknown)
                            return res;
                        res = getlen(mycenter.first, mycenter.second, _map);
                        if (res.first != -1 && res.second != -1)
                            return res;    
                    }
                    //exit(0);
                    
                }
                if (lenl + lenr != 2 && lenl + lenr != 4 && lenl + lenr != 6)
                {
                    //exit(0);
                    if (!(x == right_bound.first && y == right_bound.second )){
                        std::pair<int, int> res = getlen(right_bound.first, right_bound.second, _map);
                        if (res.first != -1 && res.second != -1 && _map[res.first][res.second] == TA::Board::State::Unknown)
                            return res;
                        res = getlen(mycenter.first, mycenter.second, _map);
                        if (res.first != -1 && res.second != -1)
                            return res;     
                    }
                   
                }
                if (lenu + lend != 2 && lenu + lend != 4 && lenu + lend != 6)
                {
                    //exit(0);0,9
                    /*if (up_bound.first == 0 && up_bound.second == 9)
                        exit(0);*/
                    if (!(x == up_bound.first && y == up_bound.second )){
                        std::pair<int, int> res = getlen(up_bound.first, up_bound.second, _map);
                        if (res.first != -1 && res.second != -1 && _map[res.first][res.second] == TA::Board::State::Unknown)
                            return res;
                        res = getlen(mycenter.first, mycenter.second, _map);
                        if (res.first != -1 && res.second != -1)
                            return res;    
                    }
                    
                }
                if (lenu + lend != 2 && lenu + lend != 4 && lenu + lend != 6)
                {
                    if (!(x == down_bound.first && y == down_bound.second )){
                        std::pair<int, int> res = getlen(down_bound.first, down_bound.second, _map);
                        if (res.first != -1 && res.second != -1 && _map[res.first][res.second] == TA::Board::State::Unknown)
                            return res;
                        res = getlen(mycenter.first, mycenter.second, _map);
                        if (res.first != -1 && res.second != -1)
                            return res;    
                    }
                    
                }
            }
        }
        return std::make_pair(-1,-1);
    }







    std::pair<int, int> queryWhereToHit(TA::Board _map) override
    {
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
        if (!ishit)
        {
            for (int i = 0; i < 20; i++)
            {
                for (int j = 0; j < 20; j++)
                {
                    if (_map[i][j] == TA::Board::State::Hit  && isdead(i, j))
                    {
                        ishit = true;
                        //explored.insert(std::make_pair(i,j));
                        mycenter = std::make_pair(i,j);
                        break;
                    }

                }
                if (ishit == true)
                    break;
            }
            
        }
        if (ishit)
        {
            auto res = getlen(mycenter.first, mycenter.second, _map);
            if (res.first != -1 && res.second != -1)
                return res;
        }

        while (1)
        {
            auto res = way.back();
            way.pop_back();
            if (_map[res.first][res.second] == TA::Board::State::Unknown && isdead(res.first, res.second))
                return res;
        }




    }

    void callbackReportHit(bool) override
    {

    }

    std::vector<std::pair<int, int>> queryHowToMoveShip(std::vector<TA::Ship>) override
    {
        return {};
    }
};
