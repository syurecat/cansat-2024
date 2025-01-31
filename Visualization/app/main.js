import { WebSocketServer } from 'ws';
import express from 'express'
import {InfluxDB, Point} from '@influxdata/influxdb-client'

const influxDB = new InfluxDB({
    http://influxdb/,
    os.environ['INFLUXDB_TOKEN']
})
const writeApi = influxDB.getWriteApi(
    os.environ['INFLUXDB_ORG'],
    os.environ['INFLUXDB_BUCKET']
)
const app = express();
app.use(express.json())
const wrap = fn => (...args) => fn(...args).catch(args[2])
const SENSER_TYPES = ["ACC", "GPU", "GYR", "MAG", "BME"];

const server = app.listen(4080, () => {
    console.log("Node.js is listening to PORT:" + server.address().port);
});

const wss = new WebSocketServer({ server });

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

wss.on('connection', function connection(ws) {
  ws.on('error', console.error);

  ws.on('message', function message(data) {
    console.log('received: %s', data);
  });

  ws.send('something');
});

app.post('/update', authenticate, wrap(async (req, res, next) => {
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
                clients.forEach(client => {
                    if (client.readyState === WebSocket.OPEN) {
                        client.json(req.body.data);
                    }
                });
            }
        } catch (error) {
            console.error('Error Updating:', error);
            res.status(500).json({ massage: "Failed to Update."});
        }
    })
);

app.all('/update', wrap(async (req, res, next) => {
    res.status(405).json({ massage: "Method Not Allowed" });
}));

app.all('/teapot', wrap(async (req, res, next) => {
    res.status(418).send("I'm a teapot" );
}));

app.all("*", wrap(async (req, res, next) => {
    res.status(404).send("Not Found");
}));

app.use(async (err, req, res, next) => {
    console.error(err.stack)
    res.status(500).send("Internal Server Error");
});
  