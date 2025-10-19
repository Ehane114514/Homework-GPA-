#include <iostream>      // 标准输入输出流
#include <vector>        // 动态数组容器
#include <string>        // 字符串处理
#include <algorithm>     // 算法库（排序、查找等）
#include <random>        // 随机数生成
#include <ctime>         // 时间函数，用于设置随机数种子
#include <map>           // 映射容器，用于统计和存储键值对
#include <limits>        // 数值限制，用于输入验证
#ifdef _WIN32
#include <windows.h>     // Windows平台API，用于设置控制台编码
#endif

// 牌型枚举 - 德州扑克中的9种牌型，从低到高排列
// 每个牌型的数值越大，表示牌型越强
enum class HandRank { 
    HIGH_CARD = 0,          // 高牌 - 没有特殊组合，按单张牌大小比较
    ONE_PAIR,               // 一对 - 两张点数相同的牌
    TWO_PAIR,               // 两对 - 两组不同点数的对子
    THREE_OF_A_KIND,        // 三条 - 三张点数相同的牌
    STRAIGHT,               // 顺子 - 五张连续点数的牌
    FLUSH,                  // 同花 - 五张同一花色的牌
    FULL_HOUSE,             // 葫芦 - 三条加一对
    FOUR_OF_A_KIND,         // 四条 - 四张点数相同的牌
    STRAIGHT_FLUSH          // 同花顺 - 同一花色的顺子
};

// 花色枚举 - 扑克牌的四种花色
enum class Suit { 
    HEARTS,     // 红桃 ♥
    DIAMONDS,   // 方块 ♦
    CLUBS,      // 梅花 ♣
    SPADES      // 黑桃 ♠
};

// 点数枚举 - 扑克牌的点数，从2到A
enum class Rank { 
    TWO = 2,      // 2
    THREE,        // 3
    FOUR,         // 4
    FIVE,         // 5
    SIX,          // 6
    SEVEN,        // 7
    EIGHT,        // 8
    NINE,         // 9
    TEN,          // 10
    JOKER,        // J (Jack)
    QUEEN,        // Q (Queen)
    KING,         // K (King)
    ACE           // A (Ace) - 德州扑克中可以作为1或14使用
};

// 卡牌类 - 表示扑克牌中的一张牌
class Card {
private:
    Suit suit;    // 牌的花色
    Rank rank;    // 牌的点数

public:
    // 构造函数 - 初始化一张牌的花色和点数
    // 参数:
    //   s - 牌的花色
    //   r - 牌的点数
    Card(Suit s, Rank r) : suit(s), rank(r) {}

    // Getter方法 - 获取牌的花色
    // 返回值: 牌的花色枚举值
    Suit getSuit() const { 
        return suit; 
    }
    
    // Getter方法 - 获取牌的点数
    // 返回值: 牌的点数枚举值
    Rank getRank() const { 
        return rank; 
    }

    // 将牌转换为字符串表示（如"A♥"、"10♠"等）
    // 返回值: 格式化后的牌字符串，包含点数和花色符号
    std::string toString() const {
        std::string suitStr;  // 花色的字符串表示
        switch (suit) {
            case Suit::HEARTS: suitStr = "♥"; break;
            case Suit::DIAMONDS: suitStr = "♦"; break;
            case Suit::CLUBS: suitStr = "♣"; break;
            case Suit::SPADES: suitStr = "♠"; break;
        }

        std::string rankStr;  // 点数的字符串表示
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
            case Rank::JOKER: rankStr = "J"; break;
            case Rank::QUEEN: rankStr = "Q"; break;
            case Rank::KING: rankStr = "K"; break;
            case Rank::ACE: rankStr = "A"; break;
        }

        return rankStr + suitStr;  // 组合点数和花色
    }

    // 获取牌的数值表示，用于比较大小
    // 返回值: 牌的点数对应的整数值，用于牌型评估和大小比较
    int getValue() const {
        return static_cast<int>(rank);  // 将枚举转换为整数，方便数值比较
    }
};

// 牌堆类 - 表示一副完整的扑克牌
class Deck {
private:
    std::vector<Card> cards;  // 存储牌堆中的所有牌

public:
    // 构造函数 - 创建一副标准的52张扑克牌
    Deck() {
        // 创建所有花色和点数的组合（4种花色 × 13种点数 = 52张牌）
        for (int s = 0; s < 4; s++) {  // 遍历4种花色
            for (int r = 2; r <= 14; r++) {  // 遍历点数2到A(14)
                // 使用emplace_back直接在vector中构造Card对象，避免复制
                cards.emplace_back(static_cast<Suit>(s), static_cast<Rank>(r));
            }
        }
    }

    // 洗牌方法 - 使用随机数生成器打乱牌的顺序
    void shuffle() {
        std::random_device rd;       // 真随机数生成器
        std::mt19937 g(rd());        // Mersenne Twister随机数引擎
        std::shuffle(cards.begin(), cards.end(), g);  // 打乱牌的顺序
    }

