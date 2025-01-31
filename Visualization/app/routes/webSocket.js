import ws from 'ws'

let clients = new Set(); // クライアントを保存する

function setupWebSocket(server) {
    const wss = new ws.Server({ server });

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

module.exports = { setupWebSocket, getClients };
