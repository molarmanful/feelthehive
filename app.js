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

Vue.createApp({

  data: _ => ({
    power: 0,
    pow: 0,
    clients: 0,
    clicked: false,
    ring: new Ring(),
  }),

  mounted() {
    this.initAll()
  },

  methods: {

    initAll() {
      app.classList.remove('unloaded')
      addEventListener('keypress', e => {
        if (e.key == ' ') this.scratch()
      })
      setInterval(_ => {
        this.ring.dec()
        this.ring.prune()
        this.powUp()
      }, 10)
    },

    scratch() {
      if (!this.clicked) this.clicked = true
      this.spoofSend()
    },

    spoofSend() {
      this.ring.push()
      this.powUp()
    },

    powUp() {
      this.power = this.ring.queue.length
    }

  },

}).mount('#app')