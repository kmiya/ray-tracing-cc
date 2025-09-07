.PHONY: all build run format lint test clean

all build:
	bazel build //...

run:
	bazel run rt -- $(ARGS)

format:
	bazel run //tools:format

lint:
	bazel build --config=lint //...

test:
	bazel test //...

clean:
	bazel clean
