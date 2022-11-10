import grpc from '@grpc/grpc-js'
import protoLoader from '@grpc/proto-loader'

let packDef = protoLoader.loadSync('./protobuf/hive.proto', {
  keepCase: true,
  longs: String,
  enums: String,
  defaults: true,
  oneofs: true,
})
let Hive = grpc.loadPackageDefinition(packDef).Hive

let port = process.env.PORT || 50051
export default new Hive('localhost:' + port, grpc.credentials.createInsecure())