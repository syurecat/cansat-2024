import express from 'express'
import http from 'http'
import path from 'path'
import indexRouter from 'routes/index.js'
import apiRouter from 'routes/api.js'
import {setWebSocket} from 'routes/WebSocket.js'

const app = express();
const server = http.createServer(app);
const wrap = fn => (...args) => fn(...args).catch(args[2])
setWebSocket(server);

// public
app.use(express.static(path.join(__dirname, "public")));
app.use(express.json());

// root index
app.get("/", indexRouter);

// api root
app.use("/api", apiRouter);

app.all('/teapot', wrap(async (req, res, next) => {
    res.status(418).send("I'm a teapot");
}));

app.all("*", wrap(async (req, res, next) => {
    res.status(404).send("Not Found");
}));

app.use(wrap(async (err, req, res, next) => {
    console.error(err.stack)
    res.status(500).send("Internal Server Error");
}));

app.listen(7000, () => {
    console.log("Server running on http://localhost:7000");
});
