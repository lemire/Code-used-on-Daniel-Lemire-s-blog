const uWS = require('uWebSockets.js')
const port = 3000;

const app = uWS.App({
}).get('/simple', (res, req) => {
  res.end('Hello!');
}).listen(port, (token) => {
  if (token) {
    console.log('Listening to port ' + port);
  } else {
    console.log('Failed to listen to port ' + port);
  }
});