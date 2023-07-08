cc_test(
    name = "matrix",
    srcs = [
        "matrix.cpp"
    ],
    deps = [
        "@gtest//:gtest_main",
        "//lib/aes-128:matrix"
    ],
)

cc_test(
    name = "aes_128",
    srcs = [
        "aes_128.cpp"
    ],
    deps = [
        "@gtest//:gtest_main",
        "//lib/aes-128:aes_128",
        "//lib/aes-128:matrix",
        "//lib/math:random",
    ],
)