    // 发牌方法 - 从牌堆顶部取出一张牌
    // 返回值: 从牌堆顶部取出的卡牌
    // 注意: 如果牌堆为空，会返回默认牌并输出错误信息
    Card dealCard() {
        if (cards.empty()) {  // 检查牌堆是否为空
            std::cerr << "Error: No more cards in the deck!" << std::endl;
            // 返回默认牌作为错误处理（实际应用中应该有更好的错误处理机制）
            return Card(Suit::HEARTS, Rank::TWO);
        }
        // 荷官发牌：从牌堆顶部（vector末尾）取牌
        Card card = cards.back();  // 获取牌堆顶部的牌
        cards.pop_back();          // 从牌堆中移除该牌
        return card;               // 返回这张牌
    }

    // 获取牌堆中剩余牌的数量
    // 返回值: 牌堆中剩余的牌数
    size_t size() const {
        return cards.size();
    }
};

// 玩家类 - 表示参与德州扑克游戏的玩家
class Player {
private:
    std::string name;          // 玩家名称
    std::vector<Card> hand;    // 玩家的手牌（两张）
    int chips;                 // 玩家的筹码数量
    int currentBet;            // 当前下注金额
    bool isInGame;             // 是否仍在本局游戏中
    bool isSmallBlind;         // 是否是小盲注
    bool isBigBlind;           // 是否是大盲注
    bool hasFolded;            // 是否已弃牌

public:
    // 构造函数 - 初始化玩家信息
    // 参数:
    //   n - 玩家名称
    //   startingChips - 初始筹码数量，默认为20000
    Player(const std::string& n, int startingChips = 20000) 
        : name(n), chips(startingChips), currentBet(0), 
          isInGame(false), isSmallBlind(false), isBigBlind(false), hasFolded(false) {}

    // Getter方法 - 获取玩家名称
    // 返回值: 玩家的名称
    const std::string& getName() const { 
        return name; 
    }
    
    // Getter方法 - 获取玩家手牌
    // 返回值: 玩家的两张手牌
    const std::vector<Card>& getHand() const { 
        return hand; 
    }
    
    // Getter方法 - 获取玩家筹码数量
    // 返回值: 玩家当前拥有的筹码数量
    int getChips() const { 
        return chips; 
    }
    
    // Getter方法 - 获取当前下注金额
    // 返回值: 玩家在当前下注轮中的下注金额
    int getCurrentBet() const { 
        return currentBet; 
    }
    
    // Getter方法 - 获取玩家是否在游戏中
    // 返回值: true表示玩家仍在游戏中，false表示已退出
    bool getIsInGame() const { 
        return isInGame; 
    }
    
    // Getter方法 - 获取是否为小盲注
    // 返回值: true表示玩家是小盲注，需要支付小盲注金额
    bool getIsSmallBlind() const { 
        return isSmallBlind; 
    }
    
    // Getter方法 - 获取是否为大盲注
    // 返回值: true表示玩家是大盲注，需要支付大盲注金额
    bool getIsBigBlind() const { 
        return isBigBlind; 
    }
    
    // Getter方法 - 获取是否已弃牌
    // 返回值: true表示玩家已弃牌，false表示玩家仍在参与当前回合
    bool getHasFolded() const { 
        return hasFolded; 
    }

    // Setter方法 - 设置玩家是否在游戏中
    // 参数: value - 设置玩家是否在游戏中的布尔值
    void setIsInGame(bool value) { 
        isInGame = value; 
    }
    
    // Setter方法 - 设置是否为小盲注
    // 参数: value - 设置玩家是否为小盲注的布尔值
    void setIsSmallBlind(bool value) { 
        isSmallBlind = value; 
    }
    
    // Setter方法 - 设置是否为大盲注
    // 参数: value - 设置玩家是否为大盲注的布尔值
    void setIsBigBlind(bool value) { 
        isBigBlind = value; 
    }
    
    // Setter方法 - 设置是否已弃牌
    // 参数: value - 设置玩家是否已弃牌的布尔值
    void setHasFolded(bool value) { 
        hasFolded = value; 
    }

    // 为玩家添加一张牌（用于发牌）
    // 参数: card - 要添加给玩家的卡牌
    void addCard(const Card& card) {
        hand.push_back(card);
    }

    // 下注方法 - 减少筹码并增加当前下注金额
    // 参数: amount - 下注金额
    // 返回值: 成功下注返回true，筹码不足返回false
    bool placeBet(int amount) {
        if (amount <= chips) {  // 检查筹码是否足够
            chips -= amount;     // 减少筹码
            currentBet += amount; // 增加当前下注
            return true;         // 下注成功
        }
        return false; // 筹码不足，下注失败
    }

    // 赢得筹码方法 - 增加玩家的筹码
    // 参数: amount - 赢得的筹码数量
    void winChips(int amount) {
        chips += amount;
    }

    // 重置手牌 - 清除手牌和相关状态，准备新的一手
    // 保留玩家筹码数量，仅重置与当前手牌相关的状态
    void resetHand() {
        hand.clear();         // 清除手牌
        currentBet = 0;       // 重置当前下注
        hasFolded = false;    // 重置弃牌状态
        isSmallBlind = false; // 重置小盲注状态
        isBigBlind = false;   // 重置大盲注状态
    }

