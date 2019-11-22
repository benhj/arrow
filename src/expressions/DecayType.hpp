#pragma once

namespace arrow {
    enum class DecayType {
        DECAYS_TO_INT,
        DECAYS_TO_REAL,
        DECAYS_TO_STRING,
        DECAYS_TO_BOOL,
        DECAYS_TO_LIST,
        DECAYS_TO_NONE
    };
}