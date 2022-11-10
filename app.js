import client from './service/client.js'
import Fastify from 'fastify'
import FastifyWS from '@fastify/websocket'
import FastifyStatic from '@fastify/static'

let server = Fastify({ logger: true })

server.register(FastifyWS)
server.register(FastifyStatic, {
  root: new URL('.', import.meta.url).pathname + '/public',
})

server.get('/ws', { websocket: true }, (conn, req) => {
  conn.socket.on('message', msg => {
    console.log(msg)
  })
})

// server.get('/add', (req, res) => {
//   client.addUser({}, (err, info) => {
//     res.send(info)
//   })
// })
// 
// server.get('/del', (req, res) => {
//   client.delUser({}, (err, info) => {
//     res.send(info)
//   })
// })
// 
// server.get('/click', (req, res) => {
//   client.sendClick({}, (err, info) => {
//     res.send(info)
//   })
// })

server.listen({ port: 8080 }, (err, addr) => {
  if (err) {
    server.log.error(err)
    process.exit(1)
  }
})