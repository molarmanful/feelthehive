import grpc from '@grpc/grpc-js'
import protoLoader from '@grpc/proto-loader'

class Ring {

  constructor(max = 100) {
    this.max = max
    this.queue = []
  }

  push(x = 300) {
    this.queue.push(x)
    while (this.queue.length > this.max) this.shift()
  }

  dec() {
    this.queue = this.queue.map(x => Math.max(0, x - 1))
  }

  prune() {
    this.queue = this.queue.filter(x => x)
  }

}

let packDef = protoLoader.loadSync(new URL('./proto/hive.proto', import.meta.url).pathname, {
  keepCase: true,
  longs: String,
  enums: String,
  defaults: true,
  oneofs: true,
})
let hiveProto = grpc.loadPackageDefinition(packDef)

let server = new grpc.Server()
let info = {
  pow: 0,
  size: 0,
}

server.addService(hiveProto.Hive.service, {

  getInfo(_, cb) {
    cb(null, info)
  },

  setUser(num, cb) {
    info.size = num.x
    cb(null, info)
  },

  addUser(_, cb) {
    info.size++
    cb(null, info)
  },

  delUser(_, cb) {
    if (info.size > 0) info.size--
    cb(null, info)
  },

  sendClick(_, cb) {
    info.pow++
    cb(null, info)
  },

})

server.bindAsync('localhost:9090', grpc.ServerCredentials.createInsecure(), (err, port) => {
  console.log('localhost:9090')
  server.start()
})