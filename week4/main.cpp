
#include <iostream>
#include <chrono>   // For high-resolution timing
#include <iomanip>  // For std::fixed and std::setprecision

// Function to perform the calculation
// Using long long for iterations to handle large counts,
// and double for floating-point accuracy.
double calculate(long long iterations, int param1, int param2) {
    double result = 1.0;

    // Precompute constants outside the loop for performance
    const double param2_double = static_cast<double>(param2);
    const double term_numerator = -2.0 * param2_double; // Part of the simplified expression: (-2 * param2)
    const double param2_sq = param2_double * param2_double; // param2^2
    const double param1_double = static_cast<double>(param1);

    // The Python loop range(1, iterations+1) translates to i from 1 to iterations (inclusive).
    for (long long i = 1; i <= iterations; ++i) {
        // Original calculation for one iteration:
        // double j_minus = static_cast<double>(i) * param1_double - param2_double;
        // result -= (1.0 / j_minus);
        // double j_plus = static_cast<double>(i) * param1_double + param2_double;
        // result += (1.0 / j_plus);

        // This can be simplified mathematically as:
        // result += (1/j_plus) - (1/j_minus)
        // = (j_minus - j_plus) / (j_plus * j_minus)
        // = ((i*param1 - param2) - (i*param1 + param2)) / ((i*param1 + param2)*(i*param1 - param2))
        // = (-2 * param2) / ((i*param1)^2 - param2^2)
        //
        // Applying the simplified form:
        double i_p1 = static_cast<double>(i) * param1_double; // i * param1
        double denominator = i_p1 * i_p1 - param2_sq;      // (i * param1)^2 - param2^2

        // Add the simplified term to the result.
        // The denominator is guaranteed not to be zero for param1=4, param2=1
        // as i*4=1 has no integer solution for i.
        result += term_numerator / denominator;
    }
    return result;
}

int main() {
    // Optimize C++ standard streams for faster input/output,
    // though for just two print statements, the impact is minimal.
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // Record the start time
    auto start_time = std::chrono::high_resolution_clock::now();

    // Call the calculate function with the specified parameters
    // 200'000'000LL explicitly makes it a long long literal
    double final_result = calculate(200'000'000LL, 4, 1) * 4;

    // Record the end time
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    std::chrono::duration<double> execution_time = end_time - start_time;

    // Print the result formatted to 12 decimal places
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Result: " << final_result << std::endl;

    // Print the execution time formatted to 6 decimal places
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Execution Time: " << execution_time.count() << " seconds" << std::endl;

    return 0;
}
