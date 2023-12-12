import { bench, group, run } from "mitata";
import { treeparseHeaders } from "./tree.js";
import { naiveparseHeaders } from "./naive.js";
import { basicparseHeaders } from "./conventional.js";
import { ternarytreeparseHeaders } from "./ternarytree.js";
import { switchparseHeaders } from "./switch.js";

const headers = Array.from(
  [
    {
      "Content-Type": "application/json",
      "Date": "Wed, 01 Nov 2023 00:00:00 GMT",
      "Powered-By": "NodeJS",
      "Content-Encoding": "gzip",
      "Set-Cookie": "__Secure-ID=123; Secure; Domain=example.com",
      "Content-Length": "150",
      "Vary": "Accept-Encoding, Accept, X-Requested-With",
    },
    {
      "Content-Type": "text/html; charset=UTF-8",
      "Content-Length": "1234",
      "Date": "Wed, 06 Dec 2023 12:47:57 GMT",
      "Server": "Bing",
    },
    {
      "Content-Type": "image/jpeg",
      "Content-Length": "56789",
      "Date": "Wed, 06 Dec 2023 12:48:12 GMT",
      "Server": "Bing",
      "ETag": '"a1b2c3d4e5f6g7h8i9j0"',
    },
    {
      "Cookie": "session_id=1234567890abcdef",
      "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64)",
      "Host": "www.bing.com",
      "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
      "Accept-Language": "en-US,en;q=0.5",
      "Accept-Encoding": "gzip, deflate, br",
    },
    {
      "Location": "https://www.bing.com/search?q=bing",
      "Status": "302 Found",
      "Date": "Wed, 06 Dec 2023 12:48:27 GMT",
      "Server": "Bing",
      "Content-Type": "text/html; charset=UTF-8",
      "Content-Length": "0",
    },
    {
      "Content-Type":
        "multipart/form-data; boundary=----WebKitFormBoundary1234567890",
      "Content-Length": "98765",
      "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64)",
      "Host": "www.bing.com",
      "Accept": "*/*",
      "Accept-Language": "en-US,en;q=0.5",
      "Accept-Encoding": "gzip, deflate, br",
    },
    {
      "Content-Type": "application/json; charset=UTF-8",
      "Content-Length": "2345",
      "Date": "Wed, 06 Dec 2023 12:48:42 GMT",
      "Server": "Bing",
      "Status": "200 OK",
      "Cache-Control": "no-cache, no-store, must-revalidate",
    },
    {
      "Host": "www.example.com",
      "Connection": "keep-alive",
      "Accept": "text/html, application/xhtml+xml, application/xml;q=0.9,;q=0.8",
      "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64)",
    },
    {
      "Host": "google.com",
      "User-Agent": "curl/7.76.1",
      "Accept": "*/*"
    }
  ],
  (x) =>
    Object.entries(x)
      .flat()
      .map((c) => Buffer.from(c))
);

bench("noop", () => {});
bench("noop", () => {});
group("parseHeaders", () => {
  bench("naive", () => {
    for (let x of headers) {
      naiveparseHeaders(x);
    }
  });
  bench("basic", () => {
    for (let x of headers) {
      basicparseHeaders(x);
    }
  });
  bench("switch", () => {
    for (let x of headers) {
      switchparseHeaders(x);
    }
  });
  bench("ternary tree", () => {
    for (let x of headers) {
      ternarytreeparseHeaders(x, undefined, true);
    }
  });
  bench("digital search trie", () => {
    for (let x of headers) {
      treeparseHeaders(x, undefined, true);
    }
  });
});
await new Promise((resolve) => setTimeout(resolve, 10000));

await run();
await run();
