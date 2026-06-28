/**
 * @file test_utils.hpp
 * @brief  Utilities for sorting algorithm tests.
 * @author MingMinNa
 */

#pragma once

#include "../include/CppSortingAlgos/sort_collection.hpp"
#include "test_datatypes.hpp"
#include <string>
#include <vector>
#include <format>
#include <sstream>
#include <iostream>
#include <algorithm>

/* Functions */
namespace sort_test 
{

template <typename T, typename Compare>
std::vector<T> ref_sort(std::vector<T> vec, const Compare &cmp) 
{
    std::stable_sort(vec.begin(), vec.end(), cmp);
    return vec;
}

template <typename T, typename Compare>
bool same_keys(const std::vector<T> &vec, const std::vector<T> &ref, Compare cmp) 
{
    if (vec.size() != ref.size()) return false;
    for (std::size_t i = 0; i < vec.size(); ++i) {
        if (!cmp(vec[i], ref[i])) return false;
    }
    return true;
}

template <typename T>
std::vector<T> random_vector(std::size_t n)
{
    std::vector<T> vec;
    vec.reserve(n);

    for (std::size_t i = 0; i < n; ++i) {
        vec.emplace_back(random<T>());
    }
    return vec;
}

template <typename T>
std::vector<T> make_with_duplicates(std::size_t n, std::size_t num_distinct)
{
    std::vector<T> vec;
    vec.reserve(n);

    for (std::size_t i = 0; i < n; ++i) {
        vec.emplace_back(random<T>(0, num_distinct - 1));
    }
    return vec;
}

template <typename T, typename Compare>
std::vector<T> make_sorted(std::size_t n, const Compare &cmp)
{
    std::vector<T> vec = random_vector<T>(n);
    return ref_sort<T>(vec, cmp);
}

template <typename T, typename Compare>
std::vector<T> make_nearly_sorted(std::size_t n, std::size_t num_swaps, const Compare &cmp)
{
    std::vector<T> vec = make_sorted<T, Compare>(n, cmp);
    
    for (std::size_t i = 0; i < num_swaps; ++i) {
        Int a = random<Int>(0, n - 1);
        Int b = random<Int>(0, n - 1);
        std::swap(vec[a], vec[b]);
    }
    
    return vec;
}

template <typename T>
std::vector<T> make_all_equal(std::size_t n)
{
    T ele = random<T>();
    return std::vector<T>(n, ele);
}

} // namespace sort_test

/* Test Framework */
namespace sort_test 
{

enum class Tests : uint32_t {
    None                = 0,
    SingleElement       = 1u << 0,
    NullPtrThrows       = 1u << 1,
    ZeroLengthNoOp      = 1u << 2,
    Random              = 1u << 3,
    Sorted              = 1u << 4,
    ReverseSorted       = 1u << 5,
    NearlySorted        = 1u << 6,
    AllEqual            = 1u << 7,
    HighDuplicateKeys   = 1u << 8,
    PartialRange        = 1u << 9,
    Stability           = 1u << 10,
};

inline Tests operator&(Tests a, Tests b)
{
    using U = std::underlying_type_t<Tests>;

    return static_cast<Tests>(
        static_cast<U>(a) & static_cast<U>(b)
    );
}

inline Tests operator|(Tests a, Tests b)
{
    using U = std::underlying_type_t<Tests>;

    return static_cast<Tests>(
        static_cast<U>(a) | static_cast<U>(b)
    );
}

struct TestCase 
{
    bool        passed;
    std::string name;
    std::string message;  // non-empty on failure

    TestCase(bool passed = false, std::string name = "", std::string message = "") 
    {
        this->passed = passed;
        this->name   = name;
        this->message = (passed) ? ("") : (message);
    }
};

struct Result 
{
    std::string           suite_name;
    std::vector<TestCase> cases;
    
    std::size_t  total() const 
    {
        return cases.size();
    }

    std::size_t passed() const 
    {
        return static_cast<std::size_t>(
            std::count_if(
                cases.begin(), cases.end(), 
                [](const TestCase &c) { return c.passed; })
            );
    }

