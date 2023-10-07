const f = require('fastify')()

console.log(`
  Node version: ${process.version}
`)

f.get('/simple', async (request) => {
    return "hello"
})


f.listen({ port: 3000})
    .then(() => console.log('listening on port 3000'))
    .catch(err => console.error(err))
