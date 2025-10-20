#!/bin/bash
# md2html: Quick Markdown to HTML converter
if [ $# -eq 0 ]; then
    echo "Usage: md2html <file.md>"
    exit 1
fi
uv run --with markdown --with pygments mdtohtml.py "$1"
