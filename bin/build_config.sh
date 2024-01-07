#/bin/bash
set -o errexit
set -o nounset
set -o pipefail

usage() {
    cat << EOF
Create an hpp file with the chosen configuration.

Options:
    -h, --hardware HARDWARE_PATH      Path to the json file holding the hardware configuration.
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
            -h|--hardware)
                HARDWARE_PATH="$2"
                shift
                ;;
            -l|--layout)
                LAYOUT_PATH="$2"
                shift
                ;;
            -d|--debug)
                set -x
                ;;
            --help|help)
                usage
                exit 0
        esac
        shift
    done
    HPP_FILE="$PROJECT_DIR/config/config.hpp"
}

generate_hpp() {
    HARDWARE_CONFIG="$(yq --no-colors --output-format json "$HARDWARE_PATH" | jq -c -M)"
    LAYOUT_CONFIG="$(yq --no-colors --output-format json "$LAYOUT_PATH" | jq -c -M)"
    cat << EOF
#ifndef HARDWARE_CONFIG_JSON
#define HARDWARE_CONFIG_JSON "$(echo "$HARDWARE_CONFIG"  | sed 's:":\\":g')"
#endif
#ifndef LAYOUT_CONFIG_JSON
#define LAYOUT_CONFIG_JSON "$(echo "$LAYOUT_CONFIG" | sed 's:":\\":g')"
#endif
EOF
}

parse_args "$@"
generate_hpp > "$HPP_FILE"
