import grpc from '@grpc/grpc-js'
import protoLoader from '@grpc/proto-loader'

let packDef = protoLoader.loadSync(new URL('.', import.meta.url).pathname + '/proto/hive.proto', {
  keepCase: true,
  longs: String,
  enums: String,
  defaults: true,
  oneofs: true,
})
let Hive = grpc.loadPackageDefinition(packDef).Hive

export default new Hive('localhost:9090', grpc.credentials.createInsecure())