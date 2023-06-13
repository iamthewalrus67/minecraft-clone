//
// Created by iamthewalrus on 6/13/23.
//

#ifndef COOL_GAME_TIME_HPP
#define COOL_GAME_TIME_HPP

#include <ctype.h>
#include <bx/timer.h>


namespace util {

class Time {
public:
    static Time& instance() {
        static Time inst;
        return inst;
    }

    Time& operator=(const Time& time) = delete;
    Time(const Time& time) = delete;

    float deltaTime() {
        return m_deltaTime;
    }

    void update() {
        int64_t now = bx::getHPCounter();
        static int64_t last = now;
        const int64_t frameTime = now - last;
        last = now;
        const double freq = double(bx::getHPFrequency());
        m_deltaTime = float(frameTime/freq);
    }


private:
    Time() {}

    float m_deltaTime = 1.0f;
}; // Time

} // util

#endif //COOL_GAME_TIME_HPP
