import Emitter from 'events'
import Fastify from 'fastify'
import FastifyWS from '@fastify/websocket'
import FastifyStatic from '@fastify/static'
import Ring from './ext/ring.js'

let server = Fastify({ logger: true })
await server.register(FastifyWS)
server.register(FastifyStatic, {
  root: new URL('.', import.meta.url).pathname + '/public',
})

let ring = new Ring()
let emit = new Emitter()
let info = new Proxy({
  pow: 0,
  size: 0
}, {
  set(t, k, v) {
    let b
    if (t[k] != v) b = 1
    t[k] = v
    if (b) emit.emit('infochange')
    return true
  }
})

let shoutInfo = conn => {
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

  emit.on('infochange', _ => {
    shoutInfo(conn)
  })

  let cdown = false
  conn.socket.on('message', msg => {
    if (!cdown) {
      if (msg == 'scratch') {
        ring.push()
        shoutInfo(conn)
        cdown = true
        setTimeout(_ => cdown = false, 100)
      }
      else if (msg == 'ping') {
        shoutInfo(conn)
      }
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