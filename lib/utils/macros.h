#pragma once

// copy from https://github.com/NVIDIA/stdexec/blob/main/include/stdexec/__detail/__concepts.hpp
#define EXPAND(...) __VA_ARGS__
#define PARNE () // parenthesis

#define EXPAND_R(...)  \
    EXPAND_R1(EXPAND_R1(EXPAND_R1(EXPAND_R1(__VA_ARGS__))))          \

#define EXPAND_R1(...) \
    EXPAND_R2(EXPAND_R2(EXPAND_R2(EXPAND_R2(__VA_ARGS__))))          \

#define EXPAND_R2(...) \
    EXPAND_R3(EXPAND_R3(EXPAND_R3(EXPAND_R3(__VA_ARGS__))))          \

#define EXPAND_R3(...) \
    EXPAND(EXPAND(EXPAND(EXPAND(__VA_ARGS__))))

#define FOR_EACH(MACRO, ...) \
    __VA_OPT__(EXPAND_R(FOR_EACH_HELPER(MACRO, __VA_ARGS__)))
#define FOR_EACH_HELPER(MACRO, A, ...) \
    MACRO(A) \
    __VA_OPT__(FOR_EACH_AGAIN PARNE (MACRO, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER
