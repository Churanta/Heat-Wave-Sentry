const mqtt = require('mqtt')

const protocol = 'mqtt'
const host = 'broker.emqx.io'
const port = '1883'
const clientId = `mqttx_b363fcb8`

const connectUrl = `${protocol}://${host}:${port}`

const client = mqtt.connect(connectUrl, {
  clientId,
  clean: true,
  connectTimeout: 4000,
  username: '12334',
  password: '1234',
  reconnectPeriod: 1000,
})

const topic = '/testtopic/chur'

client.on('connect', () => {
    console.log('Connected')
    client.subscribe([topic], () => {
    console.log(`Subscribe to topic '${topic}'`)
  })
})

client.on('message', (topic, payload) => {
  console.log('Received Message:', topic, payload.toString())
})