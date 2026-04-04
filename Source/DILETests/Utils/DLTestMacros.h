// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

/*
 *  Helper Macros to simplify testing scenarios that may lead to crash if condition is not met
 *  Write Test conditions in Tests only using these macros
 */

/**
 * Logs an error if the two values are not equal.
 *
 * @param Actual - Actual value.
 * @param Expected - Expected value.
 */
#define DileTestEqual(Actual, Expected) \
    if (!TestEqual(#Actual, Actual, Expected)) { return; }

/**
 * Logs an error if the two values are not equal with given tolerance. Used with floating point values
 *
 * @param Actual - Actual value.
 * @param Expected - Expected value.
 * @param Tolerance - Maximum allowed difference between values.
 */
#define DileTestEqualTolerance(Actual, Expected, Tolerance) \
    if (!TestEqual(#Actual, Actual, Expected, Tolerance)) { return; }

/**
 * Logs an error if the two values are equal.
 *
 * @param Actual - Actual value.
 * @param Expected - Expected value.
 */
#define DileTestNotEqual(Actual, Expected) \
    if (!TestNotEqual(#Actual, Actual, Expected)) { return; }

/**
 * Logs an error if the two values are not the same object in memory.
 *
 * @param Actual - The actual value.
 * @param Expected - The expected value.
 */
#define DileTestSame(Actual, Expected) \
    if (!TestSame(#Actual, Actual, Expected)) { return; }

/**
 * Logs an error if the two values are the same object in memory.
 *
 * @param Actual - The actual value.
 * @param Expected - The expected value.
 */
#define DileTestNotSame(Actual, Expected) \
    if (!TestNotSame(#Actual, Actual, Expected)) { return; }

/**
 * Logs an error if the specified Boolean value is not true.
 *
 * @param Value - The value to test.
 */
#define DileTestTrue(Value) \
    if (!TestTrue(#Value, Value)) { return; }

/**
 * Logs an error if the specified Boolean value is not false.
 *
 * @param Value - The value to test.
 */
#define DileTestFalse(Value) \
    if (!TestFalse(#Value, Value)) { return; }

/**
 * Logs an error if the given shared pointer is not valid.
 *
 * @param SharedPointer - The shared pointer to test.
 */
#define DileTestValid(SharedPointer) \
    if (!TestValid(#SharedPointer, SharedPointer)) { return; }

/**
 * Logs an error if the given shared pointer is valid.
 *
 * @param SharedPointer - The shared pointer to test.
 */
#define DileTestInvalid(SharedPointer) \
    if (!TestInvalid(#SharedPointer, SharedPointer)) { return; }

/**
 * Logs an error if the specified pointer is not NULL.
 *
 * @param Pointer - The pointer to test.
 */
#define DileTestNull(Pointer) \
    if (!TestNull(#Pointer, Pointer)) { return; }

/**
 * Logs an error if the specified pointer is NULL.
 *
 * @param Pointer - The pointer to test.
 */
#define DileTestNotNull(Pointer) \
    if (!TestNotNull(#Pointer, Pointer)) { return; }
