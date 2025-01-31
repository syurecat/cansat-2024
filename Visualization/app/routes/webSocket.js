import { WebSocketServer } from 'ws'

let clients = new Set();
let wss = null;

export function setWebSocket(server) {
    wss = WebSocketServer({ server });

    wss.on('connection', function connection(ws) {
        clients.add(ws);

        ws.on('error', console.error);
      
        ws.on('message', function message(data) {
          console.log('received: %s', data);
        });
      
        ws.send('something');
    });
}

export function getClients() {
    return clients;
}

export function closeWebSocket() {
    if (wss) {
        console.log("Closing WebSocket server...");
        wss.clients.forEach((ws) => {
            if (client.readyState === WebSocket.OPEN){
                ws.send(JSON.stringify({ message: "Server close" }));
                ws.close()
            }
        });
        wss.close();
    }
}


