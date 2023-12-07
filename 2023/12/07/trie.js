var strings = [
  "accept",
  "accept-encoding",
  "accept-language",
  "accept-ranges",
  "access-control-allow-credentials",
  "access-control-allow-headers",
  "access-control-allow-methods",
  "access-control-allow-origin",
  "access-control-expose-headers",
  "access-control-max-age",
  "access-control-request-headers",
  "access-control-request-method",
  "age",
  "allow",
  "alt-svc",
  "alt-used",
  "authorization",
  "cache-control",
  "clear-site-data",
  "connection",
  "content-disposition",
  "content-encoding",
  "content-language",
  "content-length",
  "content-location",
  "content-range",
  "content-security-policy",
  "content-security-policy-report-only",
  "content-type",
  "cookie",
  "cross-origin-embedder-policy",
  "cross-origin-opener-policy",
  "cross-origin-resource-policy",
  "date",
  "device-memory",
  "downlink",
  "ect",
  "etag",
  "expect",
  "expect-ct",
  "expires",
  "forwarded",
  "from",
  "host",
  "if-match",
  "if-modified-since",
  "if-none-match",
  "if-range",
  "if-unmodified-since",
  "keep-alive",
  "last-modified",
  "link",
  "location",
  "max-forwards",
  "origin",
  "permissions-policy",
  "pragma",
  "proxy-authenticate",
  "proxy-authorization",
  "rtt",
  "range",
  "referer",
  "referrer-policy",
  "refresh",
  "retry-after",
  "sec-websocket-accept",
  "sec-websocket-extensions",
  "sec-websocket-key",
  "sec-websocket-protocol",
  "sec-websocket-version",
  "server",
  "server-timing",
  "service-worker-allowed",
  "service-worker-navigation-preload",
  "set-cookie",
  "sourcemap",
  "strict-transport-security",
  "supports-loading-mode",
  "te",
  "timing-allow-origin",
  "trailer",
  "transfer-encoding",
  "upgrade",
  "upgrade-insecure-requests",
  "user-agent",
  "vary",
  "via",
  "www-authenticate",
  "x-content-type-options",
  "x-dns-prefetch-control",
  "x-frame-options",
  "x-permitted-cross-domain-policies",
  "x-powered-by",
  "x-requested-with",
  "x-xss-protection",
];

