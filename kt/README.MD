# Build Guide: Compiling React Components for WebView (No Bundler, No Config File)

This project uses raw React (`react.js` + `react-dom.js`) loaded via `<script>` tags inside
an Android WebView, with JSX components written in separate files under `js/components/`.

Because the WebView loads pages over `file://`, Babel Standalone's in-browser compilation
(`type="text/babel"` + `src="..."`) does **not** work — WebViews block `XMLHttpRequest`
requests to local files, which is how Babel Standalone fetches external `.js` files.

**Solution:** precompile all JSX files ahead of time using the Babel CLI, in **classic runtime**
mode, so the output uses plain `React.createElement(...)` calls (compatible with our global
`React` variable) instead of `import`-based JSX runtime calls (which fail with no module system).

---

## 1. Why "classic" runtime?

By default, modern Babel presets compile JSX to the **automatic runtime**, which injects a line like:

```js
import { jsx as _jsx } from "react/jsx-runtime";
```

This fails in our setup because there's no bundler/module system to resolve that import — we're
just using global `React`/`ReactDOM` from `<script>` tags. The **classic runtime** instead compiles
JSX to:

```js
React.createElement(...)
```

— which works directly with the global `React` object, no import needed.

---

## 2. One-time setup

Install Babel's core compiler and the React preset as dev dependencies:

```bash
npm install --save-dev @babel/core @babel/preset-react
```

Create a file named **`build.js`** in the project root:

```js
const fs = require("fs");
const path = require("path");
const babel = require("@babel/core");

const inputDir = "js/components";
const outputDir = "js/components-compiled";

if (!fs.existsSync(outputDir)) {
  fs.mkdirSync(outputDir, { recursive: true });
}

fs.readdirSync(inputDir).forEach(file => {
  if (!file.endsWith(".js")) return;

  const inputPath = path.join(inputDir, file);
  const outputPath = path.join(outputDir, file);

  const result = babel.transformFileSync(inputPath, {
    presets: [
      ["@babel/preset-react", { runtime: "classic" }]
    ]
  });

  fs.writeFileSync(outputPath, result.code);
  console.log(`Compiled: ${file}`);
});
```

This script is *not* a Babel config file — it's a plain Node script that calls Babel's API
directly, passing the `runtime: "classic"` option inline. That's how we avoid needing
`babel.config.json` or `.babelrc`.

---

## 3. Folder structure

```
project-root/
├── build.js
├── package.json
├── index.html
├── css/
├── images/
└── js/
    ├── react.js
    ├── react-dom.js
    ├── components/            ← write your raw .jsx-in-.js source files here
    │   ├── firstScreen.js
    │   └── Header.js
    └── components-compiled/   ← auto-generated, do not edit by hand
        ├── firstScreen.js
        └── Header.js
```

> Add `js/components-compiled/` to `.gitignore` if you'd rather generate it fresh
> on every machine, or commit it if you want the compiled output versioned too.

---

## 4. Compiling

Every time you add or edit a component, run:

```bash
node build.js
```

This compiles every `.js` file in `js/components/` and writes the classic-runtime output to
`js/components-compiled/`, keeping the same filenames.

---

## 5. Referencing compiled files in HTML

Load React first, then your compiled components in **dependency order**
(a component must be loaded *before* any other component that uses it):

```html
<script src="js/react.js"></script>
<script src="js/react-dom.js"></script>

<script src="js/components-compiled/Header.js"></script>
<script src="js/components-compiled/firstScreen.js"></script>
```

Because there's no module system, every component must be declared as a plain
top-level `function` (not `export default function`) so it becomes a global
that other component files can reference directly:

```js
// Header.js  (source, not compiled)
function Header() {
  return <h1>My App</h1>;
}
```

```js
// firstScreen.js (source, not compiled)
function FirstScreen() {
  return <Header />;
}
```

---

## 6. Known limitation

Since there's no bundler resolving a dependency graph, **script tag order in the HTML matters**.
If a component references another component that hasn't loaded yet, you'll get a
`ReferenceError: X is not defined`. Keep dependencies (e.g. shared components like `Header`,
`Note`) listed before the files that use them.

If the number of components grows and manual ordering becomes painful, consider switching to
a real bundler (e.g. esbuild) with `import`/`export` syntax — it resolves this automatically.

---

## Quick reference

| Step | Command |
|---|---|
| Install dependencies | `npm install --save-dev @babel/core @babel/preset-react` |
| Compile all components | `node build.js` |
| Where source files go | `js/components/` |
| Where compiled files go | `js/components-compiled/` (auto-generated) |
