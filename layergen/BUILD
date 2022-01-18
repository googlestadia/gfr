package(default_visibility = ["//visibility:public"])

load("@rules_python//python:defs.bzl", "py_binary")

py_binary(
    name = "gfr_layer",
    srcs = ["gfr_layer.py"],
    data = ["@vulkan_headers//:vk_xml"],
    python_version = "PY3",
    deps = [
        "//gputools/layergen:layergen_utils",
        "@rules_python//python/runfiles",
        "@vkspecgen//vkapi",
    ],
)
