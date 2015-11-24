add_executable(SoundTest
    SoundTest.cpp
)

# Use WinMain instead of main
set_target_properties(SoundTest PROPERTIES LINK_FLAGS_DEBUG   "/SUBSYSTEM:windows")
set_target_properties(SoundTest PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
