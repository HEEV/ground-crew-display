set(JUCE_BUILD_EXTRAS off CACHE BOOL "Disables projucer" FORCE)
set(JUCE_BUILD_EXAMPLES off CACHE BOOL "Disables JUCE examples" FORCE)
add_subdirectory(JUCE)
add_subdirectory(Fast-DDS-Communication-Manager)