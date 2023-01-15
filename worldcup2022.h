#ifndef WORLDCUP2022_H
#define WORLDCUP2022_H

#include <memory>
#include <string>
#include <vector>
#include <list>
#include <iostream>

#include "worldcup.h"

#define STARTING_BALANCE 1000
#define MIN_PLAYERS 2
#define MAX_PLAYERS 11
#define DIES_NUMBER 2
#define START_BONUS 50
#define BOOKMAKER_WIN_FREQUENCY 3

class WorldCup2022 : public WorldCup {
private:
    class Player {
    private:
        const std::string name;
        unsigned int position = 0;
        unsigned int zdzislaws = STARTING_BALANCE;
        bool isBankrupt = false;
    public:
        int suspension = 0;

        explicit Player(std::string const &name) : name(name) {}

        std::string getName() {
            return name;
        }

        void move(unsigned int fields, unsigned int boardSize) {
            position = (position + fields) % boardSize;
        }

        void addMoney(unsigned int amount) {
            this->zdzislaws += amount;
        }

        bool substractMoney(unsigned int amount) {
            if (this->zdzislaws >= amount) {
                this->zdzislaws -= amount;
                return true;
            } else {
                this->isBankrupt = true;
                this->zdzislaws = 0;
                return false;
            }
        }

        unsigned int getMoney() {
            return zdzislaws;
        }

        unsigned int getPosition() {
            return this->position;
        }

        bool bankrupt() {
            return isBankrupt;
        }
    };

    class Field {
    private:
        std::string name;
    public:
        explicit Field(std::string const &name) : name(name) {}

        virtual ~Field() = default;

        virtual std::string getName() {
            return name;
        }

        virtual void onPlayerStop([[maybe_unused]] Player &player) {}
        virtual void onPlayerPass([[maybe_unused]] Player &player) {}
    };

    class SeasonBeginning : public Field {
    public:
        explicit SeasonBeginning(std::string const &name) : Field(name) {}

        void onPlayerStop(Player &player) override {
            player.addMoney(START_BONUS);
        }

        void onPlayerPass(Player &player) override {
            player.addMoney(START_BONUS);
        }
    };

    class Goal : public Field {
    private:
        const unsigned int bonus;
    public:
        explicit Goal(const std::string &name, unsigned int bonus) : 
                      Field(name), bonus(bonus) {}

        void onPlayerStop(Player &player) override {
            player.addMoney(bonus);
        }
    };

    class Penalty : public Field {
    private:
        const int savePrice;
    public:
        explicit Penalty(const std::string &name, const int savePrice) : 
                         Field(name), savePrice(savePrice) {}

        void onPlayerStop(Player &player) override {
            player.substractMoney(savePrice);
        }
    };

    class Bookmaker : public Field {
    private:
        const int betSize;
        int playersCount = 0;
    public:
        explicit Bookmaker(const std::string &name, const int betSize) : 
                           Field(name), betSize(betSize) {}

        void onPlayerStop(Player &player) override {
            if (playersCount == 0) {
                player.addMoney(betSize);
            } else {
                player.substractMoney(betSize);
            }
            playersCount = (playersCount + 1) % BOOKMAKER_WIN_FREQUENCY;
        }
    };

    class YellowCard : public Field {
    private:
        const int suspensionSize;
    public:
        explicit YellowCard(const std::string &name, const int suspensionSize) : 
                            Field(name), suspensionSize(suspensionSize) {}

        void onPlayerStop(Player &player) override {
            player.suspension += suspensionSize - 1;
        }
    };

    class Match : public Field {
    public:
        enum matchType {friendly, forPoints, final};

        Match(const std::string &name, matchType type, unsigned int fee) : 
              Field(name), fee(fee) {
            switch (type) {
                case friendly:
                    matchRate = 1;
                    break;
                case forPoints:
                    matchRate = 2.5;
                    break;
                case final:
                    matchRate = 4;
                    break;
            }
        }

        void onPlayerStop(Player &player) override {
            player.addMoney(playersPassed * fee * matchRate);
            playersPassed = 0;
        }

        //TODO: czy usuwać pieniądze przy bankrutowaniu
        void onPlayerPass(Player &player) override {
            if (player.substractMoney(fee)) {
                playersPassed++;
            }
        }

    private:
        const unsigned int fee;
        float matchRate;
        unsigned int playersPassed = 0;
    };

    class FreeDay : public Field {
    public:
        explicit FreeDay(const std::string &name) : Field(name) {}
    };

    class Board {
    private:
        std::vector<std::shared_ptr<Field>> fields;

    public:
        Board() = default;
        Board(std::initializer_list<std::shared_ptr<Field>> list) {
            for(auto &&field : list) {
                fields.push_back(field);
            }
        }

        [[maybe_unused]] void addField(const std::shared_ptr<Field> &field) {
            fields.push_back(field);
        }

        unsigned int size() {
            return fields.size();
        }

        [[nodiscard]] std::shared_ptr<Field> getField(unsigned int position) const {
            assert(position < fields.size());
            return fields[position];
        }
    };

    class Dies {
    private:
        std::vector<std::shared_ptr<Die>> dies;