    // 为新游戏重置 - 完全重置玩家状态
    // 准备玩家参与新的一局游戏
    void resetForNewGame() {
        resetHand();          // 重置手牌
        isInGame = true;      // 设置玩家为游戏中状态
    }

    // 显示玩家手牌和筹码
    // 将玩家的手牌和剩余筹码输出到控制台
    void displayHand() const {
        std::cout << name << "的手牌: ";
        for (const auto& card : hand) {
            std::cout << card.toString() << " ";
        }
        std::cout << "(筹码: " << chips << ")" << std::endl;
    }

    // 获取玩家手牌和公共牌的组合，用于评估牌型
    // 参数: communityCards - 游戏中的公共牌
    // 返回值: 组合了玩家手牌和公共牌的卡牌集合
    std::vector<Card> getCombinedCards(const std::vector<Card>& communityCards) const {
        std::vector<Card> combined = hand; // 先复制玩家手牌
        // 添加所有公共牌
        combined.insert(combined.end(), communityCards.begin(), communityCards.end());
        return combined; // 返回组合后的所有牌
    }
};

// 牌型评估命名空间 - 包含评估和比较扑克牌型的辅助函数
// 提供德州扑克中判断牌型、比较手牌大小的核心逻辑
namespace HandEvaluator {
    // 按点数排序（从大到小）
    // 参数: cards - 需要排序的卡牌集合
    void sortByRank(std::vector<Card>& cards) {
        std::sort(cards.begin(), cards.end(), [](const Card& a, const Card& b) {
            return a.getValue() > b.getValue();  // 按点数降序排列
        });
    }

    // 检查是否是顺子
    // 参数: cards - 需要检查的卡牌集合
    // 返回值: true表示卡牌中存在连续的5个不同点数，即顺子
    bool isStraight(const std::vector<Card>& cards) {
        if (cards.size() < 5) return false;  // 至少需要5张牌才能组成顺子
        
        std::vector<int> ranks;  // 存储所有牌的点数
        for (const auto& card : cards) {
            ranks.push_back(card.getValue());
        }
        std::sort(ranks.begin(), ranks.end());  // 按点数升序排列
        
        // 去重，避免重复的点数影响判断
        auto last = std::unique(ranks.begin(), ranks.end());
        ranks.erase(last, ranks.end());
        
        // 检查是否有连续的5个点数
        for (size_t i = 0; i <= ranks.size() - 5; i++) {
            bool straight = true;
            for (size_t j = 0; j < 4; j++) {
                // 检查相邻点数是否连续
                if (ranks[i + j + 1] != ranks[i + j] + 1) {
                    straight = false;
                    break;
                }
            }
            if (straight) return true;  // 找到连续的5个点数
        }
        
        // 特殊情况：A-2-3-4-5的顺子（最小的顺子，也称为"小顺子"或"轮盘顺子"）
        if (std::find(ranks.begin(), ranks.end(), 14) != ranks.end() && // A
            std::find(ranks.begin(), ranks.end(), 2) != ranks.end() &&  // 2
            std::find(ranks.begin(), ranks.end(), 3) != ranks.end() &&  // 3
            std::find(ranks.begin(), ranks.end(), 4) != ranks.end() &&  // 4
            std::find(ranks.begin(), ranks.end(), 5) != ranks.end()) {   // 5
            return true;
        }
        
        return false;  // 不是顺子
    }

    // 检查是否是同花（至少5张同花色的牌）
    // 参数: cards - 需要检查的卡牌集合
    // 返回值: true表示卡牌中存在至少5张相同花色的牌，即同花
    bool isFlush(const std::vector<Card>& cards) {
        if (cards.size() < 5) return false;  // 至少需要5张牌才能组成同花
        
        std::map<Suit, int> suitCount;  // 统计每种花色出现的次数
        for (const auto& card : cards) {
            suitCount[card.getSuit()]++;
            if (suitCount[card.getSuit()] >= 5) {  // 如果某一花色有5张或更多
                return true;
            }
        }
        return false;  // 不是同花
    }

    // 计算点数出现次数，返回排序后的点数列表
    // 排序规则：先按出现次数降序，次数相同时按点数降序
    // 参数: cards - 需要统计的卡牌集合
    // 返回值: 按照出现次数和点数排序后的点数列表
    std::vector<int> getRankCounts(const std::vector<Card>& cards) {
        std::map<int, int> rankCount;  // 统计每个点数出现的次数
        for (const auto& card : cards) {
            rankCount[card.getValue()]++;
        }
        
        // 创建次数-点数对的向量
        std::vector<std::pair<int, int>> countPairs;
        for (const auto& pair : rankCount) {
            countPairs.emplace_back(pair.second, pair.first);  // (出现次数, 点数)
        }
        
        // 按出现次数降序，点数降序排序
        std::sort(countPairs.begin(), countPairs.end(), [](const auto& a, const auto& b) {
            if (a.first != b.first) {
                return a.first > b.first;  // 先按次数降序
            }
            return a.second > b.second;  // 次数相同时按点数降序
        });
        
        // 构建结果向量：按排序后的顺序重复点数
        std::vector<int> result;
        for (const auto& pair : countPairs) {
            for (int i = 0; i < pair.first; i++) {
                result.push_back(pair.second);
            }
        }
        return result;
    }

