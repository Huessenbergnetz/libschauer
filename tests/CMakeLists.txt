project(schauer_tests)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} 5.6.0 REQUIRED COMPONENTS Test)

add_executable(testdefaultvalues_exec testdefaultvalues.cpp testconfig.h testconfig.cpp testnamfactory.h testnamfactory.cpp)
add_test(NAME testdefaultvalues COMMAND testdefaultvalues_exec)
target_link_libraries(testdefaultvalues_exec Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Network SchauerQt${QT_VERSION_MAJOR}::Core)

function(schauer_unit_test _testname)
    add_executable(${_testname}_exec ${_testname}.cpp testconfig.h testconfig.cpp)
    add_test(NAME ${_testname} COMMAND ${_testname}_exec)
    target_link_libraries(${_testname}_exec Qt${QT_VERSION_MAJOR}::Test SchauerQt${QT_VERSION_MAJOR}::Core)
endfunction(schauer_unit_test)

schauer_unit_test(testmodels)
schauer_unit_test(testjobs)

if (WITH_API_TESTS)
    add_executable(testapicalls_exec testapicalls.cpp testconfig.cpp testconfig.h)
    add_test(NAME testapicalls COMMAND testapicalls_exec)
    target_link_libraries(testapicalls_exec Qt${QT_VERSION_MAJOR}::Test SchauerQt${QT_VERSION_MAJOR}::Core)
endif (WITH_API_TESTS)
