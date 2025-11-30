#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <ctime>

// --- Professional C++ Game Logic Implementation ---
// This file demonstrates how the core logic of "Neon Tycoon" would be implemented
// in a high-performance C++ backend. While the web version runs on JavaScript,
// this code serves as a blueprint for the game's architecture.

namespace NeonTycoon {

    // Configuration Constants
    const double PRICE_GROWTH_RATE = 1.15;
    const double DISCOUNT_RATE = 0.9;
    const double AUTO_CLICK_BONUS = 0.05;

    // Structure representing a Building definition
    struct BuildingDef {
        int id;
        std::string name;
        double baseCost;
        double income;
        std::string icon;
    };

    // Structure representing a Skill definition
    struct SkillDef {
        std::string id;
        std::string name;
        int cost; // in Neural Data
        std::string description;
    };

    // Database of game items
    class GameData {
    public:
        static const std::vector<BuildingDef>& getBuildings() {
            static const std::vector<BuildingDef> buildings = {
                {0, "Data Miner", 15, 1, "💾"},
                {1, "Bot Network", 100, 5, "🤖"},
                {2, "Server Rack", 1100, 22, "🔋"},
                {3, "AI Cluster", 12000, 85, "🧠"},
                {4, "Quantum Core", 130000, 350, "⚛️"},
                {5, "Dyson Swarm", 1500000, 1500, "☀️"},
                {6, "Reality Engine", 25000000, 8000, "🌀"}
            };
            return buildings;
        }

        static const std::vector<SkillDef>& getSkills() {
            static const std::vector<SkillDef> skills = {
                {"auto_click", "Auto-Clicker", 5, "Automatic clicks 1x/sec"},
                {"cheaper", "Optimization", 10, "Buildings are 10% cheaper"},
                {"offline", "Deep Sleep", 15, "Offline earnings x2"},
                {"hack_freq", "Backdoor", 25, "Hacks appear more frequently"}
            };
            return skills;
        }
    };

    // Main Game Engine Class
    class GameEngine {
    private:
        double money;
        long long neuralData;
        std::vector<int> buildingCounts;
        std::map<std::string, bool> ownedSkills;

        long long startTime;
        long long lastSaveTime;

        // Statistics
        long long totalClicks;
        double totalEarnings;

        // Multipliers
        double incomeMultiplier;
        double globalIncomePerSec;

    public:
        GameEngine() : money(0), neuralData(0), startTime(std::time(nullptr)),
                       totalClicks(0), totalEarnings(0), incomeMultiplier(1.0) {
            buildingCounts.resize(GameData::getBuildings().size(), 0);
        }

        // --- Core Economy Logic ---

        void update(double deltaTime) {
            calculateIncomePerSec();
            if (globalIncomePerSec > 0) {
                addMoney(globalIncomePerSec * deltaTime);
            }
        }

        void calculateIncomePerSec() {
            double baseIncome = 0;
            const auto& db = GameData::getBuildings();

            for (size_t i = 0; i < db.size(); ++i) {
                if (i < buildingCounts.size()) {
                    baseIncome += db[i].income * buildingCounts[i];
                }
            }

            // Apply Skills
            if (hasSkill("auto_click")) {
                baseIncome += (baseIncome * AUTO_CLICK_BONUS);
            }

            globalIncomePerSec = baseIncome * incomeMultiplier;
        }

        void addMoney(double amount) {
            money += amount;
            totalEarnings += amount;
        }

        // --- User Actions ---

        void click() {
            totalClicks++;
            double clickValue = 1 + (globalIncomePerSec * 0.05); // Dynamic click value
            addMoney(clickValue);
        }

        bool buyBuilding(int buildingId) {
            if (buildingId < 0 || buildingId >= (int)GameData::getBuildings().size()) return false;

            double cost = getBuildingCost(buildingId);
            if (money >= cost) {
                money -= cost;
                buildingCounts[buildingId]++;
                return true; // Success
            }
            return false; // Not enough funds
        }

        bool buySkill(const std::string& skillId) {
            if (ownedSkills[skillId]) return false; // Already owned

            for (const auto& skill : GameData::getSkills()) {
                if (skill.id == skillId) {
                    if (neuralData >= skill.cost) {
                        neuralData -= skill.cost;
                        ownedSkills[skillId] = true;
                        return true;
                    }
                }
            }
            return false;
        }

        // --- Helper Calculation Methods ---

        double getBuildingCost(int buildingId) const {
            const auto& b = GameData::getBuildings()[buildingId];
            double growth = std::pow(PRICE_GROWTH_RATE, buildingCounts[buildingId]);
            double discount = hasSkill("cheaper") ? DISCOUNT_RATE : 1.0;

            return std::floor(b.baseCost * growth * discount);
        }

        bool hasSkill(const std::string& skillId) const {
            auto it = ownedSkills.find(skillId);
            return (it != ownedSkills.end() && it->second);
        }

        // --- Prestige System ---

        long long calculatePrestigePotential() const {
            if (money < 1000000) return 0;
            return (long long)std::floor(std::sqrt(money / 1000000));
        }

        void doPrestige() {
            long long potential = calculatePrestigePotential();
            if (potential > 0) {
                neuralData += potential;
                // Reset economy
                money = 0;
                std::fill(buildingCounts.begin(), buildingCounts.end(), 0);
                incomeMultiplier = 1.0;
                // Keep skills and neuralData
            }
        }

        // --- Getters for UI ---
        double getMoney() const { return money; }
        double getIncomePerSec() const { return globalIncomePerSec; }
        long long getNeuralData() const { return neuralData; }
    };
}

int main() {
    // Simple test driver
    NeonTycoon::GameEngine game;

    std::cout << "Initializing Neon Tycoon Core..." << std::endl;
    std::cout << "Money: " << game.getMoney() << std::endl;

    game.click();
    std::cout << "Clicked! Money: " << game.getMoney() << std::endl;

    // Simulate buying
    if (game.buyBuilding(0)) {
        std::cout << "Bought building!" << std::endl;
    } else {
        std::cout << "Need more money for building." << std::endl;
    }

    return 0;
}
