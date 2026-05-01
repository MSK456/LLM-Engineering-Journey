#include <iostream> // Required for std::cout, std::fixed, std::endl
#include <iomanip>  // Required for std::setprecision
#include <chrono>   // Required for high_resolution_clock and duration

/**
 * @brief Calculates a series sum based on the given parameters.
 *        The calculation is: result = 1.0 + Sum_{i=1 to iterations} (1 / (i * param1 + param2) - 1 / (i * param1 - param2))
 *
 * @param iterations The number of iterations for the loop.
 * @param param1 A constant integer parameter used in the calculation.
 * @param param2 A constant integer parameter used in the calculation.
 * @return The final calculated double-precision floating-point result.
 */
double calculate(long long iterations, int param1, int param2) {
    double result = 1.0; // Initialize result as a double
    
    // Loop from 1 up to iterations (inclusive).
    // Using long long for 'i' to handle large iteration counts (e.g., 200,000,000)
    // and prevent potential overflow when calculating 'i * param1' if 'i' were int.
    for (long long i = 1; i <= iterations; ++i) {
        // Explicitly cast 'i' to double before multiplication to ensure floating-point arithmetic.
        // param1 (int) will be promoted to double during multiplication.
        double i_double_param1 = static_cast<double>(i) * param1;

        // Calculate the two distinct 'j' values as doubles.
        double j_minus = i_double_param1 - param2;
        double j_plus = i_double_param1 + param2;

        // Perform the operations. Using 1.0 for double-precision division.
        // It's equivalent to result += (1.0 / j_plus) - (1.0 / j_minus);
        result -= (1.0 / j_minus);
        result += (1.0 / j_plus);
    }
    return result;
}

int main() {
    // Record the start time using high-resolution clock for accurate timing.
    auto start_time = std::chrono::high_resolution_clock::now();

    // Call the calculate function with the specified parameters.
    // 200'000'000LL uses C++14 digit separators and 'LL' suffix for long long literal.
    double final_result = calculate(200'000'000LL, 4, 1) * 4;

    // Record the end time.
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the duration of the execution.
    // std::chrono::duration<double> converts the duration to seconds as a double.
    std::chrono::duration<double> execution_time = end_time - start_time;

    // Print the final result formatted to 12 decimal places.
    std::cout << std::fixed << std::setprecision(12) << "Result: " << final_result << std::endl;
    
    // Print the execution time formatted to 6 decimal places.
    std::cout << std::fixed << std::setprecision(6) << "Execution Time: " << execution_time.count() << " seconds" << std::endl;

    return 0; // Indicate successful execution.
}
