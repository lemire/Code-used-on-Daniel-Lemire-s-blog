#!/bin/bash
# md2html: Quick Markdown to HTML converter
if [ $# -eq 0 ]; then
    echo "Usage: md2html <file.md>"
    exit 1
fi
SCRIPT_DIR=$(dirname "$0")
uv run --with markdown --with pygments  "$SCRIPT_DIR/mdtohtml.py" "$1"
