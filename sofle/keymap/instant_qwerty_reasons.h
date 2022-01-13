#pragma once


enum instant_qwerty_activation_reasons {
    // Each value should have exactly one non-zero bit.
    NON_ENGLISH_LANGUAGE = 1,
    SHORTCUT_MODS_ACTIVATED = 2,
};
