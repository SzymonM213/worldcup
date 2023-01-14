#ifndef WORLDCUP2022_H
#define WORLDCUP2022_H

#include <memory>
#include <string>
#include <vector>

#include "worldcup.h"

#define STARTING_BALANCE 1000
#define MIN_PLAYERS 2
#define MAX_PLAYERS 11

// Reprezentuje interfejs gry WorldCup.
// Konkretną implementacją jest WorldCup2022.
// WorldCup2022 ma mieć konstruktor bezparametrowy.
class WorldCup2022 : public WorldCup {
private:
    std::vector<std::shared_ptr<Die>> dies;
    std::vector<std::string> players;
    std::shared_ptr<ScoreBoard> scoreboard;

    void checkDice() const;
    void checkPlayers() const;

    class Player {
    private:
        std::string name;
        unsigned int position;
        unsigned int zdzislaws;
    };

    class Field {
    private:
        std::string name;
    public:
        virtual ~Field() = default;

        virtual void onPlayerStop(Player player) = 0;
        
        virtual void onPlayerPass(Player player) = 0;
    };

    class SeasonBeginning : public Field {

    };

    class Goal : public Field {

    };

    class Penalty : public Field {

    };

    class Bookmaker : public Field {

    };

    class YellowCard : public Field {

    };

    class Match : public Field {

    };

public:
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