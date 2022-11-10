import client from './service/client.js'
import Fastify from 'fastify'
import FastifyStatic from '@fastify/static'

let server = Fastify({ logger: true })

server.register(FastifyStatic, {
  root: new URL('.', import.meta.url).pathname + '/static',
})

server.get('/', (req, res) => {
  res.sendFile('index.html')
})

server.get('/add', (req, res) => {
  client.addUser({}, (err, info) => {
    res.send(info)
  })
})

server.get('/del', (req, res) => {
  client.delUser({}, (err, info) => {
    res.send(info)
  })
})

server.get('/click', (req, res) => {
  client.sendClick({}, (err, info) => {
    res.send(info)
  })
})

let port = process.env.PORT || 8080
server.listen({ port }, (err, addr) => {
  if (err) {
    server.log.error(err)
    process.exit(1)
  }
})