let ws
let actx = new AudioContext()

Vue.createApp({

  data: _ => ({
    power: 0,
    clients: 0,
    clicked: false,
    cdown: false,
    conn: false,
  }),

  mounted() {
    ws = new WebSocket(`${location.protocol == 'https:' ? 'wss' : 'ws'}://${location.host}/ws`)

    ws.onopen = _ => {
      this.conn = true
      console.log('opened')
      app.classList.remove('unloaded')
      this.initAll()
    }

    ws.onmessage = ({ data }) => {
      console.log('received ' + data)
      let d = JSON.parse(data)
      this.power = d.pow
      this.clients = d.size
      this.beep(440 * 2 ** ((d.pow - 35) / 24))
      document.body.className =
        this.power >= 70 ? 'hard'
          : this.power >= 50 ? 'med'
            : this.power >= 20 ? 'light'
              : ''
    }

    ws.onclose = _ => {
      this.conn = false
      console.log('closed')
      ws = null
      document.removeEventListener('click')
      document.removeEventListener('keypress')
      this.mounted()
    }
  },

  methods: {

    initAll() {
      document.addEventListener('click', e => {
        if (!this.cdown) this.scratch(e)
      })
      document.addEventListener('keydown', e => {
        if (e.key == ' ' && !e.repeat && !this.cdown) this.scratch()
      })
    },

    scratch(e) {
      if (!ws) {
        console.error('socket disconnected')
        alert('Disconnected from server, attempting to reconnect...')
        return
      }
      if (e != void 0) e.preventDefault()
      if (!this.clicked) this.clicked = true
      ws.send('scratch')
      this.cdown = true
      setTimeout(_ => this.cdown = false, 10)
    },

    beep(f, t = 200, v = .05) {
      return new Promise((yes, no) => {
        try {
          let osc = actx.createOscillator()
          let gain = actx.createGain()
          osc.connect(gain)
          osc.frequency.value = f
          osc.type = 'sine'
          gain.connect(actx.destination)
          gain.gain.value = v
          gain.gain.setTargetAtTime(0, actx.currentTime, .02)
          osc.start(actx.currentTime)
          osc.stop(actx.currentTime + t * 0.001)
          osc.onended = _ => { yes() }
        } catch (e) { no(e) }
      })
    }

  },

}).mount('#app')