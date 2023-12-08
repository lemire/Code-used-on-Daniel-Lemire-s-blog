
function toLowerCase(s) {
    var len = s.length;
    switch (len) {
      case 2:
        // check that the buffer is equal to te and return it if so
        if ((s[0] == 116 || s[0] == 84) && (s[1] == 101 || s[1] == 69)) {
          return "te";
        }
        break;
      case 3:
        switch (s[0]) {
          case 97:
          case 65:
            // check that the buffer is equal to age and return it if so
            if ((s[1] == 103 || s[1] == 71) && (s[2] == 101 || s[2] == 69)) {
              return "age";
            }
            break;
          case 101:
          case 69:
            // check that the buffer is equal to ect and return it if so
            if ((s[1] == 99 || s[1] == 67) && (s[2] == 116 || s[2] == 84)) {
              return "ect";
            }
            break;
          case 114:
          case 82:
            // check that the buffer is equal to rtt and return it if so
            if ((s[1] == 116 || s[1] == 84) && (s[2] == 116 || s[2] == 84)) {
              return "rtt";
            }
            break;
          case 118:
          case 86:
            // check that the buffer is equal to via and return it if so
            if ((s[1] == 105 || s[1] == 73) && (s[2] == 97 || s[2] == 65)) {
              return "via";
            }
            break;
          default:
            break;
        }
        break;
      case 4:
        switch (s[0]) {
          case 100:
          case 68:
            // check that the buffer is equal to date and return it if so
            if (
              (s[1] == 97 || s[1] == 65) &&
              (s[2] == 116 || s[2] == 84) &&
              (s[3] == 101 || s[3] == 69)
            ) {
              return "date";
            }
            break;
          case 101:
          case 69:
            // check that the buffer is equal to etag and return it if so
            if (
              (s[1] == 116 || s[1] == 84) &&
              (s[2] == 97 || s[2] == 65) &&
              (s[3] == 103 || s[3] == 71)
            ) {
              return "etag";
            }
            break;
          case 102:
          case 70:
            // check that the buffer is equal to from and return it if so
            if (
              (s[1] == 114 || s[1] == 82) &&
              (s[2] == 111 || s[2] == 79) &&
              (s[3] == 109 || s[3] == 77)
            ) {
              return "from";
            }
            break;
          case 104:
          case 72:
            // check that the buffer is equal to host and return it if so
            if (
              (s[1] == 111 || s[1] == 79) &&
              (s[2] == 115 || s[2] == 83) &&
              (s[3] == 116 || s[3] == 84)
            ) {
              return "host";
            }
            break;
          case 108:
          case 76:
            // check that the buffer is equal to link and return it if so
            if (
              (s[1] == 105 || s[1] == 73) &&
              (s[2] == 110 || s[2] == 78) &&
              (s[3] == 107 || s[3] == 75)
            ) {
              return "link";
            }
            break;
          case 118:
          case 86:
            // check that the buffer is equal to vary and return it if so
            if (
              (s[1] == 97 || s[1] == 65) &&
              (s[2] == 114 || s[2] == 82) &&
              (s[3] == 121 || s[3] == 89)
            ) {
              return "vary";
            }
            break;
          default:
            break;
        }
        break;
      case 5:
        switch (s[0]) {
          case 97:
          case 65:
            // check that the buffer is equal to allow and return it if so
            if (
              (s[1] == 108 || s[1] == 76) &&
              (s[2] == 108 || s[2] == 76) &&
              (s[3] == 111 || s[3] == 79) &&
              (s[4] == 119 || s[4] == 87)
            ) {
              return "allow";
            }
            break;
          case 114:
          case 82:
            // check that the buffer is equal to range and return it if so
            if (
              (s[1] == 97 || s[1] == 65) &&
              (s[2] == 110 || s[2] == 78) &&
              (s[3] == 103 || s[3] == 71) &&
              (s[4] == 101 || s[4] == 69)
            ) {
              return "range";
            }
            break;
          default:
            break;
        }
        break;
      case 6:
        switch (s[0]) {
          case 97:
          case 65:
            // check that the buffer is equal to accept and return it if so
            if (
              (s[1] == 99 || s[1] == 67) &&
              (s[2] == 99 || s[2] == 67) &&
              (s[3] == 101 || s[3] == 69) &&
              (s[4] == 112 || s[4] == 80) &&
              (s[5] == 116 || s[5] == 84)
            ) {
              return "accept";
            }
            break;
          case 99:
          case 67:
            // check that the buffer is equal to cookie and return it if so
            if (
              (s[1] == 111 || s[1] == 79) &&
              (s[2] == 111 || s[2] == 79) &&
              (s[3] == 107 || s[3] == 75) &&
              (s[4] == 105 || s[4] == 73) &&
              (s[5] == 101 || s[5] == 69)
            ) {
              return "cookie";
            }
            break;
          case 101:
          case 69:
            // check that the buffer is equal to expect and return it if so
            if (
              (s[1] == 120 || s[1] == 88) &&
              (s[2] == 112 || s[2] == 80) &&
              (s[3] == 101 || s[3] == 69) &&
              (s[4] == 99 || s[4] == 67) &&
              (s[5] == 116 || s[5] == 84)
            ) {
              return "expect";
            }
            break;
          case 111:
          case 79:
            // check that the buffer is equal to origin and return it if so
            if (
              (s[1] == 114 || s[1] == 82) &&
              (s[2] == 105 || s[2] == 73) &&
              (s[3] == 103 || s[3] == 71) &&
              (s[4] == 105 || s[4] == 73) &&
              (s[5] == 110 || s[5] == 78)
            ) {
              return "origin";
            }
            break;
          case 112:
          case 80:
            // check that the buffer is equal to pragma and return it if so
            if (
              (s[1] == 114 || s[1] == 82) &&
              (s[2] == 97 || s[2] == 65) &&
              (s[3] == 103 || s[3] == 71) &&
              (s[4] == 109 || s[4] == 77) &&
              (s[5] == 97 || s[5] == 65)
            ) {
              return "pragma";
            }
            break;
          case 115:
          case 83:
            // check that the buffer is equal to server and return it if so
            if (
              (s[1] == 101 || s[1] == 69) &&
              (s[2] == 114 || s[2] == 82) &&
              (s[3] == 118 || s[3] == 86) &&
              (s[4] == 101 || s[4] == 69) &&
              (s[5] == 114 || s[5] == 82)
            ) {
              return "server";
            }
            break;
          default:
            break;
        }
        break;
      case 7:
        ["alt-svc", "expires", "referer", "refresh", "trailer", "upgrade"];
        switch (s[0] + s[6]) {
          case 196:
          case 228:
          case 226:
          case 194:
            // check that the buffer is equal to alt-svc and return it if so
            if (
              (s[0] == 97 || s[0] == 65) &&
              (s[1] == 108 || s[1] == 76) &&
              (s[2] == 116 || s[2] == 84) &&
              s[3] == 45 &&
              (s[4] == 115 || s[4] == 83) &&
              (s[5] == 118 || s[5] == 86) &&
              (s[6] == 99 || s[6] == 67)
            ) {
              return "alt-svc";
            }
          case 216:
          case 248:
          case 246:
          case 214:
            // check that the buffer is equal to expires and return it if so
            if (
              (s[0] == 101 || s[0] == 69) &&
              (s[1] == 120 || s[1] == 88) &&
              (s[2] == 112 || s[2] == 80) &&
              (s[3] == 105 || s[3] == 73) &&
              (s[4] == 114 || s[4] == 82) &&
              (s[5] == 101 || s[5] == 69) &&
              (s[6] == 115 || s[6] == 83)
            ) {
              return "expires";
            }
          case 228:
          case 196:
          case 224:
          case 192:
            // check that the buffer is equal to referer and return it if so
            if (
              (s[0] == 114 || s[0] == 82) &&
              (s[1] == 101 || s[1] == 69) &&
              (s[2] == 102 || s[2] == 70) &&
              (s[3] == 101 || s[3] == 69) &&
              (s[4] == 114 || s[4] == 82) &&
              (s[5] == 101 || s[5] == 69) &&
              (s[6] == 114 || s[6] == 82)
            ) {
              return "referer";
            }
          case 218:
          case 250:
          case 250:
          case 218:
            // check that the buffer is equal to refresh and return it if so
            if (
              (s[0] == 114 || s[0] == 82) &&
              (s[1] == 101 || s[1] == 69) &&
              (s[2] == 102 || s[2] == 70) &&
              (s[3] == 114 || s[3] == 82) &&
              (s[4] == 101 || s[4] == 69) &&
              (s[5] == 115 || s[5] == 83) &&
              (s[6] == 104 || s[6] == 72)
            ) {
              return "refresh";
            }
          case 230:
          case 198:
          case 230:
          case 198:
            // check that the buffer is equal to trailer and return it if so
            if (
              (s[0] == 116 || s[0] == 84) &&
              (s[1] == 114 || s[1] == 82) &&
              (s[2] == 97 || s[2] == 65) &&
              (s[3] == 105 || s[3] == 73) &&
              (s[4] == 108 || s[4] == 76) &&
              (s[5] == 101 || s[5] == 69) &&
              (s[6] == 114 || s[6] == 82)
            ) {
              return "trailer";
            }
          case 218:
          case 250:
          case 240:
          case 208:
            // check that the buffer is equal to upgrade and return it if so
            if (
              (s[0] == 117 || s[0] == 85) &&
              (s[1] == 112 || s[1] == 80) &&
              (s[2] == 103 || s[2] == 71) &&
              (s[3] == 114 || s[3] == 82) &&
              (s[4] == 97 || s[4] == 65) &&
              (s[5] == 100 || s[5] == 68) &&
              (s[6] == 101 || s[6] == 69)
            ) {
              return "upgrade";
            }
          default:
            break;
        }
        break;
      case 8:
        switch (s[3]) {
          case 45:
          case 13:
            // check that the buffer is equal to alt-used and return it if so
            if (
              (s[0] == 97 || s[0] == 65) &&
              (s[1] == 108 || s[1] == 76) &&
              (s[2] == 116 || s[2] == 84) &&
              (s[4] == 117 || s[4] == 85) &&
              (s[5] == 115 || s[5] == 83) &&
              (s[6] == 101 || s[6] == 69) &&
              (s[7] == 100 || s[7] == 68)
            ) {
              return "alt-used";
            }
            break;
          case 110:
          case 78:
            // check that the buffer is equal to downlink and return it if so
            if (
              (s[0] == 100 || s[0] == 68) &&
              (s[1] == 111 || s[1] == 79) &&
              (s[2] == 119 || s[2] == 87) &&
              (s[4] == 108 || s[4] == 76) &&
              (s[5] == 105 || s[5] == 73) &&
              (s[6] == 110 || s[6] == 78) &&
              (s[7] == 107 || s[7] == 75)
            ) {
              return "downlink";
            }
            break;
          case 109:
          case 77:
            // check that the buffer is equal to if-match and return it if so
            if (
              (s[0] == 105 || s[0] == 73) &&
              (s[1] == 102 || s[1] == 70) &&
              s[2] == 45 &&
              (s[4] == 97 || s[4] == 65) &&
              (s[5] == 116 || s[5] == 84) &&
              (s[6] == 99 || s[6] == 67) &&
              (s[7] == 104 || s[7] == 72)
            ) {
              return "if-match";
            }
            break;
          case 114:
          case 82:
            // check that the buffer is equal to if-range and return it if so
            if (
              (s[0] == 105 || s[0] == 73) &&
              (s[1] == 102 || s[1] == 70) &&
              s[2] == 45 &&
              (s[4] == 97 || s[4] == 65) &&
              (s[5] == 110 || s[5] == 78) &&
              (s[6] == 103 || s[6] == 71) &&
              (s[7] == 101 || s[7] == 69)
            ) {
              return "if-range";
            }
            break;
          case 97:
          case 65:
            // check that the buffer is equal to location and return it if so
            if (
              (s[0] == 108 || s[0] == 76) &&
              (s[1] == 111 || s[1] == 79) &&
              (s[2] == 99 || s[2] == 67) &&
              (s[4] == 116 || s[4] == 84) &&
              (s[5] == 105 || s[5] == 73) &&
              (s[6] == 111 || s[6] == 79) &&
              (s[7] == 110 || s[7] == 78)
            ) {
              return "location";
            }
            break;
          default:
            break;
        }
        break;
      case 9:
        switch (s[0]) {
          case 101:
          case 69:
            // check that the buffer is equal to expect-ct and return it if so
            if (
              (s[1] == 120 || s[1] == 88) &&
              (s[2] == 112 || s[2] == 80) &&
              (s[3] == 101 || s[3] == 69) &&
              (s[4] == 99 || s[4] == 67) &&
              (s[5] == 116 || s[5] == 84) &&
              (s[6] == 45 || s[6] == 13) &&
              (s[7] == 99 || s[7] == 67) &&
              (s[8] == 116 || s[8] == 84)
            ) {
              return "expect-ct";
            }
            break;
          case 102:
          case 70:
            // check that the buffer is equal to forwarded and return it if so
            if (
              (s[1] == 111 || s[1] == 79) &&
              (s[2] == 114 || s[2] == 82) &&
              (s[3] == 119 || s[3] == 87) &&
              (s[4] == 97 || s[4] == 65) &&
              (s[5] == 114 || s[5] == 82) &&
              (s[6] == 100 || s[6] == 68) &&
              (s[7] == 101 || s[7] == 69) &&
              (s[8] == 100 || s[8] == 68)
            ) {
              return "forwarded";
            }
            break;
          case 115:
          case 83:
            // check that the buffer is equal to sourcemap and return it if so
            if (
              (s[1] == 111 || s[1] == 79) &&
              (s[2] == 117 || s[2] == 85) &&
              (s[3] == 114 || s[3] == 82) &&
              (s[4] == 99 || s[4] == 67) &&
              (s[5] == 101 || s[5] == 69) &&
              (s[6] == 109 || s[6] == 77) &&
              (s[7] == 97 || s[7] == 65) &&
              (s[8] == 112 || s[8] == 80)
            ) {
              return "sourcemap";
            }
            break;
          default:
            break;
        }
        break;
      case 10:
        switch (s[0]) {
          case 99:
          case 67:
            // check that the buffer is equal to connection and return it if so
            if (
              (s[1] == 111 || s[1] == 79) &&
              (s[2] == 110 || s[2] == 78) &&
              (s[3] == 110 || s[3] == 78) &&
              (s[4] == 101 || s[4] == 69) &&
              (s[5] == 99 || s[5] == 67) &&
              (s[6] == 116 || s[6] == 84) &&
              (s[7] == 105 || s[7] == 73) &&
              (s[8] == 111 || s[8] == 79) &&
              (s[9] == 110 || s[9] == 78)
            ) {
              return "connection";
            }
            break;
          case 107:
          case 75:
            // check that the buffer is equal to keep-alive and return it if so
            if (
              (s[1] == 101 || s[1] == 69) &&
              (s[2] == 101 || s[2] == 69) &&
              (s[3] == 112 || s[3] == 80) &&
              (s[4] == 45 || s[4] == 13) &&
              (s[5] == 97 || s[5] == 65) &&
              (s[6] == 108 || s[6] == 76) &&
              (s[7] == 105 || s[7] == 73) &&
              (s[8] == 118 || s[8] == 86) &&
              (s[9] == 101 || s[9] == 69)
            ) {
              return "keep-alive";
            }
            break;
          case 115:
          case 83:
            // check that the buffer is equal to set-cookie and return it if so
            if (
              (s[1] == 101 || s[1] == 69) &&
              (s[2] == 116 || s[2] == 84) &&
              (s[3] == 45 || s[3] == 13) &&
              (s[4] == 99 || s[4] == 67) &&
              (s[5] == 111 || s[5] == 79) &&
              (s[6] == 111 || s[6] == 79) &&
              (s[7] == 107 || s[7] == 75) &&
              (s[8] == 105 || s[8] == 73) &&
              (s[9] == 101 || s[9] == 69)
            ) {
              return "set-cookie";
            }
            break;
          case 117:
          case 85:
            // check that the buffer is equal to user-agent and return it if so
            if (
              (s[1] == 115 || s[1] == 83) &&
              (s[2] == 101 || s[2] == 69) &&
              (s[3] == 114 || s[3] == 82) &&
              (s[4] == 45 || s[4] == 13) &&
              (s[5] == 97 || s[5] == 65) &&
              (s[6] == 103 || s[6] == 71) &&
              (s[7] == 101 || s[7] == 69) &&
              (s[8] == 110 || s[8] == 78) &&
              (s[9] == 116 || s[9] == 84)
            ) {
              return "user-agent";
            }
            break;
          default:
            break;
        }
        break;
      case 11:
        // check that the buffer is equal to retry-after and return it if so
        if (
          (s[0] == 114 || s[0] == 82) &&
          (s[1] == 101 || s[1] == 69) &&
          (s[2] == 116 || s[2] == 84) &&
          (s[3] == 114 || s[3] == 82) &&
          (s[4] == 121 || s[4] == 89) &&
          s[5] == 45 &&
          (s[6] == 97 || s[6] == 65) &&
          (s[7] == 102 || s[7] == 70) &&
          (s[8] == 116 || s[8] == 84) &&
          (s[9] == 101 || s[9] == 69) &&
          (s[10] == 114 || s[10] == 82)
        ) {
          return "retry-after";
        }
        break;
      case 12:
        switch (s[0]) {
          case 99:
          case 67:
            // check that the buffer is equal to content-type and return it if so
            if (
              (s[1] == 111 || s[1] == 79) &&
              (s[2] == 110 || s[2] == 78) &&
              (s[3] == 116 || s[3] == 84) &&
              (s[4] == 101 || s[4] == 69) &&
              (s[5] == 110 || s[5] == 78) &&
              (s[6] == 116 || s[6] == 84) &&
              (s[7] == 45 || s[7] == 13) &&
              (s[8] == 116 || s[8] == 84) &&
              (s[9] == 121 || s[9] == 89) &&
              (s[10] == 112 || s[10] == 80) &&
              (s[11] == 101 || s[11] == 69)
            ) {
              return "content-type";
            }
            break;
          case 109:
          case 77:
            // check that the buffer is equal to max-forwards and return it if so
            if (
              (s[1] == 97 || s[1] == 65) &&
              (s[2] == 120 || s[2] == 88) &&
              (s[3] == 45 || s[3] == 13) &&
              (s[4] == 102 || s[4] == 70) &&
              (s[5] == 111 || s[5] == 79) &&
              (s[6] == 114 || s[6] == 82) &&
              (s[7] == 119 || s[7] == 87) &&
              (s[8] == 97 || s[8] == 65) &&
              (s[9] == 114 || s[9] == 82) &&
              (s[10] == 100 || s[10] == 68) &&
              (s[11] == 115 || s[11] == 83)
            ) {
              return "max-forwards";
            }
            break;
          case 120:
          case 88:
            // check that the buffer is equal to x-powered-by and return it if so
            if (
              (s[1] == 45 || s[1] == 13) &&
              (s[2] == 112 || s[2] == 80) &&
              (s[3] == 111 || s[3] == 79) &&
              (s[4] == 119 || s[4] == 87) &&
              (s[5] == 101 || s[5] == 69) &&
              (s[6] == 114 || s[6] == 82) &&
              (s[7] == 101 || s[7] == 69) &&
              (s[8] == 100 || s[8] == 68) &&
              (s[9] == 45 || s[9] == 13) &&
              (s[10] == 98 || s[10] == 66) &&
              (s[11] == 121 || s[11] == 89)
            ) {
              return "x-powered-by";
            }
            break;
          default:
            break;
        }
        break;
      case 13:
        switch (s[12]) {
          case 115:
          case 83:
            // check that the buffer is equal to accept-ranges and return it if so
            if (
              (s[0] == 97 || s[0] == 65) &&
              (s[1] == 99 || s[1] == 67) &&
              (s[2] == 99 || s[2] == 67) &&
              (s[3] == 101 || s[3] == 69) &&
              (s[4] == 112 || s[4] == 80) &&
              (s[5] == 116 || s[5] == 84) &&
              s[6] == 45 &&
              (s[7] == 114 || s[7] == 82) &&
              (s[8] == 97 || s[8] == 65) &&
              (s[9] == 110 || s[9] == 78) &&
              (s[10] == 103 || s[10] == 71) &&
              (s[11] == 101 || s[11] == 69)
            ) {
              return "accept-ranges";
            }
            break;
          case 110:
          case 78:
            // check that the buffer is equal to authorization and return it if so
            if (
              (s[0] == 97 || s[0] == 65) &&
              (s[1] == 117 || s[1] == 85) &&
              (s[2] == 116 || s[2] == 84) &&
              (s[3] == 104 || s[3] == 72) &&
              (s[4] == 111 || s[4] == 79) &&
              (s[5] == 114 || s[5] == 82) &&
              (s[6] == 105 || s[6] == 73) &&
              (s[7] == 122 || s[7] == 90) &&
              (s[8] == 97 || s[8] == 65) &&
              (s[9] == 116 || s[9] == 84) &&
              (s[10] == 105 || s[10] == 73) &&
              (s[11] == 111 || s[11] == 79)
            ) {
              return "authorization";
            }
            break;
          case 108:
          case 76:
            // check that the buffer is equal to cache-control and return it if so
            if (
              (s[0] == 99 || s[0] == 67) &&
              (s[1] == 97 || s[1] == 65) &&
              (s[2] == 99 || s[2] == 67) &&
              (s[3] == 104 || s[3] == 72) &&
              (s[4] == 101 || s[4] == 69) &&
              s[5] == 45 &&
              (s[6] == 99 || s[6] == 67) &&
              (s[7] == 111 || s[7] == 79) &&
              (s[8] == 110 || s[8] == 78) &&
              (s[9] == 116 || s[9] == 84) &&
              (s[10] == 114 || s[10] == 82) &&
              (s[11] == 111 || s[11] == 79)
            ) {
              return "cache-control";
            }
            break;
          case 101:
          case 69:
            // check that the buffer is equal to content-range and return it if so
            if (
              (s[0] == 99 || s[0] == 67) &&
              (s[1] == 111 || s[1] == 79) &&
              (s[2] == 110 || s[2] == 78) &&
              (s[3] == 116 || s[3] == 84) &&
              (s[4] == 101 || s[4] == 69) &&
              (s[5] == 110 || s[5] == 78) &&
              (s[6] == 116 || s[6] == 84) &&
              s[7] == 45 &&
              (s[8] == 114 || s[8] == 82) &&
              (s[9] == 97 || s[9] == 65) &&
              (s[10] == 110 || s[10] == 78) &&
              (s[11] == 103 || s[11] == 71)
            ) {
              return "content-range";
            }
            break;
          case 121:
          case 89:
            // check that the buffer is equal to device-memory and return it if so
            if (
              (s[0] == 100 || s[0] == 68) &&
              (s[1] == 101 || s[1] == 69) &&
              (s[2] == 118 || s[2] == 86) &&
              (s[3] == 105 || s[3] == 73) &&
              (s[4] == 99 || s[4] == 67) &&
              (s[5] == 101 || s[5] == 69) &&
              s[6] == 45 &&
              (s[7] == 109 || s[7] == 77) &&
              (s[8] == 101 || s[8] == 69) &&
              (s[9] == 109 || s[9] == 77) &&
              (s[10] == 111 || s[10] == 79) &&
              (s[11] == 114 || s[11] == 82)
            ) {
              return "device-memory";
            }
            break;
          case 104:
          case 72:
            // check that the buffer is equal to if-none-match and return it if so
            if (
              (s[0] == 105 || s[0] == 73) &&
              (s[1] == 102 || s[1] == 70) &&
              s[2] == 45 &&
              (s[3] == 110 || s[3] == 78) &&
              (s[4] == 111 || s[4] == 79) &&
              (s[5] == 110 || s[5] == 78) &&
              (s[6] == 101 || s[6] == 69) &&
              s[7] == 45 &&
              (s[8] == 109 || s[8] == 77) &&
              (s[9] == 97 || s[9] == 65) &&
              (s[10] == 116 || s[10] == 84) &&
              (s[11] == 99 || s[11] == 67)
            ) {
              return "if-none-match";
            }
            break;
          case 100:
          case 68:
            // check that the buffer is equal to last-modified and return it if so
            if (
              (s[0] == 108 || s[0] == 76) &&
              (s[1] == 97 || s[1] == 65) &&
              (s[2] == 115 || s[2] == 83) &&
              (s[3] == 116 || s[3] == 84) &&
              s[4] == 45 &&
              (s[5] == 109 || s[5] == 77) &&
              (s[6] == 111 || s[6] == 79) &&
              (s[7] == 100 || s[7] == 68) &&
              (s[8] == 105 || s[8] == 73) &&
              (s[9] == 102 || s[9] == 70) &&
              (s[10] == 105 || s[10] == 73) &&
              (s[11] == 101 || s[11] == 69)
            ) {
              return "last-modified";
            }
            break;
          case 103:
          case 71:
            // check that the buffer is equal to server-timing and return it if so
            if (
              (s[0] == 115 || s[0] == 83) &&
              (s[1] == 101 || s[1] == 69) &&
              (s[2] == 114 || s[2] == 82) &&
              (s[3] == 118 || s[3] == 86) &&
              (s[4] == 101 || s[4] == 69) &&
              (s[5] == 114 || s[5] == 82) &&
              s[6] == 45 &&
              (s[7] == 116 || s[7] == 84) &&
              (s[8] == 105 || s[8] == 73) &&
              (s[9] == 109 || s[9] == 77) &&
              (s[10] == 105 || s[10] == 73) &&
              (s[11] == 110 || s[11] == 78)
            ) {
              return "server-timing";
            }
            break;
          default:
            break;
        }
        break;
      case 14:
        // check that the buffer is equal to content-length and return it if so
        if (
          (s[0] == 99 || s[0] == 67) &&
          (s[1] == 111 || s[1] == 79) &&
          (s[2] == 110 || s[2] == 78) &&
          (s[3] == 116 || s[3] == 84) &&
          (s[4] == 101 || s[4] == 69) &&
          (s[5] == 110 || s[5] == 78) &&
          (s[6] == 116 || s[6] == 84) &&
          s[7] == 45 &&
          (s[8] == 108 || s[8] == 76) &&
          (s[9] == 101 || s[9] == 69) &&
          (s[10] == 110 || s[10] == 78) &&
          (s[11] == 103 || s[11] == 71) &&
          (s[12] == 116 || s[12] == 84) &&
          (s[13] == 104 || s[13] == 72)
        ) {
          return "content-length";
        }
        break;
      case 15:
        switch (s[7]) {
          case 101:
          case 69:
            // check that the buffer is equal to accept-encoding and return it if so
            if (
              (s[0] == 97 || s[0] == 65) &&
              (s[1] == 99 || s[1] == 67) &&
              (s[2] == 99 || s[2] == 67) &&
              (s[3] == 101 || s[3] == 69) &&
              (s[4] == 112 || s[4] == 80) &&
              (s[5] == 116 || s[5] == 84) &&
              s[6] == 45 &&
              (s[8] == 110 || s[8] == 78) &&
              (s[9] == 99 || s[9] == 67) &&
              (s[10] == 111 || s[10] == 79) &&
              (s[11] == 100 || s[11] == 68) &&
              (s[12] == 105 || s[12] == 73) &&
              (s[13] == 110 || s[13] == 78) &&
              (s[14] == 103 || s[14] == 71)
            ) {
              return "accept-encoding";
            }
            break;
          case 108:
          case 76:
            // check that the buffer is equal to accept-language and return it if so
            if (
              (s[0] == 97 || s[0] == 65) &&
              (s[1] == 99 || s[1] == 67) &&
              (s[2] == 99 || s[2] == 67) &&
              (s[3] == 101 || s[3] == 69) &&
              (s[4] == 112 || s[4] == 80) &&
              (s[5] == 116 || s[5] == 84) &&
              s[6] == 45 &&
              (s[8] == 97 || s[8] == 65) &&
              (s[9] == 110 || s[9] == 78) &&
              (s[10] == 103 || s[10] == 71) &&
              (s[11] == 117 || s[11] == 85) &&
              (s[12] == 97 || s[12] == 65) &&
              (s[13] == 103 || s[13] == 71) &&
              (s[14] == 101 || s[14] == 69)
            ) {
              return "accept-language";
            }
            break;
          case 105:
          case 73:
            // check that the buffer is equal to clear-site-data and return it if so
            if (
              (s[0] == 99 || s[0] == 67) &&
              (s[1] == 108 || s[1] == 76) &&
              (s[2] == 101 || s[2] == 69) &&
              (s[3] == 97 || s[3] == 65) &&
              (s[4] == 114 || s[4] == 82) &&
              s[5] == 45 &&
              (s[6] == 115 || s[6] == 83) &&
              (s[8] == 116 || s[8] == 84) &&
              (s[9] == 101 || s[9] == 69) &&
              s[10] == 45 &&
              (s[11] == 100 || s[11] == 68) &&
              (s[12] == 97 || s[12] == 65) &&
              (s[13] == 116 || s[13] == 84) &&
              (s[14] == 97 || s[14] == 65)
            ) {
              return "clear-site-data";
            }
            break;
          case 114:
          case 82:
            // check that the buffer is equal to referrer-policy and return it if so
            if (
              (s[0] == 114 || s[0] == 82) &&
              (s[1] == 101 || s[1] == 69) &&
              (s[2] == 102 || s[2] == 70) &&
              (s[3] == 101 || s[3] == 69) &&
              (s[4] == 114 || s[4] == 82) &&
              (s[5] == 114 || s[5] == 82) &&
              (s[6] == 101 || s[6] == 69) &&
              s[8] == 45 &&
              (s[9] == 112 || s[9] == 80) &&
              (s[10] == 111 || s[10] == 79) &&
              (s[11] == 108 || s[11] == 76) &&
              (s[12] == 105 || s[12] == 73) &&
              (s[13] == 99 || s[13] == 67) &&
              (s[14] == 121 || s[14] == 89)
            ) {
              return "referrer-policy";
            }
            break;
          case 45:
          case 13:
            // check that the buffer is equal to x-frame-options and return it if so
            if (
              (s[0] == 120 || s[0] == 88) &&
              s[1] == 45 &&
              (s[2] == 102 || s[2] == 70) &&
              (s[3] == 114 || s[3] == 82) &&
              (s[4] == 97 || s[4] == 65) &&
              (s[5] == 109 || s[5] == 77) &&
              (s[6] == 101 || s[6] == 69) &&
              (s[8] == 111 || s[8] == 79) &&
              (s[9] == 112 || s[9] == 80) &&
              (s[10] == 116 || s[10] == 84) &&
              (s[11] == 105 || s[11] == 73) &&
              (s[12] == 111 || s[12] == 79) &&
              (s[13] == 110 || s[13] == 78) &&
              (s[14] == 115 || s[14] == 83)
            ) {
              return "x-frame-options";
            }
            break;
          default:
            break;
        }
        break;
      case 16:
        switch (s[11]) {
          case 111:
          case 79:
            // check that the buffer is equal to content-encoding and return it if so
            if (
              (s[0] == 99 || s[0] == 67) &&
              (s[1] == 111 || s[1] == 79) &&
              (s[2] == 110 || s[2] == 78) &&
              (s[3] == 116 || s[3] == 84) &&
              (s[4] == 101 || s[4] == 69) &&
              (s[5] == 110 || s[5] == 78) &&
              (s[6] == 116 || s[6] == 84) &&
              s[7] == 45 &&
              (s[8] == 101 || s[8] == 69) &&
              (s[9] == 110 || s[9] == 78) &&
              (s[10] == 99 || s[10] == 67) &&
              (s[12] == 100 || s[12] == 68) &&
              (s[13] == 105 || s[13] == 73) &&
              (s[14] == 110 || s[14] == 78) &&
              (s[15] == 103 || s[15] == 71)
            ) {
              return "content-encoding";
            }
            break;
          case 103:
          case 71:
            // check that the buffer is equal to content-language and return it if so
            if (
              (s[0] == 99 || s[0] == 67) &&
              (s[1] == 111 || s[1] == 79) &&
              (s[2] == 110 || s[2] == 78) &&
              (s[3] == 116 || s[3] == 84) &&
              (s[4] == 101 || s[4] == 69) &&
              (s[5] == 110 || s[5] == 78) &&
              (s[6] == 116 || s[6] == 84) &&
              s[7] == 45 &&
              (s[8] == 108 || s[8] == 76) &&
              (s[9] == 97 || s[9] == 65) &&
              (s[10] == 110 || s[10] == 78) &&
              (s[12] == 117 || s[12] == 85) &&
              (s[13] == 97 || s[13] == 65) &&
              (s[14] == 103 || s[14] == 71) &&
              (s[15] == 101 || s[15] == 69)
            ) {
              return "content-language";
            }
            break;
          case 97:
          case 65:
            // check that the buffer is equal to content-location and return it if so
            if (
              (s[0] == 99 || s[0] == 67) &&
              (s[1] == 111 || s[1] == 79) &&
              (s[2] == 110 || s[2] == 78) &&
              (s[3] == 116 || s[3] == 84) &&
              (s[4] == 101 || s[4] == 69) &&
              (s[5] == 110 || s[5] == 78) &&
              (s[6] == 116 || s[6] == 84) &&
              s[7] == 45 &&
              (s[8] == 108 || s[8] == 76) &&
              (s[9] == 111 || s[9] == 79) &&
              (s[10] == 99 || s[10] == 67) &&
              (s[12] == 116 || s[12] == 84) &&
              (s[13] == 105 || s[13] == 73) &&
              (s[14] == 111 || s[14] == 79) &&
              (s[15] == 110 || s[15] == 78)
            ) {
              return "content-location";
            }
            break;
          case 105:
          case 73:
            // check that the buffer is equal to www-authenticate and return it if so
            if (
              (s[0] == 119 || s[0] == 87) &&
              (s[1] == 119 || s[1] == 87) &&
              (s[2] == 119 || s[2] == 87) &&
              s[3] == 45 &&
              (s[4] == 97 || s[4] == 65) &&
              (s[5] == 117 || s[5] == 85) &&
              (s[6] == 116 || s[6] == 84) &&
              (s[7] == 104 || s[7] == 72) &&
              (s[8] == 101 || s[8] == 69) &&
              (s[9] == 110 || s[9] == 78) &&
              (s[10] == 116 || s[10] == 84) &&
              (s[12] == 99 || s[12] == 67) &&
              (s[13] == 97 || s[13] == 65) &&
              (s[14] == 116 || s[14] == 84) &&
              (s[15] == 101 || s[15] == 69)
            ) {
              return "www-authenticate";
            }
            break;
          case 45:
          case 13:
            // check that the buffer is equal to x-requested-with and return it if so
            if (
              (s[0] == 120 || s[0] == 88) &&
              s[1] == 45 &&
              (s[2] == 114 || s[2] == 82) &&
              (s[3] == 101 || s[3] == 69) &&
              (s[4] == 113 || s[4] == 81) &&
              (s[5] == 117 || s[5] == 85) &&
              (s[6] == 101 || s[6] == 69) &&
              (s[7] == 115 || s[7] == 83) &&
              (s[8] == 116 || s[8] == 84) &&
              (s[9] == 101 || s[9] == 69) &&
              (s[10] == 100 || s[10] == 68) &&
              (s[12] == 119 || s[12] == 87) &&
              (s[13] == 105 || s[13] == 73) &&
              (s[14] == 116 || s[14] == 84) &&
              (s[15] == 104 || s[15] == 72)
            ) {
              return "x-requested-with";
            }
            break;
          case 99:
          case 67:
            // check that the buffer is equal to x-xss-protection and return it if so
            if (
              (s[0] == 120 || s[0] == 88) &&
              s[1] == 45 &&
              (s[2] == 120 || s[2] == 88) &&
              (s[3] == 115 || s[3] == 83) &&
              (s[4] == 115 || s[4] == 83) &&
              s[5] == 45 &&
              (s[6] == 112 || s[6] == 80) &&
              (s[7] == 114 || s[7] == 82) &&
              (s[8] == 111 || s[8] == 79) &&
              (s[9] == 116 || s[9] == 84) &&
              (s[10] == 101 || s[10] == 69) &&
              (s[12] == 116 || s[12] == 84) &&
              (s[13] == 105 || s[13] == 73) &&
              (s[14] == 111 || s[14] == 79) &&
              (s[15] == 110 || s[15] == 78)
            ) {
              return "x-xss-protection";
            }
            break;
          default:
            break;
        }
        break;
      case 17:
        switch (s[0]) {
          case 105:
          case 73:
            // check that the buffer is equal to if-modified-since and return it if so
            if (
              (s[1] == 102 || s[1] == 70) &&
              (s[2] == 45 || s[2] == 13) &&
              (s[3] == 109 || s[3] == 77) &&
              (s[4] == 111 || s[4] == 79) &&
              (s[5] == 100 || s[5] == 68) &&
              (s[6] == 105 || s[6] == 73) &&
              (s[7] == 102 || s[7] == 70) &&
              (s[8] == 105 || s[8] == 73) &&
              (s[9] == 101 || s[9] == 69) &&
              (s[10] == 100 || s[10] == 68) &&
              (s[11] == 45 || s[11] == 13) &&
              (s[12] == 115 || s[12] == 83) &&
              (s[13] == 105 || s[13] == 73) &&
              (s[14] == 110 || s[14] == 78) &&
              (s[15] == 99 || s[15] == 67) &&
              (s[16] == 101 || s[16] == 69)
            ) {
              return "if-modified-since";
            }
            break;
          case 115:
          case 83:
            // check that the buffer is equal to sec-websocket-key and return it if so
            if (
              (s[1] == 101 || s[1] == 69) &&
              (s[2] == 99 || s[2] == 67) &&
              (s[3] == 45 || s[3] == 13) &&
              (s[4] == 119 || s[4] == 87) &&
              (s[5] == 101 || s[5] == 69) &&
              (s[6] == 98 || s[6] == 66) &&
              (s[7] == 115 || s[7] == 83) &&
              (s[8] == 111 || s[8] == 79) &&
              (s[9] == 99 || s[9] == 67) &&
              (s[10] == 107 || s[10] == 75) &&
              (s[11] == 101 || s[11] == 69) &&
              (s[12] == 116 || s[12] == 84) &&
              (s[13] == 45 || s[13] == 13) &&
              (s[14] == 107 || s[14] == 75) &&
              (s[15] == 101 || s[15] == 69) &&
              (s[16] == 121 || s[16] == 89)
            ) {
              return "sec-websocket-key";
            }
            break;
          case 116:
          case 84:
            // check that the buffer is equal to transfer-encoding and return it if so
            if (
              (s[1] == 114 || s[1] == 82) &&
              (s[2] == 97 || s[2] == 65) &&
              (s[3] == 110 || s[3] == 78) &&
              (s[4] == 115 || s[4] == 83) &&
              (s[5] == 102 || s[5] == 70) &&
              (s[6] == 101 || s[6] == 69) &&
              (s[7] == 114 || s[7] == 82) &&
              (s[8] == 45 || s[8] == 13) &&
              (s[9] == 101 || s[9] == 69) &&
              (s[10] == 110 || s[10] == 78) &&
              (s[11] == 99 || s[11] == 67) &&
              (s[12] == 111 || s[12] == 79) &&
              (s[13] == 100 || s[13] == 68) &&
              (s[14] == 105 || s[14] == 73) &&
              (s[15] == 110 || s[15] == 78) &&
              (s[16] == 103 || s[16] == 71)
            ) {
              return "transfer-encoding";
            }
            break;
          default:
            break;
        }
        break;
      case 18:
        switch (s[1]) {
          case 101:
          case 69:
            // check that the buffer is equal to permissions-policy and return it if so
            if (
              (s[0] == 112 || s[0] == 80) &&
              (s[2] == 114 || s[2] == 82) &&
              (s[3] == 109 || s[3] == 77) &&
              (s[4] == 105 || s[4] == 73) &&
              (s[5] == 115 || s[5] == 83) &&
              (s[6] == 115 || s[6] == 83) &&
              (s[7] == 105 || s[7] == 73) &&
              (s[8] == 111 || s[8] == 79) &&
              (s[9] == 110 || s[9] == 78) &&
              (s[10] == 115 || s[10] == 83) &&
              s[11] == 45 &&
              (s[12] == 112 || s[12] == 80) &&
              (s[13] == 111 || s[13] == 79) &&
              (s[14] == 108 || s[14] == 76) &&
              (s[15] == 105 || s[15] == 73) &&
              (s[16] == 99 || s[16] == 67) &&
              (s[17] == 121 || s[17] == 89)
            ) {
              return "permissions-policy";
            }
            break;
          case 114:
          case 82:
            // check that the buffer is equal to proxy-authenticate and return it if so
            if (
              (s[0] == 112 || s[0] == 80) &&
              (s[2] == 111 || s[2] == 79) &&
              (s[3] == 120 || s[3] == 88) &&
              (s[4] == 121 || s[4] == 89) &&
              s[5] == 45 &&
              (s[6] == 97 || s[6] == 65) &&
              (s[7] == 117 || s[7] == 85) &&
              (s[8] == 116 || s[8] == 84) &&
              (s[9] == 104 || s[9] == 72) &&
              (s[10] == 101 || s[10] == 69) &&
              (s[11] == 110 || s[11] == 78) &&
              (s[12] == 116 || s[12] == 84) &&
              (s[13] == 105 || s[13] == 73) &&
              (s[14] == 99 || s[14] == 67) &&
              (s[15] == 97 || s[15] == 65) &&
              (s[16] == 116 || s[16] == 84) &&
              (s[17] == 101 || s[17] == 69)
            ) {
              return "proxy-authenticate";
            }
            break;
          default:
            break;
        }
        break;
      case 19:
        switch (s[0]) {
          case 99:
          case 67:
            // check that the buffer is equal to content-disposition and return it if so
            if (
              (s[1] == 111 || s[1] == 79) &&
              (s[2] == 110 || s[2] == 78) &&
              (s[3] == 116 || s[3] == 84) &&
              (s[4] == 101 || s[4] == 69) &&
              (s[5] == 110 || s[5] == 78) &&
              (s[6] == 116 || s[6] == 84) &&
              (s[7] == 45 || s[7] == 13) &&
              (s[8] == 100 || s[8] == 68) &&
              (s[9] == 105 || s[9] == 73) &&
              (s[10] == 115 || s[10] == 83) &&
              (s[11] == 112 || s[11] == 80) &&
              (s[12] == 111 || s[12] == 79) &&
              (s[13] == 115 || s[13] == 83) &&
              (s[14] == 105 || s[14] == 73) &&
              (s[15] == 116 || s[15] == 84) &&
              (s[16] == 105 || s[16] == 73) &&
              (s[17] == 111 || s[17] == 79) &&
              (s[18] == 110 || s[18] == 78)
            ) {
              return "content-disposition";
            }
            break;
          case 105:
          case 73:
            // check that the buffer is equal to if-unmodified-since and return it if so
            if (
              (s[1] == 102 || s[1] == 70) &&
              (s[2] == 45 || s[2] == 13) &&
              (s[3] == 117 || s[3] == 85) &&
              (s[4] == 110 || s[4] == 78) &&
              (s[5] == 109 || s[5] == 77) &&
              (s[6] == 111 || s[6] == 79) &&
              (s[7] == 100 || s[7] == 68) &&
              (s[8] == 105 || s[8] == 73) &&
              (s[9] == 102 || s[9] == 70) &&
              (s[10] == 105 || s[10] == 73) &&
              (s[11] == 101 || s[11] == 69) &&
              (s[12] == 100 || s[12] == 68) &&
              (s[13] == 45 || s[13] == 13) &&
              (s[14] == 115 || s[14] == 83) &&
              (s[15] == 105 || s[15] == 73) &&
              (s[16] == 110 || s[16] == 78) &&
              (s[17] == 99 || s[17] == 67) &&
              (s[18] == 101 || s[18] == 69)
            ) {
              return "if-unmodified-since";
            }
            break;
          case 112:
          case 80:
            // check that the buffer is equal to proxy-authorization and return it if so
            if (
              (s[1] == 114 || s[1] == 82) &&
              (s[2] == 111 || s[2] == 79) &&
              (s[3] == 120 || s[3] == 88) &&
              (s[4] == 121 || s[4] == 89) &&
              (s[5] == 45 || s[5] == 13) &&
              (s[6] == 97 || s[6] == 65) &&
              (s[7] == 117 || s[7] == 85) &&
              (s[8] == 116 || s[8] == 84) &&
              (s[9] == 104 || s[9] == 72) &&
              (s[10] == 111 || s[10] == 79) &&
              (s[11] == 114 || s[11] == 82) &&
              (s[12] == 105 || s[12] == 73) &&
              (s[13] == 122 || s[13] == 90) &&
              (s[14] == 97 || s[14] == 65) &&
              (s[15] == 116 || s[15] == 84) &&
              (s[16] == 105 || s[16] == 73) &&
              (s[17] == 111 || s[17] == 79) &&
              (s[18] == 110 || s[18] == 78)
            ) {
              return "proxy-authorization";
            }
            break;
          case 116:
          case 84:
            // check that the buffer is equal to timing-allow-origin and return it if so
            if (
              (s[1] == 105 || s[1] == 73) &&
              (s[2] == 109 || s[2] == 77) &&
              (s[3] == 105 || s[3] == 73) &&
              (s[4] == 110 || s[4] == 78) &&
              (s[5] == 103 || s[5] == 71) &&
              (s[6] == 45 || s[6] == 13) &&
              (s[7] == 97 || s[7] == 65) &&
              (s[8] == 108 || s[8] == 76) &&
              (s[9] == 108 || s[9] == 76) &&
              (s[10] == 111 || s[10] == 79) &&
              (s[11] == 119 || s[11] == 87) &&
              (s[12] == 45 || s[12] == 13) &&
              (s[13] == 111 || s[13] == 79) &&
              (s[14] == 114 || s[14] == 82) &&
              (s[15] == 105 || s[15] == 73) &&
              (s[16] == 103 || s[16] == 71) &&
              (s[17] == 105 || s[17] == 73) &&
              (s[18] == 110 || s[18] == 78)
            ) {
              return "timing-allow-origin";
            }
            break;
          default:
            break;
        }
        break;
      case 20:
        // check that the buffer is equal to sec-websocket-accept and return it if so
        if (
          (s[0] == 115 || s[0] == 83) &&
          (s[1] == 101 || s[1] == 69) &&
          (s[2] == 99 || s[2] == 67) &&
          s[3] == 45 &&
          (s[4] == 119 || s[4] == 87) &&
          (s[5] == 101 || s[5] == 69) &&
          (s[6] == 98 || s[6] == 66) &&
          (s[7] == 115 || s[7] == 83) &&
          (s[8] == 111 || s[8] == 79) &&
          (s[9] == 99 || s[9] == 67) &&
          (s[10] == 107 || s[10] == 75) &&
          (s[11] == 101 || s[11] == 69) &&
          (s[12] == 116 || s[12] == 84) &&
          s[13] == 45 &&
          (s[14] == 97 || s[14] == 65) &&
          (s[15] == 99 || s[15] == 67) &&
          (s[16] == 99 || s[16] == 67) &&
          (s[17] == 101 || s[17] == 69) &&
          (s[18] == 112 || s[18] == 80) &&
          (s[19] == 116 || s[19] == 84)
        ) {
          return "sec-websocket-accept";
        }
        break;
      case 21:
        switch (s[1]) {
          case 101:
          case 69:
            // check that the buffer is equal to sec-websocket-version and return it if so
            if (
              (s[0] == 115 || s[0] == 83) &&
              (s[2] == 99 || s[2] == 67) &&
              s[3] == 45 &&
              (s[4] == 119 || s[4] == 87) &&
              (s[5] == 101 || s[5] == 69) &&
              (s[6] == 98 || s[6] == 66) &&
              (s[7] == 115 || s[7] == 83) &&
              (s[8] == 111 || s[8] == 79) &&
              (s[9] == 99 || s[9] == 67) &&
              (s[10] == 107 || s[10] == 75) &&
              (s[11] == 101 || s[11] == 69) &&
              (s[12] == 116 || s[12] == 84) &&
              s[13] == 45 &&
              (s[14] == 118 || s[14] == 86) &&
              (s[15] == 101 || s[15] == 69) &&
              (s[16] == 114 || s[16] == 82) &&
              (s[17] == 115 || s[17] == 83) &&
              (s[18] == 105 || s[18] == 73) &&
              (s[19] == 111 || s[19] == 79) &&
              (s[20] == 110 || s[20] == 78)
            ) {
              return "sec-websocket-version";
            }
            break;
          case 117:
          case 85:
            // check that the buffer is equal to supports-loading-mode and return it if so
            if (
              (s[0] == 115 || s[0] == 83) &&
              (s[2] == 112 || s[2] == 80) &&
              (s[3] == 112 || s[3] == 80) &&
              (s[4] == 111 || s[4] == 79) &&
              (s[5] == 114 || s[5] == 82) &&
              (s[6] == 116 || s[6] == 84) &&
              (s[7] == 115 || s[7] == 83) &&
              s[8] == 45 &&
              (s[9] == 108 || s[9] == 76) &&
              (s[10] == 111 || s[10] == 79) &&
              (s[11] == 97 || s[11] == 65) &&
              (s[12] == 100 || s[12] == 68) &&
              (s[13] == 105 || s[13] == 73) &&
              (s[14] == 110 || s[14] == 78) &&
              (s[15] == 103 || s[15] == 71) &&
              s[16] == 45 &&
              (s[17] == 109 || s[17] == 77) &&
              (s[18] == 111 || s[18] == 79) &&
              (s[19] == 100 || s[19] == 68) &&
              (s[20] == 101 || s[20] == 69)
            ) {
              return "supports-loading-mode";
            }
            break;
          default:
            break;
        }
        break;
      case 22:
        switch (s[3]) {
          case 101:
          case 69:
            // check that the buffer is equal to access-control-max-age and return it if so
            if (
              (s[0] == 97 || s[0] == 65) &&
              (s[1] == 99 || s[1] == 67) &&
              (s[2] == 99 || s[2] == 67) &&
              (s[4] == 115 || s[4] == 83) &&
              (s[5] == 115 || s[5] == 83) &&
              s[6] == 45 &&
              (s[7] == 99 || s[7] == 67) &&
              (s[8] == 111 || s[8] == 79) &&
              (s[9] == 110 || s[9] == 78) &&
              (s[10] == 116 || s[10] == 84) &&
              (s[11] == 114 || s[11] == 82) &&
              (s[12] == 111 || s[12] == 79) &&
              (s[13] == 108 || s[13] == 76) &&
              s[14] == 45 &&
              (s[15] == 109 || s[15] == 77) &&
              (s[16] == 97 || s[16] == 65) &&
              (s[17] == 120 || s[17] == 88) &&
              s[18] == 45 &&
              (s[19] == 97 || s[19] == 65) &&
              (s[20] == 103 || s[20] == 71) &&
              (s[21] == 101 || s[21] == 69)
            ) {
              return "access-control-max-age";
            }
            break;
          case 45:
          case 13:
            // check that the buffer is equal to sec-websocket-protocol and return it if so
            if (
              (s[0] == 115 || s[0] == 83) &&
              (s[1] == 101 || s[1] == 69) &&
              (s[2] == 99 || s[2] == 67) &&
              (s[4] == 119 || s[4] == 87) &&
              (s[5] == 101 || s[5] == 69) &&
              (s[6] == 98 || s[6] == 66) &&
              (s[7] == 115 || s[7] == 83) &&
              (s[8] == 111 || s[8] == 79) &&
              (s[9] == 99 || s[9] == 67) &&
              (s[10] == 107 || s[10] == 75) &&
              (s[11] == 101 || s[11] == 69) &&
              (s[12] == 116 || s[12] == 84) &&
              s[13] == 45 &&
              (s[14] == 112 || s[14] == 80) &&
              (s[15] == 114 || s[15] == 82) &&
              (s[16] == 111 || s[16] == 79) &&
              (s[17] == 116 || s[17] == 84) &&
              (s[18] == 111 || s[18] == 79) &&
              (s[19] == 99 || s[19] == 67) &&
              (s[20] == 111 || s[20] == 79) &&
              (s[21] == 108 || s[21] == 76)
            ) {
              return "sec-websocket-protocol";
            }
            break;
          case 118:
          case 86:
            // check that the buffer is equal to service-worker-allowed and return it if so
            if (
              (s[0] == 115 || s[0] == 83) &&
              (s[1] == 101 || s[1] == 69) &&
              (s[2] == 114 || s[2] == 82) &&
              (s[4] == 105 || s[4] == 73) &&
              (s[5] == 99 || s[5] == 67) &&
              (s[6] == 101 || s[6] == 69) &&
              s[7] == 45 &&
              (s[8] == 119 || s[8] == 87) &&
              (s[9] == 111 || s[9] == 79) &&
              (s[10] == 114 || s[10] == 82) &&
              (s[11] == 107 || s[11] == 75) &&
              (s[12] == 101 || s[12] == 69) &&
              (s[13] == 114 || s[13] == 82) &&
              s[14] == 45 &&
              (s[15] == 97 || s[15] == 65) &&
              (s[16] == 108 || s[16] == 76) &&
              (s[17] == 108 || s[17] == 76) &&
              (s[18] == 111 || s[18] == 79) &&
              (s[19] == 119 || s[19] == 87) &&
              (s[20] == 101 || s[20] == 69) &&
              (s[21] == 100 || s[21] == 68)
            ) {
              return "service-worker-allowed";
            }
            break;
          case 111:
          case 79:
            // check that the buffer is equal to x-content-type-options and return it if so
            if (
              (s[0] == 120 || s[0] == 88) &&
              s[1] == 45 &&
              (s[2] == 99 || s[2] == 67) &&
              (s[4] == 110 || s[4] == 78) &&
              (s[5] == 116 || s[5] == 84) &&
              (s[6] == 101 || s[6] == 69) &&
              (s[7] == 110 || s[7] == 78) &&
              (s[8] == 116 || s[8] == 84) &&
              s[9] == 45 &&
              (s[10] == 116 || s[10] == 84) &&
              (s[11] == 121 || s[11] == 89) &&
              (s[12] == 112 || s[12] == 80) &&
              (s[13] == 101 || s[13] == 69) &&
              s[14] == 45 &&
              (s[15] == 111 || s[15] == 79) &&
              (s[16] == 112 || s[16] == 80) &&
              (s[17] == 116 || s[17] == 84) &&
              (s[18] == 105 || s[18] == 73) &&
              (s[19] == 111 || s[19] == 79) &&
              (s[20] == 110 || s[20] == 78) &&
              (s[21] == 115 || s[21] == 83)
            ) {
              return "x-content-type-options";
            }
            break;
          case 110:
          case 78:
            // check that the buffer is equal to x-dns-prefetch-control and return it if so
            if (
              (s[0] == 120 || s[0] == 88) &&
              s[1] == 45 &&
              (s[2] == 100 || s[2] == 68) &&
              (s[4] == 115 || s[4] == 83) &&
              s[5] == 45 &&
              (s[6] == 112 || s[6] == 80) &&
              (s[7] == 114 || s[7] == 82) &&
              (s[8] == 101 || s[8] == 69) &&
              (s[9] == 102 || s[9] == 70) &&
              (s[10] == 101 || s[10] == 69) &&
              (s[11] == 116 || s[11] == 84) &&
              (s[12] == 99 || s[12] == 67) &&
              (s[13] == 104 || s[13] == 72) &&
              s[14] == 45 &&
              (s[15] == 99 || s[15] == 67) &&
              (s[16] == 111 || s[16] == 79) &&
              (s[17] == 110 || s[17] == 78) &&
              (s[18] == 116 || s[18] == 84) &&
              (s[19] == 114 || s[19] == 82) &&
              (s[20] == 111 || s[20] == 79) &&
              (s[21] == 108 || s[21] == 76)
            ) {
              return "x-dns-prefetch-control";
            }
            break;
          default:
            break;
        }
        break;
      case 23:
        // check that the buffer is equal to content-security-policy and return it if so
        if (
          (s[0] == 99 || s[0] == 67) &&
          (s[1] == 111 || s[1] == 79) &&
          (s[2] == 110 || s[2] == 78) &&
          (s[3] == 116 || s[3] == 84) &&
          (s[4] == 101 || s[4] == 69) &&
          (s[5] == 110 || s[5] == 78) &&
          (s[6] == 116 || s[6] == 84) &&
          s[7] == 45 &&
          (s[8] == 115 || s[8] == 83) &&
          (s[9] == 101 || s[9] == 69) &&
          (s[10] == 99 || s[10] == 67) &&
          (s[11] == 117 || s[11] == 85) &&
          (s[12] == 114 || s[12] == 82) &&
          (s[13] == 105 || s[13] == 73) &&
          (s[14] == 116 || s[14] == 84) &&
          (s[15] == 121 || s[15] == 89) &&
          s[16] == 45 &&
          (s[17] == 112 || s[17] == 80) &&
          (s[18] == 111 || s[18] == 79) &&
          (s[19] == 108 || s[19] == 76) &&
          (s[20] == 105 || s[20] == 73) &&
          (s[21] == 99 || s[21] == 67) &&
          (s[22] == 121 || s[22] == 89)
        ) {
          return "content-security-policy";
        }
        break;
      case 24:
        // check that the buffer is equal to sec-websocket-extensions and return it if so
        if (
          (s[0] == 115 || s[0] == 83) &&
          (s[1] == 101 || s[1] == 69) &&
          (s[2] == 99 || s[2] == 67) &&
          s[3] == 45 &&
          (s[4] == 119 || s[4] == 87) &&
          (s[5] == 101 || s[5] == 69) &&
          (s[6] == 98 || s[6] == 66) &&
          (s[7] == 115 || s[7] == 83) &&
          (s[8] == 111 || s[8] == 79) &&
          (s[9] == 99 || s[9] == 67) &&
          (s[10] == 107 || s[10] == 75) &&
          (s[11] == 101 || s[11] == 69) &&
          (s[12] == 116 || s[12] == 84) &&
          s[13] == 45 &&
          (s[14] == 101 || s[14] == 69) &&
          (s[15] == 120 || s[15] == 88) &&
          (s[16] == 116 || s[16] == 84) &&
          (s[17] == 101 || s[17] == 69) &&
          (s[18] == 110 || s[18] == 78) &&
          (s[19] == 115 || s[19] == 83) &&
          (s[20] == 105 || s[20] == 73) &&
          (s[21] == 111 || s[21] == 79) &&
          (s[22] == 110 || s[22] == 78) &&
          (s[23] == 115 || s[23] == 83)
        ) {
          return "sec-websocket-extensions";
        }
        break;
      case 25:
        switch (s[0]) {
          case 115:
          case 83:
            // check that the buffer is equal to strict-transport-security and return it if so
            if (
              (s[1] == 116 || s[1] == 84) &&
              (s[2] == 114 || s[2] == 82) &&
              (s[3] == 105 || s[3] == 73) &&
              (s[4] == 99 || s[4] == 67) &&
              (s[5] == 116 || s[5] == 84) &&
              (s[6] == 45 || s[6] == 13) &&
              (s[7] == 116 || s[7] == 84) &&
              (s[8] == 114 || s[8] == 82) &&
              (s[9] == 97 || s[9] == 65) &&
              (s[10] == 110 || s[10] == 78) &&
              (s[11] == 115 || s[11] == 83) &&
              (s[12] == 112 || s[12] == 80) &&
              (s[13] == 111 || s[13] == 79) &&
              (s[14] == 114 || s[14] == 82) &&
              (s[15] == 116 || s[15] == 84) &&
              (s[16] == 45 || s[16] == 13) &&
              (s[17] == 115 || s[17] == 83) &&
              (s[18] == 101 || s[18] == 69) &&
              (s[19] == 99 || s[19] == 67) &&
              (s[20] == 117 || s[20] == 85) &&
              (s[21] == 114 || s[21] == 82) &&
              (s[22] == 105 || s[22] == 73) &&
              (s[23] == 116 || s[23] == 84) &&
              (s[24] == 121 || s[24] == 89)
            ) {
              return "strict-transport-security";
            }
            break;
          case 117:
          case 85:
            // check that the buffer is equal to upgrade-insecure-requests and return it if so
            if (
              (s[1] == 112 || s[1] == 80) &&
              (s[2] == 103 || s[2] == 71) &&
              (s[3] == 114 || s[3] == 82) &&
              (s[4] == 97 || s[4] == 65) &&
              (s[5] == 100 || s[5] == 68) &&
              (s[6] == 101 || s[6] == 69) &&
              (s[7] == 45 || s[7] == 13) &&
              (s[8] == 105 || s[8] == 73) &&
              (s[9] == 110 || s[9] == 78) &&
              (s[10] == 115 || s[10] == 83) &&
              (s[11] == 101 || s[11] == 69) &&
              (s[12] == 99 || s[12] == 67) &&
              (s[13] == 117 || s[13] == 85) &&
              (s[14] == 114 || s[14] == 82) &&
              (s[15] == 101 || s[15] == 69) &&
              (s[16] == 45 || s[16] == 13) &&
              (s[17] == 114 || s[17] == 82) &&
              (s[18] == 101 || s[18] == 69) &&
              (s[19] == 113 || s[19] == 81) &&
              (s[20] == 117 || s[20] == 85) &&
              (s[21] == 101 || s[21] == 69) &&
              (s[22] == 115 || s[22] == 83) &&
              (s[23] == 116 || s[23] == 84) &&
              (s[24] == 115 || s[24] == 83)
            ) {
              return "upgrade-insecure-requests";
            }
            break;
          default:
            break;
        }
        break;
      case 26:
        // check that the buffer is equal to cross-origin-opener-policy and return it if so
        if (
          (s[0] == 99 || s[0] == 67) &&
          (s[1] == 114 || s[1] == 82) &&
          (s[2] == 111 || s[2] == 79) &&
          (s[3] == 115 || s[3] == 83) &&
          (s[4] == 115 || s[4] == 83) &&
          s[5] == 45 &&
          (s[6] == 111 || s[6] == 79) &&
          (s[7] == 114 || s[7] == 82) &&
          (s[8] == 105 || s[8] == 73) &&
          (s[9] == 103 || s[9] == 71) &&
          (s[10] == 105 || s[10] == 73) &&
          (s[11] == 110 || s[11] == 78) &&
          s[12] == 45 &&
          (s[13] == 111 || s[13] == 79) &&
          (s[14] == 112 || s[14] == 80) &&
          (s[15] == 101 || s[15] == 69) &&
          (s[16] == 110 || s[16] == 78) &&
          (s[17] == 101 || s[17] == 69) &&
          (s[18] == 114 || s[18] == 82) &&
          s[19] == 45 &&
          (s[20] == 112 || s[20] == 80) &&
          (s[21] == 111 || s[21] == 79) &&
          (s[22] == 108 || s[22] == 76) &&
          (s[23] == 105 || s[23] == 73) &&
          (s[24] == 99 || s[24] == 67) &&
          (s[25] == 121 || s[25] == 89)
        ) {
          return "cross-origin-opener-policy";
        }
        break;
      case 27:
        // check that the buffer is equal to access-control-allow-origin and return it if so
        if (
          (s[0] == 97 || s[0] == 65) &&
          (s[1] == 99 || s[1] == 67) &&
          (s[2] == 99 || s[2] == 67) &&
          (s[3] == 101 || s[3] == 69) &&
          (s[4] == 115 || s[4] == 83) &&
          (s[5] == 115 || s[5] == 83) &&
          s[6] == 45 &&
          (s[7] == 99 || s[7] == 67) &&
          (s[8] == 111 || s[8] == 79) &&
          (s[9] == 110 || s[9] == 78) &&
          (s[10] == 116 || s[10] == 84) &&
          (s[11] == 114 || s[11] == 82) &&
          (s[12] == 111 || s[12] == 79) &&
          (s[13] == 108 || s[13] == 76) &&
          s[14] == 45 &&
          (s[15] == 97 || s[15] == 65) &&
          (s[16] == 108 || s[16] == 76) &&
          (s[17] == 108 || s[17] == 76) &&
          (s[18] == 111 || s[18] == 79) &&
          (s[19] == 119 || s[19] == 87) &&
          s[20] == 45 &&
          (s[21] == 111 || s[21] == 79) &&
          (s[22] == 114 || s[22] == 82) &&
          (s[23] == 105 || s[23] == 73) &&
          (s[24] == 103 || s[24] == 71) &&
          (s[25] == 105 || s[25] == 73) &&
          (s[26] == 110 || s[26] == 78)
        ) {
          return "access-control-allow-origin";
        }
        break;
      case 28:
        [
          "access-control-allow-headers",
          "access-control-allow-methods",
          "cross-origin-embedder-policy",
          "cross-origin-resource-policy",
        ];
      case 29:
        switch (s[15]) {
          case 101:
          case 69:
            // check that the buffer is equal to access-control-expose-headers and return it if so
            if (
              (s[0] == 97 || s[0] == 65) &&
              (s[1] == 99 || s[1] == 67) &&
              (s[2] == 99 || s[2] == 67) &&
              (s[3] == 101 || s[3] == 69) &&
              (s[4] == 115 || s[4] == 83) &&
              (s[5] == 115 || s[5] == 83) &&
              s[6] == 45 &&
              (s[7] == 99 || s[7] == 67) &&
              (s[8] == 111 || s[8] == 79) &&
              (s[9] == 110 || s[9] == 78) &&
              (s[10] == 116 || s[10] == 84) &&
              (s[11] == 114 || s[11] == 82) &&
              (s[12] == 111 || s[12] == 79) &&
              (s[13] == 108 || s[13] == 76) &&
              s[14] == 45 &&
              (s[16] == 120 || s[16] == 88) &&
              (s[17] == 112 || s[17] == 80) &&
              (s[18] == 111 || s[18] == 79) &&
              (s[19] == 115 || s[19] == 83) &&
              (s[20] == 101 || s[20] == 69) &&
              s[21] == 45 &&
              (s[22] == 104 || s[22] == 72) &&
              (s[23] == 101 || s[23] == 69) &&
              (s[24] == 97 || s[24] == 65) &&
              (s[25] == 100 || s[25] == 68) &&
              (s[26] == 101 || s[26] == 69) &&
              (s[27] == 114 || s[27] == 82) &&
              (s[28] == 115 || s[28] == 83)
            ) {
              return "access-control-expose-headers";
            }
            break;
          case 114:
          case 82:
            // check that the buffer is equal to access-control-request-method and return it if so
            if (
              (s[0] == 97 || s[0] == 65) &&
              (s[1] == 99 || s[1] == 67) &&
              (s[2] == 99 || s[2] == 67) &&
              (s[3] == 101 || s[3] == 69) &&
              (s[4] == 115 || s[4] == 83) &&
              (s[5] == 115 || s[5] == 83) &&
              s[6] == 45 &&
              (s[7] == 99 || s[7] == 67) &&
              (s[8] == 111 || s[8] == 79) &&
              (s[9] == 110 || s[9] == 78) &&
              (s[10] == 116 || s[10] == 84) &&
              (s[11] == 114 || s[11] == 82) &&
              (s[12] == 111 || s[12] == 79) &&
              (s[13] == 108 || s[13] == 76) &&
              s[14] == 45 &&
              (s[16] == 101 || s[16] == 69) &&
              (s[17] == 113 || s[17] == 81) &&
              (s[18] == 117 || s[18] == 85) &&
              (s[19] == 101 || s[19] == 69) &&
              (s[20] == 115 || s[20] == 83) &&
              (s[21] == 116 || s[21] == 84) &&
              s[22] == 45 &&
              (s[23] == 109 || s[23] == 77) &&
              (s[24] == 101 || s[24] == 69) &&
              (s[25] == 116 || s[25] == 84) &&
              (s[26] == 104 || s[26] == 72) &&
              (s[27] == 111 || s[27] == 79) &&
              (s[28] == 100 || s[28] == 68)
            ) {
              return "access-control-request-method";
            }
            break;
          default:
            break;
        }
        break;
      case 30:
        // check that the buffer is equal to access-control-request-headers and return it if so
        if (
          (s[0] == 97 || s[0] == 65) &&
          (s[1] == 99 || s[1] == 67) &&
          (s[2] == 99 || s[2] == 67) &&
          (s[3] == 101 || s[3] == 69) &&
          (s[4] == 115 || s[4] == 83) &&
          (s[5] == 115 || s[5] == 83) &&
          s[6] == 45 &&
          (s[7] == 99 || s[7] == 67) &&
          (s[8] == 111 || s[8] == 79) &&
          (s[9] == 110 || s[9] == 78) &&
          (s[10] == 116 || s[10] == 84) &&
          (s[11] == 114 || s[11] == 82) &&
          (s[12] == 111 || s[12] == 79) &&
          (s[13] == 108 || s[13] == 76) &&
          s[14] == 45 &&
          (s[15] == 114 || s[15] == 82) &&
          (s[16] == 101 || s[16] == 69) &&
          (s[17] == 113 || s[17] == 81) &&
          (s[18] == 117 || s[18] == 85) &&
          (s[19] == 101 || s[19] == 69) &&
          (s[20] == 115 || s[20] == 83) &&
          (s[21] == 116 || s[21] == 84) &&
          s[22] == 45 &&
          (s[23] == 104 || s[23] == 72) &&
          (s[24] == 101 || s[24] == 69) &&
          (s[25] == 97 || s[25] == 65) &&
          (s[26] == 100 || s[26] == 68) &&
          (s[27] == 101 || s[27] == 69) &&
          (s[28] == 114 || s[28] == 82) &&
          (s[29] == 115 || s[29] == 83)
        ) {
          return "access-control-request-headers";
        }
        break;
      case 32:
        // check that the buffer is equal to access-control-allow-credentials and return it if so
        if (
          (s[0] == 97 || s[0] == 65) &&
          (s[1] == 99 || s[1] == 67) &&
          (s[2] == 99 || s[2] == 67) &&
          (s[3] == 101 || s[3] == 69) &&
          (s[4] == 115 || s[4] == 83) &&
          (s[5] == 115 || s[5] == 83) &&
          s[6] == 45 &&
          (s[7] == 99 || s[7] == 67) &&
          (s[8] == 111 || s[8] == 79) &&
          (s[9] == 110 || s[9] == 78) &&
          (s[10] == 116 || s[10] == 84) &&
          (s[11] == 114 || s[11] == 82) &&
          (s[12] == 111 || s[12] == 79) &&
          (s[13] == 108 || s[13] == 76) &&
          s[14] == 45 &&
          (s[15] == 97 || s[15] == 65) &&
          (s[16] == 108 || s[16] == 76) &&
          (s[17] == 108 || s[17] == 76) &&
          (s[18] == 111 || s[18] == 79) &&
          (s[19] == 119 || s[19] == 87) &&
          s[20] == 45 &&
          (s[21] == 99 || s[21] == 67) &&
          (s[22] == 114 || s[22] == 82) &&
          (s[23] == 101 || s[23] == 69) &&
          (s[24] == 100 || s[24] == 68) &&
          (s[25] == 101 || s[25] == 69) &&
          (s[26] == 110 || s[26] == 78) &&
          (s[27] == 116 || s[27] == 84) &&
          (s[28] == 105 || s[28] == 73) &&
          (s[29] == 97 || s[29] == 65) &&
          (s[30] == 108 || s[30] == 76) &&
          (s[31] == 115 || s[31] == 83)
        ) {
          return "access-control-allow-credentials";
        }
        break;
      case 33:
        switch (s[0]) {
          case 115:
          case 83:
            // check that the buffer is equal to service-worker-navigation-preload and return it if so
            if (
              (s[1] == 101 || s[1] == 69) &&
              (s[2] == 114 || s[2] == 82) &&
              (s[3] == 118 || s[3] == 86) &&
              (s[4] == 105 || s[4] == 73) &&
              (s[5] == 99 || s[5] == 67) &&
              (s[6] == 101 || s[6] == 69) &&
              (s[7] == 45 || s[7] == 13) &&
              (s[8] == 119 || s[8] == 87) &&
              (s[9] == 111 || s[9] == 79) &&
              (s[10] == 114 || s[10] == 82) &&
              (s[11] == 107 || s[11] == 75) &&
              (s[12] == 101 || s[12] == 69) &&
              (s[13] == 114 || s[13] == 82) &&
              (s[14] == 45 || s[14] == 13) &&
              (s[15] == 110 || s[15] == 78) &&
              (s[16] == 97 || s[16] == 65) &&
              (s[17] == 118 || s[17] == 86) &&
              (s[18] == 105 || s[18] == 73) &&
              (s[19] == 103 || s[19] == 71) &&
              (s[20] == 97 || s[20] == 65) &&
              (s[21] == 116 || s[21] == 84) &&
              (s[22] == 105 || s[22] == 73) &&
              (s[23] == 111 || s[23] == 79) &&
              (s[24] == 110 || s[24] == 78) &&
              (s[25] == 45 || s[25] == 13) &&
              (s[26] == 112 || s[26] == 80) &&
              (s[27] == 114 || s[27] == 82) &&
              (s[28] == 101 || s[28] == 69) &&
              (s[29] == 108 || s[29] == 76) &&
              (s[30] == 111 || s[30] == 79) &&
              (s[31] == 97 || s[31] == 65) &&
              (s[32] == 100 || s[32] == 68)
            ) {
              return "service-worker-navigation-preload";
            }
            break;
          case 120:
          case 88:
            // check that the buffer is equal to x-permitted-cross-domain-policies and return it if so
            if (
              (s[1] == 45 || s[1] == 13) &&
              (s[2] == 112 || s[2] == 80) &&
              (s[3] == 101 || s[3] == 69) &&
              (s[4] == 114 || s[4] == 82) &&
              (s[5] == 109 || s[5] == 77) &&
              (s[6] == 105 || s[6] == 73) &&
              (s[7] == 116 || s[7] == 84) &&
              (s[8] == 116 || s[8] == 84) &&
              (s[9] == 101 || s[9] == 69) &&
              (s[10] == 100 || s[10] == 68) &&
              (s[11] == 45 || s[11] == 13) &&
              (s[12] == 99 || s[12] == 67) &&
              (s[13] == 114 || s[13] == 82) &&
              (s[14] == 111 || s[14] == 79) &&
              (s[15] == 115 || s[15] == 83) &&
              (s[16] == 115 || s[16] == 83) &&
              (s[17] == 45 || s[17] == 13) &&
              (s[18] == 100 || s[18] == 68) &&
              (s[19] == 111 || s[19] == 79) &&
              (s[20] == 109 || s[20] == 77) &&
              (s[21] == 97 || s[21] == 65) &&
              (s[22] == 105 || s[22] == 73) &&
              (s[23] == 110 || s[23] == 78) &&
              (s[24] == 45 || s[24] == 13) &&
              (s[25] == 112 || s[25] == 80) &&
              (s[26] == 111 || s[26] == 79) &&
              (s[27] == 108 || s[27] == 76) &&
              (s[28] == 105 || s[28] == 73) &&
              (s[29] == 99 || s[29] == 67) &&
              (s[30] == 105 || s[30] == 73) &&
              (s[31] == 101 || s[31] == 69) &&
              (s[32] == 115 || s[32] == 83)
            ) {
              return "x-permitted-cross-domain-policies";
            }
            break;
          default:
            break;
        }
        break;
      case 35:
        // check that the buffer is equal to content-security-policy-report-only and return it if so
        if (
          (s[0] == 99 || s[0] == 67) &&
          (s[1] == 111 || s[1] == 79) &&
          (s[2] == 110 || s[2] == 78) &&
          (s[3] == 116 || s[3] == 84) &&
          (s[4] == 101 || s[4] == 69) &&
          (s[5] == 110 || s[5] == 78) &&
          (s[6] == 116 || s[6] == 84) &&
          s[7] == 45 &&
          (s[8] == 115 || s[8] == 83) &&
          (s[9] == 101 || s[9] == 69) &&
          (s[10] == 99 || s[10] == 67) &&
          (s[11] == 117 || s[11] == 85) &&
          (s[12] == 114 || s[12] == 82) &&
          (s[13] == 105 || s[13] == 73) &&
          (s[14] == 116 || s[14] == 84) &&
          (s[15] == 121 || s[15] == 89) &&
          s[16] == 45 &&
          (s[17] == 112 || s[17] == 80) &&
          (s[18] == 111 || s[18] == 79) &&
          (s[19] == 108 || s[19] == 76) &&
          (s[20] == 105 || s[20] == 73) &&
          (s[21] == 99 || s[21] == 67) &&
          (s[22] == 121 || s[22] == 89) &&
          s[23] == 45 &&
          (s[24] == 114 || s[24] == 82) &&
          (s[25] == 101 || s[25] == 69) &&
          (s[26] == 112 || s[26] == 80) &&
          (s[27] == 111 || s[27] == 79) &&
          (s[28] == 114 || s[28] == 82) &&
          (s[29] == 116 || s[29] == 84) &&
          s[30] == 45 &&
          (s[31] == 111 || s[31] == 79) &&
          (s[32] == 110 || s[32] == 78) &&
          (s[33] == 108 || s[33] == 76) &&
          (s[34] == 121 || s[34] == 89)
        ) {
          return "content-security-policy-report-only";
        }
        break;
      default:
        break;
    }
    return s.toString().toLowerCase();
  }
export function switchparseHeaders(headers, obj = {}) {
  // For H2 support
  if (!Array.isArray(headers)) return headers;

  for (let i = 0; i < headers.length; i += 2) {
    const key = headers[i];
    
    const lowerCasedKey = typeof key === "string" ? key.toLowerCase() : toLowerCase(key);
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
