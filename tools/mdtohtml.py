#!/usr/bin/env python3
import argparse
import markdown
from pathlib import Path

try:
    from markdown.extensions.codehilite import CodeHiliteExtension
    from pygments.formatters import HtmlFormatter
    pygments_available = True
except ImportError:
    print("Pygments not found. Code highlighting will be disabled.")
    pygments_available = False

def main():
    parser = argparse.ArgumentParser(description="Convert Markdown to HTML with syntax highlighting.")
    parser.add_argument("input", help="Input Markdown file")
    parser.add_argument("output", nargs="?", default=None, help="Output HTML file (default: input.html)")
    parser.add_argument("--css", action="store_true", help="Include inline CSS for basic styling")
    args = parser.parse_args()

    # Read input file
    with open(args.input, "r", encoding="utf-8") as f:
        text = f.read()

    # Configure Markdown
    extensions = ['tables', 'fenced_code']
    if pygments_available:
        extensions.append(CodeHiliteExtension(linenums=None, css_class="highlight", noclasses=True))
    md = markdown.Markdown(extensions=extensions)

    # Convert
    html = md.convert(text)

    # Add basic CSS if requested
    if args.css:
        css = """
        <style>
            body { font-family: Arial, sans-serif; max-width: 800px; margin: 0 auto; padding: 20px; }
            .highlight { background: #f8f8f8; border: 1px solid #ddd; padding: 10px; border-radius: 4px; }
            pre { margin: 0; }
        </style>
        """
        html = css + html

    # Write output
    output_file = args.output or Path(args.input).with_suffix(".html")
    with open(output_file, "w", encoding="utf-8") as f:
        f.write(f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <title>{Path(args.input).stem}</title>
</head>
<body>
    {html}
</body>
</html>""")

    print(f"Converted {args.input} to {output_file}")

if __name__ == "__main__":
    main()
