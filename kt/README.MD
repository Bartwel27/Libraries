```
npm install --save-dev @babel/core @babel/cli @babel/preset-react

```
### The build js file
Add this to the root directory of the project and
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