    // 评估手牌，返回牌型和排序后的点数列表
    // 这是德州扑克中最核心的函数，用于判断玩家手牌的类型和强度
    // 参数: cards - 需要评估的卡牌集合（通常是玩家手牌+公共牌）
    // 返回值: 包含牌型枚举和排序后点数列表的pair
    std::pair<HandRank, std::vector<int>> evaluateHand(const std::vector<Card>& cards) {
        if (cards.size() < 5) {
            return {HandRank::HIGH_CARD, {}};  // 牌不足5张，返回高牌
        }
        
        // 计算关键特征：是否是顺子、是否是同花、点数统计
        bool straight = isStraight(cards);
        bool flush = isFlush(cards);
        std::vector<int> rankCounts = getRankCounts(cards);
        
        // 按照牌型等级从高到低判断（重要！德州扑克规则要求从最强牌型开始检查）
        // 同花顺 - 最强牌型，同一花色的顺子
        if (straight && flush) {
            return {HandRank::STRAIGHT_FLUSH, rankCounts};
        }
        
        // 四条
        if (rankCounts.size() >= 4 && rankCounts[0] == rankCounts[3]) {
            return {HandRank::FOUR_OF_A_KIND, rankCounts};
        }
        
        // 葫芦
        if (rankCounts.size() >= 5 && 
            (rankCounts[0] == rankCounts[2] && rankCounts[3] == rankCounts[4] || 
             rankCounts[0] == rankCounts[1] && rankCounts[2] == rankCounts[4])) {
            return {HandRank::FULL_HOUSE, rankCounts};
        }
        
        // 同花
        if (flush) {
            return {HandRank::FLUSH, rankCounts};
        }
        
        // 顺子
        if (straight) {
            return {HandRank::STRAIGHT, rankCounts};
        }
        
        // 三条
        if (rankCounts.size() >= 3 && rankCounts[0] == rankCounts[2]) {
            return {HandRank::THREE_OF_A_KIND, rankCounts};
        }
        
        // 两对
        if (rankCounts.size() >= 4 && 
            (rankCounts[0] == rankCounts[1] && rankCounts[2] == rankCounts[3])) {
            return {HandRank::TWO_PAIR, rankCounts};
        }
        
        // 一对
        if (rankCounts.size() >= 2 && rankCounts[0] == rankCounts[1]) {
            return {HandRank::ONE_PAIR, rankCounts};
        }
        
        // 高牌
        return {HandRank::HIGH_CARD, rankCounts};
    }

    // 获取牌型的中文名称
    // 参数: rank - 牌型枚举值
    // 返回值: 牌型的中文描述
    std::string getHandRankName(HandRank rank) {
        switch (rank) {
            case HandRank::STRAIGHT_FLUSH: return "同花顺";
            case HandRank::FOUR_OF_A_KIND: return "四条";
            case HandRank::FULL_HOUSE: return "葫芦";
            case HandRank::FLUSH: return "同花";
            case HandRank::STRAIGHT: return "顺子";
            case HandRank::THREE_OF_A_KIND: return "三条";
            case HandRank::TWO_PAIR: return "两对";
            case HandRank::ONE_PAIR: return "一对";
            case HandRank::HIGH_CARD: return "高牌";
            default: return "未知牌型";
        }
    }

    // 比较两个玩家的手牌大小
    // 返回值：正数表示player1赢，负数表示player2赢，0表示平局
    int compareHands(const Player& player1, const Player& player2, const std::vector<Card>& communityCards) {
        // 获取玩家手牌和公共牌的组合
        auto hand1 = player1.getCombinedCards(communityCards);
        auto hand2 = player2.getCombinedCards(communityCards);
        
        // 评估两个玩家的牌型
        auto eval1 = evaluateHand(hand1);
        auto eval2 = evaluateHand(hand2);
        
        // 先比较牌型等级（牌型大的赢）
        if (static_cast<int>(eval1.first) != static_cast<int>(eval2.first)) {
            return static_cast<int>(eval1.first) - static_cast<int>(eval2.first);
        }
        
        // 牌型相同，比较点数（从大到小依次比较）
        for (size_t i = 0; i < std::min(eval1.second.size(), eval2.second.size()); i++) {
            if (eval1.second[i] != eval2.second[i]) {
                return eval1.second[i] - eval2.second[i];
            }
        }
        
        return 0; // 平局
    }
}

