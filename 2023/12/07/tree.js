import {
  wellknownHeaderNames,
  headerNameLowerCasedRecord,
} from "./constants.js";

class Tree {
  #node;
  /** @type {number} */
  #depth;
  constructor() {
    this.#node = {};
    this.#depth = 0;
  }

  get_node() {
    return this.#node;
  }

  /**
   * @param {string} value
   */
  insert(value) {
    const target = Buffer.from((value = value.toLowerCase()));
    const length = target.length;
    let node = this.#node;
    for (let i = 0; i < length; ++i) {
      const key = target[i];
      node[key] ??= {};
      if (key >= 0x61 && key <= 0x7a) {
        node[key & ~32] ??= node[key];
      }
      node = node[key];
    }
    node[256] = value;
    if (length > this.#depth) {
      this.#depth = length;
    }
  }

  /**
   * @param {Uint8Array} buffer
   * @returns {string | null}
   */
  lookup(buffer) {
    const length = buffer.length;
    if (length > this.#depth) return null;
    let node = this.#node;
    for (let i = 0; i < length; ++i) {
      if ((node = node?.[buffer[i]]) === undefined) return null;
    }
    return node?.[256] ?? null;
  }
}

const tree = new Tree();
for (let i = 0; i < wellknownHeaderNames.length; ++i) {
  tree.insert(wellknownHeaderNames[i]);
}
//console.log(JSON.stringify(tree.get_node(), null, 4))

function headerNameToStringUnsafe(value) {
  return typeof value === "string"
    ? headerNameLowerCasedRecord[value] ?? value.toLowerCase()
    : tree.lookup(value) ?? value.toString().toLowerCase();
}

export function treeparseHeaders(headers, obj) {
  // For H2 support
  if (!Array.isArray(headers)) return headers;

  if (obj === undefined) obj = {};
  for (let i = 0; i < headers.length; i += 2) {
    const key = headerNameToStringUnsafe(headers[i]);
    let val = obj[key];

    if (!val) {
      const headersValue = headers[i + 1];
      if (typeof headersValue === "string") {
        obj[key] = headersValue;
      } else {
        obj[key] = Array.isArray(headersValue)
          ? headersValue.map((x) => x.toString("utf8"))
          : headersValue.toString("utf8");
      }
    } else {
      if (typeof val === "string") {
        val = [val];
        obj[key] = val;
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
