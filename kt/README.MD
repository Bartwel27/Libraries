```
npm install --save-dev @babel/core @babel/cli @babel/preset-react
npx babel js/components/firstScreen.js --presets=@babel/preset-react -o js/components/firstScreen.compiled.js
npx babel js/components --out-dir js/components-compiled --presets=@babel/preset-react
```
