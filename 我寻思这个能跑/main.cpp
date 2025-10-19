#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

// 卡牌类定义
enum class Suit { HEARTS, DIAMONDS, CLUBS, SPADES };
enum class Rank { TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };

class Card {
private:
    Suit suit;
    Rank rank;

public:
    Card(Suit s, Rank r) : suit(s), rank(r) {}

    Suit getSuit() const { return suit; }
    Rank getRank() const { return rank; }

    std::string toString() const {
        std::string suitStr;
        switch (suit) {
            case Suit::HEARTS: suitStr = "♥"; break;
            case Suit::DIAMONDS: suitStr = "♦"; break;
            case Suit::CLUBS: suitStr = "♣"; break;
            case Suit::SPADES: suitStr = "♠"; break;
        }

        std::string rankStr;
        switch (rank) {
            case Rank::TWO: rankStr = "2"; break;
            case Rank::THREE: rankStr = "3"; break;
            case Rank::FOUR: rankStr = "4"; break;
            case Rank::FIVE: rankStr = "5"; break;
            case Rank::SIX: rankStr = "6"; break;
            case Rank::SEVEN: rankStr = "7"; break;
            case Rank::EIGHT: rankStr = "8"; break;
            case Rank::NINE: rankStr = "9"; break;
            case Rank::TEN: rankStr = "10"; break;
            case Rank::JACK: rankStr = "J"; break;
            case Rank::QUEEN: rankStr = "Q"; break;
            case Rank::KING: rankStr = "K"; break;
            case Rank::ACE: rankStr = "A"; break;
        }

        return rankStr + suitStr;
    }

    // 获取牌的数值（用于比较大小）
    int getValue() const {
        return static_cast<int>(rank);
    }
};

// 牌堆类
class Deck {
private:
    std::vector<Card> cards;

public:
    Deck() {
        // 创建一副52张牌
        for (int s = 0; s < 4; s++) {
            for (int r = 2; r <= 14; r++) {
                cards.emplace_back(static_cast<Suit>(s), static_cast<Rank>(r));
            }
        }
    }

    // 洗牌
    void shuffle() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(cards.begin(), cards.end(), g);
    }

    // 发牌
    Card dealCard() {
        if (cards.empty()) {
            std::cerr << "Error: No more cards in the deck!" << std::endl;
            // 返回一个默认牌（实际应用中应该处理这种情况）
            return Card(Suit::HEARTS, Rank::TWO);
        }
        Card card = cards.back();
        cards.pop_back();
        return card;
    }

    // 获取剩余牌数
    size_t size() const {
        return cards.size();
    }
};

// 玩家类
class Player {
private:
    std::string name;
    std::vector<Card> hand;
    int chips;
    int currentBet;

public:
    Player(const std::string& n, int startingChips = 20000) : name(n), chips(startingChips), currentBet(0) {}

    const std::string& getName() const { return name; }
    const std::vector<Card>& getHand() const { return hand; }
    int getChips() const { return chips; }
    int getCurrentBet() const { return currentBet; }

    void addCard(const Card& card) {
        hand.push_back(card);
    }

    void placeBet(int amount) {
        if (amount <= chips) {
            chips -= amount;
            currentBet += amount;
        }
    }

    void winChips(int amount) {
        chips += amount;
    }

    void resetHand() {
        hand.clear();
        currentBet = 0;
    }

    void displayHand() const {
        std::cout << name << "'s hand: ";
        for (const auto& card : hand) {
            std::cout << card.toString() << " ";
        }
        std::cout << std::endl;
    }
};

// 游戏类
class TexasHoldem {
private:
    Deck deck;
    std::vector<Player> players;
    std::vector<Card> communityCards;
    int pot;
    int currentRound;

public:
    TexasHoldem() : pot(0), currentRound(0) {
        // 设置随机数种子
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }

    void addPlayer(const Player& player) {
        players.push_back(player);
    }

    void startGame() {
        // 重置游戏状态
        pot = 0;
        currentRound = 0;
        communityCards.clear();
        deck = Deck();
        deck.shuffle();

        // 清空玩家手牌
        for (auto& player : players) {
            player.resetHand();
        }

        // 发底牌（每个玩家两张）
        for (int i = 0; i < 2; i++) {
            for (auto& player : players) {
                player.addCard(deck.dealCard());
            }
        }

        // 显示玩家手牌
        for (const auto& player : players) {
            player.displayHand();
        }

        // 这里可以继续实现下注轮次、发公共牌等逻辑
        std::cout << "游戏开始！\n";
    }

    void dealFlop() {
        if (currentRound != 0) return;
        
        // 弃一张牌（烧牌）
        deck.dealCard();
        
        // 发三张翻牌
        for (int i = 0; i < 3; i++) {
            communityCards.push_back(deck.dealCard());
        }
        
        currentRound = 1;
        displayCommunityCards();
    }

    void dealTurn() {
        if (currentRound != 1) return;
        
        // 弃一张牌
        deck.dealCard();
        
        // 发转牌
        communityCards.push_back(deck.dealCard());
        
        currentRound = 2;
        displayCommunityCards();
    }

    void dealRiver() {
        if (currentRound != 2) return;
        
        // 弃一张牌
        deck.dealCard();
        
        // 发河牌
        communityCards.push_back(deck.dealCard());
        
        currentRound = 3;
        displayCommunityCards();
    }

    void displayCommunityCards() const {
        std::cout << "公共牌: ";
        for (const auto& card : communityCards) {
            std::cout << card.toString() << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    std::cout << "欢迎来到德州扑克游戏！\n";

    TexasHoldem game;
    
    // 添加玩家
    game.addPlayer(Player("玩家1"));
    game.addPlayer(Player("玩家2"));
    
    // 开始游戏
    game.startGame();
    
    // 模拟游戏流程
    game.dealFlop();
    game.dealTurn();
    game.dealRiver();
    
    std::cout << "一局游戏结束！\n";

    return 0;
}