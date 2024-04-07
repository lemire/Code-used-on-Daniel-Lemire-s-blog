Bun.serve({
    fetch (req) {
	    const url = new URL(req.url);
            if (url.pathname === "/simple") return new Response("Hello world!");
	    return new Response('Should return an error!') 
    },
    port: Number(Bun.env.PORT || 3000),
    reusePort: true,
    address: Bun.env.ADDRESS || '127.0.0.1'
})
