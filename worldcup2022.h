#ifndef WORLDCUP2022_H
#define WORLDCUP2022_H

#include <memory>
#include <string>
#include <vector>

#include "worldcup.h"

#define STARTING_BALANCE 1000
#define MIN_PLAYERS 2
#define MAX_PLAYERS 11
#define DIES_NUMBER = 2

class WorldCup2022 : public WorldCup {
private:
    class Player {
    private:
        std::string name;
        unsigned int zdzislaws;
    };

    class Field {
    private:
        std::string name;
    public:
        Field(std::string const &name) : name(name) {}

        virtual ~Field() = default;

        virtual void onPlayerStop(Player player) = 0;
        
        virtual void onPlayerPass(Player player) = 0;
    };

    class SeasonBeginning : public Field {
    public:
        SeasonBeginning(std::string const &name) : Field(name) {}

        void onPlayerStop(Player player) override {

        }

        void onPlayerPass(Player player) override {

        }
    };

    class Goal : public Field {
    public:
        Goal(std::string name) : Field(name) {}

        void onPlayerStop(Player player) override {

        }

        void onPlayerPass(Player player) override {

        }
    };

    class Penalty : public Field {
    public:
        Penalty(std::string name) : Field(name) {}

        void onPlayerStop(Player player) override {

        }

        void onPlayerPass(Player player) override {

        }
    };

    class Bookmaker : public Field {
    public:
        Bookmaker(std::string name) : Field(name) {}

        void onPlayerStop(Player player) override {

        }

        void onPlayerPass(Player player) override {

        }

    };

    class YellowCard : public Field {
    public:
        YellowCard(std::string name) : Field(name) {}

        void onPlayerStop(Player player) override {

        }

        void onPlayerPass(Player player) override {

        }

    };  

    class Match : public Field {
    private:
        int zdzislaws;
    public:
        Match(std::string name) : Field(name) {}

        void onPlayerStop(Player player) override {

        }

        void onPlayerPass(Player player) override {

        }
    };

    class FreeDay : public Field {
    public:
        FreeDay(std::string name) : Field(name) {}

        void onPlayerStop(Player player) override {

        }

        void onPlayerPass(Player player) override {

        }

    };

    class Board {
    private:
        std::vector<Field> fields;
    public:
        Board() = default;
        Board(std::vector<Field> fields) : fields(fields) {}
    };

    std::vector<std::shared_ptr<Die>> dies;
    std::vector<Player> players;
    std::shared_ptr<ScoreBoard> scoreboard;
    Board board;

    void checkDice() const;
    void checkPlayers() const;


public:
    WorldCup2022() {
        this->board = Board({
            SeasonBeginning("Początek sezonu"),
            Match("Mecz z San Marino"),
            FreeDay("Dzień wolny od treningu"),
            Match("Mecz z Lichtensteinem"),
            YellowCard("Żółta kartka"),
            Match("Mecz z Meksykiem"),
            Match("Mecz z Arabią Saudyjską"),
            Bookmaker("Bukmacher"),
            Match("Mecz z Argentyną"),
            Goal("Gol"),
            Match("Mecz z Francją"),
            Penalty("Karny")
        });
    }
    virtual ~WorldCup2022() = default;

    // Jeżeli argumentem jest pusty wskaźnik, to nie wykonuje żadnej operacji
    // (ale nie ma błędu).
    virtual void addDie(std::shared_ptr<Die> die) = 0;

    // Dodaje nowego gracza o podanej nazwie.
    virtual void addPlayer(std::string const &name) = 0;

    // Konfiguruje tablicę wyników. Domyślnie jest skonfigurowana tablica
    // wyników, która nic nie robi.
    virtual void setScoreBoard(std::shared_ptr<ScoreBoard> scoreboard) = 0;

    // Przeprowadza rozgrywkę co najwyżej podanej liczby rund (rozgrywka może
    // skończyć się wcześniej).
    // Jedna runda obejmuje po jednym ruchu każdego gracza.
    // Gracze ruszają się w kolejności, w której zostali dodani.
    // Na początku każdej rundy przekazywana jest informacja do tablicy wyników
    // o początku rundy (onRound), a na zakończenie tury gracza informacje
    // podsumowujące dla każdego gracza (onTurn).
    // Rzuca TooManyDiceException, jeśli jest zbyt dużo kostek.
    // Rzuca TooFewDiceException, jeśli nie ma wystarczającej liczby kostek.
    // Rzuca TooManyPlayersException, jeśli jest zbyt dużo graczy.
    // Rzuca TooFewPlayersException, jeśli liczba graczy nie pozwala na
    // rozpoczęcie gry.
    // Wyjątki powinny dziedziczyć po std::exception.
    virtual void play(unsigned int rounds) = 0;

    // Konfiguruje tablicę wyników. Domyślnie jest skonfigurowana tablica
    // wyników, która nic nie robi.
    virtual void setScoreBoard(std::shared_ptr<ScoreBoard> scoreboard) {

    }

    // Dodaje nowego gracza o podanej nazwie.
    virtual void addPlayer(std::string const &name) {
        
    }
};

#endif