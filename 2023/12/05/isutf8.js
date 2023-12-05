"use strict";

import { bench, run } from "mitata";
import { existsSync, createWriteStream, readFileSync, mkdirSync } from "node:fs";
import { isUtf8 } from "node:buffer";
import path from "node:path";
import axios from "axios";

const fixturesFolderPath = new URL('fixtures', import.meta.url).pathname;
const urls = [
  "https://github.com/lemire/unicode_lipsum/raw/main/lipsum/Arabic-Lipsum.utf8.txt",
  "https://github.com/lemire/unicode_lipsum/raw/main/lipsum/Chinese-Lipsum.utf8.txt",
  "https://github.com/lemire/unicode_lipsum/raw/main/lipsum/Latin-Lipsum.utf8.txt"
];

function get_filename(url) {
  return `fixtures/${path.basename(url)}`;
}

async function downloadFile(url) {
  const response = await axios({
    method: "GET",
    url: url,
    responseType: "stream",
  });

  response.data.pipe(createWriteStream(get_filename(url)));

  return new Promise((resolve, reject) => {
    response.data.on("end", () => {
      resolve();
    });

    response.data.on("error", (err) => {
      reject(err);
    });
  });
}

if (!existsSync(fixturesFolderPath)) {
  mkdirSync(fixturesFolderPath)
}
const urls_for_download = urls.filter(url => !existsSync(get_filename(url)))
const all_promises = urls_for_download.map((url) => downloadFile(url));
await axios.all(all_promises);


var lengths = [];

for (let source of urls) {
  const filename = get_filename(source);
  const file_content = readFileSync(filename);
  lengths.push(file_content.length);

  bench(filename, () => {
    if(!isUtf8(file_content)) { console.log("not UTF-8"); }
  });
}

let z = await run();
for (let i = 0; i<urls.length; i++) {
  console.log(`${urls[i]}: ${lengths[i] / z["benchmarks"][i]["stats"].avg}  length : ${lengths[i]} time : ${z["benchmarks"][i]["stats"].avg} ns   `);
}
