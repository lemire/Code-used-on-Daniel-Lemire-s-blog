
const server = Bun.serve({
    port: 3000,
    fetch(req) {
        let url = new URL(req.url);
        let pname = url.pathname;
        if(pname == '/simple'){
            return Response('Hello');
        }
        return new Response("Not Found.");
    }
  });