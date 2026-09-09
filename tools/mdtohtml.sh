#!/bin/bash
# md2html: Quick Markdown to HTML converter
#
# Before converting, every PNG found next to the Markdown file (recursively) is
# re-encoded as lossless WebP for better compression, and the image references
# in the Markdown file are updated to point to the WebP files.
#
# Usage: md2html [--remove-png] <file.md>
#   --remove-png  delete the original PNG once the WebP has been produced

REMOVE_PNG=0
ARGS=()
for arg in "$@"; do
    case "$arg" in
        --remove-png) REMOVE_PNG=1 ;;
        *) ARGS+=("$arg") ;;
    esac
done
set -- "${ARGS[@]}"

if [ $# -eq 0 ]; then
    echo "Usage: md2html [--remove-png] <file.md>"
    exit 1
fi

SCRIPT_DIR=$(dirname "$0")
MD_FILE="$1"

if [ ! -f "$MD_FILE" ]; then
    echo "Error: '$MD_FILE' is not a file."
    exit 1
fi

MD_DIR=$(cd "$(dirname "$MD_FILE")" && pwd)

# --- PNG -> WebP -----------------------------------------------------------
if ! command -v cwebp >/dev/null 2>&1; then
    echo "Warning: cwebp not found (brew install webp); skipping PNG conversion."
else
    while IFS= read -r -d '' png; do
        webp="${png%.*}.webp"
        if [ -f "$webp" ] && [ ! "$png" -nt "$webp" ]; then
            echo "Up to date: ${webp#$MD_DIR/}"
        elif cwebp -quiet -lossless -z 9 -m 6 "$png" -o "$webp"; then
            old_size=$(wc -c < "$png" | tr -d ' ')
            new_size=$(wc -c < "$webp" | tr -d ' ')
            echo "Converted ${png#$MD_DIR/} -> ${webp#$MD_DIR/} (${old_size} -> ${new_size} bytes)"
        else
            echo "Warning: failed to convert ${png#$MD_DIR/}; keeping the PNG reference."
            continue
        fi

        # Rewrite references in the Markdown file: the path relative to the
        # Markdown file first, then the bare file name.
        old_rel="${png#$MD_DIR/}"
        new_rel="${webp#$MD_DIR/}"
        OLD="$old_rel" NEW="$new_rel" perl -0777 -i -pe 's/\Q$ENV{OLD}\E/$ENV{NEW}/g' "$MD_FILE"
        if [ "$old_rel" != "$(basename "$png")" ]; then
            OLD="$(basename "$png")" NEW="$(basename "$webp")" \
                perl -0777 -i -pe 's/\Q$ENV{OLD}\E/$ENV{NEW}/g' "$MD_FILE"
        fi

        if [ "$REMOVE_PNG" -eq 1 ]; then
            rm -f "$png"
        fi
    done < <(find "$MD_DIR" \( -name .git -o -name node_modules -o -name .venv \) -prune -o \
                            -type f \( -iname '*.png' \) -print0)
fi

# --- Markdown -> HTML ------------------------------------------------------
uv run --with markdown --with pygments  "$SCRIPT_DIR/mdtohtml.py" "$MD_FILE"
