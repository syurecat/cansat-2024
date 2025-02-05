import express from 'express'
import {InfluxDB, Point} from '@influxdata/influxdb-client'
import path from 'path'
import { fileURLToPath } from 'url';
import {getClients} from './webSocket.js'
import { predict, update, getEulerAngles } from '../services/imuCalculator.js';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const INFLUXDB_URL = 'http://influxdb:8086'
const INFLUXDB_TOKEN = process.env.INFLUXDB_TOKEN
const INFLUXDB_ORG = process.env.INFLUXDB_ORG
const INFLUXDB_BUCKET = process.env.INFLUXDB_BUCKET
const influxDB = new InfluxDB({
    url: INFLUXDB_URL,
    token: INFLUXDB_TOKEN
})
const writeApi = influxDB.getWriteApi(
    INFLUXDB_ORG,
    INFLUXDB_BUCKET
)
const queryApi = influxDB.getQueryApi(
    INFLUXDB_ORG
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
    } else if (token === AUTH_TOKEN) {
        next();
    } else {
        res.status(403).send('Forbidden');
    }
}

async function getTagKeys(measurement) {
    const fluxQuery = `
      import "influxdata/influxdb/schema"
      schema.tagValues(
        bucket: "${INFLUXDB_BUCKET}",
        tag: "sensor",
        predicate: (r) => r._measurement == "${measurement}"
      )
    `;
  
    return new Promise((resolve, reject) => {
      const tags = [];
      queryApi.queryRows(fluxQuery, {
        next(row, tableMeta) {
          tags.push(tableMeta.toObject(row)._value);
        },
        error(error) {
          reject(error);
        },
        complete() {
          resolve(tags);
        },
      });
    });
}

router.get('/status', wrap(async (req, res) => {
    res.status(200).json({ message: 'OK', uptime: process.uptime() });
}));

router.get("/senserTag/:type", wrap(async (req, res, next) => {
        if (!req.params.type) {
            res.status(404).json({ message: "Not Found "})
        } else if (!SENSER_TYPES.includes(req.params.type)) {
            res.status(400).json({ message: "Bad Request" })
        } else {
            next();
        }
    }), wrap(async (req, res, next) => {
        const tags = await getTagKeys(req.params.type)
        res.status(200).json({ message: "succese", tag: tags})
    })
)

router.get('/latest/:type/:name', wrap(async (req, res, next) => {
        if (!req.params.type || !req.params.name) {
            res.status(404).json({ message: "Not Found" })
        } else if (!SENSER_TYPES.includes(req.params.type)) {
            res.status(400).json({ message: "Bad Request" })
        } else {
            const tags = await getTagKeys(req.params.type);
            console.log(tags)
            if (!tags.includes(req.params.name)) {
                res.status(400).json({ message: "Bad Request" })
            } else {
                next();
            }
        }
    }), wrap(async (req, res, next) => {
        try {
            const fluxQuery = `from(bucket: "${INFLUXDB_BUCKET}")
                                |> range(start: -1h)
                                |> filter(fn: (r) => r._measurement == "${req.params.type}")
                                |> filter(fn: (r) => r.sensor == "${req.params.name}")
                                |> last()`
            
            const result = [];
            await queryApi.collectRows(fluxQuery, row => {
                result.push(row)
            })
            res.status(200).json({ message: "succese", data: result})
        } catch (error) {
            console.error('Error querying InfluxDB:', error);
            res.status(500).json({ message: 'Database error' });
        }
    })
);

// webSocket send 
router.post('/send', authenticate, wrap(async (req, res, next) => {
    console.log('Authenticated POST request received:', req.body);
    if (!req.body.massage) {
        res.status(400).json({ message: "Bad Request" });
    } else {
        next();
    }
    }), wrap(async (req, res, next) => {
        try{
            getClients().forEach(client => {
                if (client.readyState === WebSocket.OPEN){
                    client.send(JSON.stringify({"message": req.body.massage}));
                }
            });
            res.status(200).json({ message: "succese" })
        } catch (error) {
            console.error('Error Sending:', error)
            res.status(500).json({ message: "Faild to send" })
        }
    })
);

// db update
router.post('/update', authenticate, wrap(async (req, res, next) => {
        if (!req.body.type || !req.body.name || !req.body.data) {
            res.status(400).json({ message: "Bad Request" });
        } else if (!SENSER_TYPES.includes(req.body.type)) {
            res.status(400).json({ message: "Bad Request" });
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
            res.status(200).json({ message: "success" });
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
            res.status(500).json({ message: "Failed to Update."});
        }
    })
);

router.all('/status', wrap(async (req, res, next) => {
    res.status(405).json({ message: "Method Not Allowed" });
}));

router.all('/latest', wrap(async (req, res, next) => {
    res.status(405).json({ message: "Method Not Allowed" });
}));

router.all('/send', wrap(async (req, res, next) => {
    res.status(405).json({ message: "Method Not Allowed" });
}));

router.all('/update', wrap(async (req, res, next) => {
    res.status(405).json({ message: "Method Not Allowed" });
}));
  
export default router;