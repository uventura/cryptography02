package(default_visibility = ["//visibility:public"])

cc_library(
	name = "defines",
	srcs = ["defines.hpp"],
)

cc_library(
	name = "key",
	srcs = [
		"key.cpp"
	],
	hdrs = [
		"key.hpp"
	],
	deps = [
		":defines",
		"//lib/math:random"
		"//lib/math:matrix"
	],
)

cc_library(
	name = "aes_128",
	srcs = [
		"aes_128.cpp"
	],
	hdrs = [
		"aes_128.hpp"
	],
	deps = [
		"//lib/math:matrix",
		":key",
		":defines"
	]
)
