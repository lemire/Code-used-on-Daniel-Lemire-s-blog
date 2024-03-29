import { headerNameLowerCasedRecord, wellknownHeaderNames } from "./constants.js";

class TstNode {
  /** @type {any} */
  value = null
  /** @type {null | TstNode} */
  left = null
  /** @type {null | TstNode} */
  middle = null
  /** @type {null | TstNode} */
  right = null
  /** @type {number} */
  code
  /**
   * @param {Uint8Array} key
   * @param {any} value
   */
  constructor (key, value) {
    if (key.length === 0) {
      throw new TypeError('Unreachable')
    }
    this.code = key[0]
    if (key.length > 1) {
      this.middle = new TstNode(key.subarray(1), value)
    } else {
      this.value = value
    }
  }

  /**
   * @param {Uint8Array} key
   * @param {any} value
   */
  add (key, value) {
    const code = key[0]
    if (this.code === code) {
      if (key.length === 1) {
        this.value = value
      } else if (this.middle !== null) {
        this.middle.add(key.subarray(1), value)
      } else {
        this.middle = new TstNode(key.subarray(1), value)
      }
    } else if (this.code < code) {
      if (this.left !== null) {
        this.left.add(key, value)
      } else {
        this.left = new TstNode(key, value)
      }
    } else {
      if (this.right !== null) {
        this.right.add(key, value)
      } else {
        this.right = new TstNode(key, value)
      }
    }
  }

  /**
   * @param {Uint8Array} key
   * @return {TstNode | null}
   */
  search (key) {
    const keylength = key.length
    let index = 0
    let node = this
    while (node !== null && index < keylength) {
      let code = key[index]
      // A-Z
      if (code >= 0x41 && code <= 0x5a) {
        // Lowercase for uppercase.
        code |= 32
      }
      while (node !== null) {
        if (code === node.code) {
          if (keylength === ++index) {
            // Returns Node since it is the last key.
            return node
          }
          node = node.middle
          break
        }
        node = node.code < code ? node.left : node.right
      }
    }
    return null
  }
}

class TernarySearchTree {
  /** @type {TstNode | null} */
  node = null

  /**
   * @param {Uint8Array} key
   * @param {any} value
   * */
  insert (key, value) {
    if (this.node === null) {
      this.node = new TstNode(key, value)
    } else {
      this.node.add(key, value)
    }
  }

  /**
   * @param {Uint8Array} key
   */
  lookup (key) {
    return this.node?.search(key)?.value ?? null
  }
}

const tree = new TernarySearchTree()

for (let i = 0; i < wellknownHeaderNames.length; ++i) {
  const key = wellknownHeaderNames[i].toLowerCase()
  tree.insert(Buffer.from(key), key)
}

function headerNameToString (value) {
 return typeof value === 'string'
    ? headerNameLowerCasedRecord[value] ?? value.toLowerCase()
    : tree.lookup(value) ?? value.toString().toLowerCase()
}
  
export function ternarytreeparseHeaders(headers, obj) {
    // For H2 support
    if (!Array.isArray(headers)) return headers;
  
    if (obj === undefined) obj = {};
    for (let i = 0; i < headers.length; i += 2) {
      const key = headerNameToString(headers[i]);
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
  
