import { readFile, writeFile } from 'fs/promises'
import { compileFile } from 'pug'
import { render } from 'stylus'
import postcss from 'postcss'
import autoprefixer from 'autoprefixer'

let css = x => {
  let I = x + '.styl'
  let O = x + '.css'
  readFile(I).then(styl => {
    render(styl + '', (err, css) => {
      postcss([autoprefixer]).process(css, { from: void 0 }).then(res => {
        writeFile(O, res.css).then(_ => {
          console.log(I + ' > ' + O)
        })
      })
    })
  })
}

let html = x => {
  let I = x + '.pug'
  let O = x + '.html'
  writeFile(O, compileFile(I, {})()).then(_ => {
    console.log(I + ' > ' + O)
  })
}

css('style')
html('index')