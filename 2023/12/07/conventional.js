import { headerNameLowerCasedRecord } from "./constants.js";
export function basicparseHeaders(headers, obj = {}) {
  // For H2 support
  if (!Array.isArray(headers)) return headers;

  for (let i = 0; i < headers.length; i += 2) {
    const key = headers[i].toString();
    const lowerCasedKey = headerNameLowerCasedRecord[key] ?? key.toLowerCase();
    let val = obj[lowerCasedKey];

    if (!val) {
      const headersValue = headers[i + 1];
      if (typeof headersValue === "string") {
        obj[lowerCasedKey] = headersValue;
      } else {
        obj[lowerCasedKey] = Array.isArray(headersValue)
          ? headersValue.map((x) => x.toString("utf8"))
          : headersValue.toString("utf8");
      }
    } else {
      if (!Array.isArray(val)) {
        val = [val];
        obj[lowerCasedKey] = val;
      }
      val.push(headers[i + 1].toString("utf8"));
    }
  }

  // See https://github.com/nodejs/node/pull/46528
  if ("content-length" in obj && "content-disposition" in obj) {
    obj["content-disposition"] = Buffer.from(
      obj["content-disposition"]
    ).toString("latin1");
  }

  return obj;
}
