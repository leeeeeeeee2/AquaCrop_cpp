# Test configuration for AquaCrop C++

# Add test executable
add_executable(test_aquacrop test_main.cpp)

# Add test (just run the executable, no linking needed)
add_test(NAME aquacrop_test COMMAND test_aquacrop)
