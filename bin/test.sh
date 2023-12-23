#/bin/bash
set -o errexit
set -o nounset
set -o pipefail

usage() {
    cat << EOF
Create an hpp file with the chosen configuration.

Options:
    -t, --test $TEST_PATH   File to test.
    -d, --debug             Debug mode.
    -h, --help              Print this message.
EOF

}
PROJECT_DIR=$(
    cd $(dirname $0)/.. >/dev/null;
    pwd;
)
TEST_DIR="$PROJECT_DIR/tests"

parse_args(){
    TEST_PATH="tests/firmware.cpp"
    while [[ "$#" -gt "0" ]]; do
        case $1 in
            -t|--test)
                shift
                TEST_PATH="$1"
                ;;
            -d|--debug)
                set -x
                ;;
            --help|help)
                usage
                exit 0
                ;;
            *)
                usage
                echo "Unknown argument: $1" >&2
                exit 1
                ;;
        esac
        shift
    done
}

init() {
    cd "$PROJECT_DIR"
    TEST_NAME=$(basename "$TEST_PATH" | sed "s:.cpp::")
}

build() {
    cat << EOF > CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  $TEST_NAME
  $TEST_PATH
)
target_link_libraries(
  $TEST_NAME
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests($TEST_NAME)
EOF
    cmake -S . -B build
    cmake --build build
}

run() {
    "./build/$TEST_NAME"
}

main() {
    parse_args "$@"
    init
    build
    run
}

main "$@"