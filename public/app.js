let ws

Vue.createApp({

  data: _ => ({
    power: 0,
    pow: 0,
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
    ws.onmessage = msg => {
      console.log(msg)
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