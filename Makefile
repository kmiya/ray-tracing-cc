.PHONY: all build release run format lint test clean

all build:
	bazel build -c opt //...

debug:
	bazel build -c dbg //...

run:
	bazel run -c opt rt -- $(ARGS)

format:
	bazel run //tools:format

lint:
	bazel build --config=lint //...
	cat "$(shell bazel info bazel-bin)"/src/*.AspectRulesLintClangTidy.out

test:
	bazel test //...

clean:
	bazel clean

asan:
	bazel build --config=asan //...

msan:
	bazel build --config=msan //...

ubsan:
	bazel build --config=ubsan //...

tsan:
	bazel build --config=tsan //...

tysan:
	bazel build --config=tysan //...

asan-run: asan
	bazel run rt -- $(ARGS) > /dev/null

msan-run: msan
	bazel run rt -- $(ARGS) > /dev/null

ubsan-run: ubsan
	bazel run rt -- $(ARGS) > /dev/null

tsan-run: tsan
	bazel run rt -- $(ARGS) > /dev/null

tysan-run: tysan
	bazel run rt -- $(ARGS) > /dev/null
