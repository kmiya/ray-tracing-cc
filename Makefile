.PHONY: all build release run format lint test clean

all build:
	bazel build //...

release:
	bazel build -c opt //...

run:
	bazel run rt -- $(ARGS)

format:
	bazel run //tools:format

lint:
	bazel build --config=lint //...
	cat "$(shell bazel info bazel-bin)"/src/*.AspectRulesLintClangTidy.out

test:
	bazel test //...

clean:
	bazel clean
