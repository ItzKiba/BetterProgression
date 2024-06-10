#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/loader/Event.hpp>
#include "LevelHelper.hpp"
#include "TierBarPopup.hpp"
#include "LevelUpPopup.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace geode::prelude;

class Request {
    private:
        inline static int m_stars = 0;
        inline static int m_moons = 0;
        inline static int m_diamonds = 0;
        inline static int m_gold_coins = 0;
        inline static int m_user_coins = 0;
        inline static int m_demons = 0;
        inline static int m_cp = 0;

        inline static int m_currentTotalEXP = 0;

    public:
        static void performCPRequest();
        static std::string parseRequest(const std::string& object, const std::string& key);
        static int generateNewTotalEXP();
        static int currentTotalEXP();
        static void displayTotalEXP();
        static void setCP(int cp);
        inline static bool m_openGameChecked = false;
};