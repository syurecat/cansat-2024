import express from 'express'
import {InfluxDB, Point} from '@influxdata/influxdb-client'
import path from 'path'
import { fileURLToPath } from 'url';
import {getClients} from './webSocket.js'
import { predict, update, getEulerAngles } from '../services/imuCalculator.js';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const INFLUXDB_URL = 'http://influxdb:8086'
const influxDB = new InfluxDB({
    url: INFLUXDB_URL,
    token: process.env.INFLUXDB_TOKEN
})
const writeApi = influxDB.getWriteApi(
    process.env.INFLUXDB_ORG,
    process.env.INFLUXDB_BUCKET
)
const router = express.Router();
const wrap = fn => (...args) => fn(...args).catch(args[2])
const SENSER_TYPES = ["ACC", "GPS", "GYR", "MAG", "BME"];
const AUTH_TOKEN = process.env.AUTH_TOKEN
let lastTime = Date.now();

// api auth
function authenticate(req, res, next) {
    const token = req.body.token;
    if(!token) {
        res.status(401).send('Unauthorized')
    }else if (token === AUTH_TOKEN) {
        next();
    } else {
        res.status(403).send('Forbidden');
    }
}

// webSocket send 
router.post('/send', authenticate, wrap(async (req, res, next) => {
    console.log('Authenticated POST request received:', req.body);
    if (!req.body.massage) {
        res.status(400).json({ massage: "Bad Request" });
    } else {
        next();
    }
    }), wrap(async (req, res, next) => {
        try{
            getClients().forEach(client => {
                if (client.readyState === WebSocket.OPEN){
                    client.send(req.body.massage);
                }
            });
            res.status(200).json({ massage: "succese" })
        } catch (error) {
            console.error('Error Sending:', error)
            res.status(500).json({ massage: "Faild to send" })
        }
    })
);

// db update
router.post('/update', authenticate, wrap(async (req, res, next) => {
        if (!req.body.type || !req.body.name || !req.body.data) {
            res.status(400).json({ massage: "Bad Request" });
        } else if (!SENSER_TYPES.includes(req.body.type)) {
            res.status(421).json({ massage: "Misdirected Request" });
        } else {
            next();
        }
    }), wrap(async (req, res, next) => {
        try {
            const point = new Point(req.body.type)
                .tag("sensor", req.body.name);

            for (const key in req.body.data) {
                if (typeof req.body.data[key] === "number") {
                    point.floatField(key, req.body.data[key]);
                }
            }
            
            if (req.body.stre) {
                point.floatField("stre", req.body.stre);
            }
            writeApi.writePoint(point);
            await writeApi.flush();
            res.status(200).json({ massage: "success" });
            if (req.body.type == "GYR") {
                const currentTime = Date.now();
                const dt = lastTime ? (currentTime - lastTime) / 1000 : 0;
                const gyrData = [
                    Number(req.body.data.x),
                    Number(req.body.data.y),
                    Number(req.body.data.z)
                  ];
                console.log("Received data:", gyrData);
                predict(gyrData, dt);
                lastTime = currentTime;
            }
            if (req.body.type == "ACC") {
                const accData = [
                    Number(req.body.data.x),
                    Number(req.body.data.y),
                    Number(req.body.data.z)
                  ];
                console.log("Received data:", accData);
                update(accData);
                getClients().forEach(client => {
                    if (client.readyState === WebSocket.OPEN) {
                        client.send(JSON.stringify({"euler": getEulerAngles()}));
                    }
                });
            }
        } catch (error) {
            console.error('Error Updating:', error);
            res.status(500).json({ massage: "Failed to Update."});
        }
    })
);

router.all('/update', wrap(async (req, res, next) => {
    res.status(405).json({ massage: "Method Not Allowed" });
}));
  
export default router;