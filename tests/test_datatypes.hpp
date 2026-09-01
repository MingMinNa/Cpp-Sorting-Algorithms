/**
 * @file test_datatypes.hpp
 * @brief  Data types for sort testing.
 * @author MingMinNa
 */

#pragma once

#include <vector>
#include <limits>
#include <random>
#include <cstdint>
#include <cstddef>
#include <optional>
#include <functional>

/* Data types */
namespace sort_test 
{

extern std::vector<std::size_t> test_sizes;

/* Int Class */
using Int = int32_t;
using IntCmp = std::function<bool(const Int &, const Int &)>;
using IntFn  = std::function<void(Int*, std::size_t, IntCmp)>;

extern Int Int_range_min;
extern Int Int_range_max;

/* Float Class */
using Float = double;
using FloatCmp = std::function<bool(const Float &, const Float &)>;
using FloatFn  = std::function<void(Float*, std::size_t, FloatCmp)>;

extern Float Float_range_min;
extern Float Float_range_max;

/* Element Class */
struct Element {
    
    uint64_t key;
    uint64_t value;

    Element(uint64_t key = 0, uint64_t value = 0) 
        : key(key), value(value) {}

    bool operator== (const Element &other) const {
        return key == other.key && value == other.value;
    }

    bool operator!= (const Element &other) const {
        return !(*this == other);
    }

    /* The following operators is for non-comparison sorts */

    bool operator< (const Element &other)        const { return key < other.key; }
    bool operator> (const Element &other)        const { return key > other.key; }
    std::size_t operator- (const Element &other) const { return key - other.key; }
    std::size_t operator+ (const Element &other) const { return key + other.key; }
    std::size_t operator+ (std::size_t i)        const { return key + i; }
    std::size_t operator>>(std::size_t shift)    const { return static_cast<std::size_t>(key >> shift); }
    explicit operator std::size_t()              const { return static_cast<std::size_t>(key); }
    
    Element  operator++(int)                      { Element tmp = *this; ++ key; return tmp; }   // postfix: e ++
    Element& operator++()                         { ++ key; return *this; }                      // prefix : ++ e
    Element  operator--(int)                      { Element tmp = *this; -- key; return tmp; }   // postfix: e --
    Element& operator--()                         { -- key; return *this; }                      // prefix : -- e
    Element& operator+=(const Element& other)     { key += other.key; return *this; }            // element += element
    Element& operator-=(const Element& other)     { key -= other.key; return *this; }            // element -= element
};

using ElementCmp = std::function<bool(const Element &, const Element &)>;
using ElementFn  = std::function<void(Element*, std::size_t, ElementCmp)>;

extern decltype(Element::key) Element_range_min_key;
extern decltype(Element::key) Element_range_max_key;

} // namespace sort_test

/* Random Generators */
namespace sort_test 
{

inline std::mt19937_64& random_engine(std::optional<uint64_t> seed = std::nullopt) {
    thread_local std::mt19937_64 engine{ std::random_device{}() };
    if (seed.has_value()) {
        engine.seed(seed.value());
    }
    return engine;
}

template <typename T>
struct RandomTraits;

template <>
struct RandomTraits<Int> {
    using range_type = Int;

    static range_type range_min() { return Int_range_min; }
    static range_type range_max() { return Int_range_max; }

    static Int generate(range_type min_val, range_type max_val, std::mt19937_64 &engine) 
    {
        std::uniform_int_distribution<Int> dist(min_val, max_val);
        return dist(engine);
    }
};

template <>
struct RandomTraits<Float> {
    using range_type = Float;

    static range_type range_min() { return Float_range_min; }
    static range_type range_max() { return Float_range_max; }

    static Float generate(range_type min_val, range_type max_val, std::mt19937_64 &engine) 
    {
        std::uniform_real_distribution<Float> dist(min_val, max_val);
        return dist(engine);
    }
};

template <>
struct RandomTraits<Element> {
    using range_type = decltype(Element::key);

    static range_type range_min() { return Element_range_min_key; }
    static range_type range_max() { return Element_range_max_key; }

    static Element generate(range_type min_val, range_type max_val, std::mt19937_64 &engine) 
    {
        std::uniform_int_distribution<decltype(Element::key)> key_dist(min_val, max_val);
        std::uniform_int_distribution<decltype(Element::value)> value_dist(
            std::numeric_limits<decltype(Element::value)>::min(),
            std::numeric_limits<decltype(Element::value)>::max()
        );
        return Element(key_dist(engine), value_dist(engine));
    }
};

template <typename T>
T random(
    typename RandomTraits<T>::range_type min_val = RandomTraits<T>::range_min(),
    typename RandomTraits<T>::range_type max_val = RandomTraits<T>::range_max(),
    std::optional<uint64_t> seed = std::nullopt
) {
    auto &engine = random_engine(seed);
    return RandomTraits<T>::generate(min_val, max_val, engine);
}

} // namespace sort_test

/* Type Name */
namespace sort_test 
{

template <typename T>
struct TypeName;

template <>
struct TypeName<Int> {
    inline static const char* name = "Int";
};

template <>
struct TypeName<Float> {
    inline static const char* name = "Float";
};

template <>
struct TypeName<Element> {
    inline static const char* name = "Element";
};

} // namespace sort_test

/* Less Cmp */
namespace sort_test 
{

template <typename T>
struct LessCmp;

template <>
struct LessCmp<Int> {
    inline static const IntCmp less = std::less<Int>{};
};

template <>
struct LessCmp<Float> {
    inline static const FloatCmp less = std::less<Float>{};
};

template <>
struct LessCmp<Element> {
    inline static const ElementCmp less = 
        [](const Element &a, const Element &b) {
            return a.key < b.key;
        };
};

} // namespace sort_test

/* Greater Cmp */
namespace sort_test 
{

template <typename T>
struct GreaterCmp;

template <>
struct GreaterCmp<Int> {
    inline static const IntCmp greater = std::greater<Int>{};
};

template <>
struct GreaterCmp<Float> {
    inline static const FloatCmp greater = std::greater<Float>{};
};

template <>
struct GreaterCmp<Element> {
    inline static const ElementCmp greater = 
        [](const Element &a, const Element &b) {
            return a.key > b.key;
        };
};

} // namespace sort_test

/* Same Key Cmp */
namespace sort_test 
{

template <typename T>
struct SameKeyCmp;

template <>
struct SameKeyCmp<Int> {
    inline static const IntCmp same_key =
        [](const Int &a, const Int &b) {
            return a == b;
        };
};

template <>
struct SameKeyCmp<Float> {
    inline static const FloatCmp same_key =
        [](const Float &a, const Float &b) {
            return a == b;
        };
};

template <>
struct SameKeyCmp<Element> {
    inline static const ElementCmp same_key = 
        [](const Element &a, const Element &b) {
            return a.key == b.key;
        };
};

} // namespace sort_test