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
            -h|--help)
                usage
                exit 0
        esac
        shift
    done
    HPP_FILE="$PROJECT_DIR/config/config.hpp"
}

generate_hpp() {
    MAGIC_NUMBER=4 # Used to calculate the size of the JsonDocument in memory
    BOARD_CONFIG="$(yq --no-colors --output-format json "$BOARD_PATH" | jq -c -M)"
    LAYOUT_CONFIG="$(yq --no-colors --output-format json "$LAYOUT_PATH" | jq -c -M)"
    cat << EOF
#ifndef BOARD_CONFIG_JSON
#define BOARD_CONFIG_JSON "$(echo "$BOARD_CONFIG"  | sed 's:":\\":g')"
#endif
#ifndef BOARD_CONFIG_JSON_SIZE
#define BOARD_CONFIG_JSON_SIZE $(( $(echo "$BOARD_CONFIG" | wc -c) * MAGIC_NUMBER))
#endif
#ifndef LAYOUT_CONFIG_JSON
#define LAYOUT_CONFIG_JSON "$(echo "$LAYOUT_CONFIG" | sed 's:":\\":g')"
#endif
#ifndef LAYOUT_CONFIG_JSON_SIZE
#define LAYOUT_CONFIG_JSON_SIZE $(( $(echo "$LAYOUT_CONFIG" | wc -c) * MAGIC_NUMBER))
#endif
EOF
}

parse_args "$@"
generate_hpp > "$HPP_FILE"