    std::size_t failed() const 
    {
        return total() - passed();
    }
};

struct Reporter 
{
    static void print(const Result &res)
    {
        std::cout << std::format("Suite : {}\n", res.suite_name);

        for (const auto &c : res.cases) {
            if (c.passed) std::cout << std::format("[V] {}\n", c.name);
            else          std::cout << std::format("[X] {} : {}\n", c.name, c.message);
        }

        std::cout << std::format(
            "\nResult: {}/{} passed, {} failed\n\n",
            res.passed(), res.total(), res.failed()
        );
    }
};

template <typename T = Element, typename Compare = ElementCmp>
class TestSuite 
{
    public:
        using SortFn = std::function<void(T*, std::size_t, Compare)>;

        TestSuite(
            std::string sort_name, 
            SortFn sort_fn,
            Compare asc_cmp, 
            Compare des_cmp,
            Compare same_key_cmp,
            bool is_stable
        );
        ~TestSuite() = default;

        Result run_all(Tests exclude = Tests::None);
        TestCase test_single_element();
        TestCase test_null_ptr_throws();
        TestCase test_zero_length_no_op();
        TestCase test_random();
        TestCase test_sorted();
        TestCase test_reverse_sorted();
        TestCase test_nearly_sorted();
        TestCase test_all_equal();
        TestCase test_high_duplicate_keys();
        TestCase test_stability();
        TestCase test_partial_range();

