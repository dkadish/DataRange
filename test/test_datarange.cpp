/**
 * DataRange Library Unit Tests
 *
 * Tests all functionality of the DataRange class using Unity test framework
 */

#include <unity.h>
#include <DataRange.h>
#include <DataEnvelope.h>

// Test fixture - runs before each test
void setUp(void)
{
    // Set up code here if needed
}

// Test fixture - runs after each test
void tearDown(void)
{
    // Clean up code here if needed
}

// Test: Constructor initializes properly
void test_constructor_initialization(void)
{
    DataRange bounds;

    // After construction, before any updates, min and max should be 0
    TEST_ASSERT_EQUAL_FLOAT(0.0, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(0.0, bounds.max());
    TEST_ASSERT_EQUAL_FLOAT(0.0, bounds.range());
}

// Test: Constructor with initial bounds initializes values
void test_constructor_with_initial_bounds(void)
{
    DataRange bounds(10.0f, 20.0f);

    TEST_ASSERT_EQUAL_FLOAT(10.0f, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(20.0f, bounds.max());
    TEST_ASSERT_EQUAL_FLOAT(10.0f, bounds.range());
    TEST_ASSERT_EQUAL_FLOAT(0.0f, bounds.normalized());
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.5f, bounds.normalized(15.0f));

    bounds.update(25.0f);
    TEST_ASSERT_EQUAL_FLOAT(10.0f, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(25.0f, bounds.max());

    bounds.update(5.0f);
    TEST_ASSERT_EQUAL_FLOAT(5.0f, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(25.0f, bounds.max());
}

// Test: Constructor with only upper bound keeps lower at zero (non-floating)
void test_constructor_upper_only_fixed_lower(void)
{
    DataRange bounds(100.0f);

    TEST_ASSERT_EQUAL_FLOAT(0.0f, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(100.0f, bounds.max());
    TEST_ASSERT_EQUAL_FLOAT(100.0f, bounds.range());

    bounds.update(-20.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(100.0f, bounds.max());

    bounds.update(150.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(150.0f, bounds.max());
}

// Test: First update sets both min and max
void test_first_update_sets_min_and_max(void)
{
    DataRange bounds;

    bounds.update(50.0);

    TEST_ASSERT_EQUAL_FLOAT(0.0, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(50.0, bounds.max());
    TEST_ASSERT_EQUAL_FLOAT(50.0, bounds.range());
}

// Test: setInitialBounds primes bounds before any updates
void test_set_initial_bounds(void)
{
    DataRange bounds;

    bounds.setInitialBounds(-5.0f, 5.0f);

    TEST_ASSERT_EQUAL_FLOAT(-5.0f, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(5.0f, bounds.max());
    TEST_ASSERT_EQUAL_FLOAT(10.0f, bounds.range());
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.5f, bounds.normalized(0.0f));

    bounds.update(-10.0f);
    TEST_ASSERT_EQUAL_FLOAT(-10.0f, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(5.0f, bounds.max());

    bounds.update(6.0f);
    TEST_ASSERT_EQUAL_FLOAT(-10.0f, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(6.0f, bounds.max());
}

// Test: setInitialBounds swaps bounds when lower > upper
void test_set_initial_bounds_swapped(void)
{
    DataRange bounds;

    bounds.setInitialBounds(30.0f, 10.0f);

    TEST_ASSERT_EQUAL_FLOAT(10.0f, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(30.0f, bounds.max());
    TEST_ASSERT_EQUAL_FLOAT(20.0f, bounds.range());
}

// Test: default lower stays at zero when floating is disabled
void test_default_lower_stays_zero(void)
{
    DataRange bounds;

    bounds.update(50.0f);
    bounds.update(25.0f);

    TEST_ASSERT_EQUAL_FLOAT(0.0f, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(50.0f, bounds.max());
    TEST_ASSERT_EQUAL_FLOAT(50.0f, bounds.range());
}

// Test: enabling floating lower midstream allows min to drop
void test_enable_floating_lower_midstream(void)
{
    DataRange bounds;

    bounds.update(50.0f);
    bounds.setFloatingLower(true);
    bounds.update(10.0f);

    TEST_ASSERT_EQUAL_FLOAT(10.0f, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(50.0f, bounds.max());
}

// Test: Update with higher value updates max
void test_update_higher_value_updates_max(void)
{
    DataRange bounds;

    bounds.update(50.0);
    bounds.update(75.0);

    TEST_ASSERT_EQUAL_FLOAT(0.0, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(75.0, bounds.max());
    TEST_ASSERT_EQUAL_FLOAT(75.0, bounds.range());
}

// Test: Update with lower value updates min
void test_update_lower_value_updates_min(void)
{
    DataRange bounds;

    bounds.setFloatingLower(true);

    bounds.update(50.0);
    bounds.update(25.0);

    TEST_ASSERT_EQUAL_FLOAT(25.0, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(50.0, bounds.max());
    TEST_ASSERT_EQUAL_FLOAT(25.0, bounds.range());
}

// Test: Update with middle value doesn't change bounds
void test_update_middle_value_no_change(void)
{
    DataRange bounds;

    bounds.update(0.0);
    bounds.update(100.0);
    bounds.update(50.0);

    TEST_ASSERT_EQUAL_FLOAT(0.0, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(100.0, bounds.max());
    TEST_ASSERT_EQUAL_FLOAT(100.0, bounds.range());
}

// Test: Multiple updates track min and max correctly
void test_multiple_updates(void)
{
    DataRange bounds;

    bounds.setFloatingLower(true);

    bounds.update(50.0);
    bounds.update(30.0);
    bounds.update(80.0);
    bounds.update(45.0);
    bounds.update(20.0);
    bounds.update(90.0);

    TEST_ASSERT_EQUAL_FLOAT(20.0, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(90.0, bounds.max());
    TEST_ASSERT_EQUAL_FLOAT(70.0, bounds.range());
}

// Test: Negative values are handled correctly
void test_negative_values(void)
{
    DataRange bounds;

    bounds.setFloatingLower(true);

    bounds.update(-50.0);
    bounds.update(-100.0);
    bounds.update(50.0);

    TEST_ASSERT_EQUAL_FLOAT(-100.0, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(50.0, bounds.max());
    TEST_ASSERT_EQUAL_FLOAT(150.0, bounds.range());
}

// Test: getNormalized returns 0.0 when range is zero
void test_normalized_zero_range(void)
{
    DataRange bounds;

    bounds.setFloatingLower(true);

    bounds.update(50.0);

    float normalized = bounds.normalized();
    TEST_ASSERT_EQUAL_FLOAT(0.0, normalized);
}

// Test: getNormalized at minimum returns 0.0
void test_normalized_at_minimum(void)
{
    DataRange bounds;

    bounds.update(0.0);
    bounds.update(100.0);
    bounds.update(0.0);

    float normalized = bounds.normalized();
    TEST_ASSERT_EQUAL_FLOAT(0.0, normalized);
}

// Test: getNormalized at maximum returns 1.0
void test_normalized_at_maximum(void)
{
    DataRange bounds;

    bounds.update(0.0);
    bounds.update(100.0);

    float normalized = bounds.normalized();
    TEST_ASSERT_EQUAL_FLOAT(1.0, normalized);
}

// Test: getNormalized at midpoint returns 0.5
void test_normalized_at_midpoint(void)
{
    DataRange bounds;

    bounds.update(0.0);
    bounds.update(100.0);
    bounds.update(50.0);

    float normalized = bounds.normalized();
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.5, normalized);
}

// Test: getNormalized with arbitrary value
void test_normalized_arbitrary_value(void)
{
    DataRange bounds;

    bounds.update(0.0);
    bounds.update(100.0);

    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.25, bounds.normalized(25.0));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.75, bounds.normalized(75.0));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.5, bounds.normalized(50.0));
}

// Test: getNormalized constrains values below min to 0.0
void test_normalized_constrains_below_min(void)
{
    DataRange bounds;

    bounds.setFloatingLower(true);

    bounds.update(50.0);
    bounds.update(100.0);

    float normalized = bounds.normalized(25.0);
    TEST_ASSERT_EQUAL_FLOAT(0.0, normalized);
}

// Test: getNormalized constrains values above max to 1.0
void test_normalized_constrains_above_max(void)
{
    DataRange bounds;

    bounds.update(0.0);
    bounds.update(100.0);

    float normalized = bounds.normalized(150.0);
    TEST_ASSERT_EQUAL_FLOAT(1.0, normalized);
}

// Test: getNormalized before initialization returns 0.0
void test_normalized_before_initialization(void)
{
    DataRange bounds;

    float normalized = bounds.normalized();
    TEST_ASSERT_EQUAL_FLOAT(0.0, normalized);
}

// Test: Reset clears all state
void test_reset_clears_state(void)
{
    DataRange bounds;

    bounds.update(25.0);
    bounds.update(75.0);

    bounds.reset();

    TEST_ASSERT_EQUAL_FLOAT(0.0, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(0.0, bounds.max());
    TEST_ASSERT_EQUAL_FLOAT(0.0, bounds.range());
}

// Test: After reset, first update sets both min and max
void test_after_reset_first_update(void)
{
    DataRange bounds;

    bounds.update(25.0);
    bounds.update(75.0);
    bounds.reset();
    bounds.update(50.0);

    TEST_ASSERT_EQUAL_FLOAT(0.0, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(50.0, bounds.max());
}

// Test: Floating point precision
void test_floating_point_precision(void)
{
    DataRange bounds;

    bounds.setFloatingLower(true);

    bounds.update(0.123456);
    bounds.update(0.987654);

    TEST_ASSERT_FLOAT_WITHIN(0.000001, 0.123456, bounds.min());
    TEST_ASSERT_FLOAT_WITHIN(0.000001, 0.987654, bounds.max());
}

// Test: Very large values
void test_large_values(void)
{
    DataRange bounds;

    bounds.setFloatingLower(true);

    bounds.update(1000000.0);
    bounds.update(2000000.0);

    TEST_ASSERT_EQUAL_FLOAT(1000000.0, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(2000000.0, bounds.max());
    TEST_ASSERT_EQUAL_FLOAT(1000000.0, bounds.range());
}

// Test: Very small values
void test_small_values(void)
{
    DataRange bounds;

    bounds.setFloatingLower(true);

    bounds.update(0.0001);
    bounds.update(0.0002);

    TEST_ASSERT_FLOAT_WITHIN(0.00001, 0.0001, bounds.min());
    TEST_ASSERT_FLOAT_WITHIN(0.00001, 0.0002, bounds.max());
}

// Test: Typical sensor data (ADC range 0-1023)
void test_adc_range(void)
{
    DataRange bounds;

    // Simulate reading analog sensor
    bounds.update(0);
    bounds.update(512);
    bounds.update(1023);
    bounds.update(256);
    bounds.update(768);

    TEST_ASSERT_EQUAL_FLOAT(0.0, bounds.min());
    TEST_ASSERT_EQUAL_FLOAT(1023.0, bounds.max());

    // Test normalization at quarter points
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, bounds.normalized(0.0));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.25, bounds.normalized(255.75));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.5, bounds.normalized(511.5));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.75, bounds.normalized(767.25));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, bounds.normalized(1023.0));
}

#ifdef UNIT_TEST
// For native testing
int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_constructor_initialization);
    RUN_TEST(test_constructor_with_initial_bounds);
    RUN_TEST(test_constructor_upper_only_fixed_lower);
    RUN_TEST(test_first_update_sets_min_and_max);
    RUN_TEST(test_set_initial_bounds);
    RUN_TEST(test_set_initial_bounds_swapped);
    RUN_TEST(test_default_lower_stays_zero);
    RUN_TEST(test_enable_floating_lower_midstream);
    RUN_TEST(test_update_higher_value_updates_max);
    RUN_TEST(test_update_lower_value_updates_min);
    RUN_TEST(test_update_middle_value_no_change);
    RUN_TEST(test_multiple_updates);
    RUN_TEST(test_negative_values);
    RUN_TEST(test_normalized_zero_range);
    RUN_TEST(test_normalized_at_minimum);
    RUN_TEST(test_normalized_at_maximum);
    RUN_TEST(test_normalized_at_midpoint);
    RUN_TEST(test_normalized_arbitrary_value);
    RUN_TEST(test_normalized_constrains_below_min);
    RUN_TEST(test_normalized_constrains_above_max);
    RUN_TEST(test_normalized_before_initialization);
    RUN_TEST(test_reset_clears_state);
    RUN_TEST(test_after_reset_first_update);
    RUN_TEST(test_floating_point_precision);
    RUN_TEST(test_large_values);
    RUN_TEST(test_small_values);
    RUN_TEST(test_adc_range);

    // DataEnvelope
    RUN_TEST(test_envelope_initializes_on_first_update);
    RUN_TEST(test_envelope_grows_to_new_extremes);
    RUN_TEST(test_envelope_decays_exponential_by_default);
    RUN_TEST(test_envelope_linear_decay_optional);
    RUN_TEST(test_envelope_constructor_with_upper_and_decay);
    RUN_TEST(test_envelope_constructor_with_bounds_and_decay);
    RUN_TEST(test_envelope_normalized);

    return UNITY_END();
}
#else
// For embedded testing
void setup()
{
    delay(2000); // Wait for serial monitor

    UNITY_BEGIN();

    RUN_TEST(test_constructor_initialization);
    RUN_TEST(test_constructor_with_initial_bounds);
    RUN_TEST(test_constructor_upper_only_fixed_lower);
    RUN_TEST(test_first_update_sets_min_and_max);
    RUN_TEST(test_set_initial_bounds);
    RUN_TEST(test_set_initial_bounds_swapped);
    RUN_TEST(test_default_lower_stays_zero);
    RUN_TEST(test_enable_floating_lower_midstream);
    RUN_TEST(test_update_higher_value_updates_max);
    RUN_TEST(test_update_lower_value_updates_min);
    RUN_TEST(test_update_middle_value_no_change);
    RUN_TEST(test_multiple_updates);
    RUN_TEST(test_negative_values);
    RUN_TEST(test_normalized_zero_range);
    RUN_TEST(test_normalized_at_minimum);
    RUN_TEST(test_normalized_at_maximum);
    RUN_TEST(test_normalized_at_midpoint);
    RUN_TEST(test_normalized_arbitrary_value);
    RUN_TEST(test_normalized_constrains_below_min);
    RUN_TEST(test_normalized_constrains_above_max);
    RUN_TEST(test_normalized_before_initialization);
    RUN_TEST(test_reset_clears_state);
    RUN_TEST(test_after_reset_first_update);
    RUN_TEST(test_floating_point_precision);
    RUN_TEST(test_large_values);
    RUN_TEST(test_small_values);
    RUN_TEST(test_adc_range);

    // DataEnvelope
    RUN_TEST(test_envelope_initializes_on_first_update);
    RUN_TEST(test_envelope_grows_to_new_extremes);
    RUN_TEST(test_envelope_decays_exponential_by_default);
    RUN_TEST(test_envelope_linear_decay_optional);
    RUN_TEST(test_envelope_constructor_with_upper_and_decay);
    RUN_TEST(test_envelope_constructor_with_bounds_and_decay);
    RUN_TEST(test_envelope_normalized);

    UNITY_END();
}

void loop()
{
    // Nothing to do here
}
#endif

// ----------------------
// DataEnvelope Tests
// ----------------------

void test_envelope_initializes_on_first_update(void)
{
    DataEnvelope env;
    env.update(10.0f);

    TEST_ASSERT_EQUAL_FLOAT(10.0f, env.lower());
    TEST_ASSERT_EQUAL_FLOAT(10.0f, env.upper());
    TEST_ASSERT_EQUAL_FLOAT(0.0f, env.range());
}

void test_envelope_grows_to_new_extremes(void)
{
    DataEnvelope env;
    env.update(10.0f);
    env.update(20.0f);
    env.update(5.0f);

    TEST_ASSERT_EQUAL_FLOAT(5.0f, env.lower());
    TEST_ASSERT_EQUAL_FLOAT(20.0f, env.upper());
    TEST_ASSERT_EQUAL_FLOAT(15.0f, env.range());
}

void test_envelope_decays_exponential_by_default(void)
{
    DataEnvelope env(0.20f); // 20% decay per update (exponential)
    env.update(0.0f);
    env.update(10.0f); // grow upper to 10
    env.update(5.0f);  // decay toward 5 by scaling distance

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 2.6f, env.lower());
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 9.0f, env.upper());
}

void test_envelope_linear_decay_optional(void)
{
    DataEnvelope env(1.0f); // 1 unit per update when linear
    env.setLinearDecay();
    env.update(0.0f);
    env.update(10.0f); // grow upper to 10
    env.update(10.0f); // decay upper by 1 -> 9, lower by +1 -> 1

    TEST_ASSERT_EQUAL_FLOAT(1.0f, env.lower());
    TEST_ASSERT_EQUAL_FLOAT(9.0f, env.upper());

    env.update(2.0f); // apply additive decay again, clamp if crossed
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 2.0f, env.lower());
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 8.0f, env.upper());
}

void test_envelope_constructor_with_upper_and_decay(void)
{
    DataEnvelope env(10.0f, 0.2f); // upper initialized to 10, decay 20%, exponential
    TEST_ASSERT_EQUAL_FLOAT(0.0f, env.lower());
    TEST_ASSERT_EQUAL_FLOAT(10.0f, env.upper());

    env.update(5.0f); // decay toward 5
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.0f, env.lower());
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 9.0f, env.upper());
}

void test_envelope_constructor_with_bounds_and_decay(void)
{
    DataEnvelope env(-5.0f, 5.0f, 1.0f);
    env.setLinearDecay();
    TEST_ASSERT_EQUAL_FLOAT(-5.0f, env.lower());
    TEST_ASSERT_EQUAL_FLOAT(5.0f, env.upper());

    env.update(0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, -4.0f, env.lower());
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 4.0f, env.upper());
}

void test_envelope_normalized(void)
{
    DataEnvelope env(1.0f);
    env.update(0.0f);
    env.update(10.0f);

    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, env.normalized(10.0f));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, env.normalized(0.0f));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, env.normalized(5.0f));
}
