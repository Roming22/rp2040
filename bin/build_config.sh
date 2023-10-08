#/bin/bash
set -o errexit
set -o nounset
set -o pipefail

usage() {
    cat << EOF
Create an hpp file with the chosen configuration.

Options:
    -b, --board BOARD_PATH      Path to the json file holding the board configuration.
    -l, --layout LAYOUT_PATH    Path to the json file holding the layout configuration.
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
            -b|--board)
                BOARD_PATH="$2"
                shift
                ;;
            -l|--layout)
                LAYOUT_PATH="$2"
                shift
                ;;
            -d|--debug)
                set -x
                ;;
            -d|--debug)
                set -x
                ;;
            -h|--help)
                usage
                exit 0
        esac
        shift
    done
    HPP_FILE="$PROJECT_DIR/config/config.hpp"
}

generate_hpp() {
    cat << EOF
#ifndef BOARD_CONFIG_JSON
#define BOARD_CONFIG_JSON "$(yq --no-colors --output-format json "$BOARD_PATH" | jq -c -M | sed 's:":\\":g')"
#endif
#ifndef LAYOUT_CONFIG_JSON
#define LAYOUT_CONFIG_JSON "$(yq --no-colors --output-format json "$LAYOUT_PATH" | jq -c -M | sed 's:":\\":g')"
#endif
EOF
}

parse_args "$@"
generate_hpp > "$HPP_FILE"