    public:
        Dies() = default;

        [[maybe_unused]] void addDie(const std::shared_ptr<Die> &die) {
            dies.push_back(die);
        }

        unsigned int size() {
            return dies.size();
        }

        unsigned int roll() {
            unsigned int sum = 0;
            for (auto &&die : dies) {
                sum += die->roll();
            }
            return sum;
        }
    };

    class DefaultScoreboard : public ScoreBoard {
    public:
        virtual void onRound([[maybe_unused]] unsigned int roundNo) {}

        virtual void onTurn([[maybe_unused]] std::string const &playerName, 
                            [[maybe_unused]] std::string const &playerStatus,
                            [[maybe_unused]] std::string const &squareName, 
                            [[maybe_unused]] unsigned int money) {}

        virtual void onWin([[maybe_unused]] std::string const &playerName) {}
    };

    Dies dies;
    std::list<Player> players;
    std::shared_ptr<ScoreBoard> scoreboard = std::make_shared<DefaultScoreboard>();
    Board board;

    class TooManyDiceException : public std::exception {};
    class TooFewDiceException : public std::exception {};
    class TooManyPlayersException : public std::exception {};
    class TooFewPlayersException : public std::exception {};

    void checkDies() {
        if (dies.size() > DIES_NUMBER) {
            throw TooManyDiceException();
        }
        if (dies.size() < DIES_NUMBER) {
            throw TooFewDiceException();
        }
    }

    void checkPlayers() {
        if (players.size() > MAX_PLAYERS) {
            throw TooManyPlayersException();
        }
        if (players.size() < MIN_PLAYERS) {
            throw TooFewPlayersException();
        }
    }

    std::string movePlayer(Player *player, unsigned int fields) {
        unsigned int position = player->getPosition();
        for (unsigned int i = 1; i < fields; i++) {
            board.getField((position + i) % board.size())->onPlayerPass(*player);
        }
        player->move(fields, board.size());
        board.getField(player->getPosition())->onPlayerStop(*player);
        if (player->bankrupt()) {
            return "*** bankrut ***";                       
        }
        if (player->suspension > 0) {
            return "*** czekanie: " + std::to_string(player->suspension + 1) + " ***";
        }
        return "w grze";
    }

    std::string findWinner() {
        if (players.size() == 1) {
            return players.front().getName();
        } 
        unsigned int max_money = 0;
        std::string winnerName;
        for (Player p : players) {
            if (p.getMoney() > max_money) {
                max_money = p.getMoney();
                winnerName = p.getName();
            }
        }
        return winnerName;
    }

public:
    WorldCup2022() {
        this->board = Board({
            std::make_shared<SeasonBeginning>("Początek sezonu"),
            std::make_shared<Match>("Mecz z San Marino", Match::friendly, 160),
            std::make_shared<FreeDay>("Dzień wolny od treningu"),
            std::make_shared<Match>("Mecz z Lichtensteinem", Match::friendly, 220),
            std::make_shared<YellowCard>("Żółta kartka", 3),
            std::make_shared<Match>("Mecz z Meksykiem", Match::forPoints, 300),
            std::make_shared<Match>("Mecz z Arabią Saudyjską", Match::forPoints, 280),
            std::make_shared<Bookmaker>("Bukmacher", 100),
            std::make_shared<Match>("Mecz z Argentyną", Match::forPoints, 250),
            std::make_shared<Goal>("Gol", 120),
            std::make_shared<Match>("Mecz z Francją", Match::final, 400),
            std::make_shared<Penalty>("Karny", 180)
        });
    }

    void addDie(std::shared_ptr<Die> die) override {
        if (die != nullptr) dies.addDie(die);
    }

    void addPlayer(std::string const &name) override {
        players.emplace_back(name);
    }

    void setScoreBoard(std::shared_ptr<ScoreBoard> sb) override {
        this->scoreboard = sb;
    }

    void play(unsigned int rounds) override {
        checkDies();
        checkPlayers();
        for (unsigned int round = 0; round < rounds && players.size() > 1; round++) {
            scoreboard->onRound(round);
            std::string status;
            for (auto playerIt = players.begin(); playerIt != players.end();) {
                if (playerIt->suspension > 0) {
                    status = "*** czekanie: " + std::to_string(playerIt->suspension) + " ***";
                    playerIt->suspension--;
                } else {
                    status = movePlayer(&(*playerIt), dies.roll());
                    // do debugowania żeby nie wpisywać za każdym razem jak nie będzie działać
                    // std::cout << round << playerIt->getName() << " " << board.getField(playerIt->getPosition())->getName() << std::endl;
                }
                scoreboard->onTurn(playerIt->getName(), status, 
                        board.getField(playerIt->getPosition())->getName(),
                        playerIt->getMoney());

                if (playerIt->bankrupt()) {
                    players.erase(playerIt++);
                } else {
                    playerIt++;
                }

                // czemu to nie działa skoro to chyba to samo co ten if i else powyżej xD
                // if (playerIt->bankrupt()) players.erase(playerIt);   
                // playerIt++;             
            }
        }
        scoreboard->onWin(findWinner());
    };

};

#endif