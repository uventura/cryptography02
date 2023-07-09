package(default_visibility = ["//visibility:public"])


cc_library(
    name = "defines",
    srcs = ["defines.hpp"],
)
cc_library(
    name = "rsa",
    srcs = [
        "rsa.cpp"
    ],
    hdrs = [
        "rsa.hpp"
    ],
    deps = [
        ":defines"
    ],
    linkopts = [
		"-lgmp",
		"-lgmpxx",
	],
)
