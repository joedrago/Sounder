add_executable(WinCoreTest
    WinCoreTest.cpp
)

# Use WinMain instead of main
set_target_properties(WinCoreTest PROPERTIES LINK_FLAGS_DEBUG   "/SUBSYSTEM:windows")
set_target_properties(WinCoreTest PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
