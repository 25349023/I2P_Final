#pragma once

#include <BattleShipGame/Board.h>
#include <BattleShipGame/Wrapper/AI.h>
#include <GUI/GUIInterface.h>

#include <iostream>
#include <cassert>
#include <chrono>
#include <cstdarg>
#include <future>
#include <type_traits>

namespace TA
{
    class BattleShipGame
    {
    public:
        BattleShipGame(
            int size,
            std::chrono::milliseconds runtime_limit = std::chrono::milliseconds(1000)
        ):
            m_size(size),
            m_runtime_limit(runtime_limit),
            m_P1(nullptr),
            m_P2(nullptr),
            m_offensive(nullptr),
            m_turn("P1"),
            m_OffenseShip(nullptr),
            m_DefenseShip(nullptr),
            m_P1Board(size),
            m_P2Board(size),
            m_OffenseBoard(nullptr),
            m_DefenseBoard(nullptr)
        {
            gui = new ASCII;
            m_ship_size = {3,3,5,7};
        }

        void setPlayer1(AIInterface *ptr) { assert(checkAI(ptr)); m_P1 = ptr; }
        void setPlayer2(AIInterface *ptr) { assert(checkAI(ptr)); m_P2 = ptr; }

        void run()
        {
            gui->title();
            if( !prepareState() ) return ;
            m_offensive = m_P1;
            m_OffenseShip = &m_P1Ship;
            m_DefenseShip = &m_P2Ship;
            m_OffenseBoard = &m_P1Board;
            m_DefenseBoard = &m_P2Board;

            while (true) {
                updateGuiGame();

                /// m_offensive->callbackReportEnemy(m_last_attack);
                call(&AIInterface::callbackReportEnemy, m_offensive, m_last_attack);
                m_last_attack.clear();

                for (auto &ship : *m_OffenseShip){
                    if (ship.state != Ship::State::Sink){
                        bool hit = false;
                        /// hitPosition = m_offensive->queryWhereToHit(*m_DefenseBoard);
                        auto [hitX, hitY] = call(&AIInterface::queryWhereToHit, m_offensive, *m_DefenseBoard);

                        if (!within(hitX, hitY, 0, 0, m_size)){
                            putToGui((m_turn + " Lose: Position out of board.\n").c_str());
                            return;
                        }
                        if ((*m_DefenseBoard)[hitX][hitY] != Board::State::Unknown){
                            putToGui((m_turn + " Lose: Attack position already hit.\n").c_str());
                            return;
                        }

                        putToGui((m_turn + " Hit (%d, %d)\n").c_str(), hitX, hitY);
                        for (auto &defShip : *m_DefenseShip){
                            if (within(hitX, hitY, defShip.x, defShip.y, defShip.size)){
                                if (hitX == defShip.x + defShip.size / 2 &&
                                    hitY == defShip.y + defShip.size / 2){
                                    defShip.state = Ship::State::Sink;
                                    putToGui((m_turn + " Sank a Ship.\n").c_str());
                                }
                                else if (defShip.state != Ship::State::Sink){
                                    defShip.state = Ship::State::Hit;
                                    putToGui((m_turn + " Hit a Ship.\n").c_str());
                                }
                                hit = true;
                                (*m_DefenseBoard)[hitX][hitY] = Board::State::Hit;
                                break;
                            }
                        }
                        if (!hit){
                            (*m_DefenseBoard)[hitX][hitY] = Board::State::Empty;
                        }
                        m_last_attack.emplace_back(hitX, hitY);

                        /// m_offensive->callbackReportHit(hit);
                        call(&AIInterface::callbackReportHit, m_offensive, hit);

                        updateGuiGame();
                        if (checkGameover()){
                            return;
                        }
                    }
                }

                /// m_offensive->queryHowToMoveShip(*m_OffenseShip);
                auto movePath = call(&AIInterface::queryHowToMoveShip, m_offensive, *m_OffenseShip);
                if (!movePath.empty() && movePath.size() != m_ship_size.size()){
                    putToGui((m_turn + " Lose: # of moving doesn't match.\n").c_str());
                    return;
                }
                for (int i = 0; i < static_cast<int>(movePath.size()); i++){
                    auto [dx, dy] = movePath[i];
                    if (std::abs(dx) + std::abs(dy) > 1){
                        putToGui((m_turn + " Lose: Invalid move: too much.\n").c_str());
                        return;
                    }
                    auto &ship = (*m_OffenseShip)[i];
                    if (ship.state == Ship::State::Available){
                        ship.x += dx;
                        ship.y += dy;
                    }
                    else {
                        if (dx != 0 || dy != 0){
                            putToGui((m_turn + " Lose: Invalid move: moving hit ship.\n").c_str());
                            return;
                        }
                    }
                }
                if (!checkShipPosition(*m_OffenseShip, *m_OffenseBoard)){
                    putToGui((m_turn + " Lose: Invalid move.\n").c_str());
                    return;
                }

                if (m_turn == "P1"){
                    m_turn = "P2";
                    m_offensive = m_P2;
                    m_OffenseShip = &m_P2Ship;
                    m_DefenseShip = &m_P1Ship;
                    m_OffenseBoard = &m_P2Board;
                    m_DefenseBoard = &m_P1Board;
                }
                else {
                    m_turn = "P1";
                    m_offensive = m_P1;
                    m_OffenseShip = &m_P1Ship;
                    m_DefenseShip = &m_P2Ship;
                    m_OffenseBoard = &m_P1Board;
                    m_DefenseBoard = &m_P2Board;
                }

                using namespace std::chrono_literals;
                std::this_thread::sleep_for(0.1s);
            }
            //Todo: Play Game
        } 