// 德州扑克游戏类 - 管理整个德州扑克游戏的流程和规则
// 这是游戏的核心类，负责协调整个游戏过程，包括发牌、下注、比牌和筹码分配
class TexasHoldem {
private:
    Deck deck;                  // 游戏使用的牌堆
    std::vector<Player> players; // 游戏中的玩家列表
    std::vector<Card> communityCards; // 公共牌（最多5张）
    int pot;                    // 底池金额，所有玩家下注的筹码总和
    int currentRound;           // 当前轮次（0:pre-flop, 1:flop, 2:turn, 3:river）
    int dealerPosition;         // 庄家位置索引
    int smallBlindAmount;       // 小盲注金额（大盲注的一半）
    int bigBlindAmount;         // 大盲注金额
    int currentBetAmount;       // 当前最高下注金额
    int lastAggressorIndex;     // 最后一个加注的玩家索引

    // 获取活跃玩家数量（在游戏中且未弃牌的玩家）
    // 返回值: 当前仍在参与游戏且未弃牌的玩家数量
    int getActivePlayerCount() const {
        int count = 0;
        for (const auto& player : players) {
            if (player.getIsInGame() && !player.getHasFolded()) {
                count++;
            }
        }
        return count;
    }

    // 获取下一个活跃玩家的索引
    // 参数: currentIndex - 当前玩家索引
    // 返回值: 下一个活跃玩家的索引，如果没有活跃玩家则返回-1
    int getNextActivePlayerIndex(int currentIndex) const {
        int startIndex = currentIndex;
        do {
            currentIndex = (currentIndex + 1) % players.size(); // 循环遍历玩家
            if (players[currentIndex].getIsInGame() && !players[currentIndex].getHasFolded()) {
                return currentIndex; // 找到活跃玩家
            }
        } while (currentIndex != startIndex); // 如果绕了一圈又回到起点，则没有活跃玩家
        return -1; // 没有活跃玩家
    }

