import grpc from '@grpc/grpc-js'
import protoLoader from '@grpc/proto-loader'

let packDef = protoLoader.loadSync('./protobuf/hive.proto', {
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

let port = process.env.PORT || 50051
server.bindAsync('localhost:' + port, grpc.ServerCredentials.createInsecure(), (err, port) => {
  console.log('localhost:' + port)
  server.start()
})