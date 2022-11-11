import client from './service/client.js'
import Fastify, { fastify } from 'fastify'
import FastifyWS from '@fastify/websocket'
import FastifyStatic from '@fastify/static'

let server = Fastify({ logger: true })
await server.register(FastifyWS)
server.register(FastifyStatic, {
  root: new URL('.', import.meta.url).pathname + '/public',
})

let shoutInfo = conn => (err, info) => {
}

let shoutUser = conn => {
  client.setUser({ x: server.websocketServer.clients.size }, shoutInfo(conn))
}

server.get('/ws', { websocket: true }, (conn, req) => {
  console.log('+conn')
  shoutUser(conn)
  conn.socket.on('message', msg => {
    console.log(msg)
    if (msg == 'scratch')
      client.sendClick({}, shoutInfo)
  })

  conn.socket.on('close', _ => {
    console.log('-conn')
    shoutUser(conn)
  })
})

let port = process.env.PORT || 8080
server.listen({ port }, (err, addr) => {
  if (err) {
    server.log.error(err)
    process.exit(1)
  }
})