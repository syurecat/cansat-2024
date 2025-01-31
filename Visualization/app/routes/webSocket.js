import ws from 'ws'

let clients = new Set();
let wss = null;

function setupWebSocket(server) {
    wss = new ws.Server({ server });

    wss.on('connection', function connection(ws) {
        ws.on('error', console.error);
      
        ws.on('message', function message(data) {
          console.log('received: %s', data);
        });
      
        ws.send('something');
    });
}

function getClients() {
    return clients;
}

function closeWebSocket() {
    if (wss) {
        console.log("Closing WebSocket server...");
        wss.clients.forEach((ws) => ws.json({ message: "Server close"}));
        wss.clients.forEach((ws) => ws.close());
        wss.close();
    }
}

module.exports = { setupWebSocket, getClients, closeWebSocket};
