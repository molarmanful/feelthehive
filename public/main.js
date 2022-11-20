let ws

Vue.createApp({

  data: _ => ({
    power: 0,
    clients: 0,
    clicked: false,
  }),

  mounted() {
    if (ws) {
      ws.onerror = ws.onopen = ws.onclose = null
      ws.close()
    }
    ws = new WebSocket(`wss://${window.location.host}/ws`)

    ws.onopen = _ => {
      console.log('opened')
      app.classList.remove('unloaded')
      this.initAll()
    }

    ws.onmessage = ({ data }) => {
      console.log('received ' + data)
      let d = JSON.parse(data)
      this.power = d.pow
      this.clients = d.size
      document.body.className =
        this.power >= 70 ? 'hard'
          : this.power >= 50 ? 'med'
            : this.power >= 20 ? 'light'
              : ''
    }

    ws.onclose = _ => {
      console.log('closed')
      ws = null
    }
  },

  methods: {

    initAll() {
      ['click', 'keypress'].map(x => document.addEventListener(x, e => { this.scratch() }))
    },

    scratch(e) {
      if (!ws) {
        console.error('socket disconnected')
        alert('Disconnected from server, page will now reload.')
        location.reload()
        return
      }
      if (e != void 0) e.preventDefault()
      if (!this.clicked) this.clicked = true
      ws.send('scratch')
    },

  },

}).mount('#app')