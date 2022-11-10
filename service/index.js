import client from './client.js'
import fastify from 'fastify'

let fy = fastify({ logger: true })

fy.get('/', (req, res) => {
  client.getInfo({}, (err, info) => {
    res.send(info)
  })
})

fy.get('/add', (req, res) => {
  client.addUser({}, (err, info) => {
    res.send(info)
  })
})

fy.get('/del', (req, res) => {
  client.delUser({}, (err, info) => {
    res.send(info)
  })
})

fy.get('/click', (req, res) => {
  client.sendClick({}, (err, info) => {
    res.send(info)
  })
})

let port = process.env.PORT || 50050
fy.listen({ port }, (err, addr) => {
  if (err) {
    fy.log.error(err)
    process.exit(1)
  }
  fy.log.info('started localhost:' + port)
})