function classify(s) {
  var len = s.length;
  if (len == 0) {
    return -1;
  }
  switch (s[0]) {
    case 97:
    case 65:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 99:
        case 67:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 99:
            case 67:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 101:
                case 69:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 112:
                    case 80:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 116:
                        case 84:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 45:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 101:
                                case 69:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 110:
                                    case 78:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 99:
                                        case 67:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 111:
                                            case 79:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 100:
                                                case 68:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 105:
                                                    case 73:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 110:
                                                        case 78:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 103:
                                                            case 71:
                                                              return len == 15
                                                                ? 1
                                                                : -1;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                case 108:
                                case 76:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 97:
                                    case 65:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 110:
                                        case 78:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 103:
                                            case 71:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 117:
                                                case 85:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 97:
                                                    case 65:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 103:
                                                        case 71:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 101:
                                                            case 69:
                                                              return len == 15
                                                                ? 2
                                                                : -1;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                case 114:
                                case 82:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 97:
                                    case 65:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 110:
                                        case 78:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 103:
                                            case 71:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 101:
                                                case 69:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 115:
                                                    case 83:
                                                      return len == 13 ? 3 : -1;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return len == 6 ? 0 : -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    case 115:
                    case 83:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 115:
                        case 83:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 45:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 99:
                                case 67:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 111:
                                    case 79:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 110:
                                        case 78:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 116:
                                            case 84:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 114:
                                                case 82:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 111:
                                                    case 79:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 108:
                                                        case 76:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 45:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 97:
                                                                case 65:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 108:
                                                                    case 76:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 108:
                                                                        case 76:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 111:
                                                                            case 79:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 119:
                                                                                case 87:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 45:
                                                                                      if (
                                                                                        len ==
                                                                                        20
                                                                                      ) {
                                                                                        return -1;
                                                                                      }
                                                                                      switch (
                                                                                        s[21]
                                                                                      ) {
                                                                                        case 99:
                                                                                        case 67:
                                                                                          if (
                                                                                            len ==
                                                                                            21
                                                                                          ) {
                                                                                            return -1;
                                                                                          }
                                                                                          switch (
                                                                                            s[22]
                                                                                          ) {
                                                                                            case 114:
                                                                                            case 82:
                                                                                              if (
                                                                                                len ==
                                                                                                22
                                                                                              ) {
                                                                                                return -1;
                                                                                              }
                                                                                              switch (
                                                                                                s[23]
                                                                                              ) {
                                                                                                case 101:
                                                                                                case 69:
                                                                                                  if (
                                                                                                    len ==
                                                                                                    23
                                                                                                  ) {
                                                                                                    return -1;
                                                                                                  }
                                                                                                  switch (
                                                                                                    s[24]
                                                                                                  ) {
                                                                                                    case 100:
                                                                                                    case 68:
                                                                                                      if (
                                                                                                        len ==
                                                                                                        24
                                                                                                      ) {
                                                                                                        return -1;
                                                                                                      }
                                                                                                      switch (
                                                                                                        s[25]
                                                                                                      ) {
                                                                                                        case 101:
                                                                                                        case 69:
                                                                                                          if (
                                                                                                            len ==
                                                                                                            25
                                                                                                          ) {
                                                                                                            return -1;
                                                                                                          }
                                                                                                          switch (
                                                                                                            s[26]
                                                                                                          ) {
                                                                                                            case 110:
                                                                                                            case 78:
                                                                                                              if (
                                                                                                                len ==
                                                                                                                26
                                                                                                              ) {
                                                                                                                return -1;
                                                                                                              }
                                                                                                              switch (
                                                                                                                s[27]
                                                                                                              ) {
                                                                                                                case 116:
                                                                                                                case 84:
                                                                                                                  if (
                                                                                                                    len ==
                                                                                                                    27
                                                                                                                  ) {
                                                                                                                    return -1;
                                                                                                                  }
                                                                                                                  switch (
                                                                                                                    s[28]
                                                                                                                  ) {
                                                                                                                    case 105:
                                                                                                                    case 73:
                                                                                                                      if (
                                                                                                                        len ==
                                                                                                                        28
                                                                                                                      ) {
                                                                                                                        return -1;
                                                                                                                      }
                                                                                                                      switch (
                                                                                                                        s[29]
                                                                                                                      ) {
                                                                                                                        case 97:
                                                                                                                        case 65:
                                                                                                                          if (
                                                                                                                            len ==
                                                                                                                            29
                                                                                                                          ) {
                                                                                                                            return -1;
                                                                                                                          }
                                                                                                                          switch (
                                                                                                                            s[30]
                                                                                                                          ) {
                                                                                                                            case 108:
                                                                                                                            case 76:
                                                                                                                              if (
                                                                                                                                len ==
                                                                                                                                30
                                                                                                                              ) {
                                                                                                                                return -1;
                                                                                                                              }
                                                                                                                              switch (
                                                                                                                                s[31]
                                                                                                                              ) {
                                                                                                                                case 115:
                                                                                                                                case 83:
                                                                                                                                  return len ==
                                                                                                                                    32
                                                                                                                                    ? 4
                                                                                                                                    : -1;
                                                                                                                                default:
                                                                                                                                  return -1;
                                                                                                                              }
                                                                                                                              break;
                                                                                                                            default:
                                                                                                                              return -1;
                                                                                                                          }
                                                                                                                          break;
                                                                                                                        default:
                                                                                                                          return -1;
                                                                                                                      }
                                                                                                                      break;
                                                                                                                    default:
                                                                                                                      return -1;
                                                                                                                  }
                                                                                                                  break;
                                                                                                                default:
                                                                                                                  return -1;
                                                                                                              }
                                                                                                              break;
                                                                                                            default:
                                                                                                              return -1;
                                                                                                          }
                                                                                                          break;
                                                                                                        default:
                                                                                                          return -1;
                                                                                                      }
                                                                                                      break;
                                                                                                    default:
                                                                                                      return -1;
                                                                                                  }
                                                                                                  break;
                                                                                                default:
                                                                                                  return -1;
                                                                                              }
                                                                                              break;
                                                                                            default:
                                                                                              return -1;
                                                                                          }
                                                                                          break;
                                                                                        case 104:
                                                                                        case 72:
                                                                                          if (
                                                                                            len ==
                                                                                            21
                                                                                          ) {
                                                                                            return -1;
                                                                                          }
                                                                                          switch (
                                                                                            s[22]
                                                                                          ) {
                                                                                            case 101:
                                                                                            case 69:
                                                                                              if (
                                                                                                len ==
                                                                                                22
                                                                                              ) {
                                                                                                return -1;
                                                                                              }
                                                                                              switch (
                                                                                                s[23]
                                                                                              ) {
                                                                                                case 97:
                                                                                                case 65:
                                                                                                  if (
                                                                                                    len ==
                                                                                                    23
                                                                                                  ) {
                                                                                                    return -1;
                                                                                                  }
                                                                                                  switch (
                                                                                                    s[24]
                                                                                                  ) {
                                                                                                    case 100:
                                                                                                    case 68:
                                                                                                      if (
                                                                                                        len ==
                                                                                                        24
                                                                                                      ) {
                                                                                                        return -1;
                                                                                                      }
                                                                                                      switch (
                                                                                                        s[25]
                                                                                                      ) {
                                                                                                        case 101:
                                                                                                        case 69:
                                                                                                          if (
                                                                                                            len ==
                                                                                                            25
                                                                                                          ) {
                                                                                                            return -1;
                                                                                                          }
                                                                                                          switch (
                                                                                                            s[26]
                                                                                                          ) {
                                                                                                            case 114:
                                                                                                            case 82:
                                                                                                              if (
                                                                                                                len ==
                                                                                                                26
                                                                                                              ) {
                                                                                                                return -1;
                                                                                                              }
                                                                                                              switch (
                                                                                                                s[27]
                                                                                                              ) {
                                                                                                                case 115:
                                                                                                                case 83:
                                                                                                                  return len ==
                                                                                                                    28
                                                                                                                    ? 5
                                                                                                                    : -1;
                                                                                                                default:
                                                                                                                  return -1;
                                                                                                              }
                                                                                                              break;
                                                                                                            default:
                                                                                                              return -1;
                                                                                                          }
                                                                                                          break;
                                                                                                        default:
                                                                                                          return -1;
                                                                                                      }
                                                                                                      break;
                                                                                                    default:
                                                                                                      return -1;
                                                                                                  }
                                                                                                  break;
                                                                                                default:
                                                                                                  return -1;
                                                                                              }
                                                                                              break;
                                                                                            default:
                                                                                              return -1;
                                                                                          }
                                                                                          break;
                                                                                        case 109:
                                                                                        case 77:
                                                                                          if (
                                                                                            len ==
                                                                                            21
                                                                                          ) {
                                                                                            return -1;
                                                                                          }
                                                                                          switch (
                                                                                            s[22]
                                                                                          ) {
                                                                                            case 101:
                                                                                            case 69:
                                                                                              if (
                                                                                                len ==
                                                                                                22
                                                                                              ) {
                                                                                                return -1;
                                                                                              }
                                                                                              switch (
                                                                                                s[23]
                                                                                              ) {
                                                                                                case 116:
                                                                                                case 84:
                                                                                                  if (
                                                                                                    len ==
                                                                                                    23
                                                                                                  ) {
                                                                                                    return -1;
                                                                                                  }
                                                                                                  switch (
                                                                                                    s[24]
                                                                                                  ) {
                                                                                                    case 104:
                                                                                                    case 72:
                                                                                                      if (
                                                                                                        len ==
                                                                                                        24
                                                                                                      ) {
                                                                                                        return -1;
                                                                                                      }
                                                                                                      switch (
                                                                                                        s[25]
                                                                                                      ) {
                                                                                                        case 111:
                                                                                                        case 79:
                                                                                                          if (
                                                                                                            len ==
                                                                                                            25
                                                                                                          ) {
                                                                                                            return -1;
                                                                                                          }
                                                                                                          switch (
                                                                                                            s[26]
                                                                                                          ) {
                                                                                                            case 100:
                                                                                                            case 68:
                                                                                                              if (
                                                                                                                len ==
                                                                                                                26
                                                                                                              ) {
                                                                                                                return -1;
                                                                                                              }
                                                                                                              switch (
                                                                                                                s[27]
                                                                                                              ) {
                                                                                                                case 115:
                                                                                                                case 83:
                                                                                                                  return len ==
                                                                                                                    28
                                                                                                                    ? 6
                                                                                                                    : -1;
                                                                                                                default:
                                                                                                                  return -1;
                                                                                                              }
                                                                                                              break;
                                                                                                            default:
                                                                                                              return -1;
                                                                                                          }
                                                                                                          break;
                                                                                                        default:
                                                                                                          return -1;
                                                                                                      }
                                                                                                      break;
                                                                                                    default:
                                                                                                      return -1;
                                                                                                  }
                                                                                                  break;
                                                                                                default:
                                                                                                  return -1;
                                                                                              }
                                                                                              break;
                                                                                            default:
                                                                                              return -1;
                                                                                          }
                                                                                          break;
                                                                                        case 111:
                                                                                        case 79:
                                                                                          if (
                                                                                            len ==
                                                                                            21
                                                                                          ) {
                                                                                            return -1;
                                                                                          }
                                                                                          switch (
                                                                                            s[22]
                                                                                          ) {
                                                                                            case 114:
                                                                                            case 82:
                                                                                              if (
                                                                                                len ==
                                                                                                22
                                                                                              ) {
                                                                                                return -1;
                                                                                              }
                                                                                              switch (
                                                                                                s[23]
                                                                                              ) {
                                                                                                case 105:
                                                                                                case 73:
                                                                                                  if (
                                                                                                    len ==
                                                                                                    23
                                                                                                  ) {
                                                                                                    return -1;
                                                                                                  }
                                                                                                  switch (
                                                                                                    s[24]
                                                                                                  ) {
                                                                                                    case 103:
                                                                                                    case 71:
                                                                                                      if (
                                                                                                        len ==
                                                                                                        24
                                                                                                      ) {
                                                                                                        return -1;
                                                                                                      }
                                                                                                      switch (
                                                                                                        s[25]
                                                                                                      ) {
                                                                                                        case 105:
                                                                                                        case 73:
                                                                                                          if (
                                                                                                            len ==
                                                                                                            25
                                                                                                          ) {
                                                                                                            return -1;
                                                                                                          }
                                                                                                          switch (
                                                                                                            s[26]
                                                                                                          ) {
                                                                                                            case 110:
                                                                                                            case 78:
                                                                                                              return len ==
                                                                                                                27
                                                                                                                ? 7
                                                                                                                : -1;
                                                                                                            default:
                                                                                                              return -1;
                                                                                                          }
                                                                                                          break;
                                                                                                        default:
                                                                                                          return -1;
                                                                                                      }
                                                                                                      break;
                                                                                                    default:
                                                                                                      return -1;
                                                                                                  }
                                                                                                  break;
                                                                                                default:
                                                                                                  return -1;
                                                                                              }
                                                                                              break;
                                                                                            default:
                                                                                              return -1;
                                                                                          }
                                                                                          break;
                                                                                        default:
                                                                                          return -1;
                                                                                      }
                                                                                      break;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                case 101:
                                                                case 69:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 120:
                                                                    case 88:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 112:
                                                                        case 80:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 111:
                                                                            case 79:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 115:
                                                                                case 83:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 101:
                                                                                    case 69:
                                                                                      if (
                                                                                        len ==
                                                                                        20
                                                                                      ) {
                                                                                        return -1;
                                                                                      }
                                                                                      switch (
                                                                                        s[21]
                                                                                      ) {
                                                                                        case 45:
                                                                                          if (
                                                                                            len ==
                                                                                            21
                                                                                          ) {
                                                                                            return -1;
                                                                                          }
                                                                                          switch (
                                                                                            s[22]
                                                                                          ) {
                                                                                            case 104:
                                                                                            case 72:
                                                                                              if (
                                                                                                len ==
                                                                                                22
                                                                                              ) {
                                                                                                return -1;
                                                                                              }
                                                                                              switch (
                                                                                                s[23]
                                                                                              ) {
                                                                                                case 101:
                                                                                                case 69:
                                                                                                  if (
                                                                                                    len ==
                                                                                                    23
                                                                                                  ) {
                                                                                                    return -1;
                                                                                                  }
                                                                                                  switch (
                                                                                                    s[24]
                                                                                                  ) {
                                                                                                    case 97:
                                                                                                    case 65:
                                                                                                      if (
                                                                                                        len ==
                                                                                                        24
                                                                                                      ) {
                                                                                                        return -1;
                                                                                                      }
                                                                                                      switch (
                                                                                                        s[25]
                                                                                                      ) {
                                                                                                        case 100:
                                                                                                        case 68:
                                                                                                          if (
                                                                                                            len ==
                                                                                                            25
                                                                                                          ) {
                                                                                                            return -1;
                                                                                                          }
                                                                                                          switch (
                                                                                                            s[26]
                                                                                                          ) {
                                                                                                            case 101:
                                                                                                            case 69:
                                                                                                              if (
                                                                                                                len ==
                                                                                                                26
                                                                                                              ) {
                                                                                                                return -1;
                                                                                                              }
                                                                                                              switch (
                                                                                                                s[27]
                                                                                                              ) {
                                                                                                                case 114:
                                                                                                                case 82:
                                                                                                                  if (
                                                                                                                    len ==
                                                                                                                    27
                                                                                                                  ) {
                                                                                                                    return -1;
                                                                                                                  }
                                                                                                                  switch (
                                                                                                                    s[28]
                                                                                                                  ) {
                                                                                                                    case 115:
                                                                                                                    case 83:
                                                                                                                      return len ==
                                                                                                                        29
                                                                                                                        ? 8
                                                                                                                        : -1;
                                                                                                                    default:
                                                                                                                      return -1;
                                                                                                                  }
                                                                                                                  break;
                                                                                                                default:
                                                                                                                  return -1;
                                                                                                              }
                                                                                                              break;
                                                                                                            default:
                                                                                                              return -1;
                                                                                                          }
                                                                                                          break;
                                                                                                        default:
                                                                                                          return -1;
                                                                                                      }
                                                                                                      break;
                                                                                                    default:
                                                                                                      return -1;
                                                                                                  }
                                                                                                  break;
                                                                                                default:
                                                                                                  return -1;
                                                                                              }
                                                                                              break;
                                                                                            default:
                                                                                              return -1;
                                                                                          }
                                                                                          break;
                                                                                        default:
                                                                                          return -1;
                                                                                      }
                                                                                      break;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                case 109:
                                                                case 77:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 97:
                                                                    case 65:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 120:
                                                                        case 88:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 45:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 97:
                                                                                case 65:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 103:
                                                                                    case 71:
                                                                                      if (
                                                                                        len ==
                                                                                        20
                                                                                      ) {
                                                                                        return -1;
                                                                                      }
                                                                                      switch (
                                                                                        s[21]
                                                                                      ) {
                                                                                        case 101:
                                                                                        case 69:
                                                                                          return len ==
                                                                                            22
                                                                                            ? 9
                                                                                            : -1;
                                                                                        default:
                                                                                          return -1;
                                                                                      }
                                                                                      break;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                case 114:
                                                                case 82:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 101:
                                                                    case 69:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 113:
                                                                        case 81:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 117:
                                                                            case 85:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 101:
                                                                                case 69:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 115:
                                                                                    case 83:
                                                                                      if (
                                                                                        len ==
                                                                                        20
                                                                                      ) {
                                                                                        return -1;
                                                                                      }
                                                                                      switch (
                                                                                        s[21]
                                                                                      ) {
                                                                                        case 116:
                                                                                        case 84:
                                                                                          if (
                                                                                            len ==
                                                                                            21
                                                                                          ) {
                                                                                            return -1;
                                                                                          }
                                                                                          switch (
                                                                                            s[22]
                                                                                          ) {
                                                                                            case 45:
                                                                                              if (
                                                                                                len ==
                                                                                                22
                                                                                              ) {
                                                                                                return -1;
                                                                                              }
                                                                                              switch (
                                                                                                s[23]
                                                                                              ) {
                                                                                                case 104:
                                                                                                case 72:
                                                                                                  if (
                                                                                                    len ==
                                                                                                    23
                                                                                                  ) {
                                                                                                    return -1;
                                                                                                  }
                                                                                                  switch (
                                                                                                    s[24]
                                                                                                  ) {
                                                                                                    case 101:
                                                                                                    case 69:
                                                                                                      if (
                                                                                                        len ==
                                                                                                        24
                                                                                                      ) {
                                                                                                        return -1;
                                                                                                      }
                                                                                                      switch (
                                                                                                        s[25]
                                                                                                      ) {
                                                                                                        case 97:
                                                                                                        case 65:
                                                                                                          if (
                                                                                                            len ==
                                                                                                            25
                                                                                                          ) {
                                                                                                            return -1;
                                                                                                          }
                                                                                                          switch (
                                                                                                            s[26]
                                                                                                          ) {
                                                                                                            case 100:
                                                                                                            case 68:
                                                                                                              if (
                                                                                                                len ==
                                                                                                                26
                                                                                                              ) {
                                                                                                                return -1;
                                                                                                              }
                                                                                                              switch (
                                                                                                                s[27]
                                                                                                              ) {
                                                                                                                case 101:
                                                                                                                case 69:
                                                                                                                  if (
                                                                                                                    len ==
                                                                                                                    27
                                                                                                                  ) {
                                                                                                                    return -1;
                                                                                                                  }
                                                                                                                  switch (
                                                                                                                    s[28]
                                                                                                                  ) {
                                                                                                                    case 114:
                                                                                                                    case 82:
                                                                                                                      if (
                                                                                                                        len ==
                                                                                                                        28
                                                                                                                      ) {
                                                                                                                        return -1;
                                                                                                                      }
                                                                                                                      switch (
                                                                                                                        s[29]
                                                                                                                      ) {
                                                                                                                        case 115:
                                                                                                                        case 83:
                                                                                                                          return len ==
                                                                                                                            30
                                                                                                                            ? 10
                                                                                                                            : -1;
                                                                                                                        default:
                                                                                                                          return -1;
                                                                                                                      }
                                                                                                                      break;
                                                                                                                    default:
                                                                                                                      return -1;
                                                                                                                  }
                                                                                                                  break;
                                                                                                                default:
                                                                                                                  return -1;
                                                                                                              }
                                                                                                              break;
                                                                                                            default:
                                                                                                              return -1;
                                                                                                          }
                                                                                                          break;
                                                                                                        default:
                                                                                                          return -1;
                                                                                                      }
                                                                                                      break;
                                                                                                    default:
                                                                                                      return -1;
                                                                                                  }
                                                                                                  break;
                                                                                                case 109:
                                                                                                case 77:
                                                                                                  if (
                                                                                                    len ==
                                                                                                    23
                                                                                                  ) {
                                                                                                    return -1;
                                                                                                  }
                                                                                                  switch (
                                                                                                    s[24]
                                                                                                  ) {
                                                                                                    case 101:
                                                                                                    case 69:
                                                                                                      if (
                                                                                                        len ==
                                                                                                        24
                                                                                                      ) {
                                                                                                        return -1;
                                                                                                      }
                                                                                                      switch (
                                                                                                        s[25]
                                                                                                      ) {
                                                                                                        case 116:
                                                                                                        case 84:
                                                                                                          if (
                                                                                                            len ==
                                                                                                            25
                                                                                                          ) {
                                                                                                            return -1;
                                                                                                          }
                                                                                                          switch (
                                                                                                            s[26]
                                                                                                          ) {
                                                                                                            case 104:
                                                                                                            case 72:
                                                                                                              if (
                                                                                                                len ==
                                                                                                                26
                                                                                                              ) {
                                                                                                                return -1;
                                                                                                              }
                                                                                                              switch (
                                                                                                                s[27]
                                                                                                              ) {
                                                                                                                case 111:
                                                                                                                case 79:
                                                                                                                  if (
                                                                                                                    len ==
                                                                                                                    27
                                                                                                                  ) {
                                                                                                                    return -1;
                                                                                                                  }
                                                                                                                  switch (
                                                                                                                    s[28]
                                                                                                                  ) {
                                                                                                                    case 100:
                                                                                                                    case 68:
                                                                                                                      return len ==
                                                                                                                        29
                                                                                                                        ? 11
                                                                                                                        : -1;
                                                                                                                    default:
                                                                                                                      return -1;
                                                                                                                  }
                                                                                                                  break;
                                                                                                                default:
                                                                                                                  return -1;
                                                                                                              }
                                                                                                              break;
                                                                                                            default:
                                                                                                              return -1;
                                                                                                          }
                                                                                                          break;
                                                                                                        default:
                                                                                                          return -1;
                                                                                                      }
                                                                                                      break;
                                                                                                    default:
                                                                                                      return -1;
                                                                                                  }
                                                                                                  break;
                                                                                                default:
                                                                                                  return -1;
                                                                                              }
                                                                                              break;
                                                                                            default:
                                                                                              return -1;
                                                                                          }
                                                                                          break;
                                                                                        default:
                                                                                          return -1;
                                                                                      }
                                                                                      break;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 103:
        case 71:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 101:
            case 69:
              return len == 3 ? 12 : -1;
            default:
              return -1;
          }
          break;
        case 108:
        case 76:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 108:
            case 76:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 111:
                case 79:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 119:
                    case 87:
                      return len == 5 ? 13 : -1;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            case 116:
            case 84:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 45:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 115:
                    case 83:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 118:
                        case 86:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 99:
                            case 67:
                              return len == 7 ? 14 : -1;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    case 117:
                    case 85:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 115:
                        case 83:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 101:
                            case 69:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 100:
                                case 68:
                                  return len == 8 ? 15 : -1;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 117:
        case 85:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 116:
            case 84:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 104:
                case 72:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 111:
                    case 79:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 114:
                        case 82:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 105:
                            case 73:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 122:
                                case 90:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 97:
                                    case 65:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 116:
                                        case 84:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 105:
                                            case 73:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 111:
                                                case 79:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 110:
                                                    case 78:
                                                      return len == 13
                                                        ? 16
                                                        : -1;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 99:
    case 67:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 97:
        case 65:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 99:
            case 67:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 104:
                case 72:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 101:
                    case 69:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 45:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 99:
                            case 67:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 111:
                                case 79:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 110:
                                    case 78:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 116:
                                        case 84:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 114:
                                            case 82:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 111:
                                                case 79:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 108:
                                                    case 76:
                                                      return len == 13
                                                        ? 17
                                                        : -1;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 108:
        case 76:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 101:
            case 69:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 97:
                case 65:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 114:
                    case 82:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 45:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 115:
                            case 83:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 105:
                                case 73:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 116:
                                    case 84:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 101:
                                        case 69:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 45:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 100:
                                                case 68:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 97:
                                                    case 65:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 116:
                                                        case 84:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 97:
                                                            case 65:
                                                              return len == 15
                                                                ? 18
                                                                : -1;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 111:
        case 79:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 110:
            case 78:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 110:
                case 78:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 101:
                    case 69:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 99:
                        case 67:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 116:
                            case 84:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 105:
                                case 73:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 111:
                                    case 79:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 110:
                                        case 78:
                                          return len == 10 ? 19 : -1;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                case 116:
                case 84:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 101:
                    case 69:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 110:
                        case 78:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 116:
                            case 84:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 45:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 100:
                                    case 68:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 105:
                                        case 73:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 115:
                                            case 83:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 112:
                                                case 80:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 111:
                                                    case 79:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 115:
                                                        case 83:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 105:
                                                            case 73:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 116:
                                                                case 84:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 105:
                                                                    case 73:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 111:
                                                                        case 79:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 110:
                                                                            case 78:
                                                                              return len ==
                                                                                19
                                                                                ? 20
                                                                                : -1;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    case 101:
                                    case 69:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 110:
                                        case 78:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 99:
                                            case 67:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 111:
                                                case 79:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 100:
                                                    case 68:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 105:
                                                        case 73:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 110:
                                                            case 78:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 103:
                                                                case 71:
                                                                  return len ==
                                                                    16
                                                                    ? 21
                                                                    : -1;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    case 108:
                                    case 76:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 97:
                                        case 65:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 110:
                                            case 78:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 103:
                                                case 71:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 117:
                                                    case 85:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 97:
                                                        case 65:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 103:
                                                            case 71:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 101:
                                                                case 69:
                                                                  return len ==
                                                                    16
                                                                    ? 22
                                                                    : -1;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        case 101:
                                        case 69:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 110:
                                            case 78:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 103:
                                                case 71:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 116:
                                                    case 84:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 104:
                                                        case 72:
                                                          return len == 14
                                                            ? 23
                                                            : -1;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        case 111:
                                        case 79:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 99:
                                            case 67:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 97:
                                                case 65:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 116:
                                                    case 84:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 105:
                                                        case 73:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 111:
                                                            case 79:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 110:
                                                                case 78:
                                                                  return len ==
                                                                    16
                                                                    ? 24
                                                                    : -1;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    case 114:
                                    case 82:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 97:
                                        case 65:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 110:
                                            case 78:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 103:
                                                case 71:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 101:
                                                    case 69:
                                                      return len == 13
                                                        ? 25
                                                        : -1;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    case 115:
                                    case 83:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 101:
                                        case 69:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 99:
                                            case 67:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 117:
                                                case 85:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 114:
                                                    case 82:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 105:
                                                        case 73:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 116:
                                                            case 84:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 121:
                                                                case 89:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 45:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 112:
                                                                        case 80:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 111:
                                                                            case 79:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 108:
                                                                                case 76:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 105:
                                                                                    case 73:
                                                                                      if (
                                                                                        len ==
                                                                                        20
                                                                                      ) {
                                                                                        return -1;
                                                                                      }
                                                                                      switch (
                                                                                        s[21]
                                                                                      ) {
                                                                                        case 99:
                                                                                        case 67:
                                                                                          if (
                                                                                            len ==
                                                                                            21
                                                                                          ) {
                                                                                            return -1;
                                                                                          }
                                                                                          switch (
                                                                                            s[22]
                                                                                          ) {
                                                                                            case 121:
                                                                                            case 89:
                                                                                              if (
                                                                                                len ==
                                                                                                22
                                                                                              ) {
                                                                                                return -1;
                                                                                              }
                                                                                              switch (
                                                                                                s[23]
                                                                                              ) {
                                                                                                case 45:
                                                                                                  if (
                                                                                                    len ==
                                                                                                    23
                                                                                                  ) {
                                                                                                    return -1;
                                                                                                  }
                                                                                                  switch (
                                                                                                    s[24]
                                                                                                  ) {
                                                                                                    case 114:
                                                                                                    case 82:
                                                                                                      if (
                                                                                                        len ==
                                                                                                        24
                                                                                                      ) {
                                                                                                        return -1;
                                                                                                      }
                                                                                                      switch (
                                                                                                        s[25]
                                                                                                      ) {
                                                                                                        case 101:
                                                                                                        case 69:
                                                                                                          if (
                                                                                                            len ==
                                                                                                            25
                                                                                                          ) {
                                                                                                            return -1;
                                                                                                          }
                                                                                                          switch (
                                                                                                            s[26]
                                                                                                          ) {
                                                                                                            case 112:
                                                                                                            case 80:
                                                                                                              if (
                                                                                                                len ==
                                                                                                                26
                                                                                                              ) {
                                                                                                                return -1;
                                                                                                              }
                                                                                                              switch (
                                                                                                                s[27]
                                                                                                              ) {
                                                                                                                case 111:
                                                                                                                case 79:
                                                                                                                  if (
                                                                                                                    len ==
                                                                                                                    27
                                                                                                                  ) {
                                                                                                                    return -1;
                                                                                                                  }
                                                                                                                  switch (
                                                                                                                    s[28]
                                                                                                                  ) {
                                                                                                                    case 114:
                                                                                                                    case 82:
                                                                                                                      if (
                                                                                                                        len ==
                                                                                                                        28
                                                                                                                      ) {
                                                                                                                        return -1;
                                                                                                                      }
                                                                                                                      switch (
                                                                                                                        s[29]
                                                                                                                      ) {
                                                                                                                        case 116:
                                                                                                                        case 84:
                                                                                                                          if (
                                                                                                                            len ==
                                                                                                                            29
                                                                                                                          ) {
                                                                                                                            return -1;
                                                                                                                          }
                                                                                                                          switch (
                                                                                                                            s[30]
                                                                                                                          ) {
                                                                                                                            case 45:
                                                                                                                              if (
                                                                                                                                len ==
                                                                                                                                30
                                                                                                                              ) {
                                                                                                                                return -1;
                                                                                                                              }
                                                                                                                              switch (
                                                                                                                                s[31]
                                                                                                                              ) {
                                                                                                                                case 111:
                                                                                                                                case 79:
                                                                                                                                  if (
                                                                                                                                    len ==
                                                                                                                                    31
                                                                                                                                  ) {
                                                                                                                                    return -1;
                                                                                                                                  }
                                                                                                                                  switch (
                                                                                                                                    s[32]
                                                                                                                                  ) {
                                                                                                                                    case 110:
                                                                                                                                    case 78:
                                                                                                                                      if (
                                                                                                                                        len ==
                                                                                                                                        32
                                                                                                                                      ) {
                                                                                                                                        return -1;
                                                                                                                                      }
                                                                                                                                      switch (
                                                                                                                                        s[33]
                                                                                                                                      ) {
                                                                                                                                        case 108:
                                                                                                                                        case 76:
                                                                                                                                          if (
                                                                                                                                            len ==
                                                                                                                                            33
                                                                                                                                          ) {
                                                                                                                                            return -1;
                                                                                                                                          }
                                                                                                                                          switch (
                                                                                                                                            s[34]
                                                                                                                                          ) {
                                                                                                                                            case 121:
                                                                                                                                            case 89:
                                                                                                                                              return len ==
                                                                                                                                                35
                                                                                                                                                ? 27
                                                                                                                                                : -1;
                                                                                                                                            default:
                                                                                                                                              return -1;
                                                                                                                                          }
                                                                                                                                          break;
                                                                                                                                        default:
                                                                                                                                          return -1;
                                                                                                                                      }
                                                                                                                                      break;
                                                                                                                                    default:
                                                                                                                                      return -1;
                                                                                                                                  }
                                                                                                                                  break;
                                                                                                                                default:
                                                                                                                                  return -1;
                                                                                                                              }
                                                                                                                              break;
                                                                                                                            default:
                                                                                                                              return -1;
                                                                                                                          }
                                                                                                                          break;
                                                                                                                        default:
                                                                                                                          return -1;
                                                                                                                      }
                                                                                                                      break;
                                                                                                                    default:
                                                                                                                      return -1;
                                                                                                                  }
                                                                                                                  break;
                                                                                                                default:
                                                                                                                  return -1;
                                                                                                              }
                                                                                                              break;
                                                                                                            default:
                                                                                                              return -1;
                                                                                                          }
                                                                                                          break;
                                                                                                        default:
                                                                                                          return -1;
                                                                                                      }
                                                                                                      break;
                                                                                                    default:
                                                                                                      return -1;
                                                                                                  }
                                                                                                  break;
                                                                                                default:
                                                                                                  return len ==
                                                                                                    23
                                                                                                    ? 26
                                                                                                    : -1;
                                                                                              }
                                                                                              break;
                                                                                            default:
                                                                                              return -1;
                                                                                          }
                                                                                          break;
                                                                                        default:
                                                                                          return -1;
                                                                                      }
                                                                                      break;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    case 116:
                                    case 84:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 121:
                                        case 89:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 112:
                                            case 80:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 101:
                                                case 69:
                                                  return len == 12 ? 28 : -1;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            case 111:
            case 79:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 107:
                case 75:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 105:
                    case 73:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 101:
                        case 69:
                          return len == 6 ? 29 : -1;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 114:
        case 82:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 111:
            case 79:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 115:
                case 83:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 115:
                    case 83:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 45:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 111:
                            case 79:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 114:
                                case 82:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 105:
                                    case 73:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 103:
                                        case 71:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 105:
                                            case 73:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 110:
                                                case 78:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 45:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 101:
                                                        case 69:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 109:
                                                            case 77:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 98:
                                                                case 66:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 101:
                                                                    case 69:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 100:
                                                                        case 68:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 100:
                                                                            case 68:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 101:
                                                                                case 69:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 114:
                                                                                    case 82:
                                                                                      if (
                                                                                        len ==
                                                                                        20
                                                                                      ) {
                                                                                        return -1;
                                                                                      }
                                                                                      switch (
                                                                                        s[21]
                                                                                      ) {
                                                                                        case 45:
                                                                                          if (
                                                                                            len ==
                                                                                            21
                                                                                          ) {
                                                                                            return -1;
                                                                                          }
                                                                                          switch (
                                                                                            s[22]
                                                                                          ) {
                                                                                            case 112:
                                                                                            case 80:
                                                                                              if (
                                                                                                len ==
                                                                                                22
                                                                                              ) {
                                                                                                return -1;
                                                                                              }
                                                                                              switch (
                                                                                                s[23]
                                                                                              ) {
                                                                                                case 111:
                                                                                                case 79:
                                                                                                  if (
                                                                                                    len ==
                                                                                                    23
                                                                                                  ) {
                                                                                                    return -1;
                                                                                                  }
                                                                                                  switch (
                                                                                                    s[24]
                                                                                                  ) {
                                                                                                    case 108:
                                                                                                    case 76:
                                                                                                      if (
                                                                                                        len ==
                                                                                                        24
                                                                                                      ) {
                                                                                                        return -1;
                                                                                                      }
                                                                                                      switch (
                                                                                                        s[25]
                                                                                                      ) {
                                                                                                        case 105:
                                                                                                        case 73:
                                                                                                          if (
                                                                                                            len ==
                                                                                                            25
                                                                                                          ) {
                                                                                                            return -1;
                                                                                                          }
                                                                                                          switch (
                                                                                                            s[26]
                                                                                                          ) {
                                                                                                            case 99:
                                                                                                            case 67:
                                                                                                              if (
                                                                                                                len ==
                                                                                                                26
                                                                                                              ) {
                                                                                                                return -1;
                                                                                                              }
                                                                                                              switch (
                                                                                                                s[27]
                                                                                                              ) {
                                                                                                                case 121:
                                                                                                                case 89:
                                                                                                                  return len ==
                                                                                                                    28
                                                                                                                    ? 30
                                                                                                                    : -1;
                                                                                                                default:
                                                                                                                  return -1;
                                                                                                              }
                                                                                                              break;
                                                                                                            default:
                                                                                                              return -1;
                                                                                                          }
                                                                                                          break;
                                                                                                        default:
                                                                                                          return -1;
                                                                                                      }
                                                                                                      break;
                                                                                                    default:
                                                                                                      return -1;
                                                                                                  }
                                                                                                  break;
                                                                                                default:
                                                                                                  return -1;
                                                                                              }
                                                                                              break;
                                                                                            default:
                                                                                              return -1;
                                                                                          }
                                                                                          break;
                                                                                        default:
                                                                                          return -1;
                                                                                      }
                                                                                      break;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        case 111:
                                                        case 79:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 112:
                                                            case 80:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 101:
                                                                case 69:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 110:
                                                                    case 78:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 101:
                                                                        case 69:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 114:
                                                                            case 82:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 45:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 112:
                                                                                    case 80:
                                                                                      if (
                                                                                        len ==
                                                                                        20
                                                                                      ) {
                                                                                        return -1;
                                                                                      }
                                                                                      switch (
                                                                                        s[21]
                                                                                      ) {
                                                                                        case 111:
                                                                                        case 79:
                                                                                          if (
                                                                                            len ==
                                                                                            21
                                                                                          ) {
                                                                                            return -1;
                                                                                          }
                                                                                          switch (
                                                                                            s[22]
                                                                                          ) {
                                                                                            case 108:
                                                                                            case 76:
                                                                                              if (
                                                                                                len ==
                                                                                                22
                                                                                              ) {
                                                                                                return -1;
                                                                                              }
                                                                                              switch (
                                                                                                s[23]
                                                                                              ) {
                                                                                                case 105:
                                                                                                case 73:
                                                                                                  if (
                                                                                                    len ==
                                                                                                    23
                                                                                                  ) {
                                                                                                    return -1;
                                                                                                  }
                                                                                                  switch (
                                                                                                    s[24]
                                                                                                  ) {
                                                                                                    case 99:
                                                                                                    case 67:
                                                                                                      if (
                                                                                                        len ==
                                                                                                        24
                                                                                                      ) {
                                                                                                        return -1;
                                                                                                      }
                                                                                                      switch (
                                                                                                        s[25]
                                                                                                      ) {
                                                                                                        case 121:
                                                                                                        case 89:
                                                                                                          return len ==
                                                                                                            26
                                                                                                            ? 31
                                                                                                            : -1;
                                                                                                        default:
                                                                                                          return -1;
                                                                                                      }
                                                                                                      break;
                                                                                                    default:
                                                                                                      return -1;
                                                                                                  }
                                                                                                  break;
                                                                                                default:
                                                                                                  return -1;
                                                                                              }
                                                                                              break;
                                                                                            default:
                                                                                              return -1;
                                                                                          }
                                                                                          break;
                                                                                        default:
                                                                                          return -1;
                                                                                      }
                                                                                      break;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        case 114:
                                                        case 82:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 101:
                                                            case 69:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 115:
                                                                case 83:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 111:
                                                                    case 79:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 117:
                                                                        case 85:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 114:
                                                                            case 82:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 99:
                                                                                case 67:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 101:
                                                                                    case 69:
                                                                                      if (
                                                                                        len ==
                                                                                        20
                                                                                      ) {
                                                                                        return -1;
                                                                                      }
                                                                                      switch (
                                                                                        s[21]
                                                                                      ) {
                                                                                        case 45:
                                                                                          if (
                                                                                            len ==
                                                                                            21
                                                                                          ) {
                                                                                            return -1;
                                                                                          }
                                                                                          switch (
                                                                                            s[22]
                                                                                          ) {
                                                                                            case 112:
                                                                                            case 80:
                                                                                              if (
                                                                                                len ==
                                                                                                22
                                                                                              ) {
                                                                                                return -1;
                                                                                              }
                                                                                              switch (
                                                                                                s[23]
                                                                                              ) {
                                                                                                case 111:
                                                                                                case 79:
                                                                                                  if (
                                                                                                    len ==
                                                                                                    23
                                                                                                  ) {
                                                                                                    return -1;
                                                                                                  }
                                                                                                  switch (
                                                                                                    s[24]
                                                                                                  ) {
                                                                                                    case 108:
                                                                                                    case 76:
                                                                                                      if (
                                                                                                        len ==
                                                                                                        24
                                                                                                      ) {
                                                                                                        return -1;
                                                                                                      }
                                                                                                      switch (
                                                                                                        s[25]
                                                                                                      ) {
                                                                                                        case 105:
                                                                                                        case 73:
                                                                                                          if (
                                                                                                            len ==
                                                                                                            25
                                                                                                          ) {
                                                                                                            return -1;
                                                                                                          }
                                                                                                          switch (
                                                                                                            s[26]
                                                                                                          ) {
                                                                                                            case 99:
                                                                                                            case 67:
                                                                                                              if (
                                                                                                                len ==
                                                                                                                26
                                                                                                              ) {
                                                                                                                return -1;
                                                                                                              }
                                                                                                              switch (
                                                                                                                s[27]
                                                                                                              ) {
                                                                                                                case 121:
                                                                                                                case 89:
                                                                                                                  return len ==
                                                                                                                    28
                                                                                                                    ? 32
                                                                                                                    : -1;
                                                                                                                default:
                                                                                                                  return -1;
                                                                                                              }
                                                                                                              break;
                                                                                                            default:
                                                                                                              return -1;
                                                                                                          }
                                                                                                          break;
                                                                                                        default:
                                                                                                          return -1;
                                                                                                      }
                                                                                                      break;
                                                                                                    default:
                                                                                                      return -1;
                                                                                                  }
                                                                                                  break;
                                                                                                default:
                                                                                                  return -1;
                                                                                              }
                                                                                              break;
                                                                                            default:
                                                                                              return -1;
                                                                                          }
                                                                                          break;
                                                                                        default:
                                                                                          return -1;
                                                                                      }
                                                                                      break;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 100:
    case 68:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 97:
        case 65:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 116:
            case 84:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 101:
                case 69:
                  return len == 4 ? 33 : -1;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 101:
        case 69:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 118:
            case 86:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 105:
                case 73:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 99:
                    case 67:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 101:
                        case 69:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 45:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 109:
                                case 77:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 101:
                                    case 69:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 109:
                                        case 77:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 111:
                                            case 79:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 114:
                                                case 82:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 121:
                                                    case 89:
                                                      return len == 13
                                                        ? 34
                                                        : -1;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 111:
        case 79:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 119:
            case 87:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 110:
                case 78:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 108:
                    case 76:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 105:
                        case 73:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 110:
                            case 78:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 107:
                                case 75:
                                  return len == 8 ? 35 : -1;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 101:
    case 69:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 99:
        case 67:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 116:
            case 84:
              return len == 3 ? 36 : -1;
            default:
              return -1;
          }
          break;
        case 116:
        case 84:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 97:
            case 65:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 103:
                case 71:
                  return len == 4 ? 37 : -1;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 120:
        case 88:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 112:
            case 80:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 101:
                case 69:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 99:
                    case 67:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 116:
                        case 84:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 45:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 99:
                                case 67:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 116:
                                    case 84:
                                      return len == 9 ? 39 : -1;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return len == 6 ? 38 : -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                case 105:
                case 73:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 114:
                    case 82:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 101:
                        case 69:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 115:
                            case 83:
                              return len == 7 ? 40 : -1;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 102:
    case 70:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 111:
        case 79:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 114:
            case 82:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 119:
                case 87:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 97:
                    case 65:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 114:
                        case 82:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 100:
                            case 68:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 101:
                                case 69:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 100:
                                    case 68:
                                      return len == 9 ? 41 : -1;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 114:
        case 82:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 111:
            case 79:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 109:
                case 77:
                  return len == 4 ? 42 : -1;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 104:
    case 72:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 111:
        case 79:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 115:
            case 83:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 116:
                case 84:
                  return len == 4 ? 43 : -1;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 105:
    case 73:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 102:
        case 70:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 45:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 109:
                case 77:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 97:
                    case 65:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 116:
                        case 84:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 99:
                            case 67:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 104:
                                case 72:
                                  return len == 8 ? 44 : -1;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    case 111:
                    case 79:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 100:
                        case 68:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 105:
                            case 73:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 102:
                                case 70:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 105:
                                    case 73:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 101:
                                        case 69:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 100:
                                            case 68:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 45:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 115:
                                                    case 83:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 105:
                                                        case 73:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 110:
                                                            case 78:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 99:
                                                                case 67:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 101:
                                                                    case 69:
                                                                      return len ==
                                                                        17
                                                                        ? 45
                                                                        : -1;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                case 110:
                case 78:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 111:
                    case 79:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 110:
                        case 78:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 101:
                            case 69:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 45:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 109:
                                    case 77:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 97:
                                        case 65:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 116:
                                            case 84:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 99:
                                                case 67:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 104:
                                                    case 72:
                                                      return len == 13
                                                        ? 46
                                                        : -1;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                case 114:
                case 82:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 97:
                    case 65:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 110:
                        case 78:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 103:
                            case 71:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 101:
                                case 69:
                                  return len == 8 ? 47 : -1;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                case 117:
                case 85:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 110:
                    case 78:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 109:
                        case 77:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 111:
                            case 79:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 100:
                                case 68:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 105:
                                    case 73:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 102:
                                        case 70:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 105:
                                            case 73:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 101:
                                                case 69:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 100:
                                                    case 68:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 45:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 115:
                                                            case 83:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 105:
                                                                case 73:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 110:
                                                                    case 78:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 99:
                                                                        case 67:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 101:
                                                                            case 69:
                                                                              return len ==
                                                                                19
                                                                                ? 48
                                                                                : -1;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 107:
    case 75:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 101:
        case 69:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 101:
            case 69:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 112:
                case 80:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 45:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 97:
                        case 65:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 108:
                            case 76:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 105:
                                case 73:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 118:
                                    case 86:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 101:
                                        case 69:
                                          return len == 10 ? 49 : -1;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 108:
    case 76:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 97:
        case 65:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 115:
            case 83:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 116:
                case 84:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 45:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 109:
                        case 77:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 111:
                            case 79:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 100:
                                case 68:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 105:
                                    case 73:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 102:
                                        case 70:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 105:
                                            case 73:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 101:
                                                case 69:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 100:
                                                    case 68:
                                                      return len == 13
                                                        ? 50
                                                        : -1;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 105:
        case 73:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 110:
            case 78:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 107:
                case 75:
                  return len == 4 ? 51 : -1;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 111:
        case 79:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 99:
            case 67:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 97:
                case 65:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 116:
                    case 84:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 105:
                        case 73:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 111:
                            case 79:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 110:
                                case 78:
                                  return len == 8 ? 52 : -1;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 109:
    case 77:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 97:
        case 65:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 120:
            case 88:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 45:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 102:
                    case 70:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 111:
                        case 79:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 114:
                            case 82:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 119:
                                case 87:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 97:
                                    case 65:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 114:
                                        case 82:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 100:
                                            case 68:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 115:
                                                case 83:
                                                  return len == 12 ? 53 : -1;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 111:
    case 79:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 114:
        case 82:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 105:
            case 73:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 103:
                case 71:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 105:
                    case 73:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 110:
                        case 78:
                          return len == 6 ? 54 : -1;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 112:
    case 80:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 101:
        case 69:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 114:
            case 82:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 109:
                case 77:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 105:
                    case 73:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 115:
                        case 83:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 115:
                            case 83:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 105:
                                case 73:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 111:
                                    case 79:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 110:
                                        case 78:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 115:
                                            case 83:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 45:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 112:
                                                    case 80:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 111:
                                                        case 79:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 108:
                                                            case 76:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 105:
                                                                case 73:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 99:
                                                                    case 67:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 121:
                                                                        case 89:
                                                                          return len ==
                                                                            18
                                                                            ? 55
                                                                            : -1;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 114:
        case 82:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 97:
            case 65:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 103:
                case 71:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 109:
                    case 77:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 97:
                        case 65:
                          return len == 6 ? 56 : -1;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            case 111:
            case 79:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 120:
                case 88:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 121:
                    case 89:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 45:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 97:
                            case 65:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 117:
                                case 85:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 116:
                                    case 84:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 104:
                                        case 72:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 101:
                                            case 69:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 110:
                                                case 78:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 116:
                                                    case 84:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 105:
                                                        case 73:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 99:
                                                            case 67:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 97:
                                                                case 65:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 116:
                                                                    case 84:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 101:
                                                                        case 69:
                                                                          return len ==
                                                                            18
                                                                            ? 57
                                                                            : -1;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            case 111:
                                            case 79:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 114:
                                                case 82:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 105:
                                                    case 73:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 122:
                                                        case 90:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 97:
                                                            case 65:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 116:
                                                                case 84:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 105:
                                                                    case 73:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 111:
                                                                        case 79:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 110:
                                                                            case 78:
                                                                              return len ==
                                                                                19
                                                                                ? 58
                                                                                : -1;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 114:
    case 82:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 116:
        case 84:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 116:
            case 84:
              return len == 3 ? 59 : -1;
            default:
              return -1;
          }
          break;
        case 97:
        case 65:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 110:
            case 78:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 103:
                case 71:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 101:
                    case 69:
                      return len == 5 ? 60 : -1;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 101:
        case 69:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 102:
            case 70:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 101:
                case 69:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 114:
                    case 82:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 101:
                        case 69:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 114:
                            case 82:
                              return len == 7 ? 61 : -1;
                            default:
                              return -1;
                          }
                          break;
                        case 114:
                        case 82:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 101:
                            case 69:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 114:
                                case 82:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 45:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 112:
                                        case 80:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 111:
                                            case 79:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 108:
                                                case 76:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 105:
                                                    case 73:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 99:
                                                        case 67:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 121:
                                                            case 89:
                                                              return len == 15
                                                                ? 62
                                                                : -1;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                case 114:
                case 82:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 101:
                    case 69:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 115:
                        case 83:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 104:
                            case 72:
                              return len == 7 ? 63 : -1;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            case 116:
            case 84:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 114:
                case 82:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 121:
                    case 89:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 45:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 97:
                            case 65:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 102:
                                case 70:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 116:
                                    case 84:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 101:
                                        case 69:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 114:
                                            case 82:
                                              return len == 11 ? 64 : -1;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 115:
    case 83:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 101:
        case 69:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 99:
            case 67:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 45:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 119:
                    case 87:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 101:
                        case 69:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 98:
                            case 66:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 115:
                                case 83:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 111:
                                    case 79:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 99:
                                        case 67:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 107:
                                            case 75:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 101:
                                                case 69:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 116:
                                                    case 84:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 45:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 97:
                                                            case 65:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 99:
                                                                case 67:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 99:
                                                                    case 67:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 101:
                                                                        case 69:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 112:
                                                                            case 80:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 116:
                                                                                case 84:
                                                                                  return len ==
                                                                                    20
                                                                                    ? 65
                                                                                    : -1;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            case 101:
                                                            case 69:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 120:
                                                                case 88:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 116:
                                                                    case 84:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 101:
                                                                        case 69:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 110:
                                                                            case 78:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 115:
                                                                                case 83:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 105:
                                                                                    case 73:
                                                                                      if (
                                                                                        len ==
                                                                                        20
                                                                                      ) {
                                                                                        return -1;
                                                                                      }
                                                                                      switch (
                                                                                        s[21]
                                                                                      ) {
                                                                                        case 111:
                                                                                        case 79:
                                                                                          if (
                                                                                            len ==
                                                                                            21
                                                                                          ) {
                                                                                            return -1;
                                                                                          }
                                                                                          switch (
                                                                                            s[22]
                                                                                          ) {
                                                                                            case 110:
                                                                                            case 78:
                                                                                              if (
                                                                                                len ==
                                                                                                22
                                                                                              ) {
                                                                                                return -1;
                                                                                              }
                                                                                              switch (
                                                                                                s[23]
                                                                                              ) {
                                                                                                case 115:
                                                                                                case 83:
                                                                                                  return len ==
                                                                                                    24
                                                                                                    ? 66
                                                                                                    : -1;
                                                                                                default:
                                                                                                  return -1;
                                                                                              }
                                                                                              break;
                                                                                            default:
                                                                                              return -1;
                                                                                          }
                                                                                          break;
                                                                                        default:
                                                                                          return -1;
                                                                                      }
                                                                                      break;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            case 107:
                                                            case 75:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 101:
                                                                case 69:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 121:
                                                                    case 89:
                                                                      return len ==
                                                                        17
                                                                        ? 67
                                                                        : -1;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            case 112:
                                                            case 80:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 114:
                                                                case 82:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 111:
                                                                    case 79:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 116:
                                                                        case 84:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 111:
                                                                            case 79:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 99:
                                                                                case 67:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 111:
                                                                                    case 79:
                                                                                      if (
                                                                                        len ==
                                                                                        20
                                                                                      ) {
                                                                                        return -1;
                                                                                      }
                                                                                      switch (
                                                                                        s[21]
                                                                                      ) {
                                                                                        case 108:
                                                                                        case 76:
                                                                                          return len ==
                                                                                            22
                                                                                            ? 68
                                                                                            : -1;
                                                                                        default:
                                                                                          return -1;
                                                                                      }
                                                                                      break;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            case 118:
                                                            case 86:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 101:
                                                                case 69:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 114:
                                                                    case 82:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 115:
                                                                        case 83:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 105:
                                                                            case 73:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 111:
                                                                                case 79:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 110:
                                                                                    case 78:
                                                                                      return len ==
                                                                                        21
                                                                                        ? 69
                                                                                        : -1;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            case 114:
            case 82:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 118:
                case 86:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 101:
                    case 69:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 114:
                        case 82:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 45:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 116:
                                case 84:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 105:
                                    case 73:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 109:
                                        case 77:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 105:
                                            case 73:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 110:
                                                case 78:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 103:
                                                    case 71:
                                                      return len == 13
                                                        ? 71
                                                        : -1;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return len == 6 ? 70 : -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    case 105:
                    case 73:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 99:
                        case 67:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 101:
                            case 69:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 45:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 119:
                                    case 87:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 111:
                                        case 79:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 114:
                                            case 82:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 107:
                                                case 75:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 101:
                                                    case 69:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 114:
                                                        case 82:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 45:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 97:
                                                                case 65:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 108:
                                                                    case 76:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 108:
                                                                        case 76:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 111:
                                                                            case 79:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 119:
                                                                                case 87:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 101:
                                                                                    case 69:
                                                                                      if (
                                                                                        len ==
                                                                                        20
                                                                                      ) {
                                                                                        return -1;
                                                                                      }
                                                                                      switch (
                                                                                        s[21]
                                                                                      ) {
                                                                                        case 100:
                                                                                        case 68:
                                                                                          return len ==
                                                                                            22
                                                                                            ? 72
                                                                                            : -1;
                                                                                        default:
                                                                                          return -1;
                                                                                      }
                                                                                      break;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                case 110:
                                                                case 78:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 97:
                                                                    case 65:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 118:
                                                                        case 86:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 105:
                                                                            case 73:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 103:
                                                                                case 71:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 97:
                                                                                    case 65:
                                                                                      if (
                                                                                        len ==
                                                                                        20
                                                                                      ) {
                                                                                        return -1;
                                                                                      }
                                                                                      switch (
                                                                                        s[21]
                                                                                      ) {
                                                                                        case 116:
                                                                                        case 84:
                                                                                          if (
                                                                                            len ==
                                                                                            21
                                                                                          ) {
                                                                                            return -1;
                                                                                          }
                                                                                          switch (
                                                                                            s[22]
                                                                                          ) {
                                                                                            case 105:
                                                                                            case 73:
                                                                                              if (
                                                                                                len ==
                                                                                                22
                                                                                              ) {
                                                                                                return -1;
                                                                                              }
                                                                                              switch (
                                                                                                s[23]
                                                                                              ) {
                                                                                                case 111:
                                                                                                case 79:
                                                                                                  if (
                                                                                                    len ==
                                                                                                    23
                                                                                                  ) {
                                                                                                    return -1;
                                                                                                  }
                                                                                                  switch (
                                                                                                    s[24]
                                                                                                  ) {
                                                                                                    case 110:
                                                                                                    case 78:
                                                                                                      if (
                                                                                                        len ==
                                                                                                        24
                                                                                                      ) {
                                                                                                        return -1;
                                                                                                      }
                                                                                                      switch (
                                                                                                        s[25]
                                                                                                      ) {
                                                                                                        case 45:
                                                                                                          if (
                                                                                                            len ==
                                                                                                            25
                                                                                                          ) {
                                                                                                            return -1;
                                                                                                          }
                                                                                                          switch (
                                                                                                            s[26]
                                                                                                          ) {
                                                                                                            case 112:
                                                                                                            case 80:
                                                                                                              if (
                                                                                                                len ==
                                                                                                                26
                                                                                                              ) {
                                                                                                                return -1;
                                                                                                              }
                                                                                                              switch (
                                                                                                                s[27]
                                                                                                              ) {
                                                                                                                case 114:
                                                                                                                case 82:
                                                                                                                  if (
                                                                                                                    len ==
                                                                                                                    27
                                                                                                                  ) {
                                                                                                                    return -1;
                                                                                                                  }
                                                                                                                  switch (
                                                                                                                    s[28]
                                                                                                                  ) {
                                                                                                                    case 101:
                                                                                                                    case 69:
                                                                                                                      if (
                                                                                                                        len ==
                                                                                                                        28
                                                                                                                      ) {
                                                                                                                        return -1;
                                                                                                                      }
                                                                                                                      switch (
                                                                                                                        s[29]
                                                                                                                      ) {
                                                                                                                        case 108:
                                                                                                                        case 76:
                                                                                                                          if (
                                                                                                                            len ==
                                                                                                                            29
                                                                                                                          ) {
                                                                                                                            return -1;
                                                                                                                          }
                                                                                                                          switch (
                                                                                                                            s[30]
                                                                                                                          ) {
                                                                                                                            case 111:
                                                                                                                            case 79:
                                                                                                                              if (
                                                                                                                                len ==
                                                                                                                                30
                                                                                                                              ) {
                                                                                                                                return -1;
                                                                                                                              }
                                                                                                                              switch (
                                                                                                                                s[31]
                                                                                                                              ) {
                                                                                                                                case 97:
                                                                                                                                case 65:
                                                                                                                                  if (
                                                                                                                                    len ==
                                                                                                                                    31
                                                                                                                                  ) {
                                                                                                                                    return -1;
                                                                                                                                  }
                                                                                                                                  switch (
                                                                                                                                    s[32]
                                                                                                                                  ) {
                                                                                                                                    case 100:
                                                                                                                                    case 68:
                                                                                                                                      return len ==
                                                                                                                                        33
                                                                                                                                        ? 73
                                                                                                                                        : -1;
                                                                                                                                    default:
                                                                                                                                      return -1;
                                                                                                                                  }
                                                                                                                                  break;
                                                                                                                                default:
                                                                                                                                  return -1;
                                                                                                                              }
                                                                                                                              break;
                                                                                                                            default:
                                                                                                                              return -1;
                                                                                                                          }
                                                                                                                          break;
                                                                                                                        default:
                                                                                                                          return -1;
                                                                                                                      }
                                                                                                                      break;
                                                                                                                    default:
                                                                                                                      return -1;
                                                                                                                  }
                                                                                                                  break;
                                                                                                                default:
                                                                                                                  return -1;
                                                                                                              }
                                                                                                              break;
                                                                                                            default:
                                                                                                              return -1;
                                                                                                          }
                                                                                                          break;
                                                                                                        default:
                                                                                                          return -1;
                                                                                                      }
                                                                                                      break;
                                                                                                    default:
                                                                                                      return -1;
                                                                                                  }
                                                                                                  break;
                                                                                                default:
                                                                                                  return -1;
                                                                                              }
                                                                                              break;
                                                                                            default:
                                                                                              return -1;
                                                                                          }
                                                                                          break;
                                                                                        default:
                                                                                          return -1;
                                                                                      }
                                                                                      break;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            case 116:
            case 84:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 45:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 99:
                    case 67:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 111:
                        case 79:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 111:
                            case 79:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 107:
                                case 75:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 105:
                                    case 73:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 101:
                                        case 69:
                                          return len == 10 ? 74 : -1;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 111:
        case 79:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 117:
            case 85:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 114:
                case 82:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 99:
                    case 67:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 101:
                        case 69:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 109:
                            case 77:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 97:
                                case 65:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 112:
                                    case 80:
                                      return len == 9 ? 75 : -1;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 116:
        case 84:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 114:
            case 82:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 105:
                case 73:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 99:
                    case 67:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 116:
                        case 84:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 45:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 116:
                                case 84:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 114:
                                    case 82:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 97:
                                        case 65:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 110:
                                            case 78:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 115:
                                                case 83:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 112:
                                                    case 80:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 111:
                                                        case 79:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 114:
                                                            case 82:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 116:
                                                                case 84:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 45:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 115:
                                                                        case 83:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 101:
                                                                            case 69:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 99:
                                                                                case 67:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 117:
                                                                                    case 85:
                                                                                      if (
                                                                                        len ==
                                                                                        20
                                                                                      ) {
                                                                                        return -1;
                                                                                      }
                                                                                      switch (
                                                                                        s[21]
                                                                                      ) {
                                                                                        case 114:
                                                                                        case 82:
                                                                                          if (
                                                                                            len ==
                                                                                            21
                                                                                          ) {
                                                                                            return -1;
                                                                                          }
                                                                                          switch (
                                                                                            s[22]
                                                                                          ) {
                                                                                            case 105:
                                                                                            case 73:
                                                                                              if (
                                                                                                len ==
                                                                                                22
                                                                                              ) {
                                                                                                return -1;
                                                                                              }
                                                                                              switch (
                                                                                                s[23]
                                                                                              ) {
                                                                                                case 116:
                                                                                                case 84:
                                                                                                  if (
                                                                                                    len ==
                                                                                                    23
                                                                                                  ) {
                                                                                                    return -1;
                                                                                                  }
                                                                                                  switch (
                                                                                                    s[24]
                                                                                                  ) {
                                                                                                    case 121:
                                                                                                    case 89:
                                                                                                      return len ==
                                                                                                        25
                                                                                                        ? 76
                                                                                                        : -1;
                                                                                                    default:
                                                                                                      return -1;
                                                                                                  }
                                                                                                  break;
                                                                                                default:
                                                                                                  return -1;
                                                                                              }
                                                                                              break;
                                                                                            default:
                                                                                              return -1;
                                                                                          }
                                                                                          break;
                                                                                        default:
                                                                                          return -1;
                                                                                      }
                                                                                      break;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 117:
        case 85:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 112:
            case 80:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 112:
                case 80:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 111:
                    case 79:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 114:
                        case 82:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 116:
                            case 84:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 115:
                                case 83:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 45:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 108:
                                        case 76:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 111:
                                            case 79:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 97:
                                                case 65:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 100:
                                                    case 68:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 105:
                                                        case 73:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 110:
                                                            case 78:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 103:
                                                                case 71:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 45:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 109:
                                                                        case 77:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 111:
                                                                            case 79:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 100:
                                                                                case 68:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 101:
                                                                                    case 69:
                                                                                      return len ==
                                                                                        21
                                                                                        ? 77
                                                                                        : -1;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 116:
    case 84:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 101:
        case 69:
          return len == 2 ? 78 : -1;
        case 105:
        case 73:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 109:
            case 77:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 105:
                case 73:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 110:
                    case 78:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 103:
                        case 71:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 45:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 97:
                                case 65:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 108:
                                    case 76:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 108:
                                        case 76:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 111:
                                            case 79:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 119:
                                                case 87:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 45:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 111:
                                                        case 79:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 114:
                                                            case 82:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 105:
                                                                case 73:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 103:
                                                                    case 71:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 105:
                                                                        case 73:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 110:
                                                                            case 78:
                                                                              return len ==
                                                                                19
                                                                                ? 79
                                                                                : -1;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 114:
        case 82:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 97:
            case 65:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 105:
                case 73:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 108:
                    case 76:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 101:
                        case 69:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 114:
                            case 82:
                              return len == 7 ? 80 : -1;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                case 110:
                case 78:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 115:
                    case 83:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 102:
                        case 70:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 101:
                            case 69:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 114:
                                case 82:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 45:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 101:
                                        case 69:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 110:
                                            case 78:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 99:
                                                case 67:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 111:
                                                    case 79:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 100:
                                                        case 68:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 105:
                                                            case 73:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 110:
                                                                case 78:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 103:
                                                                    case 71:
                                                                      return len ==
                                                                        17
                                                                        ? 81
                                                                        : -1;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 117:
    case 85:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 112:
        case 80:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 103:
            case 71:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 114:
                case 82:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 97:
                    case 65:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 100:
                        case 68:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 101:
                            case 69:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 45:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 105:
                                    case 73:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 110:
                                        case 78:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 115:
                                            case 83:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 101:
                                                case 69:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 99:
                                                    case 67:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 117:
                                                        case 85:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 114:
                                                            case 82:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 101:
                                                                case 69:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 45:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 114:
                                                                        case 82:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 101:
                                                                            case 69:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 113:
                                                                                case 81:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 117:
                                                                                    case 85:
                                                                                      if (
                                                                                        len ==
                                                                                        20
                                                                                      ) {
                                                                                        return -1;
                                                                                      }
                                                                                      switch (
                                                                                        s[21]
                                                                                      ) {
                                                                                        case 101:
                                                                                        case 69:
                                                                                          if (
                                                                                            len ==
                                                                                            21
                                                                                          ) {
                                                                                            return -1;
                                                                                          }
                                                                                          switch (
                                                                                            s[22]
                                                                                          ) {
                                                                                            case 115:
                                                                                            case 83:
                                                                                              if (
                                                                                                len ==
                                                                                                22
                                                                                              ) {
                                                                                                return -1;
                                                                                              }
                                                                                              switch (
                                                                                                s[23]
                                                                                              ) {
                                                                                                case 116:
                                                                                                case 84:
                                                                                                  if (
                                                                                                    len ==
                                                                                                    23
                                                                                                  ) {
                                                                                                    return -1;
                                                                                                  }
                                                                                                  switch (
                                                                                                    s[24]
                                                                                                  ) {
                                                                                                    case 115:
                                                                                                    case 83:
                                                                                                      return len ==
                                                                                                        25
                                                                                                        ? 83
                                                                                                        : -1;
                                                                                                    default:
                                                                                                      return -1;
                                                                                                  }
                                                                                                  break;
                                                                                                default:
                                                                                                  return -1;
                                                                                              }
                                                                                              break;
                                                                                            default:
                                                                                              return -1;
                                                                                          }
                                                                                          break;
                                                                                        default:
                                                                                          return -1;
                                                                                      }
                                                                                      break;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return len == 7 ? 82 : -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 115:
        case 83:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 101:
            case 69:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 114:
                case 82:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 45:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 97:
                        case 65:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 103:
                            case 71:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 101:
                                case 69:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 110:
                                    case 78:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 116:
                                        case 84:
                                          return len == 10 ? 84 : -1;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 118:
    case 86:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 97:
        case 65:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 114:
            case 82:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 121:
                case 89:
                  return len == 4 ? 85 : -1;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        case 105:
        case 73:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 97:
            case 65:
              return len == 3 ? 86 : -1;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 119:
    case 87:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 119:
        case 87:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 119:
            case 87:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 45:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 97:
                    case 65:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 117:
                        case 85:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 116:
                            case 84:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 104:
                                case 72:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 101:
                                    case 69:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 110:
                                        case 78:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 116:
                                            case 84:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 105:
                                                case 73:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 99:
                                                    case 67:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 97:
                                                        case 65:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 116:
                                                            case 84:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 101:
                                                                case 69:
                                                                  return len ==
                                                                    16
                                                                    ? 87
                                                                    : -1;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    case 120:
    case 88:
      if (len == 0) {
        return -1;
      }
      switch (s[1]) {
        case 45:
          if (len == 1) {
            return -1;
          }
          switch (s[2]) {
            case 99:
            case 67:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 111:
                case 79:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 110:
                    case 78:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 116:
                        case 84:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 101:
                            case 69:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 110:
                                case 78:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 116:
                                    case 84:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 45:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 116:
                                            case 84:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 121:
                                                case 89:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 112:
                                                    case 80:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 101:
                                                        case 69:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 45:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 111:
                                                                case 79:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 112:
                                                                    case 80:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 116:
                                                                        case 84:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 105:
                                                                            case 73:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 111:
                                                                                case 79:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 110:
                                                                                    case 78:
                                                                                      if (
                                                                                        len ==
                                                                                        20
                                                                                      ) {
                                                                                        return -1;
                                                                                      }
                                                                                      switch (
                                                                                        s[21]
                                                                                      ) {
                                                                                        case 115:
                                                                                        case 83:
                                                                                          return len ==
                                                                                            22
                                                                                            ? 88
                                                                                            : -1;
                                                                                        default:
                                                                                          return -1;
                                                                                      }
                                                                                      break;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            case 100:
            case 68:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 110:
                case 78:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 115:
                    case 83:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 45:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 112:
                            case 80:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 114:
                                case 82:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 101:
                                    case 69:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 102:
                                        case 70:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 101:
                                            case 69:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 116:
                                                case 84:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 99:
                                                    case 67:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 104:
                                                        case 72:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 45:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 99:
                                                                case 67:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 111:
                                                                    case 79:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 110:
                                                                        case 78:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 116:
                                                                            case 84:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 114:
                                                                                case 82:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 111:
                                                                                    case 79:
                                                                                      if (
                                                                                        len ==
                                                                                        20
                                                                                      ) {
                                                                                        return -1;
                                                                                      }
                                                                                      switch (
                                                                                        s[21]
                                                                                      ) {
                                                                                        case 108:
                                                                                        case 76:
                                                                                          return len ==
                                                                                            22
                                                                                            ? 89
                                                                                            : -1;
                                                                                        default:
                                                                                          return -1;
                                                                                      }
                                                                                      break;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            case 102:
            case 70:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 114:
                case 82:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 97:
                    case 65:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 109:
                        case 77:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 101:
                            case 69:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 45:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 111:
                                    case 79:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 112:
                                        case 80:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 116:
                                            case 84:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 105:
                                                case 73:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 111:
                                                    case 79:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 110:
                                                        case 78:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 115:
                                                            case 83:
                                                              return len == 15
                                                                ? 90
                                                                : -1;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            case 112:
            case 80:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 101:
                case 69:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 114:
                    case 82:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 109:
                        case 77:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 105:
                            case 73:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 116:
                                case 84:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 116:
                                    case 84:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 101:
                                        case 69:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 100:
                                            case 68:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 45:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 99:
                                                    case 67:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 114:
                                                        case 82:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 111:
                                                            case 79:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 115:
                                                                case 83:
                                                                  if (
                                                                    len == 15
                                                                  ) {
                                                                    return -1;
                                                                  }
                                                                  switch (
                                                                    s[16]
                                                                  ) {
                                                                    case 115:
                                                                    case 83:
                                                                      if (
                                                                        len ==
                                                                        16
                                                                      ) {
                                                                        return -1;
                                                                      }
                                                                      switch (
                                                                        s[17]
                                                                      ) {
                                                                        case 45:
                                                                          if (
                                                                            len ==
                                                                            17
                                                                          ) {
                                                                            return -1;
                                                                          }
                                                                          switch (
                                                                            s[18]
                                                                          ) {
                                                                            case 100:
                                                                            case 68:
                                                                              if (
                                                                                len ==
                                                                                18
                                                                              ) {
                                                                                return -1;
                                                                              }
                                                                              switch (
                                                                                s[19]
                                                                              ) {
                                                                                case 111:
                                                                                case 79:
                                                                                  if (
                                                                                    len ==
                                                                                    19
                                                                                  ) {
                                                                                    return -1;
                                                                                  }
                                                                                  switch (
                                                                                    s[20]
                                                                                  ) {
                                                                                    case 109:
                                                                                    case 77:
                                                                                      if (
                                                                                        len ==
                                                                                        20
                                                                                      ) {
                                                                                        return -1;
                                                                                      }
                                                                                      switch (
                                                                                        s[21]
                                                                                      ) {
                                                                                        case 97:
                                                                                        case 65:
                                                                                          if (
                                                                                            len ==
                                                                                            21
                                                                                          ) {
                                                                                            return -1;
                                                                                          }
                                                                                          switch (
                                                                                            s[22]
                                                                                          ) {
                                                                                            case 105:
                                                                                            case 73:
                                                                                              if (
                                                                                                len ==
                                                                                                22
                                                                                              ) {
                                                                                                return -1;
                                                                                              }
                                                                                              switch (
                                                                                                s[23]
                                                                                              ) {
                                                                                                case 110:
                                                                                                case 78:
                                                                                                  if (
                                                                                                    len ==
                                                                                                    23
                                                                                                  ) {
                                                                                                    return -1;
                                                                                                  }
                                                                                                  switch (
                                                                                                    s[24]
                                                                                                  ) {
                                                                                                    case 45:
                                                                                                      if (
                                                                                                        len ==
                                                                                                        24
                                                                                                      ) {
                                                                                                        return -1;
                                                                                                      }
                                                                                                      switch (
                                                                                                        s[25]
                                                                                                      ) {
                                                                                                        case 112:
                                                                                                        case 80:
                                                                                                          if (
                                                                                                            len ==
                                                                                                            25
                                                                                                          ) {
                                                                                                            return -1;
                                                                                                          }
                                                                                                          switch (
                                                                                                            s[26]
                                                                                                          ) {
                                                                                                            case 111:
                                                                                                            case 79:
                                                                                                              if (
                                                                                                                len ==
                                                                                                                26
                                                                                                              ) {
                                                                                                                return -1;
                                                                                                              }
                                                                                                              switch (
                                                                                                                s[27]
                                                                                                              ) {
                                                                                                                case 108:
                                                                                                                case 76:
                                                                                                                  if (
                                                                                                                    len ==
                                                                                                                    27
                                                                                                                  ) {
                                                                                                                    return -1;
                                                                                                                  }
                                                                                                                  switch (
                                                                                                                    s[28]
                                                                                                                  ) {
                                                                                                                    case 105:
                                                                                                                    case 73:
                                                                                                                      if (
                                                                                                                        len ==
                                                                                                                        28
                                                                                                                      ) {
                                                                                                                        return -1;
                                                                                                                      }
                                                                                                                      switch (
                                                                                                                        s[29]
                                                                                                                      ) {
                                                                                                                        case 99:
                                                                                                                        case 67:
                                                                                                                          if (
                                                                                                                            len ==
                                                                                                                            29
                                                                                                                          ) {
                                                                                                                            return -1;
                                                                                                                          }
                                                                                                                          switch (
                                                                                                                            s[30]
                                                                                                                          ) {
                                                                                                                            case 105:
                                                                                                                            case 73:
                                                                                                                              if (
                                                                                                                                len ==
                                                                                                                                30
                                                                                                                              ) {
                                                                                                                                return -1;
                                                                                                                              }
                                                                                                                              switch (
                                                                                                                                s[31]
                                                                                                                              ) {
                                                                                                                                case 101:
                                                                                                                                case 69:
                                                                                                                                  if (
                                                                                                                                    len ==
                                                                                                                                    31
                                                                                                                                  ) {
                                                                                                                                    return -1;
                                                                                                                                  }
                                                                                                                                  switch (
                                                                                                                                    s[32]
                                                                                                                                  ) {
                                                                                                                                    case 115:
                                                                                                                                    case 83:
                                                                                                                                      return len ==
                                                                                                                                        33
                                                                                                                                        ? 91
                                                                                                                                        : -1;
                                                                                                                                    default:
                                                                                                                                      return -1;
                                                                                                                                  }
                                                                                                                                  break;
                                                                                                                                default:
                                                                                                                                  return -1;
                                                                                                                              }
                                                                                                                              break;
                                                                                                                            default:
                                                                                                                              return -1;
                                                                                                                          }
                                                                                                                          break;
                                                                                                                        default:
                                                                                                                          return -1;
                                                                                                                      }
                                                                                                                      break;
                                                                                                                    default:
                                                                                                                      return -1;
                                                                                                                  }
                                                                                                                  break;
                                                                                                                default:
                                                                                                                  return -1;
                                                                                                              }
                                                                                                              break;
                                                                                                            default:
                                                                                                              return -1;
                                                                                                          }
                                                                                                          break;
                                                                                                        default:
                                                                                                          return -1;
                                                                                                      }
                                                                                                      break;
                                                                                                    default:
                                                                                                      return -1;
                                                                                                  }
                                                                                                  break;
                                                                                                default:
                                                                                                  return -1;
                                                                                              }
                                                                                              break;
                                                                                            default:
                                                                                              return -1;
                                                                                          }
                                                                                          break;
                                                                                        default:
                                                                                          return -1;
                                                                                      }
                                                                                      break;
                                                                                    default:
                                                                                      return -1;
                                                                                  }
                                                                                  break;
                                                                                default:
                                                                                  return -1;
                                                                              }
                                                                              break;
                                                                            default:
                                                                              return -1;
                                                                          }
                                                                          break;
                                                                        default:
                                                                          return -1;
                                                                      }
                                                                      break;
                                                                    default:
                                                                      return -1;
                                                                  }
                                                                  break;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                case 111:
                case 79:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 119:
                    case 87:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 101:
                        case 69:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 114:
                            case 82:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 101:
                                case 69:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 100:
                                    case 68:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 45:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 98:
                                            case 66:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 121:
                                                case 89:
                                                  return len == 12 ? 92 : -1;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            case 114:
            case 82:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 101:
                case 69:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 113:
                    case 81:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 117:
                        case 85:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 101:
                            case 69:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 115:
                                case 83:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 116:
                                    case 84:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 101:
                                        case 69:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 100:
                                            case 68:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 45:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 119:
                                                    case 87:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 105:
                                                        case 73:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 116:
                                                            case 84:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 104:
                                                                case 72:
                                                                  return len ==
                                                                    16
                                                                    ? 93
                                                                    : -1;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            case 120:
            case 88:
              if (len == 2) {
                return -1;
              }
              switch (s[3]) {
                case 115:
                case 83:
                  if (len == 3) {
                    return -1;
                  }
                  switch (s[4]) {
                    case 115:
                    case 83:
                      if (len == 4) {
                        return -1;
                      }
                      switch (s[5]) {
                        case 45:
                          if (len == 5) {
                            return -1;
                          }
                          switch (s[6]) {
                            case 112:
                            case 80:
                              if (len == 6) {
                                return -1;
                              }
                              switch (s[7]) {
                                case 114:
                                case 82:
                                  if (len == 7) {
                                    return -1;
                                  }
                                  switch (s[8]) {
                                    case 111:
                                    case 79:
                                      if (len == 8) {
                                        return -1;
                                      }
                                      switch (s[9]) {
                                        case 116:
                                        case 84:
                                          if (len == 9) {
                                            return -1;
                                          }
                                          switch (s[10]) {
                                            case 101:
                                            case 69:
                                              if (len == 10) {
                                                return -1;
                                              }
                                              switch (s[11]) {
                                                case 99:
                                                case 67:
                                                  if (len == 11) {
                                                    return -1;
                                                  }
                                                  switch (s[12]) {
                                                    case 116:
                                                    case 84:
                                                      if (len == 12) {
                                                        return -1;
                                                      }
                                                      switch (s[13]) {
                                                        case 105:
                                                        case 73:
                                                          if (len == 13) {
                                                            return -1;
                                                          }
                                                          switch (s[14]) {
                                                            case 111:
                                                            case 79:
                                                              if (len == 14) {
                                                                return -1;
                                                              }
                                                              switch (s[15]) {
                                                                case 110:
                                                                case 78:
                                                                  return len ==
                                                                    16
                                                                    ? 94
                                                                    : -1;
                                                                default:
                                                                  return -1;
                                                              }
                                                              break;
                                                            default:
                                                              return -1;
                                                          }
                                                          break;
                                                        default:
                                                          return -1;
                                                      }
                                                      break;
                                                    default:
                                                      return -1;
                                                  }
                                                  break;
                                                default:
                                                  return -1;
                                              }
                                              break;
                                            default:
                                              return -1;
                                          }
                                          break;
                                        default:
                                          return -1;
                                      }
                                      break;
                                    default:
                                      return -1;
                                  }
                                  break;
                                default:
                                  return -1;
                              }
                              break;
                            default:
                              return -1;
                          }
                          break;
                        default:
                          return -1;
                      }
                      break;
                    default:
                      return -1;
                  }
                  break;
                default:
                  return -1;
              }
              break;
            default:
              return -1;
          }
          break;
        default:
          return -1;
      }
      break;
    default:
      return -1;
  }
}

function headerNameToString(value) {
  let idx = classify(value);
  if (idx === -1) return value.toString().toLowerCase();
  return strings[idx];
}

export function trieparseHeaders(headers, obj = {}) {
  // For H2 support
  if (!Array.isArray(headers)) return headers;

  for (let i = 0; i < headers.length; i += 2) {
    const key = headers[i];
    const lowerCasedKey = headerNameToString(key);
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
