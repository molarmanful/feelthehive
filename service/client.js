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

export default new Hive('localhost:50051', grpc.credentials.createInsecure())