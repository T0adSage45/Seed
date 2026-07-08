#pragma once
#include <cstdint>
#include <string>

namespace Seed {

class Texture {
public:
    virtual ~Texture() = default;

    virtual void Bind(uint32_t slot = 0) const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual const std::string &GetPath() const = 0;

    static Texture *Create(const std::string &path);
};

} // namespace Seed
