#pragma once

/*
    ExpFilter Library
    -----------------
    Simple exponential smoothing filter for Arduino.

    Supported Types:
        - float
        - double
        - int
        - long
        - unsigned long

    Notes:
        - Internal calculations use double precision
        - Integer types are safely filtered internally
        - Weight range is constrained to [0.0, 1.0]
*/

#include <Arduino.h>
#include <type_traits>

// -----------------------------------------------------------------------------
// Supported Type Trait
// -----------------------------------------------------------------------------

template<typename T>
struct is_supported_filter_type : std::false_type {};

template<>
struct is_supported_filter_type<float> : std::true_type {};

template<>
struct is_supported_filter_type<double> : std::true_type {};

template<>
struct is_supported_filter_type<int> : std::true_type {};

template<>
struct is_supported_filter_type<long> : std::true_type {};

template<>
struct is_supported_filter_type<unsigned long> : std::true_type {};


// -----------------------------------------------------------------------------
// ExpFilter Class
// -----------------------------------------------------------------------------

template <typename T = float>
class ExpFilter {

    static_assert(
        is_supported_filter_type<T>::value,
        "Unsupported type for ExpFilter"
    );

public:

    // -------------------------------------------------------------------------
    // Constructors
    // -------------------------------------------------------------------------

    ExpFilter() = default;

    explicit ExpFilter(double weight) {
        setWeight(weight);
    }

    ExpFilter(double weight, T initialValue) {
        setWeight(weight);
        setValue(initialValue);
    }

    // -------------------------------------------------------------------------
    // Configuration
    // -------------------------------------------------------------------------

    void setWeight(double weight) {

        // Clamp weight to valid range
        _weight = constrain(weight, 0.0, 1.0);
    }

    double getWeight() const {
        return _weight;
    }

    // -------------------------------------------------------------------------
    // Value Access
    // -------------------------------------------------------------------------

    void setValue(T value) {

        _value = static_cast<double>(value);
        _initialized = true;
    }

    T getValue() const {

        return static_cast<T>(_value);
    }

    void reset(T value = T{}) {

        _value = static_cast<double>(value);
        _initialized = false;
    }

    // -------------------------------------------------------------------------
    // Filter Operation
    // -------------------------------------------------------------------------

    T filter(T measurement) {

        // First sample initializes filter
        if (!_initialized) {

            _value = static_cast<double>(measurement);
            _initialized = true;

            return static_cast<T>(_value);
        }

        _value =
            (_weight * _value) +
            ((1.0 - _weight) * static_cast<double>(measurement));

        return static_cast<T>(_value);
    }

private:

    // Filter weight [0.0 -> 1.0]
    double _weight = 0.0;

    // Internal accumulator
    double _value = 0.0;

    // Tracks first sample initialization
    bool _initialized = false;
};


// -----------------------------------------------------------------------------
// Optional Convenience Aliases
// -----------------------------------------------------------------------------

using FloatFilter = ExpFilter<float>;
using DoubleFilter = ExpFilter<double>;
using IntFilter = ExpFilter<int>;
using LongFilter = ExpFilter<long>;
using ULongFilter = ExpFilter<unsigned long>;