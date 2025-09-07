"""rules_lint config
Copied from: https://github.com/aspect-build/rules_lint/blob/v1.7.0/example/tools/lint/linters.bzl
"""

load("@aspect_rules_lint//lint:clang_tidy.bzl", "lint_clang_tidy_aspect")

clang_tidy = lint_clang_tidy_aspect(
    binary = Label("@llvm_toolchain//:clang-tidy"),
    configs = [
        Label("//:.clang-tidy"),
    ],
    lint_target_headers = False,
    angle_includes_are_system = True,
    verbose = False,
)