    private:
        std::string sort_name;
        SortFn      sort_fn;
        Compare     asc_cmp;
        Compare     des_cmp;
        Compare     same_key_cmp;
        bool        is_stable; 
};

template <typename T, typename Compare>
TestSuite<T, Compare>::TestSuite(
    std::string sort_name, 
    SortFn sort_fn,
    Compare asc_cmp, 
    Compare des_cmp, 
    Compare same_key_cmp,
    bool is_stable
) {
    this->sort_name = sort_name;
    this->sort_fn   = std::move(sort_fn);
    this->asc_cmp   = std::move(asc_cmp);
    this->des_cmp   = std::move(des_cmp);
    this->same_key_cmp = std::move(same_key_cmp);
    this->is_stable = is_stable;
}

template <typename T, typename Compare>
Result TestSuite<T, Compare>::run_all(Tests exclude)
{
    Result res;
    res.suite_name = sort_name;

    auto enabled = [&](Tests tc) {
        return (exclude & tc) == Tests::None;
    };

    using TestFn = std::function<TestCase()>;

    const std::vector<std::pair<Tests, TestFn>> tests = {
        {Tests::SingleElement,     [this]{ return test_single_element(); }},
        {Tests::NullPtrThrows,     [this]{ return test_null_ptr_throws(); }},
        {Tests::ZeroLengthNoOp,    [this]{ return test_zero_length_no_op(); }},
        {Tests::Random,            [this]{ return test_random(); }},
        {Tests::Sorted,            [this]{ return test_sorted(); }},
        {Tests::ReverseSorted,     [this]{ return test_reverse_sorted(); }},
        {Tests::NearlySorted,      [this]{ return test_nearly_sorted(); }},
        {Tests::AllEqual,          [this]{ return test_all_equal(); }},
        {Tests::HighDuplicateKeys, [this]{ return test_high_duplicate_keys(); }},
        {Tests::PartialRange,      [this]{ return test_partial_range(); }},
    };

    for (const auto& [flag, fn] : tests) {
        if (enabled(flag)) res.cases.push_back(fn());
    }
    
    if (is_stable && enabled(Tests::Stability)) {
        res.cases.push_back(test_stability());
    }

    return res;
}

template <typename T, typename Compare>
TestCase TestSuite<T, Compare>::test_single_element()
{
    std::string func_name = std::format("({}) single element", TypeName<T>::name);
    std::vector<T> vec = { random<T>() };
    T expected = vec[0];
    bool passed = true;

    sort_fn(vec.data(), 1, asc_cmp);
    passed = passed && (vec.size() == 1) && (vec[0] == expected);

    sort_fn(vec.data(), 1, des_cmp);
    passed = passed && (vec.size() == 1) && (vec[0] == expected);
    return TestCase(passed, func_name, "element was mutated");
}

template <typename T, typename Compare>
TestCase TestSuite<T, Compare>::test_null_ptr_throws()
{
    std::string func_name = std::format("({}) nullptr throws", TypeName<T>::name);

    bool valid = true;
    try         { sort_fn(nullptr, 0, asc_cmp); }
    catch (...) { valid = false; }

    if (!valid) {
        std::ostringstream msg;
        msg << "nullptr with n = 0 threw an exception";
        return TestCase(false, func_name, msg.str());
    }

    for (std::size_t n : test_sizes) {

        bool threw = false;
        try                                   { sort_fn(nullptr, n, asc_cmp); }
        catch (const std::invalid_argument &) { threw = true; }
        catch (...)                           {               }

        if (!threw) {
            std::ostringstream msg;
            msg << "nullptr with n = " << n << " did not throw invalid_argument";
            return TestCase(false, func_name, msg.str());
        }
    }
    return TestCase(true, func_name);
}

template <typename T, typename Compare>
TestCase TestSuite<T, Compare>::test_zero_length_no_op()
{
    std::string func_name = std::format("({}) zero length", TypeName<T>::name);

    for (const auto &cmp : { asc_cmp, des_cmp }) {
        auto vec  = random_vector<T>(8);
        auto copy = vec;
        sort_fn(vec.data(), 0, cmp);

        if (vec != copy) 
            return TestCase(
                false, func_name, "array was modified despite n = 0"
            );
    }
    return TestCase(true, func_name);
}

template <typename T, typename Compare>
TestCase TestSuite<T, Compare>::test_random()
{
    std::string func_name = std::format("({}) random tests", TypeName<T>::name);

    for (std::size_t n : test_sizes) {

        auto vec = random_vector<T>(n);
        auto ref = ref_sort(vec, asc_cmp);
        sort_fn(vec.data(), n, asc_cmp);
        
        if (!same_keys(vec, ref, same_key_cmp)) {
            std::ostringstream msg; msg << "mismatch (ascending) at n = " << n;
            return TestCase(false, func_name, msg.str());
        }

        vec = random_vector<T>(n);
        ref = ref_sort(vec, des_cmp);
        sort_fn(vec.data(), n, des_cmp);
        
        if (!same_keys(vec, ref, same_key_cmp)) {
            std::ostringstream msg; msg << "mismatch (descending) at n = " << n;
            return TestCase(false, func_name, msg.str());
        }
    }
    return TestCase(true, func_name);
}

template <typename T, typename Compare>
TestCase TestSuite<T, Compare>::test_sorted()
{
    std::string func_name = std::format("({}) sorted array", TypeName<T>::name);

    auto asc_vec = make_sorted<T>(test_sizes.back(), asc_cmp);
    auto asc_ref = ref_sort(asc_vec, asc_cmp);
    sort_fn(asc_vec.data(), asc_vec.size(), asc_cmp);

    auto des_vec = make_sorted<T>(test_sizes.back(), des_cmp);
    auto des_ref = ref_sort(des_vec, des_cmp);
    sort_fn(des_vec.data(), des_vec.size(), des_cmp);

    return TestCase(
        same_keys(asc_vec, asc_ref, same_key_cmp) && same_keys(des_vec, des_ref, same_key_cmp), 
        func_name, "result differs from reference"
    );
}

template <typename T, typename Compare>
TestCase TestSuite<T, Compare>::test_reverse_sorted()
{
    std::string func_name = std::format("({}) reverse-sorted array", TypeName<T>::name);

    for (std::size_t n : test_sizes) {

        auto vec = random_vector<T>(n);
        std::stable_sort(vec.begin(), vec.end(), des_cmp); 

        auto ref = ref_sort(vec, asc_cmp);
        sort_fn(vec.data(), vec.size(), asc_cmp);

        if (!same_keys(vec, ref, same_key_cmp)) {
            std::ostringstream msg; msg << "mismatch at reverse-sorted n = " << n;
            return TestCase(false, func_name, msg.str());
        }
    }
    return TestCase(true, func_name);
}

template <typename T, typename Compare>
TestCase TestSuite<T, Compare>::test_nearly_sorted()
{
    std::string func_name = std::format("({}) nearly-sorted array", TypeName<T>::name);

    for (std::size_t n : test_sizes) {

        std::size_t swaps = std::max<std::size_t>(1, n / 10);
        auto vec = make_nearly_sorted<T>(n, swaps, asc_cmp);
        auto ref = ref_sort(vec, asc_cmp);
        sort_fn(vec.data(), vec.size(), asc_cmp);

        if (!same_keys(vec, ref, same_key_cmp)) {
            std::ostringstream msg; msg << "mismatch at nearly-sorted n = " << n;
            return TestCase(false, func_name, msg.str());
        }
    }
    return TestCase(true, func_name);
}

template <typename T, typename Compare>
TestCase TestSuite<T, Compare>::test_all_equal()
{
    std::string func_name = std::format("({}) all-equal array", TypeName<T>::name);

    for (std::size_t n : test_sizes) {

        auto vec = make_all_equal<T>(n);
        auto ref = ref_sort(vec, asc_cmp);
        sort_fn(vec.data(), vec.size(), asc_cmp);

        if (!same_keys(vec, ref, same_key_cmp)) {
            std::ostringstream msg; msg << "mismatch at all-equal n = " << n;
            return TestCase(false, func_name, msg.str());
        }
    }
    return TestCase(true, func_name);
}

template <typename T, typename Compare>
TestCase TestSuite<T, Compare>::test_high_duplicate_keys()
{
    std::string func_name = std::format("({}) duplicate keys", TypeName<T>::name);

    for (std::size_t n : test_sizes) {

        int32_t distinct = random<Int>(2, std::max(2, static_cast<Int>(n / 10)));
        auto vec = make_with_duplicates<T>(n, distinct);
        auto ref = ref_sort(vec, asc_cmp);
        sort_fn(vec.data(), vec.size(), asc_cmp);

        if (!same_keys(vec, ref, same_key_cmp)) {
            std::ostringstream msg;
            msg << "mismatch at n = " << n << " with " << distinct << " distinct keys";
            return TestCase(false, func_name, msg.str());
        }
    }
    return TestCase(true, func_name);
}

template <typename T, typename Compare>
TestCase TestSuite<T, Compare>::test_stability()
{
    std::string func_name = std::format("({}) stability", TypeName<T>::name);

    for (std::size_t n : test_sizes) {

        int32_t distinct = random<Int>(2, std::max(2, static_cast<Int>(n / 10)));
        auto vec = make_with_duplicates<T>(n, distinct);
        auto ref = ref_sort(vec, asc_cmp);
        sort_fn(vec.data(), vec.size(), asc_cmp);

        if (vec != ref) {
            std::ostringstream msg;
            msg << "stability violation" << " (n = " << n << ")";
            return TestCase(false, func_name, msg.str());
        }
    }
    return TestCase(true, func_name);
}

template <typename T, typename Compare>
TestCase TestSuite<T, Compare>::test_partial_range()
{
    std::string func_name = std::format("({}) partial range sort", TypeName<T>::name);
    const std::size_t N = test_sizes.back();

    for (std::size_t trial = 0; trial < 5; ++ trial) {
        auto vec = random_vector<T>(N);
        auto ref = vec;

        std::size_t offset = random<Int>(0, N - 2);
        std::size_t len    = random<Int>(1, N - offset);

        std::stable_sort(
            ref.begin() + offset,
            ref.begin() + offset + len, 
            asc_cmp
        );
        sort_fn(vec.data() + offset, len, asc_cmp);

        if (!same_keys(vec, ref, same_key_cmp)) {
            std::ostringstream msg;
            msg << "mismatch for partial range [" << offset
                << ", " << offset + len << ") of n = " << N;
            return TestCase(false, func_name, msg.str());
        }
    }
    return TestCase(true, func_name);
}

template<typename Sort, typename T, typename Cmp, typename Fn>
bool test(std::string sort_name, Tests exclude = Tests::None)
{
    Fn sort_fn = [](T* arr, std::size_t n, Cmp cmp) {
        Sort::sort(arr, n, cmp);
    };

    TestSuite<T, Cmp> suite(
        sort_name, 
        sort_fn,
        LessCmp<T>::less,
        GreaterCmp<T>::greater,
        SameKeyCmp<T>::same_key,
        Sort::is_stable
    );

    Result result = suite.run_all(exclude);
    Reporter::print(result);
    return result.failed() == 0;
}

} // namespace sort_test

