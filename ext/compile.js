import { readFile, writeFile } from 'fs/promises'
import { compileFile } from 'pug'
import { render } from 'stylus'
import postcss from 'postcss'
import autoprefixer from 'autoprefixer'

let css = async x => {
  let I = new URL(x + '.styl', import.meta.url)
  let O = new URL(x + '.css', import.meta.url)
  let styl = await readFile(I)
  let css = render(styl + '')
  let res = await postcss([autoprefixer]).process(css, { from: void 0 })
  await writeFile(O, res.css)
  console.log(I + ' > ' + O)
}

let html = async x => {
  let I = new URL(x + '.pug', import.meta.url)
  let O = new URL(x + '.html', import.meta.url)
  await writeFile(O, compileFile(I, {})())
  console.log(I + ' > ' + O)
}

await css('../public/style')
await html('../public/index')