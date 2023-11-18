#/bin/bash
set -o errexit
set -o nounset
set -o pipefail

usage() {
    cat << EOF
Create an h file with the project files.
This allows for both for inlining and improving optimizations
significantly.

Options:
    -d, --debug                 Debug mode.
    -h, --help                  Print this message.
EOF

}
PROJECT_DIR=$(
    cd $(dirname $0)/.. >/dev/null;
    pwd;
)
cd "$PROJECT_DIR"

parse_args(){
    while [[ "$#" -gt "0" ]]; do
        case $1 in
            -d|--debug)
                set -x
                ;;
            -h|--help)
                usage
                exit 0
        esac
        shift
    done
    HPP_FILE="$PROJECT_DIR/firmware.hpp"
}

generate_hpp() {
    for extension in hpp h cpp; do
        find firmware/ -name \*.$extension | sort
    done | sed 's:.*:#include "\0":'
}

parse_args "$@"
generate_hpp > "$HPP_FILE"
