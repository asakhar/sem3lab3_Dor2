project(TimingChart LANGUAGES CXX)
cmake_minimum_required(VERSION 3.12)

add_executable(Static main.cpp TimingChart.cpp)
add_executable(Operators mainOperators.cpp TimingChartOperators.cpp)

# add_executable(StaticEdition main1.cpp LogicGate.cpp)

# add_executable(DynamicEdition main.cpp LogicGateDynamic.cpp)
# # without menu
# add_executable(OperatorsEdition LogicGateOperators.cpp)