cc_test(
    name = "aes_128",
    srcs = [
        "aes_128.cpp"
    ],
    deps = [
        "@gtest//:gtest_main",
        "//lib/aes-128:aes_128",
        "//lib/math:matrix",
        "//lib/math:random",
    ],
)

cc_test(
    name = "key",
    srcs = [
        "key.cpp"
    ],
    deps = [
        "@gtest//:gtest_main",
        "//lib/aes-128:key"
    ]
)

cc_test(
    name = "key_schedule",
    srcs = [
        "key_schedule.cpp"
    ],
    deps = [
        "@gtest//:gtest_main",
        "//lib/aes-128:aes_128",
        "//lib/aes-128:key_schedule",
        "//lib/aes-128:defines",
        "//lib/math:matrix",
    ]
)
