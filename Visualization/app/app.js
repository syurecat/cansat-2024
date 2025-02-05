import express from 'express'
import http from 'http'
import path from 'path'
import cors from 'cors'
import { fileURLToPath } from 'url';
import indexRouter from './routes/index.js'
import apiRouter from './routes/api.js'
import debugRouter from './routes/debug.js'
import {setWebSocket, closeWebSocket} from './routes/webSocket.js'

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const app = express();
const server = http.createServer(app);
const wrap = fn => (...args) => fn(...args).catch(args[2])
setWebSocket(server);

// public
app.use(express.static(path.join(__dirname, "public")));
app.use(express.json());
app.use(cors())

// root index
app.get("/", indexRouter);

// api root
app.use("/api", apiRouter);

// debug index
app.use("/debug", debugRouter);

// teapot
app.all('/teapot', wrap(async (req, res, next) => {
    res.status(418).send("I'm a teapot");
}));

// not found
app.all("*", wrap(async (req, res, next) => {
    res.status(404).send("Not Found");
}));

app.use(wrap(async (err, req, res, next) => {
    console.error(err.stack)
    res.status(500).send("Internal Server Error");
}));

server.listen(7080, () => {
    console.log("Server running on http://localhost:7080");
});

process.on("SIGINT", () => {
    console.log("Shutting down server...");
    closeWebSocket();
    server.close(() => {
        console.log("HTTP server closed.");
        process.exit(0);
    });
});

process.on("SIGTERM", () => {
    console.log("Received SIGTERM, shutting down...");
    closeWebSocket();
    server.close(() => {
        console.log("HTTP server closed.");
        process.exit(0);
    });
});