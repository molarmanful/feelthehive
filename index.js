import Fastify from 'fastify'
import FastifyWS from '@fastify/websocket'
import FastifyStatic from '@fastify/static'
import Ring from './ext/ring.js'
import './ext/compile.js'

let server = Fastify({ logger: true })
await server.register(FastifyWS)
server.register(FastifyStatic, {
  root: new URL('.', import.meta.url).pathname + '/public',
})

let ring = new Ring()
let info = new Proxy({
  pow: 0,
  size: 0
}, {
  set(t, k, v) {
    t[k] = v
    return true
  }
})

let shoutInfo = conn => {
  console.log(info)
  conn.socket.send(JSON.stringify(info))
}

let shoutUser = conn => {
  info.size = server.websocketServer.clients.size
  shoutInfo(conn)
}

setInterval(_ => {
  ring.dec()
  ring.prune()
  info.pow = ring.queue.length
}, 10)

server.get('/ws', { websocket: true }, (conn, req) => {
  console.log('+conn')
  shoutUser(conn)

  setInterval(_ => {
    shoutInfo(conn)
  }, 10)

  conn.socket.on('message', msg => {
    if (msg == 'scratch') {
      ring.push()
      shoutInfo(conn)
    }
  })

  conn.socket.on('close', _ => {
    console.log('-conn')
    shoutUser(conn)
  })

})

let port = process.env.PORT || 8080
server.listen({ port, host: '0.0.0.0' }, (err, addr) => {
  if (err) {
    server.log.error(err)
    process.exit(1)
  }
})