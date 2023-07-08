package(default_visibility = ["//visibility:public"])

cc_library(
	name = "algebra",
	hdrs = [
		"algebra.hpp"
	],
)

cc_library(
    name = "random",
    hdrs = [
        "random.hpp"
    ]
)

cc_library(
	name = "matrix",
	srcs = [
		"matrix.cpp"
	],
	hdrs = [
		"matrix.hpp"
	],
	deps = [
		"//lib/aes-128:defines",
		":algebra"
	]
)
