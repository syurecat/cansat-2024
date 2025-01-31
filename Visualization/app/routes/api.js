import express from 'express'
import {InfluxDB, Point} from '@influxdata/influxdb-client'
import {getClients} from './webSocket.js'

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

router.get('/send', authenticate, wrap(async (req, res, next) => {
    try{
        getClients().forEach(client => {
            if (client.readyState === WebSocket.OPEN){
                client.JSON.stringify({ massage: req});
            }
        });
        res.status(200).json({ massage: "succese" })
    } catch (error) {
        console.error('Error Sending:', error)
        res.status(500).json({ massage: "Faild to send" })
    }
}));

router.post('/update', authenticate, wrap(async (req, res, next) => {
        console.log('Authenticated POST request received:', req.body);
        if (!req.body.type || !req.body.name || !req.body.data) {
            res.status(400).json({ massage: "Bad Request" });
        } else if (!SENSER_TYPES.includes(type)) {
            res.status(421).json({ massage: "Misdirected Request" });
        } else {
            next();
        }
    }), wrap(async (req, res, next) => {
        try {
            const point = new Point(type)
                .tag("sensor", req.body.name);

            for (const key in data) {
                if (typeof data[key] === "number") {
                    point.floatField(key, data[key]);
                }
            }
            
            if (!req.body.stre) {
                point.floatField("stre", stre);
            }
            writeApi.writePoint(point);
            await writeApi.flush();
            res.status(200).json({ massage: "success" });
            if (req.body.type == "IMU") {
                getClients().forEach(client => {
                    if (client.readyState === WebSocket.OPEN) {
                        client.JSON.stringify(req.body.data);
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