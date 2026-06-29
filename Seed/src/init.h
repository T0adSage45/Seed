#pragma once

#include "app.h"

#ifdef SEED_PLATFORM_LINUX

extern Seed::Application *Seed::CreateApp();

int main(void) {
    auto leaf =
        std::unique_ptr<Seed::Application>(Seed::CreateApp()); // application instance (factory)
    leaf->Run();
}

#else
#error "not defined for windows now";
#endif