    // 处理玩家操作（弃牌/跟注/加注）
    // 这是游戏中玩家交互的核心方法
    // 参数:
    //   playerIndex - 当前玩家的索引
    //   maxBet - 当前最高下注金额的引用，用于更新
    void handlePlayerAction(int playerIndex, int& maxBet) {
        Player& player = players[playerIndex];
        if (!player.getIsInGame() || player.getHasFolded()) return; // 跳过不在游戏或已弃牌的玩家

        // 显示玩家信息，让玩家了解当前状态
        std::cout << "\n" << player.getName() << " 的回合（筹码: " << player.getChips() 
                  << ", 当前下注: " << player.getCurrentBet() << "）" << std::endl;
        std::cout << "请选择操作：" << std::endl;
        std::cout << "1. 弃牌" << std::endl;
        
        // 计算需要跟注的金额
        int toCall = maxBet - player.getCurrentBet();
        if (toCall > 0) {
            std::cout << "2. 跟注 (" << toCall << ")" << std::endl;
        }
        
        std::cout << "3. 加注" << std::endl;
        
        int choice;
        // 获取并验证用户输入
        while (true) {
            std::cout << "请输入选择 (1-3): ";
            if (!(std::cin >> choice)) { // 处理非数字输入
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "无效输入，请重新输入数字。" << std::endl;
                continue;
            }
            
            // 验证选择是否有效，避免当toCall<=0时选择跟注
            if (choice >= 1 && choice <= 3 && !(choice == 2 && toCall <= 0)) {
                // 清理输入缓冲区中的剩余字符
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            
            std::cout << "无效选择，请重新输入。" << std::endl;
            // 清理输入缓冲区
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        // 根据玩家选择执行相应操作
        switch (choice) {
            case 1: // 弃牌
                std::cout << player.getName() << " 选择弃牌。" << std::endl;
                player.setHasFolded(true);
                break;
                
            case 2: // 跟注
                if (player.placeBet(toCall)) { // 检查筹码是否足够
                    pot += toCall; // 增加底池
                    std::cout << player.getName() << " 选择跟注 " << toCall << "。" << std::endl;
                } else {
                    std::cout << "筹码不足，自动弃牌。" << std::endl;
                    player.setHasFolded(true);
                }
                break;
                
            case 3: // 加注
                int raiseAmount;
                // 计算最小加注金额（需要跟注的金额 + 大盲注）
                int minRaise = (toCall > 0 ? toCall : 0) + bigBlindAmount;
                
                // 获取并验证加注金额
                while (true) {
                    std::cout << "请输入加注金额（最小 " << minRaise << "，筹码: " << player.getChips() << "）: ";
                    if (!(std::cin >> raiseAmount)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "无效的加注金额，请重新输入。" << std::endl;
                        continue;
                    }
                    
                    if (raiseAmount < minRaise || raiseAmount > player.getChips()) {
                        std::cout << "无效的加注金额，请重新输入。" << std::endl;
                        // 清理输入缓冲区
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        continue;
                    }
                    
                    // 清理输入缓冲区中的剩余字符
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
                
                // 执行加注
                if (player.placeBet(toCall + raiseAmount)) { // 检查筹码是否足够
                    pot += (toCall + raiseAmount); // 增加底池
                    maxBet = player.getCurrentBet(); // 更新最高下注金额
                    lastAggressorIndex = playerIndex; // 更新最后加注者
                    std::cout << player.getName() << " 选择加注到 " << player.getCurrentBet() << "。" << std::endl;
                } else {
                    std::cout << "筹码不足，自动弃牌。" << std::endl;
                    player.setHasFolded(true);
                }
                break;
        }
    }

    // 进行一轮下注
    // 这是德州扑克游戏的核心环节，处理玩家之间的下注交互
    // 实现了标准的德州扑克下注规则，包括跟注、加注和弃牌
    // 参数:
    //   startPlayerIndex - 本轮下注开始的玩家索引
    void bettingRound(int startPlayerIndex) {
        // 如果只有一个或没有活跃玩家，无需下注
        if (getActivePlayerCount() <= 1) return;
        
        // 重置当前最高下注金额和最后加注者索引
        currentBetAmount = 0;
        lastAggressorIndex = -1;
        
        // 重置当前下注金额，考虑盲注或之前下注
        for (auto& player : players) {
            if (player.getIsInGame() && !player.getHasFolded()) {
                currentBetAmount = std::max(currentBetAmount, player.getCurrentBet());
            }
        }
        
        // 设置当前玩家和第一个玩家索引
        int currentPlayerIndex = startPlayerIndex;
        int firstPlayerIndex = startPlayerIndex;
        bool allCalled = false;
        
        // 下注主循环，直到所有玩家都完成了跟注或只剩一个玩家
        while (!allCalled) {
            // 处理当前玩家的操作
            handlePlayerAction(currentPlayerIndex, currentBetAmount);
            
            // 检查是否只有一个玩家剩余
            if (getActivePlayerCount() <= 1) break;
            
            // 检查是否所有玩家都已跟注到当前最高金额
            allCalled = true;
            for (const auto& player : players) {
                if (player.getIsInGame() && !player.getHasFolded() && player.getCurrentBet() < currentBetAmount) {
                    allCalled = false;
                    break;
                }
            }
            
            if (allCalled) break;
            
            // 获取下一个活跃玩家
            currentPlayerIndex = getNextActivePlayerIndex(currentPlayerIndex);
            
            // 如果回到第一个玩家且有玩家加注过，则从最后加注者的下一位继续下注轮
            // 这是德州扑克规则中的必要环节，确保每位玩家都有机会对新的加注做出反应
            if (currentPlayerIndex == firstPlayerIndex && lastAggressorIndex != -1) {
                firstPlayerIndex = getNextActivePlayerIndex(lastAggressorIndex);
                currentPlayerIndex = firstPlayerIndex;
            }
        }
    }

    // 进行比牌并分配筹码
    // 在游戏的最后阶段，所有未弃牌的玩家展示手牌进行比较
    // 实现了牌型评估、比较和筹码分配的完整逻辑
    void showdown() {
        // 获取所有活跃玩家（未弃牌）
        std::vector<int> remainingPlayers;
        for (size_t i = 0; i < players.size(); i++) {
            if (players[i].getIsInGame() && !players[i].getHasFolded()) {
                remainingPlayers.push_back(i);
            }
        }
        
        // 如果没有剩余玩家，输出提示
        if (remainingPlayers.empty()) {
            std::cout << "没有剩余玩家进行比牌。" << std::endl;
            return;
        }
        
        // 如果只有一个玩家剩余，直接赢取底池（没有对手）
        if (remainingPlayers.size() == 1) {
            // 只有一个玩家剩余，直接赢取底池
            int winnerIndex = remainingPlayers[0];
            players[winnerIndex].winChips(pot);
            std::cout << "\n" << players[winnerIndex].getName() << " 赢得了底池 " << pot << "！" << std::endl;
            return;
        }
        
        // 显示所有剩余玩家的手牌和牌型
        std::cout << "\n===== 比牌阶段 =====" << std::endl;
        for (int playerIndex : remainingPlayers) {
            players[playerIndex].displayHand();
            auto combinedCards = players[playerIndex].getCombinedCards(communityCards);
            auto eval = HandEvaluator::evaluateHand(combinedCards);
            std::cout << "牌型: " << HandEvaluator::getHandRankName(eval.first) << std::endl;
        }
        
        // 找出获胜者，使用冒泡排序的思路比较每个玩家的手牌
        int winnerIndex = remainingPlayers[0];
        bool tie = false;
        std::vector<int> winners = {winnerIndex};
        
        for (size_t i = 1; i < remainingPlayers.size(); i++) {
            // 比较当前玩家与已知最佳玩家的手牌
            int compareResult = HandEvaluator::compareHands(
                players[winnerIndex], 
                players[remainingPlayers[i]], 
                communityCards
            );
            
            if (compareResult < 0) {
                // 新的获胜者
                winnerIndex = remainingPlayers[i];
                winners = {winnerIndex};
                tie = false;
            } else if (compareResult == 0) {
                // 平局，添加到获胜者列表
                winners.push_back(remainingPlayers[i]);
                tie = true;
            }
        }
        
        // 分配底池，处理单人和多人获胜的情况
        if (tie) {
            // 平局情况下，平分底池
            int splitPot = pot / winners.size();
            std::cout << "\n平局！底池将平分给以下玩家：" << std::endl;
            for (int winIdx : winners) {
                players[winIdx].winChips(splitPot);
                std::cout << "- " << players[winIdx].getName() << " 获得 " << splitPot << std::endl;
            }
            // 处理余数（可能由于除法取整），将余数分配给第一个玩家
            int remainder = pot % winners.size();
            if (remainder > 0) {
                std::cout << "余数 " << remainder << " 归第一个玩家。" << std::endl;
                players[winners[0]].winChips(remainder);
            }
        } else {
            // 单人获胜情况
            players[winnerIndex].winChips(pot);
            std::cout << "\n" << players[winnerIndex].getName() << " 赢得了底池 " << pot << "！" << std::endl;
        }
    }

public:
    // 构造函数 - 初始化德州扑克游戏实例
    // 设置游戏的初始状态，包括底池、轮次、庄家位置和盲注金额
    // 同时设置随机数种子以确保随机性
    TexasHoldem() : pot(0), currentRound(0), dealerPosition(0), 
                   smallBlindAmount(50), bigBlindAmount(100), 
                   currentBetAmount(0), lastAggressorIndex(-1) {
        // 设置随机数种子
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }

    // 添加玩家（确保玩家数量在2-22之间）
    // 参数:
    //   player - 要添加到游戏中的玩家对象
    // 返回值:
    //   如果添加成功返回true，如果达到玩家数量上限返回false
    bool addPlayer(const Player& player) {
        if (players.size() >= 22) {
            std::cout << "达到最大玩家数量限制（22人）。" << std::endl;
            return false;
        }
        players.push_back(player);
        return true;
    }

    // 开始一局游戏
    // 这是游戏的核心方法，协调整个德州扑克游戏的进行
    // 处理发牌、下注、公共牌展示和最终比牌的完整流程
    void startGame() {
        // 检查玩家数量
        if (players.size() < 2) {
            std::cout << "玩家数量不足，至少需要2名玩家。" << std::endl;
            return;
        }

        std::cout << "\n===== 开始新的一局 =====" << std::endl;
        
        // 重置游戏状态
        pot = 0;
        currentRound = 0;
        communityCards.clear();
        deck = Deck();
        deck.shuffle();

        // 重置玩家状态
        for (auto& player : players) {
            player.resetForNewGame();
        }

        // 设置盲注
        int smallBlindIndex = (dealerPosition + 1) % players.size();
        int bigBlindIndex = (dealerPosition + 2) % players.size();
        
        players[smallBlindIndex].setIsSmallBlind(true);
        players[bigBlindIndex].setIsBigBlind(true);
        
        // 支付盲注
        if (players[smallBlindIndex].placeBet(smallBlindAmount)) {
            pot += smallBlindAmount;
            std::cout << players[smallBlindIndex].getName() << " 支付小盲注 " << smallBlindAmount << std::endl;
        }
        
        if (players[bigBlindIndex].placeBet(bigBlindAmount)) {
            pot += bigBlindAmount;
            std::cout << players[bigBlindIndex].getName() << " 支付大盲注 " << bigBlindAmount << std::endl;
        }

        // 发底牌（每个玩家两张）
        for (int i = 0; i < 2; i++) {
            for (auto& player : players) {
                if (player.getIsInGame()) {
                    player.addCard(deck.dealCard());
                }
            }
        }

        // 显示玩家手牌
        for (const auto& player : players) {
            if (player.getIsInGame()) {
                player.displayHand();
            }
        }

        // Pre-flop 下注轮
        std::cout << "\n===== Pre-flop 阶段 =====" << std::endl;
        int startPlayerIndex = getNextActivePlayerIndex(bigBlindIndex);
        bettingRound(startPlayerIndex);

        // 如果还有多个玩家，继续游戏
        if (getActivePlayerCount() > 1) {
            // Flop 阶段
            dealFlop();
            std::cout << "\n===== Flop 阶段 =====" << std::endl;
            bettingRound(bigBlindIndex);

            if (getActivePlayerCount() > 1) {
                // Turn 阶段
                dealTurn();
                std::cout << "\n===== Turn 阶段 =====" << std::endl;
                bettingRound(bigBlindIndex);

                if (getActivePlayerCount() > 1) {
                    // River 阶段
                    dealRiver();
                    std::cout << "\n===== River 阶段 =====" << std::endl;
                    bettingRound(bigBlindIndex);
                }
            }
        }

        // 比牌阶段
        showdown();

        // 更新庄家位置
        dealerPosition = (dealerPosition + 1) % players.size();
        
        std::cout << "\n===== 本局结束 =====" << std::endl;
    }

    // 发三张公共牌（flop）
    // 实现德州扑克中的翻牌阶段
    // 包含烧牌（burn card）操作，这是正规德州扑克的标准流程
    void dealFlop() {
        // 弃一张牌（烧牌）
        // 烧牌是为了防止作弊，增加游戏公平性
        deck.dealCard();
        
        // 发三张翻牌，这是德州扑克中第一个重要的牌局阶段
        for (int i = 0; i < 3; i++) {
            communityCards.push_back(deck.dealCard());
        }
        
        // 更新当前轮次为翻牌阶段
        currentRound = 1;
        // 显示公共牌，让所有玩家看到翻牌结果
        displayCommunityCards();
    }

    // 发第四张公共牌（turn）
    // 实现德州扑克中的转牌阶段
    void dealTurn() {
        // 弃一张牌，保持游戏公平性
        deck.dealCard();
        
        // 发转牌，游戏进入倒数第二个阶段
        communityCards.push_back(deck.dealCard());
        
        // 更新当前轮次为转牌阶段
        currentRound = 2;
        // 显示公共牌
        displayCommunityCards();
    }

    // 发第五张公共牌（river）
    // 实现德州扑克中的河牌阶段，这是最后一张公共牌
    void dealRiver() {
        // 弃一张牌，保持游戏公平性
        deck.dealCard();
        
        // 发河牌，游戏进入最终阶段
        communityCards.push_back(deck.dealCard());
        
        // 更新当前轮次为河牌阶段
        currentRound = 3;
        // 显示公共牌
        displayCommunityCards();
    }

    // 显示公共牌
    // 格式化输出当前的所有公共牌，方便玩家查看当前可用的公共牌
    void displayCommunityCards() const {
        std::cout << "公共牌: ";
        for (const auto& card : communityCards) {
            std::cout << card.toString() << " ";
        }
        std::cout << std::endl;
    }

    // 获取游戏状态摘要
    // 输出当前游戏的核心信息，包括底池大小、玩家数量和各玩家状态
    void displayGameStatus() const {
        std::cout << "\n===== 游戏状态 =====" << std::endl;
        std::cout << "底池: " << pot << std::endl;
        std::cout << "玩家数量: " << players.size() << std::endl;
        std::cout << "活跃玩家: " << getActivePlayerCount() << std::endl;
        
        std::cout << "\n玩家状态:" << std::endl;
        for (const auto& player : players) {
            std::cout << player.getName() << " - 筹码: " << player.getChips();
            if (player.getIsSmallBlind()) std::cout << " [小盲注]";
            if (player.getIsBigBlind()) std::cout << " [大盲注]";
            if (player.getHasFolded()) std::cout << " [已弃牌]";
            std::cout << std::endl;
        }
    }
};

// 主函数 - 程序入口点
// 负责初始化游戏环境、获取用户输入、创建游戏实例并启动游戏
int main() {
    // 搜了下，这玩意支持中文输出。
    // 但是为什么我不直接设计UI？
    #ifdef _WIN32
    SetConsoleOutputCP(65001); // 设置控制台输出为UTF-8编码
    SetConsoleCP(65001);       // 设置控制台输入为UTF-8编码
    #endif

    std::cout << "========================================" << std::endl;
    std::cout << "        欢迎来到德州扑克（赌博）游戏！          " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "规则说明：" << std::endl;
    std::cout << "1. 游戏支持2-10名玩家参与" << std::endl;
    std::cout << "2. 每个玩家初始筹码为20000" << std::endl;
    std::cout << "3. 小盲注50，大盲注100" << std::endl;
    std::cout << "4. 游戏分为Pre-flop、Flop、Turn、River四个阶段" << std::endl;
    std::cout << "========================================\n" << std::endl;

    // 创建德州扑克游戏实例
    TexasHoldem game;
    int playerCount;
    
    // 获取并验证玩家数量
    // 使用无限循环确保获取有效的玩家数量输入
    while (true) {
        std::cout << "请输入玩家数量（2-22）: ";
        std::cin >> playerCount;
        
        // 输入验证：检查是否为有效数字且在范围内
        if (std::cin.fail() || playerCount < 2 || playerCount > 22) {
            std::cin.clear();  // 清除错误状态
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // 清空输入缓冲区
            std::cout << "无效的玩家数量，请输入2-22之间的数字。\n" << std::endl;
        } else {
            std::cin.ignore(); // 清除输入缓冲区的换行符，避免影响后续的getline操作
            break;  // 获取到有效输入，退出循环
        }
    }
    
    // 简单添加玩家到游戏中，不使用循环和名称输入
    for (int i = 1; i <= playerCount; i++) {
        // 直接创建玩家对象并添加到游戏中，使用默认名称
        game.addPlayer(Player("玩家" + std::to_string(i)));
    }
    
    // 提示玩家添加完成，准备开始游戏
    std::cout << "\n欢乐时光要开始了...\n" << std::endl;
    
    // 游戏主循环：允许玩家进行多局游戏
    char continuePlaying = 'y';
    while (continuePlaying == 'y' || continuePlaying == 'Y') {
        // 启动一局完整的德州扑克游戏
        game.startGame();
        
        // 询问玩家是否继续下一局游戏
        std::cout << "\n开始下一局游戏？(y/n): ";
        std::cin >> continuePlaying;
        std::cin.ignore(); // 清除输入缓冲区的换行符
    }
    
    std::cout << "        结束咯            " << std::endl;

    return 0;
}