Bun.serve({
    fetch () {
      return new Response('Hello, World!') 
    },
    port: Number(Bun.env.PORT || 3000),
    reusePort: true,
    address: Bun.env.ADDRESS || '127.0.0.1'
})