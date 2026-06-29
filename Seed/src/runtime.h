#pragma once

namespace Seed {

class Timestep {
public:
    Timestep(float delta)
        : m_deltatime(delta) {};

    float GetSeconds() const { return m_deltatime; };

private:
    float m_deltatime;
};

}; // namespace Seed
