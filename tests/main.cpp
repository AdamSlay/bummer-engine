#include <iostream>

int test_resources();
int test_ui();

int main()
{
    // Run test suites
    int resourcesResult = test_resources();
    int uiResult = test_ui();

    // Print test results
//    std::cout << "Test Results:" << std::endl;
//    std::cout << "  Resource Tests: " << (resourcesResult == 0 ? "PASSED" : "FAILED") << std::endl;
//    std::cout << "  UI Tests: " << (uiResult == 0 ? "PASSED" : "FAILED") << std::endl;

    return resourcesResult || uiResult;
}