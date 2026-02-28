We locate web content using special addresses called URLs. We are all familiar with addresses like https://google.com.
Sometimes, URLs can get long and they can become difficult to read. Thus, we might be tempted to format them
like so in HTML using newline and tab characters, like so:

```html
<a href="https://lemire.me/blog/2026/02/21/
        how-fast-do-browsers-correct-utf-16-strings/">my blog post</a>
```

It will!

Let us refer to the WHATWG URL specification that browsers follow. It makes two statements in sequence.

1. If input contains any ASCII tab or newline, invalid-URL-unit validation error.
2. Remove all ASCII tab or newline from input.

Notice how it reports an error if there is a tab or newline character, but continues anyway? The specification says that *A validation error does not mean that the parser terminates* and it encourages systems to report errors *somewhere*. Effectively, the error is ignored although it might be logged. Thus our HTML is fine in practice.

The following is also fine:

```html
<a href="https://go
ogle.c
om" class="button">Visit Google</a>
```

You can also use tabs. But you cannot arbitrarily insert any other whitespace.

Yet there are cases when you can use any ASCII whitespace character: data URLs. Data URLs (also called data URIs) embed small files—like images, text, or other content—directly inside a URL string, instead of linking to an external resource. Data URLs are a *special kind* of URL and they follow different rules.


A typical data URL might look like `data:image/png;base64,iVBORw0KGgoAAAANSUhEUg...` where the string `iVBORw0KGgoAAAANSUhEUg...` is the binary data of the image that has been encoded with base64. Base64 is a text format that can represent any binary content: we use 64 ASCII characters so that each character encodes 6 bits. Your binary email attachments are base64 encoded.

On the web, when decoding a base64 string, you ignore all ASCII whitespaces (including the space character itself).
Thus you can embed a PNG image in HTML as follows.

```html
<img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAQAAAAECAIAAAAmkwkpAAAAEUlEQVR4nG
                                P8z4AATEhsPBwAM9EBBzDn4UwAAAAASUVORK5CYII=" />
```

This HTML code is valid and will insert a tiny image in your page.


But there is more. A data URL can also be used to insert an SVG image. SVG (Scalable Vector Graphics) is an XML-based vector image format that describes 2D graphics using mathematical paths, shapes, and text instead of pixels.
The following should draw a very simple sunset:

```html
<img src='data:image/svg+xml,
<svg width="200" height="200" xmlns="http://www.w3.org/2000/svg">
  <rect width="100%" height="100%" fill="blue" /> <!-- the sky -->
  <circle cx="100" cy="110" r="50" fill="yellow" />  <!-- the sun -->
  <rect x="0" y="120" width="200" height="80" fill="brown" />  <!-- the ground -->
</svg>' />
```

Observe how I was able to format the SVG code so that it is readable.

