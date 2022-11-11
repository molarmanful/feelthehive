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
    ws = new WebSocket(`ws://${window.location.host}/ws`)

    ws.onopen = _ => {
      console.log('opened')
      app.classList.remove('unloaded')
      this.initAll()
    }

    ws.onmessage = async ({ data }) => {
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
      addEventListener('keypress', e => {
        if (e.key == ' ') this.scratch()
      })
    },

    scratch(e) {
      if (!ws) {
        console.error('socket disconnected')
        return
      }
      if (e != void 0) e.preventDefault()
      if (!this.clicked) this.clicked = true
      ws.send('scratch')
    },

  },

}).mount('#app')