   private:
        bool within(int x, int y, int x0, int y0, int size){
            return x >= x0 && x < x0 + size && y >= y0 && y < y0 + size;
        }

        void updateGuiGame()
        {
            gui->updateGame(m_P1Board, m_P1Ship, m_P2Board, m_P2Ship);
        }

        bool checkGameover()
        {
            bool flag = true;

            for( Ship s : m_P1Ship )
                if( s.state != Ship::State::Sink )
                    flag = false;
            if( flag )
            {
                putToGui("P2 win!\n");
                return true;
            }
            for( Ship s : m_P2Ship )
                if( s.state != Ship::State::Sink )
                    return false;
            putToGui("P1 win!\n");
            return true;
        }

        bool prepareState()
        {
            std::vector<Ship> initPos;

            putToGui("P1 Prepareing...\n");
            initPos = call(&AIInterface::init, m_P1, m_size, m_ship_size, true, m_runtime_limit);
            if( !checkShipPosition(initPos, m_P1Board) )
            {
                putToGui("P1 Init() Invaild...\n");
                return false;
            }
            for( auto &ship : initPos )
                ship.state = Ship::State::Available;
            m_P1Ship = initPos;
            putToGui("Done.\n");

            initPos.clear();

            putToGui("P2 Prepareing...\n");
            initPos = call(&AIInterface::init, m_P2, m_size, m_ship_size, false, m_runtime_limit);
            if( !checkShipPosition(initPos, m_P2Board) )
            {
                putToGui("P2 Init() Invaild...\n");
                return false;
            }
            for( auto &ship : initPos )
                ship.state = Ship::State::Available;
            m_P2Ship = initPos;
            putToGui("Done.\n");
            return true;
        }

        template<typename Func ,typename... Args, 
            std::enable_if_t< std::is_void<
                    std::invoke_result_t<Func, AIInterface, Args...>
                >::value , int> = 0 >
        void call(Func func, AIInterface *ptr, Args... args)
        {
            std::future_status status;
            auto val = std::async(std::launch::async, func, ptr, args...);
            status = val.wait_for(std::chrono::milliseconds(m_runtime_limit));

            if( status != std::future_status::ready )
            {
                putToGui((m_turn + "Lose: Timeout\n").c_str());
                exit(-1);
            }
            val.get();
        }

        template<typename Func ,typename... Args, 
            std::enable_if_t< std::is_void<
                    std::invoke_result_t<Func, AIInterface, Args...>
                >::value == false, int> = 0 >
        auto call(Func func, AIInterface *ptr, Args... args)
            -> std::invoke_result_t<Func, AIInterface, Args...>
        {
            std::future_status status;
            auto val = std::async(std::launch::async, func, ptr, args...);
            status = val.wait_for(std::chrono::milliseconds(m_runtime_limit));

            if( status != std::future_status::ready )
            {
                putToGui((m_turn + "Lose: Timeout\n").c_str());
                exit(-1);
            }
            return val.get();
        }

        void putToGui(const char *fmt, ...)
        {
            va_list args1;
            va_start(args1, fmt);
            va_list args2;
            va_copy(args2, args1);
            std::vector<char> buf(1+std::vsnprintf(nullptr, 0, fmt, args1));
            va_end(args1);
            std::vsnprintf(buf.data(), buf.size(), fmt, args2);
            va_end(args2);

            if( buf.back() == 0 ) buf.pop_back();
            gui->appendText( std::string(buf.begin(), buf.end()) );
        }

        bool checkAI(AIInterface *ptr) 
        {
            return ptr->abi() == AI_ABI_VER;
        }

        bool checkShipPosition(std::vector<Ship> ships, Board board) {

            if (ships.size() != m_ship_size.size()){
                putToGui("Ship number not match : real %zu ,expect %zu,\n", ships.size(), m_ship_size.size());
                return false;
            }

            std::sort(ships.begin(), ships.end(), [](Ship a, Ship b) {
                return a.size < b.size;
            });

            std::vector<std::vector<int>> map(m_size, std::vector<int>(m_size));

            int id = -1;
            for (auto[size, x, y, state] : ships){
                id++;
                if (size != m_ship_size[id]){
                    putToGui("Ship %d size not match : real %zu ,expect %zu,\n", id, size, m_ship_size[id]);
                    return false;
                }

                for (int dx = 0; dx < size; dx++){
                    for (int dy = 0; dy < size; dy++){
                        int nx = x + dx;
                        int ny = y + dy;

                        if (nx < 0 || nx >= m_size || ny < 0 || ny >= m_size){
                            putToGui("Ship %d out of range at (%d,%d),\n", id, nx, ny);
                            return false;
                        }

                        if (map[nx][ny] != 0){
                            putToGui("Ship %d and %d overlapping at (%d,%d),\n", id, map[nx][ny] - 1, nx, ny);
                            return false;
                        }
                        map[nx][ny] = id + 1;

                        if (state == Ship::State::Available && board[nx][ny] != Board::State::Unknown){
                            putToGui("Ship %d move onto already hit point (%d,%d).\n", id, nx, ny);
                            return false;
                        }
                    }
                }
            }
            return true;
        }

        int m_size;
        std::vector<int> m_ship_size;
        std::chrono::milliseconds m_runtime_limit;

        std::vector<std::pair<int, int>> m_last_attack;

        AIInterface *m_P1;
        AIInterface *m_P2;
        AIInterface *m_offensive;
        GUIInterface *gui;
        std::string m_turn;

        std::vector<Ship> m_P1Ship;
        std::vector<Ship> m_P2Ship;
        std::vector<Ship> *m_OffenseShip;
        std::vector<Ship> *m_DefenseShip;
        Board m_P1Board;
        Board m_P2Board;
        Board *m_OffenseBoard;
        Board *m_DefenseBoard;
    } ;
}
