#pragma once

using namespace std;
using namespace geode::prelude;

#define STAR_EXP 5
#define MOON_EXP 5
#define DIAMOND_EXP 2
#define GOLD_COIN_EXP 100
#define USER_COIN_EXP 20
#define DEMON_EXP 75
#define CP_EXP 5000

class LevelHelper {
    public:
        inline static int calculateTotalEXP(GJUserScore* score) {
            int totalEXP = 0;
            totalEXP += (STAR_EXP * score->m_stars);
            totalEXP += (MOON_EXP * score->m_moons);
            totalEXP += (DIAMOND_EXP * score->m_diamonds);
            totalEXP += (GOLD_COIN_EXP * score->m_secretCoins);
            totalEXP += (USER_COIN_EXP * score->m_userCoins);
            totalEXP += (DEMON_EXP * score->m_demons);
            totalEXP += (CP_EXP * score->m_creatorPoints);

            return totalEXP;
        }

        inline static int getEXPRequiredForLevel(int level) {
            return (50 * (level * level) + (50 * level));
        }

        inline static int getLevelFromEXP(int exp) {
            return (sqrt(2*exp + 25) - 5) / 10.0;
        }


};