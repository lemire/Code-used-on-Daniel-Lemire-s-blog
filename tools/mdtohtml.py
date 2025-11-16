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
    parser.add_argument("--style", default="tango", help="Pygments style to use for highlighting (default: tango)")
    args = parser.parse_args()

    # Read input file
    with open(args.input, "r", encoding="utf-8") as f:
        text = f.read()

    # Configure Markdown
    extensions = ['tables', 'fenced_code']
    if pygments_available:
        # Determine available styles and pick a valid one (fall back if requested style is missing)
        try:
            from pygments.styles import get_all_styles
            available_styles = set(get_all_styles())
        except Exception:
            available_styles = set()

        requested_style = args.style or "tango"
        final_style = requested_style
        if available_styles and requested_style not in available_styles:
            # try a few sensible fallbacks
            fallbacks = [requested_style + '-light', requested_style + '-dark', 'github-dark', 'default', 'xcode', 'solarized-light', 'tango']
            for s in fallbacks:
                if s in available_styles:
                    final_style = s
                    break
            else:
                final_style = 'default'
            print(f"Warning: requested Pygments style '{requested_style}' not found; using '{final_style}' instead.")

        extensions.append(CodeHiliteExtension(linenums=None, pygments_style=final_style, css_class="highlight", noclasses=True))
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
