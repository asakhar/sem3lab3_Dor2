project(TimingChart LANGUAGES CXX)
cmake_minimum_required(VERSION 3.12)

find_package(GTest REQUIRED)

add_executable(Static main.cpp TimingChart.cpp)
add_executable(Operators mainOperators.cpp TimingChartOperators.cpp)

enable_testing()

add_executable(Tests test.cpp TimingChartOperators.cpp)
target_link_libraries(Tests GTest::GTest GTest::Main)

add_test(AllTests Tests)
# add_executable(StaticEdition main1.cpp LogicGate.cpp)

# add_executable(DynamicEdition main.cpp LogicGateDynamic.cpp)
# # without menu
# add_executable(OperatorsEdition LogicGateOperators.cpp)